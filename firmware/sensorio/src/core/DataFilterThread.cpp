//
//  This file is part of Sensorio.
//
//  Sensorio is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  Sensorio is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with Sensorio.  If not, see <https://www.gnu.org/licenses/>.
//

#include <core/DataFilterThread.hpp>
#include <core/DataLoggerThread.hpp>
#include <core/ImuManagerThread.hpp>
#include <core/LogFile.hpp>
#include <core/PressureReaderThread.hpp>
#include <dashboard/Dashboard.hpp>
#include <kalmanfilter/MerweScaledSigmaPoints.hpp>
#include <kalmanfilter/Ukf.hpp>

#include <driver/timer.h>
#include <esp_log.h>
#include <freertos/FreeRTOS.h>
#include <freertos/semphr.h>
#include <freertos/task.h>
#include <math.h>

#include <platform/Log.hpp>

static constexpr uint32_t SAMPLE_PERIOD_IN_MS{20U};
static constexpr auto SAMPLING_TIMER_GROUP{TIMER_GROUP_0};
static constexpr auto SAMPLING_TIMER{TIMER_1};
static constexpr uint64_t SAMPLING_TIMER_DIVIDER{8U};

static inline uint64_t MillisecToTimerTick(uint64_t ms) {
  return ms * ((TIMER_BASE_CLK / SAMPLING_TIMER_DIVIDER) / 1000);
}

static const char *tag = "data-filter-thread";
static SemaphoreHandle_t filterDataStart;

static constexpr uint32_t dim_x = 3;
static constexpr uint32_t dim_z = 2;
static constexpr double alpha = 0.3;
static constexpr double beta = 2.0;
static constexpr double kappa = 0.1;

// State transition function
const typename UnscentedKalmanFilter<dim_x, dim_z>::StateVector
fx(const typename UnscentedKalmanFilter<dim_x, dim_z>::StateVector &x,
   double dt) {
  typename UnscentedKalmanFilter<dim_x, dim_z>::StateVector xout{};

  // Position     = a*dt^2/2 + v*dt + x0
  // Speed        = a*dt + v
  // Acceleration = a

  xout[0][0] = (x[2][0] * dt * dt / 2) + (x[1][0] * dt) + x[0][0];
  xout[1][0] = (x[2][0] * dt) + x[1][0];
  xout[2][0] = x[2][0];

  return xout;
}

// Measurement function
typename UnscentedKalmanFilter<dim_x, dim_z>::MeasurementVector
hx(const typename UnscentedKalmanFilter<dim_x, dim_z>::StateVector &x) {
  // Convert height to pressure at altitude using barometric formula
  double press = 101325 * pow((1 - 2.25577 * pow(10, -5) * x[0][0]), 5.25588);
  double acc = x[2][0];

  typename UnscentedKalmanFilter<3, 2>::MeasurementVector zout{};
  zout[0][0] = press;
  zout[1][0] = acc;

  return zout;
}

static double calculateVerticalAcceleration(Dashboard::Imu &imu) {
  double gx = imu.gravity.x;
  double gy = imu.gravity.y;
  double gz = imu.gravity.z;

  double absg = sqrt((gx * gx) + (gy * gy) + (gz * gz));

  double ax = imu.acceleration.x;
  double ay = imu.acceleration.y;
  double az = imu.acceleration.z;

  double acc = 0.0;

  if ((9.0 < absg) && (absg < 11.0)) {
    double skag = (gx * ax) + (gy * ay) + (gz * az);
    acc = skag / absg;
  } else {
    // Sensor error, the gravity vector is reported to be invalid
    acc = 0;
  }

  return acc;
}

static double calculateDtInSec(void) {
  static TickType_t last;
  TickType_t current = xTaskGetTickCount();

  double dt;
  if (0 == last) {
    dt = SAMPLE_PERIOD_IN_MS;
  } else if (last < current) {
    dt = current - last;
    dt = portTICK_RATE_MS * dt;
  } else {
    dt = (TickType_t)(-1) - last + current;
    dt = portTICK_RATE_MS * dt;
  }

  // Millisec to second
  dt /= 1000;

  last = current;

  return dt;
}

static bool IRAM_ATTR timerCallback(void *args) {
  (void)args;

  BaseType_t xHigherPriorityTaskWoken = pdFALSE;
  xSemaphoreGiveFromISR(filterDataStart, &xHigherPriorityTaskWoken);
  if (pdTRUE == xHigherPriorityTaskWoken) {
    portYIELD_FROM_ISR();
  }

  timer_set_counter_value(SAMPLING_TIMER_GROUP, SAMPLING_TIMER, 0);
  timer_start(SAMPLING_TIMER_GROUP, SAMPLING_TIMER);

  SampleBps();
  SampleImu();

  return false;
}

static void initAndStartSamplingTimer(void) {
  timer_config_t config = {
      .alarm_en = TIMER_ALARM_EN,
      .counter_en = TIMER_PAUSE,
      .intr_type = TIMER_INTR_LEVEL,
      .counter_dir = TIMER_COUNT_UP,
      .auto_reload = TIMER_AUTORELOAD_EN,
      .divider = SAMPLING_TIMER_DIVIDER,
  };
  timer_init(SAMPLING_TIMER_GROUP, SAMPLING_TIMER, &config);
  timer_set_counter_value(SAMPLING_TIMER_GROUP, SAMPLING_TIMER, 0);
  timer_set_alarm_value(SAMPLING_TIMER_GROUP,
                        SAMPLING_TIMER,
                        MillisecToTimerTick(SAMPLE_PERIOD_IN_MS));
  timer_enable_intr(SAMPLING_TIMER_GROUP, SAMPLING_TIMER);
  timer_isr_callback_add(
      SAMPLING_TIMER_GROUP, SAMPLING_TIMER, timerCallback, NULL, 0);
  timer_set_counter_value(SAMPLING_TIMER_GROUP, SAMPLING_TIMER, 0);
  timer_start(SAMPLING_TIMER_GROUP, SAMPLING_TIMER);
}

static void unlockSemaphore(SemaphoreHandle_t &sem) {
  BaseType_t res{pdFALSE};
  do {
    res = xSemaphoreGive(sem);
  } while (pdTRUE != res);
}

static void lockSemaphore(SemaphoreHandle_t &sem) {
  BaseType_t res{pdFALSE};
  do {
    res = xSemaphoreTake(sem, portMAX_DELAY);
  } while (pdTRUE != res);
}

static void waitForImu() {
  StaticSemaphore_t alloc{};
  SemaphoreHandle_t sem = xSemaphoreCreateBinaryStatic(&alloc);
  configASSERT(nullptr != sem);
  Mask mask = static_cast<Mask>(Dashboard::Dashboard::Data::IMU);
  dashboard.subscribe(mask, [&sem]() { unlockSemaphore(sem); });
  lockSemaphore(sem);
}

static void waitForBps() {
  StaticSemaphore_t alloc{};
  SemaphoreHandle_t sem = xSemaphoreCreateBinaryStatic(&alloc);
  configASSERT(nullptr != sem);
  Mask mask = static_cast<Mask>(Dashboard::Dashboard::Data::BPS);
  dashboard.subscribe(mask, [&sem]() { unlockSemaphore(sem); });
  lockSemaphore(sem);
}

static void waitForImuAndBps() {
  StaticSemaphore_t alloc{};
  SemaphoreHandle_t sem = xSemaphoreCreateBinaryStatic(&alloc);
  configASSERT(nullptr != sem);
  Mask mask = static_cast<Mask>(Dashboard::Dashboard::Data::BPS) |
              static_cast<Mask>(Dashboard::Dashboard::Data::IMU);
  dashboard.subscribe(mask, [&sem]() { unlockSemaphore(sem); });
  lockSemaphore(sem);
}

void DataFilterThread(void *p) {
  MerweScaledSigmaPoints<dim_x> sigmas(alpha, beta, kappa);
  UnscentedKalmanFilter<dim_x, dim_z> ukf(fx, hx, sigmas);

  initAndStartSamplingTimer();

  // Wait for the IMU to start up properly
  bool ready = false;
  do {
    waitForImu();

    Dashboard::Imu imu{dashboard.imu.get()};

    ready = (Dashboard::Imu::Status::RUNNING_FUSION == imu.system.status);
    ready &= (3 == imu.calibration.mag);
    ready &= (3 == imu.calibration.gyro);

  } while (!ready);

  // Set initial conditions
  ready = false;
  do {
    waitForBps();

    Dashboard::Bps bps{dashboard.bps.get()};

    if (0 < bps.cooked.pressure) {
      double p0 = bps.cooked.pressure;
      double height = 44330 * (1 - pow(p0 / 101325.0, 0.1902));
      ukf.x[0][0] = height; // Initial height
      ukf.x[1][0] = 0.0;    // Initial speed
      ukf.x[2][0] = 0.0;    // Initial acceleration

      ready = true;
    } else {
      ready = false;
    }

  } while (!ready);

  // Initialize process state covariance matrix
  ukf.P[0][0] = 1, ukf.P[0][1] = 0, ukf.P[0][2] = 0;
  ukf.P[1][0] = 0, ukf.P[1][1] = 1, ukf.P[1][2] = 0;
  ukf.P[2][0] = 0, ukf.P[2][1] = 0, ukf.P[2][2] = 1;

  // Set measurement covariance matrix (based on previous measurement)
  const double p_std = 3.74;
  const double a_std = 0.02;

  ukf.R[0][0] = (p_std * p_std), ukf.R[0][1] = 0.0;
  ukf.R[1][0] = 0.0, ukf.R[1][1] = (a_std * a_std);

  // Initialize process noise matrix
  ukf.Q[0][0] = 1.2e-09, ukf.Q[0][1] = 1.2e-07, ukf.Q[0][2] = 6.0e-06;
  ukf.Q[1][0] = 1.2e-07, ukf.Q[1][1] = 1.2e-05, ukf.Q[1][2] = 6.0e-04;
  ukf.Q[2][0] = 6.0e-06, ukf.Q[2][1] = 6.0e-04, ukf.Q[2][2] = 3.0e-02;

  ESP_LOGI(tag, "Kalman filter started");

  LogFile::File<1024> logfile{"sensorio.dat"};
  logfile.setCallback(DataLoggerSave);

  logfile << "ts dt p gx gy gz ax ay az h v a\n";

  while (1) {
    waitForImuAndBps();

    TickType_t timeStamp = xTaskGetTickCount();
    double dt = calculateDtInSec();

    static uint32_t counter{0};
    static double min{25.0};
    static double max{};
    static double avg{};

    if (dt < min) {
      min = dt;
    }

    if (max < dt) {
      max = dt;
    }

    avg += dt;

    if (++counter == 50) {
      ESP_LOGE("zzzz", "%lf %lf %lf", min, max, avg / 50.0);
      max = avg = 0.0;
      min = 25.0;
      counter = 0;
    }

    Dashboard::Bps bps{dashboard.bps.get()};
    Dashboard::Imu imu{dashboard.imu.get()};

    // Update measurement vector
    typename UnscentedKalmanFilter<dim_x, dim_z>::MeasurementVector z{};
    z[0][0] = bps.cooked.pressure;
    z[1][0] = calculateVerticalAcceleration(imu);

    // Run epoche
    ukf.predict(dt);
    ukf.update(z);

    // Update the results in the database
    Dashboard::Filter filter{};
    filter.height = ukf.x[0][0];
    filter.vario.instant = ukf.x[1][0];
    filter.vario.averaged = 0;
    dashboard.filter.set(filter);

    logfile << timeStamp << " ";
    logfile << dt * 1000 << " ";
    logfile << bps.cooked.pressure << " ";
    logfile << imu.gravity.x << " ";
    logfile << imu.gravity.y << " ";
    logfile << imu.gravity.z << " ";
    logfile << imu.acceleration.x << " ";
    logfile << imu.acceleration.y << " ";
    logfile << imu.acceleration.z << " ";
    logfile << ukf.x[0][0] << " ";
    logfile << ukf.x[1][0] << " ";
    logfile << ukf.x[2][0] << "\n";
  }
}

void DataFilterThreadInit(void) {
  filterDataStart = xSemaphoreCreateBinary();
}
