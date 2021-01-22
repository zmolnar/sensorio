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
  memset(p, 0, sizeof(*p));
}

void DbDataGpsSet(GpsData_t *p)
{
}

void DbDataBpsGet(BpsData_t *p)
{
  static uint8_t offset = 0;

  memset(p, 0, sizeof(*p));
  p->cooked.pressure = 101325 + (offset++ % 20);
  p->cooked.temp = 2450;
}

void DbDataBpsSet(BpsData_t *p)
{
}

void DbDataImuGet(ImuData_t *p)
{
  memset(p, 0, sizeof(*p));
}

void DbDataImuSet(ImuData_t *p)
{
}

/****************************** END OF FILE **********************************/