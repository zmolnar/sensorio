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
}

void DbSaveConfig(void)
{
  printf("Simulator config saved\n");
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
  p->vario.averaged        = 2.95;
  p->vario.instant         = 4.19;
  p->height.fieldElevation = -49;
  p->height.nautical       = 287;
  p->pressure              = 101325;
}

void DbDataFilterOutputSet(FilterOutput_t *p)
{
}

void DbDataImuGet(ImuData_t *p)
{
  memset(p, 0, sizeof(*p));
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
