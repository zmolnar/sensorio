/**
 * @file ImuManagerThread.cpp
 * @brief
 */

/*****************************************************************************/
/* INCLUDES                                                                  */
/*****************************************************************************/
#include <Arduino.h>
#include <Wire.h>

#include "ImuManagerThread.h"
#include "dashboard/Dashboard.h"
#include "drivers/bno055/bno055.h"

/*****************************************************************************/
/* DEFINED CONSTANTS                                                         */
/*****************************************************************************/
#define BNO_SCL  16
#define BNO_SDA  17
#define BNO_FREQ 400000

/*****************************************************************************/
/* TYPE DEFINITIONS                                                          */
/*****************************************************************************/

/*****************************************************************************/
/* MACRO DEFINITIONS                                                         */
/*****************************************************************************/

/*****************************************************************************/
/* DEFINITION OF GLOBAL CONSTANTS AND VARIABLES                              */
/*****************************************************************************/
TaskHandle_t imuTask = NULL;

static TwoWire bno055_twi = TwoWire(1);

/*****************************************************************************/
/* DECLARATION OF LOCAL FUNCTIONS                                            */
/*****************************************************************************/

/*****************************************************************************/
/* DEFINITION OF LOCAL FUNCTIONS                                             */
/*****************************************************************************/
static bool i2c_init(void)
{
  return bno055_twi.begin(BNO_SDA, BNO_SCL, BNO_FREQ);
}

static s8 i2c_bus_write(u8 dev_addr, u8 reg_addr, u8 *reg_data, u8 cnt)
{
  bno055_twi.beginTransmission(dev_addr);
  bno055_twi.write(reg_addr);
  size_t i;
  for (i = 0; i < cnt; ++i) {
    if (0 == bno055_twi.write(static_cast<uint8_t>(reg_data[i]))) {
      break;
    }
  }
  bno055_twi.endTransmission();

  return (i == cnt) ? BNO055_SUCCESS : BNO055_ERROR;
}

static s8 i2c_bus_read(u8 dev_addr, u8 reg_addr, u8 *reg_data, u8 cnt)
{
  bno055_twi.beginTransmission(dev_addr);
  bno055_twi.write(reg_addr);
  bno055_twi.endTransmission();

  bno055_twi.requestFrom(dev_addr, cnt);
  size_t i;
  for (i = 0; i < cnt; ++i) {
    if (bno055_twi.available()) {
      reg_data[i] = static_cast<u8>(bno055_twi.read());
    } else {
      break;
    }
  }

  return (i == cnt) ? BNO055_SUCCESS : BNO055_ERROR;
}

static bool sendCalibrationToDevice(BNO055 &imu, ImuOffset_t &offset)
{
  BNO055::AccelOffset_t accoffset;
  accoffset.x  = offset.acc.x;
  accoffset.y  = offset.acc.y;
  accoffset.z  = offset.acc.z;
  accoffset.r  = offset.acc.r;
  bool success = imu.setAccOffset(accoffset);
  Serial.print("Acc offset x:");
  Serial.print(accoffset.x);
  Serial.print(" y:");
  Serial.print(accoffset.y);
  Serial.print(" z:");
  Serial.print(accoffset.z);
  Serial.print(" r:");
  Serial.print(accoffset.r);
  Serial.println();
  configASSERT(success);

  BNO055::GyroOffset_t gyrooffset;
  gyrooffset.x = offset.gyro.x;
  gyrooffset.y = offset.gyro.y;
  gyrooffset.z = offset.gyro.z;
  success      = imu.setGyroOffset(gyrooffset);
  Serial.print("Gyro offset x:");
  Serial.print(gyrooffset.x);
  Serial.print(" y:");
  Serial.print(gyrooffset.y);
  Serial.print(" z:");
  Serial.print(gyrooffset.z);
  Serial.println();
  configASSERT(success);

  BNO055::MagOffset_t magoffset;
  magoffset.x = offset.mag.x;
  magoffset.y = offset.mag.y;
  magoffset.z = offset.mag.z;
  magoffset.r = offset.mag.r;
  success     = imu.setMagOffset(magoffset);
  Serial.print("Mag offset x:");
  Serial.print(magoffset.x);
  Serial.print(" y:");
  Serial.print(magoffset.y);
  Serial.print(" z:");
  Serial.print(magoffset.z);
  Serial.print(" r:");
  Serial.print(magoffset.r);
  Serial.println();
  configASSERT(success);

  return success;
}

/*****************************************************************************/
/* DEFINITION OF GLOBAL FUNCTIONS                                            */
/*****************************************************************************/
void ImuManagerThread(void *p)
{
  imuTask = xTaskGetCurrentTaskHandle();

  BNO055 bno055 = BNO055(i2c_init, i2c_bus_read, i2c_bus_write, delay);

  // Initialize the sensor
  bool success = bno055.begin();
  configASSERT(success);

  success = bno055.setPowerMode(BNO055::PowerMode::NORMAL);
  configASSERT(success);

  success = bno055.setClockSource(BNO055::ClockSource::EXT);
  configASSERT(success);
  
  delay(1000);

  BNO055::ClockSource clk = BNO055::ClockSource::UNDEF;
  success = bno055.getClockSource(clk);
  configASSERT(success);

  // Restore calibration constants
  bool calibrationNeeded = false;

  if (DbCalibrationIsValid()) {
    ImuOffset_t offset;
    DbCalibrationGet(&offset);
    success = sendCalibrationToDevice(bno055, offset);
    configASSERT(success);
    calibrationNeeded = false;
  } else {
    calibrationNeeded = true;
  }

  // Start fusion firmware
  success = bno055.setOperationMode(BNO055::OperationMode::NDOF);
  configASSERT(success);

  // Refer to page 22 of the datasheet.
  delay(30);

  while (1) {
    uint32_t notification = ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
    if (0 < notification) {
      ImuData_t data;
      memset(&data, 0, sizeof(data));

      BNO055::Status status = BNO055::Status::SYS_UNKNOWN;
      bno055.getDeviceStatus(status);
      switch (status) {
      case BNO055::Status::SYS_IDLE:
        data.system.status = IMU_SYS_IDLE;
        break;
      case BNO055::Status::SYS_ERROR:
        data.system.status = IMU_SYS_ERROR;
        break;
      case BNO055::Status::SYS_PERIPHERAL_INIT:
        data.system.status = IMU_SYS_PERIPHERAL_INIT;
        break;
      case BNO055::Status::SYS_INITIALIZING:
        data.system.status = IMU_SYS_INITIALIZING;
        break;
      case BNO055::Status::SYS_RUNNING_SELFTEST:
        data.system.status = IMU_SYS_RUNNING_SELFTEST;
        break;
      case BNO055::Status::SYS_RUNNING_FUSION:
        data.system.status = IMU_SYS_RUNNING_FUSION;
        break;
      case BNO055::Status::SYS_RUNNING_NO_FUSION:
        data.system.status = IMU_SYS_RUNNING_NO_FUSION;
        break;
      default:
        data.system.status = IMU_SYS_UNKNOWN;
        break;
      }

      BNO055::ClockSource clk = BNO055::ClockSource::UNDEF;
      bno055.getClockSource(clk);
      switch (clk) {
      case BNO055::ClockSource::INT:
        data.system.clk = IMU_CLK_INTERNAL;
        break;
      case BNO055::ClockSource::EXT:
        data.system.clk = IMU_CLK_EXTERNAL;
        break;
      default:
        data.system.clk = IMU_CLK_UNKNOWN;
      }

      BNO055::Euler_t euler;
      if (bno055.getEulerAngles(euler, BNO055::Unit::DEG)) {
        data.euler.yaw   = euler.h;
        data.euler.pitch = euler.p;
        data.euler.roll  = euler.r;
      }

      BNO055::Gravity_t gravity;
      if (bno055.getGravity(gravity)) {
        data.gravity.x = gravity.x;
        data.gravity.y = gravity.y;
        data.gravity.z = gravity.z;
      }

      BNO055::LinearAccel_t acc;
      if (bno055.getLinearAcceleration(acc)) {
        data.acceleration.x = acc.x;
        data.acceleration.y = acc.y;
        data.acceleration.z = acc.z;
      }

      bno055.getAccCalibrationStatus(data.calibration.acc);
      bno055.getGyroCalibrationStatus(data.calibration.gyro);
      bno055.getMagCalibrationStatus(data.calibration.mag);
      bno055.getSystemCalibrationStatus(data.calibration.sys);

      DbDataImuSet(&data);

      if (calibrationNeeded) {
        if ((3 == data.calibration.acc) && (3 == data.calibration.gyro) &&
            (3 == data.calibration.mag) && (3 == data.calibration.sys)) {
          BNO055::AccelOffset_t accoffset;
          BNO055::GyroOffset_t  gyrooffset;
          BNO055::MagOffset_t   magoffset;

          success = bno055.setOperationMode(BNO055::OperationMode::CONFIG);
          configASSERT(success);

          // Refer to page 22 of the datasheet.
          delay(10);

          bool success;
          success = bno055.getAccOffset(accoffset);
          Serial.print("Acc offset x:");
          Serial.print(accoffset.x);
          Serial.print(" y:");
          Serial.print(accoffset.y);
          Serial.print(" z:");
          Serial.print(accoffset.z);
          Serial.print(" r:");
          Serial.print(accoffset.r);
          Serial.println();
          configASSERT(success);

          success = bno055.getGyroOffset(gyrooffset);
          Serial.print("Gyro offset x:");
          Serial.print(gyrooffset.x);
          Serial.print(" y:");
          Serial.print(gyrooffset.y);
          Serial.print(" z:");
          Serial.print(gyrooffset.z);
          Serial.println();
          configASSERT(success);

          success = bno055.getMagOffset(magoffset);
          Serial.print("Mag offset x:");
          Serial.print(magoffset.x);
          Serial.print(" y:");
          Serial.print(magoffset.y);
          Serial.print(" z:");
          Serial.print(magoffset.z);
          Serial.print(" r:");
          Serial.print(magoffset.r);
          Serial.println();
          configASSERT(success);

          ImuOffset_t offset;
          offset.acc.x  = accoffset.x;
          offset.acc.y  = accoffset.y;
          offset.acc.z  = accoffset.z;
          offset.acc.r  = accoffset.r;
          offset.gyro.x = gyrooffset.x;
          offset.gyro.y = gyrooffset.y;
          offset.gyro.z = gyrooffset.z;
          offset.mag.x  = magoffset.x;
          offset.mag.y  = magoffset.y;
          offset.mag.z  = magoffset.z;
          offset.mag.r  = magoffset.r;

          DbCalibrationSet(&offset);
          configASSERT(DbCalibrationIsValid());

          calibrationNeeded = false;

          Serial.println("Sensor offsets saved to EEPROM");
          success = bno055.setOperationMode(BNO055::OperationMode::NDOF);
          configASSERT(success);

          // Refer to page 22 of the datasheet.
          delay(30);
        }
      }
    } else {
      Serial.println("IMU task notification failed");
    }
  }
}

/****************************** END OF FILE ********************************/
