/**
 * @file Dashboard.c
 * @brief
 */

#if !defined(SIMULATOR)
#error Use this file only in the simulator!
#endif

/*****************************************************************************/
/* INCLUDES                                                                  */
/*****************************************************************************/
#include "dashboard/Dashboard.h"

#include <stdio.h>
#include <string.h>

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
static SysParams_t default_config;

/*****************************************************************************/
/* DECLARATION OF LOCAL FUNCTIONS                                            */
/*****************************************************************************/

/*****************************************************************************/
/* DEFINITION OF LOCAL FUNCTIONS                                             */
/*****************************************************************************/

/*****************************************************************************/
/* DEFINITION OF GLOBAL FUNCTIONS                                            */
/*****************************************************************************/
void DbInit(void)
{
  printf("Simulator dashboard initialized\n");
  default_config.location.utcOffset = 1;
  default_config.screens.vario.chart_refresh_period = 1000;
}

void DbParamsLock(void)
{
}

void DbParamsUnlock(void)
{
}

void DbParamsGet(SysParams_t *p)
{
  memcpy(p, &default_config, sizeof(SysParams_t));
}

void DbParamsSet(SysParams_t *p)
{
  memcpy(&default_config, p, sizeof(SysParams_t));
}

void DbDataGpsGet(GpsData_t *p)
{
  p->locked          = true;
  p->altitude        = 1089;
  p->course          = 129;
  p->latitude        = 190;
  p->longitude       = 39;
  p->numOfSatellites = 9;
  p->speed           = 49;
  p->time.day        = 22;
  p->time.month      = 1;
  p->time.year       = 2021;
  p->time.hour       = 20;
  p->time.minute     = 51;
  p->time.second     = 10;
}

void DbDataGpsSet(GpsData_t *p)
{
}

void DbDataBpsGet(BpsData_t *p)
{
  static uint8_t offset = 0;

  memset(p, 0, sizeof(*p));
  p->cooked.pressure = 101325 + (offset++ % 20);
  p->cooked.temp     = 2450;
}

void DbDataBpsSet(BpsData_t *p)
{
}

void DbDataFilterOutputGet(FilterOutput_t *p)
{
  static uint32_t offset;
  if (1200 < offset) {
    offset = 0;
  } else {
    offset += 30;
  }

  p->vario.averaged        = 12.95;
  p->vario.instant         = -14.19;
  p->height.fieldElevation = -49;
  p->height.nautical       = 287 + offset;
  p->pressure              = 101325;
}

void DbDataFilterOutputSet(FilterOutput_t *p)
{
}

void DbDataImuGet(ImuData_t *p)
{
  memset(p, 0, sizeof(*p));
  p->calibration.acc = 2;
  p->calibration.mag = 3;
  p->calibration.gyro = 3;
  p->calibration.sys = 2;
}

void DbDataImuSet(ImuData_t *p)
{
}

void DbDataBatteryGet(Battery_t *p)
{
  p->voltage    = 4.08;
  p->percentage = 80;
  p->status     = BAT_CHARGED;
  p->value      = 2929;
}

void DbDataBatterySet(Battery_t *p)
{
}

void DbDataBoardGet(Board_t *p)
{
  p->usbConnected = true;
}

void DbDataBoardSet(Board_t *p)
{
}

/****************************** END OF FILE **********************************/
