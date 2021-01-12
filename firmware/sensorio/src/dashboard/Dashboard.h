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

  // GPS section
  void DbDataGpsLock(void);
  void DbDataGpsUnlock(void);
  void DbDataGpsGet(GpsData_t *p);
  void DbDataGpsSet(GpsData_t *p);

#ifdef __cplusplus
}
#endif

#endif /* DASHBOARD_H */

/****************************** END OF FILE **********************************/
