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

static constexpr uint32_t dim_x = 2;
static constexpr uint32_t dim_z = 1;
static constexpr double alpha = 0.3;
static constexpr double beta = 2.0;
static constexpr double kappa = 0.1;

// State transition function
const typename UnscentedKalmanFilter<dim_x, dim_z>::StateVector
fx(const typename UnscentedKalmanFilter<dim_x, dim_z>::StateVector &x,
   double dt) {
  typename UnscentedKalmanFilter<dim_x, dim_z>::StateVector xout{};

  auto v{x[1][0]};
  auto h{x[0][0]};

  xout[0][0] = v * dt + h; // Height
  xout[1][0] = v;          // Vertical speed

  return xout;
}

// Measurement function
typename UnscentedKalmanFilter<dim_x, dim_z>::MeasurementVector
hx(const typename UnscentedKalmanFilter<dim_x, dim_z>::StateVector &x) {
  // Convert height to pressure at altitude using barometric formula
  double p = 101325 * pow((1 - 2.25577 * pow(10, -5) * x[0][0]), 5.25588);

  typename UnscentedKalmanFilter<dim_x, dim_z>::MeasurementVector zout{};
  zout[0][0] = p;

  return zout;
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

static void waitForBps() {
  StaticSemaphore_t alloc{};
  SemaphoreHandle_t sem = xSemaphoreCreateBinaryStatic(&alloc);
  configASSERT(nullptr != sem);
  Mask mask = static_cast<Mask>(Dashboard::Dashboard::Data::BPS);
  dashboard.subscribe(mask, [&sem]() { unlockSemaphore(sem); });
  lockSemaphore(sem);
}

void DataFilterThread(void *p) {
  MerweScaledSigmaPoints<dim_x> sigmas(alpha, beta, kappa);
  UnscentedKalmanFilter<dim_x, dim_z> ukf(fx, hx, sigmas);

  initAndStartSamplingTimer();

  // Set initial conditions
  auto ready{false};
  do {
    waitForBps();

    Dashboard::Bps bps{dashboard.bps.get()};

    if (0 < bps.cooked.pressure) {
      double p0 = bps.cooked.pressure;
      double height = 44330 * (1 - pow(p0 / 101325.0, 0.1902));
      ukf.x[0][0] = height; // Initial height
      ukf.x[1][0] = 0.0;    // Initial speed
      ready = true;
    } else {
      ready = false;
    }

  } while (!ready);

  // Initialize process state covariance matrix
  ukf.P[0][0] = 1, ukf.P[0][1] = 0;
  ukf.P[1][0] = 0, ukf.P[1][1] = 1;

  // Set measurement covariance matrix (based on previous measurement)
  static constexpr auto p_std {1.0};
  ukf.R[0][0] = (p_std * p_std);

  // Initialize process noise matrix
  ukf.Q[0][0] = 1.2e-09, ukf.Q[0][1] = 1.2e-07;
  ukf.Q[1][0] = 1.2e-07, ukf.Q[1][1] = 1.2e-05;

  LogFile::File<1024> logfile{"sensorio.dat"};
  logfile.setCallback(DataLoggerSave);
  logfile << "ts dt p h v\n";

  Platform::Log::Info(tag) << "Kalman filter started";

  while (1) {
    waitForBps();

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

    // Update measurement vector
    typename UnscentedKalmanFilter<dim_x, dim_z>::MeasurementVector z{};
    z[0][0] = bps.cooked.pressure;

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
    logfile << ukf.x[0][0] << " ";
    logfile << ukf.x[1][0] << "\n";
  }
}

void DataFilterThreadInit(void) {
  filterDataStart = xSemaphoreCreateBinary();
}
