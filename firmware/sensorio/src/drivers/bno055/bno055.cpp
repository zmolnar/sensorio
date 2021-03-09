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
    delay(100);
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

bool BNO055::getDeviceStatus(BNO055::Status &status)
{
  u8 code = 0;
  error   = convertError(bno055_get_sys_stat_code(&code));

  if (OK == error) {
    status = static_cast<Status>(code);
  } else {
    status = BNO055::Status::SYS_UNKNOWN;
  }

  return OK == error;
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

bool BNO055::getClockSource(ClockSource &s)
{
  u8 src = 0;
  error  = convertError(bno055_get_clk_src(&src));

  if (OK == error) {
    s = static_cast<ClockSource>(src);
  } else {
    s = UNDEF;
  }

  return OK == error;
}

bool BNO055::setClockSource(ClockSource s)
{
  error = convertError(bno055_set_clk_src(static_cast<u8>(s)));
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

bool BNO055::getAccCalibrationStatus(u8 &status)
{
  error = convertError(bno055_get_accel_calib_stat(&status));
  return OK == error;  
}

bool BNO055::getGyroCalibrationStatus(u8 &status)
{
  error = convertError(bno055_get_gyro_calib_stat(&status));
  return OK == error;  
}

bool BNO055::getMagCalibrationStatus(u8 &status)
{
  error = convertError(bno055_get_mag_calib_stat(&status));
  return OK == error;  
}

bool BNO055::getSystemCalibrationStatus(u8 &status)
{
  error = convertError(bno055_get_sys_calib_stat(&status));
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
