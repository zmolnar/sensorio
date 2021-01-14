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

/*****************************************************************************/
/* DEFINITION OF GLOBAL FUNCTIONS                                            */
/*****************************************************************************/
void ImuManagerThread(void *p)
{
  imuTask = xTaskGetCurrentTaskHandle();

  BNO055 bno055 = BNO055(i2c_init, i2c_bus_read, i2c_bus_write, delay);

  bool success = false;
  while (!success) {
    if (bno055.begin()) {
      // Switch power mode to NORMAL
      if (bno055.setPowerMode(BNO055::PowerMode::NORMAL)) {
        // Enable to use the external crystal
        if (bno055.setClockSource(BNO055::ClockSource::EXT)) {
          delay(1000);
          BNO055::ClockSource clk = BNO055::ClockSource::UNDEF;

          // Check if the crystal started properly
          if (bno055.getClockSource(clk)) {
            if (BNO055::ClockSource::EXT == clk) {
              // Start embedded firmware
              if (bno055.setOperationMode(BNO055::OperationMode::NDOF)) {
                Serial.println("BNO055 is ready");
                success = true;
              } else {
                Serial.println("Failed to set BNO055 operation mode");
              }
            } else {
              Serial.println("Failed to start external crystal");
            }
          } else {
            Serial.println("Failed to get external clock source");
          }
        } else {
          Serial.println("Failed to enable external crystal");
        }
      } else {
        Serial.println("Failed to set BNO005 power mode");
      }
    } else {
      Serial.println("Failed to initialize BNO055");
    }

    if (!success) {
      delay(1000);
    }
  }

  while (1) {
    uint32_t notification = ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
    if (1 == notification) {
      ImuData_t data;
      memset(&data, 0, sizeof(data));

      BNO055::Status status = BNO055::Status::SYS_UNKNOWN;
      bno055.getDeviceStatus(status);
      switch(status) {
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

      // TODO calculate vertical acceleration

      // TODO update calibration status

      DbDataImuSet(&data);

#if 0
        Serial.printf("%f/%f/%f\r\n", euler.h, euler.p, euler.r);
#endif

    } else {
      Serial.println("IMU task notification failed");
    }
  }
}

/****************************** END OF FILE **********************************/
