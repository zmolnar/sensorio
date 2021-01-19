/**
 * @file Power.c
 * @brief
 */

#if !defined(SIMULATOR)
#error Use this file only in the simulator!
#endif

/*****************************************************************************/
/* INCLUDES                                                                  */
/*****************************************************************************/
#include "Power.h"

#include <stdio.h>

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
void PowerStart(void)
{
  printf("Simulator Power started\n");
}

void PowerStartupFinished(void)
{
  printf("Simulator Power startup finished\n");
}

void PowerStop(void)
{
  printf("Simulator Power stopped\n");
}

/****************************** END OF FILE **********************************/
