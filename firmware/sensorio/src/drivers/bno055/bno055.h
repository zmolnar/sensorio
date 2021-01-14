/**
 * @file Bno055.h
 * @brief
 */

#ifndef BNO055_H
#define BNO055_H

/*****************************************************************************/
/* INCLUDES                                                                  */
/*****************************************************************************/
extern "C"
{
#include "bst/bst_bno055.h"
}

/*****************************************************************************/
/* DEFINED CONSTANTS                                                         */
/*****************************************************************************/
#define BNO055_ADDRESS_COM3_LOW  0x28
#define BNO055_ADDRESS_COM3_HIGH 0x29

/*****************************************************************************/
/* MACRO DEFINITIONS                                                         */
/*****************************************************************************/

/*****************************************************************************/
/* TYPE DEFINITIONS                                                          */
/*****************************************************************************/
typedef bool (*bus_init_t)(void);
typedef s8 (*bus_read_t)(u8, u8, u8 *, u8);
typedef s8 (*bus_write_t)(u8, u8, u8 *, u8);
typedef void (*delay_t)(u32 msec);

class BNO055 {
public:
  enum Error {
    OK           = 0,
    ERROR        = 1,
    OUT_OF_RANGE = 2,
    NULL_POINTER = 3,
    UNIT_ERROR   = 4,
    UNKNOWN      = 5,
  };

  enum PowerMode {
    NORMAL   = BNO055_POWER_MODE_NORMAL,
    LOWPOWER = BNO055_POWER_MODE_LOWPOWER,
    SUSPEND  = BNO055_POWER_MODE_SUSPEND,
  };

  enum OperationMode {
    CONFIG       = BNO055_OPERATION_MODE_CONFIG,
    ACCONLY      = BNO055_OPERATION_MODE_ACCONLY,
    MAGONLY      = BNO055_OPERATION_MODE_MAGONLY,
    GYRONLY      = BNO055_OPERATION_MODE_GYRONLY,
    ACCMAG       = BNO055_OPERATION_MODE_ACCMAG,
    ACCGYRO      = BNO055_OPERATION_MODE_ACCGYRO,
    MAGGYRO      = BNO055_OPERATION_MODE_MAGGYRO,
    AMG          = BNO055_OPERATION_MODE_AMG,
    IMUPLUS      = BNO055_OPERATION_MODE_IMUPLUS,
    COMPASS      = BNO055_OPERATION_MODE_COMPASS,
    M4G          = BNO055_OPERATION_MODE_M4G,
    NDOF_FMC_OFF = BNO055_OPERATION_MODE_NDOF_FMC_OFF,
    NDOF         = BNO055_OPERATION_MODE_NDOF,
  };

  enum Status {
    SYS_IDLE              = 0,
    SYS_ERROR             = 1,
    SYS_PERIPHERAL_INIT   = 2,
    SYS_INITIALIZING      = 3,
    SYS_RUNNING_SELFTEST  = 4,
    SYS_RUNNING_FUSION    = 5,
    SYS_RUNNING_NO_FUSION = 6,
    SYS_UNKNOWN           = 7,
  };

  enum Unit {
    // Acceleration units
    MSQ,    // Meter per seconds square
    MILLIG, // Milli-G

    // Gyro units
    DPS, // Degree per second
    RPS, // Radian per second

    // Mag units
    UT, // Micro Tesla

    // Euler units
    DEG, // Degree
    RAD, // Radian

    // Temperature
    CELSIUS,    // Celsius
    FAHRENHEIT, // Fahrenheit
  };

  typedef enum {
    INT   = 0,
    EXT   = 1,
    UNDEF = 2,
  } ClockSource;

  typedef struct bno055_accel_double_t        Accel_t;
  typedef struct bno055_gyro_double_t         Gyro_t;
  typedef struct bno055_mag_double_t          Mag_t;
  typedef struct bno055_euler_double_t        Euler_t;
  typedef struct bno055_quaternion_t          Quaternion_t;
  typedef struct bno055_linear_accel_double_t LinearAccel_t;
  typedef struct bno055_gravity_double_t      Gravity_t;

  BNO055(bus_init_t  init,
         bus_read_t  rd,
         bus_write_t wr,
         delay_t     dl,
         int         addr = BNO055_ADDRESS_COM3_HIGH) :
      init(init), error(UNKNOWN)
  {
    bno055.bus_write  = wr;
    bno055.bus_read   = rd;
    bno055.delay_msec = dl;
    bno055.dev_addr   = addr;
  }
  ~BNO055()
  {
  }

  bool  begin(void);
  bool  getDeviceStatus(BNO055::Status &status);
  bool  setPowerMode(PowerMode mode);
  bool  setOperationMode(OperationMode mode);
  bool  getClockSource(ClockSource &s);
  bool  setClockSource(ClockSource s);
  Error getError(void);
  bool  getAcceleration(Accel_t &data, Unit unit = MILLIG);
  bool  getGyro(Gyro_t &data, Unit unit = DPS);
  bool  getMag(Mag_t &data);
  bool  getEulerAngles(Euler_t &data, Unit unit = DEG);
  bool  getQuaternion(Quaternion_t &data);
  bool  getLinearAcceleration(LinearAccel_t &data);
  bool  getGravity(Gravity_t &data);

private:
  bus_init_t init;
  bno055_t   bno055;
  Error      error;

  Error convertError(BNO055_RETURN_FUNCTION_TYPE e);
};

/*****************************************************************************/
/* DECLARATION OF GLOBAL VARIABLES                                           */
/*****************************************************************************/

/*****************************************************************************/
/* DECLARATION OF GLOBAL FUNCTIONS                                           */
/*****************************************************************************/

#endif /* BNO055_H */

/****************************** END OF FILE **********************************/
