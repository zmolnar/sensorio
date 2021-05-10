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
#include "gui/Sensorio.h"

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
SemaphoreHandle_t readImu;

static TwoWire bno055_twi = TwoWire(1);
static bool calibrationDataNeeded = false;

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

  configASSERT(i == cnt);

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

static inline void saveLogAndResetInCaseOfError(bool error)
{
  configASSERT(!error);
  if (error) {
    // TODO Write logfile
    SensorioStop();
  }
}

static bool resetAndInitializeImu(BNO055 &imu)
{
  bool success = false;

  const uint32_t maxNumOfRetries = 5;
  for (uint32_t i = 0; (!success) && (i < maxNumOfRetries); ++i) {
    success = imu.begin();
    success = success && imu.reset();
    success = success && imu.setPowerMode(BNO055::PowerMode::NORMAL);
    success = success && imu.setClockSource(BNO055::ClockSource::EXT);
    success = success && imu.isExternalClockInUse();
    success = success && imu.setOperationMode(BNO055::OperationMode::NDOF);

    if (!success) {
      Serial.print("BNO055 device init failed #");
      Serial.println(i);
      delay(500);
    }
  }

  return success;
}

static bool sendCalibrationToDevice(BNO055 &imu, ImuOffset_t &offset)
{
  BNO055::AccelOffset_t accoffset;
  accoffset.x = offset.acc.x;
  accoffset.y = offset.acc.y;
  accoffset.z = offset.acc.z;
  accoffset.r = offset.acc.r;
  bool success = imu.setAccOffset(accoffset);

  BNO055::GyroOffset_t gyrooffset;
  gyrooffset.x = offset.gyro.x;
  gyrooffset.y = offset.gyro.y;
  gyrooffset.z = offset.gyro.z;
  success = success && imu.setGyroOffset(gyrooffset);

  BNO055::MagOffset_t magoffset;
  magoffset.x = offset.mag.x;
  magoffset.y = offset.mag.y;
  magoffset.z = offset.mag.z;
  magoffset.r = offset.mag.r;
  success = success && imu.setMagOffset(magoffset);

  return success;
}

static bool isImuFullyCalibrated(const ImuData_t &data)
{
  return (3 == data.calibration.acc) && (3 == data.calibration.gyro) &&
         (3 == data.calibration.mag) && (3 == data.calibration.sys);
}

static bool receiveCalibrationFromDevice(BNO055 &imu, ImuOffset_t &offset)
{
  bool success = imu.setOperationMode(BNO055::OperationMode::CONFIG);

  BNO055::AccelOffset_t accoffset;
  success = success && imu.getAccOffset(accoffset);

  BNO055::GyroOffset_t gyrooffset;
  success = success && imu.getGyroOffset(gyrooffset);

  BNO055::MagOffset_t magoffset;
  success = success && imu.getMagOffset(magoffset);

  success = success && imu.setOperationMode(BNO055::OperationMode::NDOF);

  if (success) {
    offset.acc.x = accoffset.x;
    offset.acc.y = accoffset.y;
    offset.acc.z = accoffset.z;
    offset.acc.r = accoffset.r;
    offset.gyro.x = gyrooffset.x;
    offset.gyro.y = gyrooffset.y;
    offset.gyro.z = gyrooffset.z;
    offset.mag.x = magoffset.x;
    offset.mag.y = magoffset.y;
    offset.mag.z = magoffset.z;
    offset.mag.r = magoffset.r;
  }

  return success;
}

static void checkImuStatusAndResetIfNeeded(BNO055 &imu,
                                           ImuSystemStatus_t status)
{
  static uint32_t errorCounter = 0;

  errorCounter = (IMU_SYS_ERROR == status) ? (errorCounter + 1) : 0;

  if (4 < errorCounter) {
    bool success = resetAndInitializeImu(imu);
    saveLogAndResetInCaseOfError(!success);
  }
}

static ImuSystemStatus_t deviceStatusToImuStatus(BNO055::Status devstatus)
{
  static const ImuSystemStatus_t statusMap[] = {
      [BNO055::Status::SYS_IDLE] = IMU_SYS_IDLE,
      [BNO055::Status::SYS_ERROR] = IMU_SYS_ERROR,
      [BNO055::Status::SYS_PERIPHERAL_INIT] = IMU_SYS_PERIPHERAL_INIT,
      [BNO055::Status::SYS_INITIALIZING] = IMU_SYS_INITIALIZING,
      [BNO055::Status::SYS_RUNNING_SELFTEST] = IMU_SYS_RUNNING_SELFTEST,
      [BNO055::Status::SYS_RUNNING_FUSION] = IMU_SYS_RUNNING_FUSION,
      [BNO055::Status::SYS_RUNNING_NO_FUSION] = IMU_SYS_RUNNING_NO_FUSION,
      [BNO055::Status::SYS_UNKNOWN] = IMU_SYS_UNKNOWN,
  };

  if ((devstatus < 0) || (BNO055::Status::SYS_UNKNOWN < devstatus)) {
    devstatus = BNO055::Status::SYS_UNKNOWN;
  }

  return statusMap[devstatus];
}

static ImuClockSource_t
deviceClockSourceToImuClockSource(BNO055::ClockSource src)
{
  static const ImuClockSource_t sourceMap[] = {
      [BNO055::ClockSource::INT] = IMU_CLK_INTERNAL,
      [BNO055::ClockSource::EXT] = IMU_CLK_EXTERNAL,
      [BNO055::ClockSource::UNDEF] = IMU_CLK_UNKNOWN,
  };

  if ((src < 0) || (BNO055::ClockSource::UNDEF < src)) {
    src = BNO055::ClockSource::UNDEF;
  }

  return sourceMap[src];
}

static bool updateDeviceStatus(BNO055 &imu, ImuData_t &data)
{
  BNO055::Status devstatus = BNO055::Status::SYS_UNKNOWN;
  bool success = imu.getDeviceStatus(devstatus);
  if (success) {
    data.system.status = deviceStatusToImuStatus(devstatus);
  } else {
    data.system.status = IMU_SYS_UNKNOWN;
  }

  return success;
}

static bool updateClockSource(BNO055 &imu, ImuData_t &data)
{
  BNO055::ClockSource clk = BNO055::ClockSource::UNDEF;
  bool success = imu.getClockSource(clk);
  if (success) {
    data.system.clk = deviceClockSourceToImuClockSource(clk);
  } else {
    data.system.clk = IMU_CLK_UNKNOWN;
  }

  return success;
}

static bool updateEulerAngles(BNO055 &imu, ImuData_t &data)
{
  BNO055::Euler_t euler;
  bool success = imu.getEulerAngles(euler, BNO055::Unit::DEG);

  data.euler.yaw = success ? euler.h : 0;
  data.euler.pitch = success ? euler.p : 0;
  data.euler.roll = success ? euler.r : 0;

  return success;
}

static bool updateGravityVector(BNO055 &imu, ImuData_t &data)
{
  BNO055::Gravity_t gravity;
  bool success = imu.getGravity(gravity);

  data.gravity.x = success ? gravity.x : 0;
  data.gravity.y = success ? gravity.y : 0;
  data.gravity.z = success ? gravity.z : 0;

  return success;
}

static bool updateAccelerationVector(BNO055 &imu, ImuData_t &data)
{
  BNO055::LinearAccel_t acc;
  bool success = imu.getLinearAcceleration(acc);

  data.acceleration.x = success ? acc.x : 0;
  data.acceleration.y = success ? acc.y : 0;
  data.acceleration.z = success ? acc.z : 0;

  return success;
}

static bool updateAccCalibrationStatus(BNO055 &imu, ImuData_t &data)
{
  uint8_t byte = 0;
  bool success = imu.getAccCalibrationStatus(byte);

  data.calibration.acc = success ? byte : 0;

  return success;
}

static bool updateGyroCalibrationStatus(BNO055 &imu, ImuData_t &data)
{
  uint8_t byte = 0;
  bool success = imu.getGyroCalibrationStatus(byte);

  data.calibration.gyro = success ? byte : 0;

  return success;
}

static bool updateMagCalibrationStatus(BNO055 &imu, ImuData_t &data)
{
  uint8_t byte = 0;
  bool success = imu.getMagCalibrationStatus(byte);

  data.calibration.mag = success ? byte : 0;

  return success;
}

static bool updateSysCalibrationStatus(BNO055 &imu, ImuData_t &data)
{
  uint8_t byte = 0;
  bool success = imu.getSystemCalibrationStatus(byte);

  data.calibration.sys = success ? byte : 0;

  return success;
}

/*****************************************************************************/
/* DEFINITION OF GLOBAL FUNCTIONS                                            */
/*****************************************************************************/
void ImuManagerThread(void *p)
{
  BNO055 imu = BNO055(i2c_init, i2c_bus_read, i2c_bus_write, delay);

  bool success = resetAndInitializeImu(imu);
  saveLogAndResetInCaseOfError(!success);

  if (DbCfgImuCalibrationIsValid()) {
    ImuOffset_t offset;
    DbCfgImuCalibrationGet(&offset);
    success = sendCalibrationToDevice(imu, offset);
    saveLogAndResetInCaseOfError(!success);
    calibrationDataNeeded = false;
  } else {
    calibrationDataNeeded = true;
  }

  while (1) {
    xSemaphoreTake(readImu, portMAX_DELAY);

    ImuData_t data;
    memset(&data, 0, sizeof(data));

    bool success = updateDeviceStatus(imu, data);

    checkImuStatusAndResetIfNeeded(imu, data.system.status);

    success = success && updateClockSource(imu, data);
    success = success && updateEulerAngles(imu, data);
    success = success && updateAccCalibrationStatus(imu, data);
    success = success && updateGyroCalibrationStatus(imu, data);
    success = success && updateMagCalibrationStatus(imu, data);
    success = success && updateSysCalibrationStatus(imu, data);
    success = success && updateGravityVector(imu, data);
    success = success && updateAccelerationVector(imu, data);

    DbDataImuSet(&data);

    if (calibrationDataNeeded && isImuFullyCalibrated(data)) {
      ImuOffset_t offset;
      memset(&offset, 0, sizeof(offset));

      success = receiveCalibrationFromDevice(imu, offset);

      DbCfgImuCalibrationSet(&offset);
      DbCfgSaveToEeprom();

      calibrationDataNeeded = false;

      if (DbCfgImuCalibrationIsValid()) {
        Serial.println("IMU offsets updated in EEPROM");
      } else {
        Serial.println("Failed to update IMU offsets in EEPROM");
      }
    }
  }
}

void ImuManagerInit(void)
{
  readImu = xSemaphoreCreateBinary();
}

/****************************** END OF FILE ********************************/
