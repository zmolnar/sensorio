/**
 * @file Dashboard.h
 * @brief
 */

#ifndef DASHBOARD_H
#define DASHBOARD_H

/*****************************************************************************/
/* INCLUDES                                                                  */
/*****************************************************************************/
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/*****************************************************************************/
/* DEFINED CONSTANTS                                                         */
/*****************************************************************************/

/*****************************************************************************/
/* MACRO DEFINITIONS                                                         */
/*****************************************************************************/

/*****************************************************************************/
/* TYPE DEFINITIONS                                                          */
/*****************************************************************************/
typedef struct GpsData_s {
  bool     locked;
  uint32_t altitude;
  uint32_t course;
  uint32_t latitude;
  uint32_t longitude;
  uint32_t numOfSatellites;
  uint32_t speed;
  struct Time_s {
    uint32_t year;
    uint8_t  month;
    uint8_t  day;
    uint8_t  hour;
    uint8_t  minute;
    uint8_t  second;
  } time;
} GpsData_t;

typedef struct BpsData_s {
  struct {
    uint32_t pressure;
    uint32_t temp;
  } raw;
  struct {
    uint32_t pressure;
    uint32_t temp;
  } cooked;
} BpsData_t;

typedef enum {
  IMU_SYS_IDLE              = 0,
  IMU_SYS_ERROR             = 1,
  IMU_SYS_PERIPHERAL_INIT   = 2,
  IMU_SYS_INITIALIZING      = 3,
  IMU_SYS_RUNNING_SELFTEST  = 4,
  IMU_SYS_RUNNING_FUSION    = 5,
  IMU_SYS_RUNNING_NO_FUSION = 6,
  IMU_SYS_UNKNOWN           = 7,
} ImuSystemStatus_t;

typedef enum {
  IMU_CLK_INTERNAL,
  IMU_CLK_EXTERNAL,
  IMU_CLK_UNKNOWN,
} ImuClockSource_t;

typedef struct ImuData_s {
  struct {
    ImuSystemStatus_t status;
    ImuClockSource_t  clk;
  } system;
  struct {
    double yaw;
    double pitch;
    double roll;
  } euler;
  struct {
    double x;
    double y;
    double z;
  } gravity;
  struct {
    double x;
    double y;
    double z;
  } acceleration;
} ImuData_t;

typedef enum {
  BAT_DISCHARGE,
  BAT_CHARGE,
  BAT_CHARGE_FINISHED,
  BAT_INVALID,
} BatteryStatus_t;

typedef struct Battery_s {
  BatteryStatus_t status;
  double voltage;
  uint32_t percentage;
  uint32_t value;
} Battery_t;

/*****************************************************************************/
/* DECLARATION OF GLOBAL VARIABLES                                           */
/*****************************************************************************/

/*****************************************************************************/
/* DECLARATION OF GLOBAL FUNCTIONS                                           */
/*****************************************************************************/
#ifdef __cplusplus
extern "C"
{
#endif

  void DbInit(void);
  void DbSaveConfig(void);

  void DbDataGpsGet(GpsData_t *p);
  void DbDataGpsSet(GpsData_t *p);

  void DbDataBpsGet(BpsData_t *p);
  void DbDataBpsSet(BpsData_t *p);

  void DbDataImuGet(ImuData_t *p);
  void DbDataImuSet(ImuData_t *p);

  void DbDataBatteryGet(Battery_t *p);
  void DbDataBatterySet(Battery_t *p);

#ifdef __cplusplus
}
#endif

#endif /* DASHBOARD_H */

/****************************** END OF FILE **********************************/
