/**
 * @file bno055.cpp
 * @brief
 */

/*****************************************************************************/
/* INCLUDES                                                                  */
/*****************************************************************************/
#include "bno055.h"

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

/*****************************************************************************/
/* DECLARATION OF LOCAL FUNCTIONS                                            */
/*****************************************************************************/

/*****************************************************************************/
/* DEFINITION OF LOCAL FUNCTIONS                                             */
/*****************************************************************************/

/*****************************************************************************/
/* DEFINITION OF GLOBAL FUNCTIONS                                            */
/*****************************************************************************/
bool BNO055::begin(void)
{
  bool success = false;

  if (init()) {
    error = convertError(bno055_init(&bno055));
    if (OK == error) {
      u8 chipId;
      error = convertError(bno055_read_chip_id(&chipId));
      if (OK == error) {
        success = (0xA0 == chipId);
      } else {
        Serial.println("failed to read the bno055 chip-id");
      }
    } else {
      Serial.println("bno055 init failed");
    }
  } else {
    Serial.println("bno055 TWI begin failed");
  }

  return success;
}

bool BNO055::setPowerMode(PowerMode mode)
{
  error = convertError(bno055_set_power_mode(static_cast<u8>(mode)));
  return OK == error;
}

bool BNO055::setOperationMode(OperationMode mode)
{
  error = convertError(bno055_set_operation_mode(static_cast<u8>(mode)));
  return OK == error;
}

BNO055::Error BNO055::getError(void)
{
  return error;
}

bool BNO055::getAcceleration(BNO055::Accel_t &data, Unit unit)
{
  switch (unit) {
  case MILLIG:
    error = convertError(bno055_convert_double_accel_xyz_mg(&data));
    break;
  case MSQ:
    error = convertError(bno055_convert_double_accel_xyz_msq(&data));
    break;
  default:
    error = UNIT_ERROR;
    break;
  }

  return OK == error;
}

bool BNO055::getGyro(Gyro_t &data, Unit unit)
{
  switch (unit) {
  case DPS:
    error = convertError(bno055_convert_double_gyro_xyz_dps(&data));
    break;
  case RPS:
    error = convertError(bno055_convert_double_gyro_xyz_rps(&data));
    break;
  default:
    error = UNIT_ERROR;
    break;
  }

  return OK == error;
}

bool BNO055::getMag(Mag_t &data)
{
  error = convertError(bno055_convert_double_mag_xyz_uT(&data));
  return OK == error;
}

bool BNO055::getEulerAngles(BNO055::Euler_t &data, Unit unit)
{
  switch (unit) {
  case DEG:
    error = convertError(bno055_convert_double_euler_hpr_deg(&data));
    break;
  case RAD:
    error = convertError(bno055_convert_double_euler_hpr_rad(&data));
    break;
  default:
    error = UNIT_ERROR;
    break;
  }

  return OK == error;
}

bool BNO055::getQuaternion(Quaternion_t &data)
{
  error = convertError(bno055_read_quaternion_wxyz(&data));
  return OK == error;
}

bool BNO055::getLinearAcceleration(LinearAccel_t &data)
{
  error = convertError(bno055_convert_double_linear_accel_xyz_msq(&data));
  return OK == error;
}

bool BNO055::getGravity(Gravity_t &data)
{
  error = convertError(bno055_convert_double_gravity_xyz_msq(&data));
  return OK == error;
}

bool BNO055::enableInterrupt(Interrupt irq)
{
  BNO055_RETURN_FUNCTION_TYPE err = BNO055_ERROR;

  u8 reg = 0;
  err    = bno055.bus_read(bno055.dev_addr, BNO055_INT_ADDR, &reg, sizeof(u8));

  if (BNO055_SUCCESS == err) {
    u8 mask = static_cast<u8>(irq);
    reg |= mask;
    err = bno055.bus_write(bno055.dev_addr, BNO055_INT_ADDR, &reg, sizeof(u8));
  }

  error = convertError(err);

  return OK == error;
}

bool BNO055::disableInterrupt(Interrupt irq)
{
  BNO055_RETURN_FUNCTION_TYPE err = BNO055_ERROR;

  u8 reg = 0;
  err    = bno055.bus_read(bno055.dev_addr, BNO055_INT_ADDR, &reg, sizeof(u8));

  if (BNO055_SUCCESS == err) {
    u8 mask = static_cast<u8>(irq);
    reg &= ~mask;
    err = bno055.bus_write(bno055.dev_addr, BNO055_INT_ADDR, &reg, sizeof(u8));
  }

  error = convertError(err);

  return OK == error;
}

bool BNO055::maskInterrupt(Interrupt irq)
{
  BNO055_RETURN_FUNCTION_TYPE err = BNO055_ERROR;

  u8 reg = 0;
  err    = bno055.bus_read(bno055.dev_addr, BNO055_INT_MASK_ADDR, &reg, sizeof(u8));

  if (BNO055_SUCCESS == err) {
    u8 mask = static_cast<u8>(irq);
    reg &= ~mask;
    err = bno055.bus_write(bno055.dev_addr, BNO055_INT_MASK_ADDR, &reg, sizeof(u8));
  }

  error = convertError(err);

  return OK == error;
}

bool BNO055::unmaskInterrupt(Interrupt irq)
{
  BNO055_RETURN_FUNCTION_TYPE err = BNO055_ERROR;

  u8 reg = 0;
  err    = bno055.bus_read(bno055.dev_addr, BNO055_INT_MASK_ADDR, &reg, sizeof(u8));

  if (BNO055_SUCCESS == err) {
    u8 mask = static_cast<u8>(irq);
    reg |= mask;
    err = bno055.bus_write(bno055.dev_addr, BNO055_INT_MASK_ADDR, &reg, sizeof(u8));
  }

  error = convertError(err);

  return OK == error;
}

bool BNO055::readInterruptStatus(u8 &status)
{
  BNO055_RETURN_FUNCTION_TYPE err = BNO055_ERROR;
  err = bno055.bus_read(bno055.dev_addr, BNO055_INTR_STAT_ADDR, &status, sizeof(u8));
  error = convertError(err);

  return OK == error;
}

BNO055::Error BNO055::convertError(BNO055_RETURN_FUNCTION_TYPE e)
{
  Error error;

  switch (e) {
  case BNO055_SUCCESS:
    error = OK;
    break;
  case BNO055_ERROR:
    error = ERROR;
    break;
  case BNO055_OUT_OF_RANGE:
    error = OUT_OF_RANGE;
    break;
  case BNO055_E_NULL_PTR:
    error = NULL_POINTER;
    break;
  default:
    error = UNKNOWN;
    break;
  }

  return error;
}

/****************************** END OF FILE **********************************/
