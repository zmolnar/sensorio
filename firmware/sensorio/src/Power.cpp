/**
 * @file Power.c
 * @brief
 */

/*****************************************************************************/
/* INCLUDES                                                                  */
/*****************************************************************************/
#include "Power.h"

#include <Arduino.h>

#include "core/LvglThread.h"

/*****************************************************************************/
/* DEFINED CONSTANTS                                                         */
/*****************************************************************************/
#define nBT0   36
#define POW_EN 4

/*****************************************************************************/
/* TYPE DEFINITIONS                                                          */
/*****************************************************************************/

/*****************************************************************************/
/* MACRO DEFINITIONS                                                         */
/*****************************************************************************/

/*****************************************************************************/
/* DEFINITION OF GLOBAL CONSTANTS AND VARIABLES                              */
/*****************************************************************************/
static bool startupFinished = false;

/*****************************************************************************/
/* DECLARATION OF LOCAL FUNCTIONS                                            */
/*****************************************************************************/

/*****************************************************************************/
/* DEFINITION OF LOCAL FUNCTIONS                                             */
/*****************************************************************************/
static void powerSwitchReleasedCb(void)
{
  detachInterrupt(nBT0);

  if (startupFinished) {
    LvglStartupFinished();
    Serial.println("Startup finished");
  } else {
    Serial.print("Startup aborted");
    digitalWrite(POW_EN, LOW);
  }
}

/*****************************************************************************/
/* DEFINITION OF GLOBAL FUNCTIONS                                            */
/*****************************************************************************/
void PowerStart(void)
{
  pinMode(nBT0, INPUT);
  pinMode(POW_EN, OUTPUT);

  if (LOW == digitalRead(nBT0)) {
    digitalWrite(POW_EN, HIGH);
    startupFinished = false;
    attachInterrupt(nBT0, powerSwitchReleasedCb, RISING);
  } else {
    digitalWrite(POW_EN, LOW);
  }
}

void PowerStartupFinished(void)
{
  startupFinished = true;
}

void PowerStop(void)
{
  digitalWrite(POW_EN, LOW);
}

/****************************** END OF FILE **********************************/
