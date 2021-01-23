/**
 * @file BatteryMonitorThread.cpp
 * @brief
 */

/*****************************************************************************/
/* INCLUDES                                                                  */
/*****************************************************************************/
#include "BatteryMonitorThread.h"

#include <Arduino.h>

#include "dashboard/Dashboard.h"

/*****************************************************************************/
/* DEFINED CONSTANTS                                                         */
/*****************************************************************************/
#define BAT_ADC     32
#define USB_PRESENT 34
#define BAT_STAT    39

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
static bool detectUSB(void)
{
  return (HIGH == digitalRead(USB_PRESENT));
}

static bool detectSTAT(void)
{
  return (HIGH == digitalRead(BAT_STAT));
}

static BatteryStatus_t decodeStatus(void)
{
  BatteryStatus_t status;

  bool usbPresent     = detectUSB();
  bool chargeFinished = detectSTAT();

  if (usbPresent) {
    status = chargeFinished ? BAT_CHARGED : BAT_CHARGING;
  } else {
    status = BAT_DISCHARGING;
  }

  return status;
}

static uint32_t voltage2percentage(double voltage)
{
  uint32_t p = 0;

  if (4.2 <= voltage) {
    p = 100;
  } else if (4.15 <= voltage) {
    p = 95;
  } else if (4.11 <= voltage) {
    p = 90;
  } else if (4.08 <= voltage) {
    p = 85;
  } else if (4.02 <= voltage) {
    p = 80;
  } else if (3.98 <= voltage) {
    p = 75;
  } else if (3.95 <= voltage) {
    p = 70;
  } else if (3.91 <= voltage) {
    p = 65;
  } else if (3.87 <= voltage) {
    p = 60;
  } else if (3.85 <= voltage) {
    p = 55;
  } else if (3.84 <= voltage) {
    p = 50;
  } else if (3.82 <= voltage) {
    p = 45;
  } else if (3.8 <= voltage) {
    p = 40;
  } else if (3.79 <= voltage) {
    p = 35;
  } else if (3.77 <= voltage) {
    p = 30;
  } else if (3.75 <= voltage) {
    p = 25;
  } else if (3.73 <= voltage) {
    p = 20;
  } else if (3.71 <= voltage) {
    p = 15;
  } else if (3.69 <= voltage) {
    p = 10;
  } else if (3.61 <= voltage) {
    p = 5;
  } else {
    p = 0;
  }

  return p;
}

/*****************************************************************************/
/* DEFINITION OF GLOBAL FUNCTIONS                                            */
/*****************************************************************************/
void BatteryMonitorThread(void *p)
{
  pinMode(BAT_STAT, INPUT);
  pinMode(USB_PRESENT, INPUT);

  while (1) {
    double value   = (double)analogRead(BAT_ADC);
    double voltage = ((value / 4096.0) * 3.57) * (330.0 / 220.0);

    Battery_t battery;
    battery.status     = decodeStatus();
    battery.voltage    = voltage;
    battery.percentage = voltage2percentage(voltage);
    battery.value      = (uint32_t)value;
    DbDataBatterySet(&battery);

    Board_t board;
    board.usbConnected = detectUSB();
    DbDataBoardSet(&board);

    delay(1000);
  }
}

/****************************** END OF FILE **********************************/
