/**
 * @file DataFilterThread.cpp
 * @brief
 */

/*****************************************************************************/
/* INCLUDES                                                                  */
/*****************************************************************************/
#include <core/BeepControlThread.h>
#include <core/DataFilterThread.h>
#include <core/DataLoggerThread.h>
#include <core/ImuManagerThread.h>
#include <core/PressureReaderThread.h>
#include <dashboard/Dashboard.h>
#include <kalmanfilter/MerweScaledSigmaPoints.h>
#include <kalmanfilter/Ukf.h>

#include <driver/timer.h>
#include <esp_log.h>
#include <freertos/FreeRTOS.h>
#include <freertos/semphr.h>
#include <freertos/task.h>
#include <math.h>

/*****************************************************************************/
/* DEFINED CONSTANTS                                                         */
/*****************************************************************************/
#define SAMPLE_PERIOD_IN_MS    20U
#define SAMPLING_TIMER_GROUP   TIMER_GROUP_0
#define SAMPLING_TIMER         TIMER_1
#define SAMPLING_TIMER_DIVIDER (8U)
#define MS_TO_TIMER_TICK(ms)                                                   \
  ((ms) * ((TIMER_BASE_CLK / SAMPLING_TIMER_DIVIDER) / 1000))

/*****************************************************************************/
/* TYPE DEFINITIONS                                                          */
/*****************************************************************************/

/*****************************************************************************/
/* MACRO DEFINITIONS                                                         */
/*****************************************************************************/

/*****************************************************************************/
/* DEFINITION OF GLOBAL CONSTANTS AND VARIABLES                              */
/*****************************************************************************/
static const char *tag = "data-filter-thread";
static SemaphoreHandle_t filterDataStart;

static const size_t dim_x = 3;
static const size_t dim_z = 2;
static const double alpha = 0.3;
static const double beta = 2.0;
static const double kappa = 0.1;

/*****************************************************************************/
/* DECLARATION OF LOCAL FUNCTIONS                                            */
/*****************************************************************************/

/*****************************************************************************/
/* DEFINITION OF LOCAL FUNCTIONS                                             */
/*****************************************************************************/
// State transition function
static Matrix fx(const Vector &x, double dt)
{
  Matrix xout = Matrix(dim_x, 1);

  // Position     = a*dt^2/2 + v*dt + x0
  // Speed        = a*dt + v
  // Acceleration = a

  xout(0) = (x(2) * dt * dt / 2) + (x(1) * dt) + x(0);
  xout(1) = (x(2) * dt) + x(1);
  xout(2) = x(2);

  return xout;
}

// Measurement function
static Matrix hx(const Vector &x)
{
  assert(dim_x == x.length);

  // Convert height to pressure at altitude using barometric formula
  double press = 101325 * pow((1 - 2.25577 * pow(10, -5) * x(0)), 5.25588);
  double acc = x(2);

  Matrix zout = Matrix(2, 1);
  zout(0) = press;
  zout(1) = acc;

  return zout;
}

static double calculateVerticalAcceleration(ImuData_t &imu)
{
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
    // Sensor error, the gravity vector is reported to be (0,0,0)
    acc = 0;
  }

  return acc;
}

static double calculateDt(void)
{
  static TickType_t last;
  TickType_t current = xTaskGetTickCount();

  double dt;
  if (0 == last) {
    dt = 0.025;
  } else if (last < current) {
    dt = current - last;
    dt = portTICK_RATE_MS * dt;
    dt /= 1000.0;
  } else if (current == last) {
    dt = 0.025;
  } else {
    dt = (TickType_t)(-1) - last + current;
    dt = portTICK_RATE_MS * dt;
    dt /= 1000.0;
  }

  last = current;

  return dt;
}

static bool IRAM_ATTR timerCallback(void *args)
{
  (void)args;

  BaseType_t xHigherPriorityTaskWoken = pdFALSE;
  xSemaphoreGiveFromISR(filterDataStart, &xHigherPriorityTaskWoken);
  if (pdTRUE == xHigherPriorityTaskWoken) {
    portYIELD_FROM_ISR();
  }

  timer_set_counter_value(SAMPLING_TIMER_GROUP, SAMPLING_TIMER, 0);
  timer_start(SAMPLING_TIMER_GROUP, SAMPLING_TIMER);

  return false;
}

static void initAndStartSamplingTimer(void)
{
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
                        MS_TO_TIMER_TICK(SAMPLE_PERIOD_IN_MS));
  timer_enable_intr(SAMPLING_TIMER_GROUP, SAMPLING_TIMER);
  timer_isr_callback_add(
      SAMPLING_TIMER_GROUP, SAMPLING_TIMER, timerCallback, NULL, 0);
  timer_set_counter_value(SAMPLING_TIMER_GROUP, SAMPLING_TIMER, 0);
  timer_start(SAMPLING_TIMER_GROUP, SAMPLING_TIMER);
}

/*****************************************************************************/
/* DEFINITION OF GLOBAL FUNCTIONS                                            */
/*****************************************************************************/
void DataFilterThread(void *p)
{
  MerweScaledSigmaPoints sigmas(dim_x, alpha, beta, kappa);
  UnscentedKalmanFilter ukf(dim_x, dim_z, fx, hx, sigmas);

  initAndStartSamplingTimer();

  // Wait for the sensors to start up properly
  bool ready = false;
  do {
    SampleImu();
    xSemaphoreTake(filterDataStart, portMAX_DELAY);

    ImuData_t imu;
    DbDataImuGet(&imu);

    ready = (IMU_SYS_RUNNING_FUSION == imu.system.status);
    ready &= (3 == imu.calibration.mag);
    ready &= (3 == imu.calibration.gyro);

  } while (!ready);

  // Set initial conditions
  ready = false;
  do {
    SampleBps();
    xSemaphoreTake(filterDataStart, portMAX_DELAY);

    BpsData_t bps;
    DbDataBpsGet(&bps);

    if (0 < bps.cooked.pressure) {
      double p0 = bps.cooked.pressure;
      double height = 44330 * (1 - pow(p0 / 101325.0, 0.1902));
      ukf.x(0) = height; // Initial height
      ukf.x(1) = 0.0;    // Initial speed
      ukf.x(2) = 0.0;    // Initial acceleration

      ready = true;
    } else {
      ready = false;
    }

  } while (!ready);

  // Initialize process state covariance matrix
  ukf.P(0, 0) = 1, ukf.P(0, 1) = 0, ukf.P(0, 2) = 0;
  ukf.P(1, 0) = 0, ukf.P(1, 1) = 1, ukf.P(1, 2) = 0;
  ukf.P(2, 0) = 0, ukf.P(2, 1) = 0, ukf.P(2, 2) = 1;

  // Set measurement covariance matrix (based on previous measurement)
  const double p_std = 3.74;
  const double a_std = 0.2;

  ukf.R(0, 0) = (p_std * p_std), ukf.R(0, 1) = 0.0;
  ukf.R(1, 0) = 0.0, ukf.R(1, 1) = (a_std * a_std);

  // Initialize process noise matrix
  ukf.Q(0, 0) = 1.2e-09, ukf.Q(0, 1) = 1.2e-07, ukf.Q(0, 2) = 6.0e-06;
  ukf.Q(1, 0) = 1.2e-07, ukf.Q(1, 1) = 1.2e-05, ukf.Q(1, 2) = 6.0e-04;
  ukf.Q(2, 0) = 6.0e-06, ukf.Q(2, 1) = 6.0e-04, ukf.Q(2, 2) = 3.0e-02;

  Matrix z(dim_z, 1);

  ESP_LOGI(tag, "Kalman filter started");

  LogAppend("ts dt p gx gy gz ax ay az h v a\n\n");

  while (1) {
    xSemaphoreTake(filterDataStart, portMAX_DELAY);
    TickType_t timeStamp = xTaskGetTickCount();

    // Calculate dt
    double dt = calculateDt();

    BpsData_t bps;
    DbDataBpsGet(&bps);

    ImuData_t imu;
    DbDataImuGet(&imu);

    // Wake up data sampling threads.
    SampleBps();
    SampleImu();

    // Update measurement vector
    z(0) = bps.cooked.pressure;
    z(1) = calculateVerticalAcceleration(imu);

    // Run epoche
    ukf.predict(dt);
    ukf.update(z);

    // Update the results in the database
    FilterOutput_t out;
    out.height = ukf.x(0);
    out.vario.instant = ukf.x(1);
    out.vario.averaged = 0;

    DbDataFilterOutputSet(&out);
    BeepControlUpdate();

    LogAppend(
        "%d %d "
        "%d "
        "%3.2f %3.2f %3.2f "
        "%3.2f %3.2f %3.2f "
        "%5.1f %3.2f %3.2f\n",
        (int)timeStamp,
        (int)(dt * 1000),
        (int)bps.cooked.pressure,
        imu.gravity.x,
        imu.gravity.y,
        imu.gravity.z,
        imu.acceleration.x,
        imu.acceleration.y,
        imu.acceleration.z,
        ukf.x(0),
        ukf.x(1),
        ukf.x(2));
  }
}

void DataFilterThreadInit(void)
{
  filterDataStart = xSemaphoreCreateBinary();
}

/****************************** END OF FILE **********************************/
