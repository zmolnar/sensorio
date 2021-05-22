/**
 * @file bno055.cpp
 * @brief
 */

/*****************************************************************************/
/* INCLUDES                                                                  */
/*****************************************************************************/
#include "bno055.h"
#include <esp_log.h>

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
bool BNO055::init(void)
{
  return bus_init();
}

bool BNO055::start(void)
{
  return OK != convertError(bno055_init(&bno055));
}

bool BNO055::reset(void)
{
  errorCode = convertError(bno055_set_sys_rst(1));
  if (OK == errorCode) {
    bno055.delay_msec(1000);
  }

  return OK != errorCode;
}

bool BNO055::getDeviceStatus(BNO055::Status &status)
{
  u8 code = 0;
  errorCode = convertError(bno055_get_sys_stat_code(&code));

  if (OK == errorCode) {
    status = static_cast<Status>(code);
  } else {
    status = BNO055::Status::SYS_UNKNOWN;
  }

  return OK != errorCode;
}

bool BNO055::setPowerMode(PowerMode mode)
{
  errorCode = convertError(bno055_set_power_mode(static_cast<u8>(mode)));
  return OK != errorCode;
}

bool BNO055::setOperationMode(OperationMode mode)
{
  errorCode = convertError(bno055_set_operation_mode(static_cast<u8>(mode)));

  if (OK == errorCode) {
    // Refer to the datasheet at page 22.
    bno055.delay_msec(30);
  }

  return OK != errorCode;
}

bool BNO055::getClockSource(ClockSource &s)
{
  u8 src = 0;
  errorCode = convertError(bno055_get_clk_src(&src));

  if (OK == errorCode) {
    s = static_cast<ClockSource>(src);
  } else {
    s = UNDEF;
  }

  return OK != errorCode;
}

bool BNO055::setClockSource(ClockSource s)
{
  errorCode = convertError(bno055_set_clk_src(static_cast<u8>(s)));
  if (OK == errorCode) {
    bno055.delay_msec(1000);
  }
  return OK != errorCode;
}

bool BNO055::isExternalClockInUse(void)
{
  BNO055::ClockSource clk = BNO055::ClockSource::UNDEF;

  bool error = getClockSource(clk);

  return error || (BNO055::ClockSource::EXT == clk);
}

BNO055::Error BNO055::getError(void)
{
  return errorCode;
}

bool BNO055::getAcceleration(BNO055::Accel_t &data, Unit unit)
{
  switch (unit) {
  case MILLIG:
    errorCode = convertError(bno055_convert_double_accel_xyz_mg(&data));
    break;
  case MSQ:
    errorCode = convertError(bno055_convert_double_accel_xyz_msq(&data));
    break;
  default:
    errorCode = UNIT_ERROR;
    break;
  }

  return OK != errorCode;
}

bool BNO055::getGyro(Gyro_t &data, Unit unit)
{
  switch (unit) {
  case DPS:
    errorCode = convertError(bno055_convert_double_gyro_xyz_dps(&data));
    break;
  case RPS:
    errorCode = convertError(bno055_convert_double_gyro_xyz_rps(&data));
    break;
  default:
    errorCode = UNIT_ERROR;
    break;
  }

  return OK != errorCode;
}

bool BNO055::getMag(Mag_t &data)
{
  errorCode = convertError(bno055_convert_double_mag_xyz_uT(&data));
  return OK != errorCode;
}

bool BNO055::getEulerAngles(BNO055::Euler_t &data, Unit unit)
{
  switch (unit) {
  case DEG:
    errorCode = convertError(bno055_convert_double_euler_hpr_deg(&data));
    break;
  case RAD:
    errorCode = convertError(bno055_convert_double_euler_hpr_rad(&data));
    break;
  default:
    errorCode = UNIT_ERROR;
    break;
  }

  return OK != errorCode;
}

bool BNO055::getQuaternion(Quaternion_t &data)
{
  errorCode = convertError(bno055_read_quaternion_wxyz(&data));
  return OK != errorCode;
}

bool BNO055::getLinearAcceleration(LinearAccel_t &data)
{
  errorCode = convertError(bno055_convert_double_linear_accel_xyz_msq(&data));
  return OK != errorCode;
}

bool BNO055::getGravity(Gravity_t &data)
{
  errorCode = convertError(bno055_convert_double_gravity_xyz_msq(&data));
  return OK != errorCode;
}

bool BNO055::getAccCalibrationStatus(u8 &status)
{
  errorCode = convertError(bno055_get_accel_calib_stat(&status));
  return OK != errorCode;
}

bool BNO055::getGyroCalibrationStatus(u8 &status)
{
  errorCode = convertError(bno055_get_gyro_calib_stat(&status));
  return OK != errorCode;
}

bool BNO055::getMagCalibrationStatus(u8 &status)
{
  errorCode = convertError(bno055_get_mag_calib_stat(&status));
  return OK != errorCode;
}

bool BNO055::getSystemCalibrationStatus(u8 &status)
{
  errorCode = convertError(bno055_get_sys_calib_stat(&status));
  return OK != errorCode;
}

bool BNO055::setAccOffset(AccelOffset_t &offset)
{
  errorCode = convertError(bno055_write_accel_offset(&offset));
  return OK != errorCode;
}

bool BNO055::getAccOffset(AccelOffset_t &offset)
{
  errorCode = convertError(bno055_read_accel_offset(&offset));
  return OK != errorCode;
}

bool BNO055::setGyroOffset(GyroOffset_t &offset)
{
  errorCode = convertError(bno055_write_gyro_offset(&offset));
  return OK != errorCode;
}

bool BNO055::getGyroOffset(GyroOffset_t &offset)
{
  errorCode = convertError(bno055_read_gyro_offset(&offset));
  return OK != errorCode;
}

bool BNO055::setMagOffset(MagOffset_t &offset)
{
  errorCode = convertError(bno055_write_mag_offset(&offset));
  return OK != errorCode;
}

bool BNO055::getMagOffset(MagOffset_t &offset)
{
  errorCode = convertError(bno055_read_mag_offset(&offset));
  return OK != errorCode;
}

BNO055::Error BNO055::convertError(BNO055_RETURN_FUNCTION_TYPE e)
{
  Error errorCode;

  switch (e) {
  case BNO055_SUCCESS:
    errorCode = OK;
    break;
  case BNO055_ERROR:
    errorCode = ERROR;
    break;
  case BNO055_OUT_OF_RANGE:
    errorCode = OUT_OF_RANGE;
    break;
  case BNO055_E_NULL_PTR:
    errorCode = NULL_POINTER;
    break;
  default:
    errorCode = UNKNOWN;
    break;
  }

  return errorCode;
}

/****************************** END OF FILE **********************************/
