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

#include <core/ImuManagerThread.hpp>

#include <driver/i2c.h>
#include <freertos/FreeRTOS.h>
#include <freertos/semphr.h>

#include <dashboard/Config.hpp>
#include <dashboard/Dashboard.hpp>
#include <drivers/bno055/bno055.h>
#include <gui/Sensorio.h>
#include <platform/Log.hpp>

using namespace Dashboard;
using namespace Platform;

static const char *tag = "imu-thread";

static constexpr auto I2C_SDA{GPIO_NUM_17};
static constexpr auto I2C_SCL{GPIO_NUM_16};
static constexpr uint32_t I2C_FREQ{400000};
static constexpr auto I2C_PORT{I2C_NUM_1};

static inline bool installMasterDriver(i2c_port_t port) {
  return ESP_OK != i2c_driver_install(port, I2C_MODE_MASTER, 0, 0, 0);
}

static inline bool paramConfig(i2c_port_t port, const i2c_config_t *conf) {
  return ESP_OK != i2c_param_config(port, conf);
}

static inline uint8_t readAddress(uint8_t addr) {
  return (addr << 1) | I2C_MASTER_READ;
}

static inline uint8_t writeAddress(uint8_t addr) {
  return (addr << 1) | I2C_MASTER_WRITE;
}

static bool i2c_init(void) {
  i2c_config_t conf = {.mode = I2C_MODE_MASTER,
                       .sda_io_num = I2C_SDA,
                       .scl_io_num = I2C_SCL,
                       .sda_pullup_en = GPIO_PULLUP_ENABLE,
                       .scl_pullup_en = GPIO_PULLUP_ENABLE,
                       .master = {
                           .clk_speed = I2C_FREQ,
                       }};

  bool error = paramConfig(I2C_PORT, &conf);
  error = error || installMasterDriver(I2C_PORT);

  // Set timeout to 10 ms to mimic clock stretching.
  i2c_set_timeout(I2C_PORT, I2C_APB_CLK_FREQ * 10 / 1000);

  return error;
}

static s8 i2c_bus_write(u8 dev_addr, u8 reg_addr, u8 *reg_data, u8 cnt) {
  configASSERT(1 == cnt);

  i2c_cmd_handle_t cmd = i2c_cmd_link_create();
  i2c_master_start(cmd);
  i2c_master_write_byte(cmd, writeAddress(dev_addr), true);
  i2c_master_write_byte(cmd, reg_addr, true);
  i2c_master_write(cmd, reg_data, cnt, true);
  i2c_master_stop(cmd);

  esp_err_t error = i2c_master_cmd_begin(I2C_PORT, cmd, portMAX_DELAY);
  i2c_cmd_link_delete(cmd);

  return ESP_OK == error ? BNO055_SUCCESS : BNO055_ERROR;
}

static s8 i2c_bus_read(u8 dev_addr, u8 reg_addr, u8 *reg_data, u8 cnt) {
  i2c_cmd_handle_t cmd = i2c_cmd_link_create();
  i2c_master_start(cmd);
  i2c_master_write_byte(cmd, writeAddress(dev_addr), true);
  i2c_master_write_byte(cmd, reg_addr, true);
  i2c_master_start(cmd);
  i2c_master_write_byte(cmd, readAddress(dev_addr), true);
  i2c_master_read(cmd, reg_data, cnt, I2C_MASTER_LAST_NACK);
  i2c_master_stop(cmd);

  esp_err_t error = i2c_master_cmd_begin(I2C_PORT, cmd, portMAX_DELAY);
  i2c_cmd_link_delete(cmd);

  return ESP_OK == error ? BNO055_SUCCESS : BNO055_ERROR;
}

static void delay(uint32_t ms) {
  vTaskDelay(pdMS_TO_TICKS(ms));
}

static bool startAndResetImu(BNO055 &imu) {
  bool error = true;
  error = imu.start();
  error = error || imu.reset();
  error = error || imu.setPowerMode(BNO055::PowerMode::NORMAL);
  error = error || imu.setClockSource(BNO055::ClockSource::EXT);
  error = error || !imu.isExternalClockInUse();
  error = error || imu.setOperationMode(BNO055::OperationMode::NDOF);

  return error;
}

static bool sendCalibrationToImu(BNO055 &imu, Config::Imu::Offset &offset) {
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

static bool isImuFullyCalibrated(const Dashboard::Imu &data) {
  return (3 == data.calibration.acc) && (3 == data.calibration.gyro) &&
         (3 == data.calibration.mag) && (3 == data.calibration.sys);
}

static bool receiveCalibrationFromImu(BNO055 &imu,
                                      Config::Imu::Offset &offset) {
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
                                           Dashboard::Imu::Status status) {
  static uint32_t errorCounter = 0;

  errorCounter =
      (Dashboard::Imu::Status::ERROR == status) ? (errorCounter + 1) : 0;

  if (4 < errorCounter) {
    bool error = startAndResetImu(imu);
    configASSERT(!error);
  }
}

static Imu::Status deviceStatusToImuStatus(BNO055::Status devstatus) {
  static const Imu::Status statusMap[] = {
      [BNO055::Status::SYS_IDLE] = Imu::Status::IDLE,
      [BNO055::Status::SYS_ERROR] = Imu::Status::ERROR,
      [BNO055::Status::SYS_PERIPHERAL_INIT] = Imu::Status::PERIPHERAL_INIT,
      [BNO055::Status::SYS_INITIALIZING] = Imu::Status::INITIALIZING,
      [BNO055::Status::SYS_RUNNING_SELFTEST] = Imu::Status::RUNNING_SELFTEST,
      [BNO055::Status::SYS_RUNNING_FUSION] = Imu::Status::RUNNING_FUSION,
      [BNO055::Status::SYS_RUNNING_NO_FUSION] = Imu::Status::RUNNING_NO_FUSION,
      [BNO055::Status::SYS_UNKNOWN] = Imu::Status::UNKNOWN,
  };

  if ((devstatus < 0) || (BNO055::Status::SYS_UNKNOWN < devstatus)) {
    devstatus = BNO055::Status::SYS_UNKNOWN;
  }

  return statusMap[devstatus];
}

static Imu::ClockSource deviceClkSrcToImuClkSrc(BNO055::ClockSource src) {
  static const Imu::ClockSource sourceMap[] = {
      [BNO055::ClockSource::INT] = Imu::ClockSource::INTERNAL,
      [BNO055::ClockSource::EXT] = Imu::ClockSource::EXTERNAL,
      [BNO055::ClockSource::UNDEF] = Imu::ClockSource::UNKNOWN,
  };

  if ((src < 0) || (BNO055::ClockSource::UNDEF < src)) {
    src = BNO055::ClockSource::UNDEF;
  }

  return sourceMap[src];
}

static bool updateDeviceStatus(BNO055 &imu, Dashboard::Imu &data) {
  BNO055::Status devstatus = BNO055::Status::SYS_UNKNOWN;
  bool error{imu.getDeviceStatus(devstatus)};
  data.system.status = deviceStatusToImuStatus(devstatus);

  return error;
}

static bool updateClockSource(BNO055 &imu, Dashboard::Imu &data) {
  BNO055::ClockSource clk = BNO055::ClockSource::UNDEF;
  bool error{imu.getClockSource(clk)};
  data.system.clock = deviceClkSrcToImuClkSrc(clk);

  return error;
}

static bool updateEulerAngles(BNO055 &imu, Dashboard::Imu &data) {
  BNO055::Euler_t euler;
  bool error = imu.getEulerAngles(euler, BNO055::Unit::DEG);

  data.euler.yaw = error ? 0 : euler.h;
  data.euler.pitch = error ? 0 : euler.p;
  data.euler.roll = error ? 0 : euler.r;

  return error;
}

static bool updateGravityVector(BNO055 &imu, Dashboard::Imu &data) {
  BNO055::Gravity_t gravity;
  bool error = imu.getGravity(gravity);

  data.gravity.x = error ? 0 : gravity.x;
  data.gravity.y = error ? 0 : gravity.y;
  data.gravity.z = error ? 0 : gravity.z;

  return error;
}

static bool updateAccelerationVector(BNO055 &imu, Dashboard::Imu &data) {
  BNO055::LinearAccel_t acc;
  bool error = imu.getLinearAcceleration(acc);

  data.acceleration.x = error ? 0 : acc.x;
  data.acceleration.y = error ? 0 : acc.y;
  data.acceleration.z = error ? 0 : acc.z;

  return error;
}

static bool updateAccCalibrationStatus(BNO055 &imu, Dashboard::Imu &data) {
  uint8_t byte = 0;
  bool error = imu.getAccCalibrationStatus(byte);

  data.calibration.acc = error ? 0 : byte;

  return error;
}

static bool updateGyroCalibrationStatus(BNO055 &imu, Dashboard::Imu &data) {
  uint8_t byte = 0;
  bool error = imu.getGyroCalibrationStatus(byte);

  data.calibration.gyro = error ? 0 : byte;

  return error;
}

static bool updateMagCalibrationStatus(BNO055 &imu, Dashboard::Imu &data) {
  uint8_t byte = 0;
  bool error = imu.getMagCalibrationStatus(byte);

  data.calibration.mag = error ? 0 : byte;

  return error;
}

static bool updateSysCalibrationStatus(BNO055 &imu, Dashboard::Imu &data) {
  uint8_t byte = 0;
  bool error = imu.getSystemCalibrationStatus(byte);

  data.calibration.sys = error ? 0 : byte;

  return error;
}

static SemaphoreHandle_t readImu;
static bool calibrationDataNeeded{false};

void ImuManagerThread(void *p) {
  BNO055 imu = BNO055(i2c_init, i2c_bus_read, i2c_bus_write, delay);

  bool error = imu.init();
  Assert::Assert(!error);

  while ((error = startAndResetImu(imu))) {
    Log::Error(tag) << "failed to start IMU";
    vTaskDelay(pdMS_TO_TICKS(1000));
  }

  Config::Imu imucfg{config.imu.get()};

  if (imucfg.isValid()) {
    error = sendCalibrationToImu(imu, imucfg.offset);
    configASSERT(!error);
    Log::Info(tag) << "IMU calibration values loaded";
    calibrationDataNeeded = false;
  } else {
    Log::Error(tag) << "IMU calibration values not found";
    calibrationDataNeeded = true;
  }

  while (1) {
    xSemaphoreTake(readImu, portMAX_DELAY);

    Dashboard::Imu data{};

    error = updateDeviceStatus(imu, data);

    while (error || (Imu::Status::ERROR == data.system.status)) {
      dashboard.imu.set(data);
      while ((error = startAndResetImu(imu))) {
        Log::Error(tag) << "failed to restart IMU, status is";
        vTaskDelay(pdMS_TO_TICKS(1000));
      }
      error = error || updateDeviceStatus(imu, data);
    }

    checkImuStatusAndResetIfNeeded(imu, data.system.status);

    error = error || updateClockSource(imu, data);
    error = error || updateEulerAngles(imu, data);
    error = error || updateAccCalibrationStatus(imu, data);
    error = error || updateGyroCalibrationStatus(imu, data);
    error = error || updateMagCalibrationStatus(imu, data);
    error = error || updateSysCalibrationStatus(imu, data);
    error = error || updateGravityVector(imu, data);
    error = error || updateAccelerationVector(imu, data);

    dashboard.imu.set(data);

    if ((!error) && calibrationDataNeeded && isImuFullyCalibrated(data)) {
      Config::Imu imucfg{};
      if (!receiveCalibrationFromImu(imu, imucfg.offset)) {
        config.imu.set(imucfg);
        config.save();
        calibrationDataNeeded = false;
      }

      imucfg = config.imu.get();
      if (imucfg.isValid()) {
        Log::Info(tag) << "IMU offsets updated in EEPROM";
      } else {
        Log::Error(tag) << "Failed to update IMU offsets in EEPROM";
      }
    }
  }
}

void ImuManagerThreadInit(void) {
  readImu = xSemaphoreCreateBinary();
}

void SampleImu(void) {
  BaseType_t xHigherPriorityTaskWoken = pdFALSE;
  xSemaphoreGiveFromISR(readImu, &xHigherPriorityTaskWoken);
  if (pdTRUE == xHigherPriorityTaskWoken) {
    portYIELD_FROM_ISR();
  }
}
