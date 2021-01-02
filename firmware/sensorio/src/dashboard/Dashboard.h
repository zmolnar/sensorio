/**
 * @file Dashboard.h
 * @brief
 */

#ifndef DASHBOARD_H
#define DASHBOARD_H

/*****************************************************************************/
/* INCLUDES                                                                  */
/*****************************************************************************/
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/*****************************************************************************/
/* DEFINED CONSTANTS                                                         */
/*****************************************************************************/

/*****************************************************************************/
/* MACRO DEFINITIONS                                                         */
/*****************************************************************************/

/*****************************************************************************/
/* TYPE DEFINITIONS                                                          */
/*****************************************************************************/

/*****************************************************************************/
/* DECLARATION OF GLOBAL VARIABLES                                           */
/*****************************************************************************/

/*****************************************************************************/
/* DECLARATION OF GLOBAL FUNCTIONS                                           */
/*****************************************************************************/
#ifdef __cplusplus
extern "C" {
#endif

void DbInit(void);

// GPS section
void DbDataGpsLock(void);
void DbDataGpsUnlock(void);

void DbDataGpsSetLocked(bool state);
void DbDataGpsSetAltitude(uint32_t altitude);
void DbDataGpsSetCourse(uint32_t course);
void DbDataGpsSetLatitude(uint32_t latitude);
void DbDataGpsSetLongitude(uint32_t longitude);
void DbDataGpsSetNumOfSatellites(uint32_t sats);
void DbDataGpsSetSpeed(uint32_t speed);
void DbDataGpsSetYear(uint32_t year);
void DbDataGpsSetMonth(uint8_t month);
void DbDataGpsSetDay(uint8_t day);
void DbDataGpsSetHour(uint8_t hour);
void DbDataGpsSetMinute(uint8_t minute);
void DbDataGpsSetSecond(uint8_t sec);

bool DbDataGpsGetLocked(void);
uint32_t DbDataGpsGetAltitude(void);
uint32_t DbDataGpsGetCourse(void);
uint32_t DbDataGpsGetLatitude(void);
uint32_t DbDataGpsGetLongitude(void);
uint32_t DbDataGpsGetnumOfSatellites(void);
uint32_t DbDataGpsGetSpeed(void);
uint32_t DbDataGpsGetYear(void);
uint8_t DbDataGpsGetMonth(void);
uint8_t DbDataGpsGetDay(void);
uint8_t DbDataGpsGetHour(void);
uint8_t DbDataGpsGetMinute(void);
uint8_t DbDataGpsGetSecond(void);
#ifdef __cplusplus
}
#endif

#endif /* DASHBOARD_H */

/****************************** END OF FILE **********************************/
