/**
 * @file DataFilterThread.cpp
 * @brief
 */

/*****************************************************************************/
/* INCLUDES                                                                  */
/*****************************************************************************/
#include "DataFilterThread.h"
#include "DataLoggerThread.h"
#include "dashboard/Dashboard.h"
#include "kalmanfilter/MerweScaledSigmaPoints.h"
#include "kalmanfilter/Ukf.h"

#include <Arduino.h>

/*****************************************************************************/
/* DEFINED CONSTANTS                                                         */
/*****************************************************************************/

/*****************************************************************************/
/* TYPE DEFINITIONS                                                          */
/*****************************************************************************/

/*****************************************************************************/
/* MACRO DEFINITIONS                                                         */
/*****************************************************************************/

/*****************************************************************************/
/* DEFINITION OF GLOBAL CONSTANTS AND VARIABLES                              */
/*****************************************************************************/
SemaphoreHandle_t filterDataReady;

static const size_t dim_x = 3;
static const size_t dim_z = 2;
static const double dt    = 0.02;
static const double alpha = 0.3;
static const double beta  = 2.0;
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

  // Position     = a*dt2/2 + v*dt + x0
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
  double acc   = x(2);

  Matrix zout = Matrix(2, 1);
  zout(0)     = press;
  zout(1)     = acc;

  return zout;
}

double calculateVerticalAcceleration(ImuData_t &imu)
{
  double gx = imu.gravity.x;
  double gy = imu.gravity.y;
  double gz = imu.gravity.z;

  double absg = sqrt((gx * gx) + (gy * gy) + (gz * gz));

  double ax = imu.acceleration.x;
  double ay = imu.acceleration.y;
  double az = imu.acceleration.z;

  double skag = (gx * ax) + (gy * ay) + (gz * az);
  double acc  = skag / absg;

  return acc;
}

/*****************************************************************************/
/* DEFINITION OF GLOBAL FUNCTIONS                                            */
/*****************************************************************************/
void DataFilterThread(void *p)
{
  filterDataReady = xSemaphoreCreateBinary();

  MerweScaledSigmaPoints sigmas(dim_x, alpha, beta, kappa);
  UnscentedKalmanFilter  ukf(dim_x, dim_z, dt, fx, hx, sigmas);

  // Wait for the sensors to start up properly
  bool ready = false;
  do {
    xSemaphoreTake(filterDataReady, portMAX_DELAY);
    ImuData_t imu;
    DbDataImuGet(&imu);

    ready = (IMU_SYS_RUNNING_FUSION == imu.system.status);
    ready &= (3 == imu.calibration.acc);
    ready &= (3 == imu.calibration.mag);
    ready &= (3 == imu.calibration.gyro);
    ready &= (3 == imu.calibration.sys);

  } while (!ready);

  // Set initial conditions
  ready = false;
  do {
    xSemaphoreTake(filterDataReady, portMAX_DELAY);
    BpsData_t bps;
    DbDataBpsGet(&bps);

    double p0     = bps.cooked.pressure;
    double height = 44330 * (1 - pow(p0 / 101325.0, 0.1902));
    ukf.x(0)      = height; // Initial height
    ukf.x(1)      = 0.0;    // Initial speed
    ukf.x(2)      = 0.0;    // Initial acceleration

    ready = true;

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

  Serial.println("Kalman filter started");

  while (1) {
    xSemaphoreTake(filterDataReady, portMAX_DELAY);
    TickType_t timeStamp = xTaskGetTickCount();

    BpsData_t bps;
    DbDataBpsGet(&bps);

    ImuData_t imu;
    DbDataImuGet(&imu);

    // Update measurement vector
    z(0) = bps.cooked.pressure;
    z(1) = calculateVerticalAcceleration(imu);

    // Run epoche
    ukf.predict();
    ukf.update(z);

    // Update the results in the database
    FilterOutput_t out;
    out.height         = ukf.x(0);
    out.vario.instant  = ukf.x(1);
    out.vario.averaged = 0;

    DbDataFilterOutputSet(&out);

    LogAppend("%d %d %3.2f %3.2f %3.2f %3.2f %3.2f %3.2f %5.1f %3.2f %3.2f\n",
              (int)timeStamp,
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

#if 0
      Serial.print(ukf.x(0));
      Serial.print(" ");
      Serial.print(ukf.x(1));
      Serial.print(" ");
      Serial.print(ukf.x(2));
      Serial.print(" ");
      Serial.print(end - start);
      Serial.print(" ");
      Serial.print(bps.cooked.pressure);
      Serial.print(" ");
      Serial.print(imu.gravity.x);
      Serial.print(" ");
      Serial.print(imu.gravity.y);
      Serial.print(" ");
      Serial.print(imu.gravity.z);
      Serial.print(" ");
      Serial.print(imu.acceleration.x);
      Serial.print(" ");
      Serial.print(imu.acceleration.y);
      Serial.print(" ");
      Serial.print(imu.acceleration.z);
      Serial.println();
#endif
  }
}

/****************************** END OF FILE **********************************/
