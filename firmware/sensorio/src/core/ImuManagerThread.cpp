/**
 * @file ImuManagerThread.cpp
 * @brief
 */

/*****************************************************************************/
/* INCLUDES                                                                  */
/*****************************************************************************/
#include "ImuManagerThread.h"

#include <driver/i2c.h>
#include <esp_log.h>
#include <freertos/FreeRTOS.h>
#include <freertos/semphr.h>

#include "dashboard/Dashboard.h"
#include "drivers/bno055/bno055.h"
#include "gui/Sensorio.h"

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
static const char *tag = "imu-thread";
SemaphoreHandle_t readImu;

static gpio_num_t i2c_gpio_sda = GPIO_NUM_17;
static gpio_num_t i2c_gpio_scl = GPIO_NUM_16;
static uint32_t i2c_frequency = 400000;
static i2c_port_t i2c_port = I2C_NUM_1;

static bool calibrationDataNeeded = false;

/*****************************************************************************/
/* DECLARATION OF LOCAL FUNCTIONS                                            */
/*****************************************************************************/

/*****************************************************************************/
/* DEFINITION OF LOCAL FUNCTIONS                                             */
/*****************************************************************************/
static inline bool installMasterDriver(i2c_port_t port)
{
  return ESP_OK != i2c_driver_install(port, I2C_MODE_MASTER, 0, 0, 0);
}

static inline bool paramConfig(i2c_port_t port, const i2c_config_t *conf)
{
  return ESP_OK != i2c_param_config(port, conf);
}

static inline uint8_t readAddress(uint8_t addr)
{
  return (addr << 1) | I2C_MASTER_READ;
}

static inline uint8_t writeAddress(uint8_t addr)
{
  return (addr << 1) | I2C_MASTER_WRITE;
}

static bool i2c_init(void)
{
  i2c_config_t conf = {.mode = I2C_MODE_MASTER,
                       .sda_io_num = i2c_gpio_sda,
                       .scl_io_num = i2c_gpio_scl,
                       .sda_pullup_en = GPIO_PULLUP_ENABLE,
                       .scl_pullup_en = GPIO_PULLUP_ENABLE,
                       .master = {
                           .clk_speed = i2c_frequency,
                       }};

  bool error = paramConfig(i2c_port, &conf);
  error = error || installMasterDriver(i2c_port);

  // Set timeout to 10 ms to mimic clock stretching.
  i2c_set_timeout(i2c_port, I2C_APB_CLK_FREQ * 10 / 1000);

  return error;
}

static s8 i2c_bus_write(u8 dev_addr, u8 reg_addr, u8 *reg_data, u8 cnt)
{
  configASSERT(1 == cnt);

  i2c_cmd_handle_t cmd = i2c_cmd_link_create();
  i2c_master_start(cmd);
  i2c_master_write_byte(cmd, writeAddress(dev_addr), true);
  i2c_master_write_byte(cmd, reg_addr, true);
  i2c_master_write(cmd, reg_data, cnt, true);
  i2c_master_stop(cmd);

  esp_err_t error = i2c_master_cmd_begin(i2c_port, cmd, portMAX_DELAY);
  i2c_cmd_link_delete(cmd);

  return ESP_OK == error ? BNO055_SUCCESS : BNO055_ERROR;
}

static s8 i2c_bus_read(u8 dev_addr, u8 reg_addr, u8 *reg_data, u8 cnt)
{  
  i2c_cmd_handle_t cmd = i2c_cmd_link_create();
  i2c_master_start(cmd);
  i2c_master_write_byte(cmd, writeAddress(dev_addr), true);
  i2c_master_write_byte(cmd, reg_addr, true);
  i2c_master_start(cmd);
  i2c_master_write_byte(cmd, readAddress(dev_addr), true);
  i2c_master_read(cmd, reg_data, cnt, I2C_MASTER_LAST_NACK);
  i2c_master_stop(cmd);

  esp_err_t error = i2c_master_cmd_begin(i2c_port, cmd, portMAX_DELAY);
  i2c_cmd_link_delete(cmd);

  return ESP_OK == error ? BNO055_SUCCESS : BNO055_ERROR;
}

static void delay(uint32_t ms)
{
  vTaskDelay(pdMS_TO_TICKS(ms));
}

static bool resetAndInitializeImu(BNO055 &imu)
{
  static const uint32_t maxNumOfRetries = 5;

  bool error = true;
  for (uint32_t i = 0; error && (i < maxNumOfRetries); ++i) {
    error = imu.start();
    error = error || imu.reset();
    error = error || imu.setPowerMode(BNO055::PowerMode::NORMAL);
    error = error || imu.setClockSource(BNO055::ClockSource::EXT);
    error = error || !imu.isExternalClockInUse();
    error = error || imu.setOperationMode(BNO055::OperationMode::NDOF);

    if (error) {
      ESP_LOGE(tag, "BNO055 device init failed #%d", (int)i);
      vTaskDelay(pdMS_TO_TICKS(500));
    }
  }

  return error;
}

static bool sendCalibrationToDevice(BNO055 &imu, ImuOffset_t &offset)
{
  BNO055::AccelOffset_t accoffset;
  accoffset.x = offset.acc.x;
  accoffset.y = offset.acc.y;
  accoffset.z = offset.acc.z;
  accoffset.r = offset.acc.r;
  bool error = imu.setAccOffset(accoffset);

  BNO055::GyroOffset_t gyrooffset;
  gyrooffset.x = offset.gyro.x;
  gyrooffset.y = offset.gyro.y;
  gyrooffset.z = offset.gyro.z;
  error = error || imu.setGyroOffset(gyrooffset);

  BNO055::MagOffset_t magoffset;
  magoffset.x = offset.mag.x;
  magoffset.y = offset.mag.y;
  magoffset.z = offset.mag.z;
  magoffset.r = offset.mag.r;
  error = error || imu.setMagOffset(magoffset);

  return error;
}

static bool isImuFullyCalibrated(const ImuData_t &data)
{
  return (3 == data.calibration.acc) && (3 == data.calibration.gyro) &&
         (3 == data.calibration.mag) && (3 == data.calibration.sys);
}

static bool receiveCalibrationFromDevice(BNO055 &imu, ImuOffset_t &offset)
{
  bool error = imu.setOperationMode(BNO055::OperationMode::CONFIG);

  BNO055::AccelOffset_t accoffset;
  error = error || imu.getAccOffset(accoffset);

  BNO055::GyroOffset_t gyrooffset;
  error = error || imu.getGyroOffset(gyrooffset);

  BNO055::MagOffset_t magoffset;
  error = error || imu.getMagOffset(magoffset);

  error = error || imu.setOperationMode(BNO055::OperationMode::NDOF);

  if (!error) {
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

  return error;
}

static void checkImuStatusAndResetIfNeeded(BNO055 &imu,
                                           ImuSystemStatus_t status)
{
  static uint32_t errorCounter = 0;

  errorCounter = (IMU_SYS_ERROR == status) ? (errorCounter + 1) : 0;

  if (4 < errorCounter) {
    bool error = resetAndInitializeImu(imu);
    configASSERT(!error);
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
  bool error = imu.getDeviceStatus(devstatus);
  if (error) {
    data.system.status = IMU_SYS_UNKNOWN;
  } else {
    data.system.status = deviceStatusToImuStatus(devstatus);
  }

  return error;
}

static bool updateClockSource(BNO055 &imu, ImuData_t &data)
{
  BNO055::ClockSource clk = BNO055::ClockSource::UNDEF;
  bool error = imu.getClockSource(clk);
  if (error) {
    data.system.clk = IMU_CLK_UNKNOWN;
  } else {
    data.system.clk = deviceClockSourceToImuClockSource(clk);
  }

  return error;
}

static bool updateEulerAngles(BNO055 &imu, ImuData_t &data)
{
  BNO055::Euler_t euler;
  bool error = imu.getEulerAngles(euler, BNO055::Unit::DEG);

  data.euler.yaw = error ? 0 : euler.h;
  data.euler.pitch = error ? 0 : euler.p;
  data.euler.roll = error ? 0 : euler.r;

  return error;
}

static bool updateGravityVector(BNO055 &imu, ImuData_t &data)
{
  BNO055::Gravity_t gravity;
  bool error = imu.getGravity(gravity);

  data.gravity.x = error ? 0 : gravity.x;
  data.gravity.y = error ? 0 : gravity.y;
  data.gravity.z = error ? 0 : gravity.z;

  return error;
}

static bool updateAccelerationVector(BNO055 &imu, ImuData_t &data)
{
  BNO055::LinearAccel_t acc;
  bool error = imu.getLinearAcceleration(acc);

  data.acceleration.x = error ? 0 : acc.x;
  data.acceleration.y = error ? 0 : acc.y;
  data.acceleration.z = error ? 0 : acc.z;

  return error;
}

static bool updateAccCalibrationStatus(BNO055 &imu, ImuData_t &data)
{
  uint8_t byte = 0;
  bool error = imu.getAccCalibrationStatus(byte);

  data.calibration.acc = error ? 0 : byte;

  return error;
}

static bool updateGyroCalibrationStatus(BNO055 &imu, ImuData_t &data)
{
  uint8_t byte = 0;
  bool error = imu.getGyroCalibrationStatus(byte);

  data.calibration.gyro = error ? 0 : byte;

  return error;
}

static bool updateMagCalibrationStatus(BNO055 &imu, ImuData_t &data)
{
  uint8_t byte = 0;
  bool error = imu.getMagCalibrationStatus(byte);

  data.calibration.mag = error ? 0 : byte;

  return error;
}

static bool updateSysCalibrationStatus(BNO055 &imu, ImuData_t &data)
{
  uint8_t byte = 0;
  bool error = imu.getSystemCalibrationStatus(byte);

  data.calibration.sys = error ? 0 : byte;

  return error;
}

/*****************************************************************************/
/* DEFINITION OF GLOBAL FUNCTIONS                                            */
/*****************************************************************************/
void ImuManagerThread(void *p)
{
  BNO055 imu = BNO055(i2c_init, i2c_bus_read, i2c_bus_write, delay);

  bool error = imu.init();
  error = error || resetAndInitializeImu(imu);
  configASSERT(!error);

  if (DbCfgImuCalibrationIsValid()) {
    ImuOffset_t offset;
    DbCfgImuCalibrationGet(&offset);
    error = sendCalibrationToDevice(imu, offset);
    configASSERT(!error);
    calibrationDataNeeded = false;
  } else {
    calibrationDataNeeded = true;
  }

  while (1) {
    xSemaphoreTake(readImu, portMAX_DELAY);

    ImuData_t data;
    memset(&data, 0, sizeof(data));

    error = updateDeviceStatus(imu, data);

    checkImuStatusAndResetIfNeeded(imu, data.system.status);

    error = error || updateClockSource(imu, data);
    error = error || updateEulerAngles(imu, data);
    error = error || updateAccCalibrationStatus(imu, data);
    error = error || updateGyroCalibrationStatus(imu, data);
    error = error || updateMagCalibrationStatus(imu, data);
    error = error || updateSysCalibrationStatus(imu, data);
    error = error || updateGravityVector(imu, data);
    error = error || updateAccelerationVector(imu, data);

    DbDataImuSet(&data);

    if ((!error) && calibrationDataNeeded && isImuFullyCalibrated(data)) {
      ImuOffset_t offset;
      memset(&offset, 0, sizeof(offset));

      error = receiveCalibrationFromDevice(imu, offset);

      DbCfgImuCalibrationSet(&offset);
      DbCfgSaveToEeprom();

      calibrationDataNeeded = false;

      if (DbCfgImuCalibrationIsValid()) {
        ESP_LOGI(tag, "IMU offsets updated in EEPROM");
      } else {
        ESP_LOGI(tag, "Failed to update IMU offsets in EEPROM");
      }
    }
  }
}

void ImuManagerThreadInit(void)
{
  readImu = xSemaphoreCreateBinary();
}

/****************************** END OF FILE ********************************/
