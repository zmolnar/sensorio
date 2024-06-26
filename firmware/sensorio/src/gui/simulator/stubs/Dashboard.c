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
  default_config.location.utcOffset                 = 1;
  default_config.screens.vario.chart_refresh_period = 1000;
}

void DbCfgSysParamsGet(SysParams_t *p)
{
  memcpy(p, &default_config, sizeof(SysParams_t));
}

void DbCfgSysParamsSet(SysParams_t *p)
{
  memcpy(&default_config, p, sizeof(SysParams_t));
}

bool DbCfgImuCalibrationIsValid(void)
{
  return true;
}

void DbCfgImuCalibrationGet(ImuOffset_t *offset)
{
  memset(offset, 0, sizeof(ImuOffset_t));
}

void DbCfgImuCalibrationSet(ImuOffset_t *offset)
{
}

static VolumeLevel_t volume = VOL_MEDIUM;

void DbCfgBeepSettingsGet(BeepSettings_t *beep)
{
  beep->volume = volume;
}

void DbCfgBeepSettingsSet(BeepSettings_t *beep)
{
  volume = beep->volume;
}

void DbCfgSaveToEeprom(void)
{
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

void DbDataLocalTimeGet(LocalTime_t *p)
{
  p->year = 2021;
  p->month = 6;
  p->day = 1;
  p->hour = 8;
  p->minute = 29;
  p->second = 54;
}

void DbDataFilterOutputGet(FilterOutput_t *p)
{
  static uint32_t offset;
  if (1200 < offset) {
    offset = 0;
  } else {
    offset += 30;
  }

  p->vario.averaged = 12.95;
  p->vario.instant  = -14.19;
  p->height         = 1014.6 + offset;
}

void DbDataFilterOutputSet(FilterOutput_t *p)
{
}

void DbDataImuGet(ImuData_t *p)
{
  memset(p, 0, sizeof(*p));
  p->calibration.acc  = 2;
  p->calibration.mag  = 3;
  p->calibration.gyro = 3;
  p->calibration.sys  = 2;
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
