/**
 * @file GpsManagerThread.cpp
 * @brief
 */

/*****************************************************************************/
/* INCLUDES                                                                  */
/*****************************************************************************/
#include "GpsManagerThread.h"

#include <Arduino.h>
#include <MicroNMEA.h>

#include "dashboard/Dashboard.h"

/*****************************************************************************/
/* DEFINED CONSTANTS                                                         */
/*****************************************************************************/
#define GPS_3DFIX 18

#define GPS_RX 26
#define GPS_TX 27

#define UART_TX GPS_RX
#define UART_RX GPS_TX

/*****************************************************************************/
/* TYPE DEFINITIONS                                                          */
/*****************************************************************************/

/*****************************************************************************/
/* MACRO DEFINITIONS                                                         */
/*****************************************************************************/

/*****************************************************************************/
/* DEFINITION OF GLOBAL CONSTANTS AND VARIABLES                              */
/*****************************************************************************/
static TaskHandle_t gpsTask = NULL;
static bool         isValid = true;

/*****************************************************************************/
/* DECLARATION OF LOCAL FUNCTIONS                                            */
/*****************************************************************************/

/*****************************************************************************/
/* DEFINITION OF LOCAL FUNCTIONS                                             */
/*****************************************************************************/
static void badChecksumHandler(MicroNMEA &nmea)
{
  isValid = false;
}

static void unknownSentenceHandler(MicroNMEA &nmea)
{
  isValid = false;
}

static void incomingDataISR(void)
{
  detachInterrupt(UART_RX);

  BaseType_t xHigherPriorityTaskWoken = pdFALSE;
  vTaskNotifyGiveFromISR(gpsTask, &xHigherPriorityTaskWoken);
  if (pdTRUE == xHigherPriorityTaskWoken) {
    portYIELD_FROM_ISR();
  }
}

/*****************************************************************************/
/* DEFINITION OF GLOBAL FUNCTIONS                                            */
/*****************************************************************************/
void GpsManagerThread(void *p)
{
  // Set task handler used for task notification
  gpsTask = xTaskGetCurrentTaskHandle();

  pinMode(GPS_3DFIX, INPUT);

  // Setup NMEA parser
  char      buffer[100];
  MicroNMEA nmea(buffer, sizeof(buffer));
  nmea.setBadChecksumHandler(badChecksumHandler);
  nmea.setUnknownSentenceHandler(unknownSentenceHandler);
  nmea.clear();

  // Setup the UART interface
  HardwareSerial &gpsSerial = Serial2;
  gpsSerial.begin(9600, SERIAL_8N1, UART_RX, UART_TX);
  while (!gpsSerial.available()) {
    delay(1);
  }
  while (gpsSerial.available()) {
    (void)gpsSerial.read();
  }

  // Set baudrate to 115200
  gpsSerial.println("$PMTK251,115200*1F");
  while (!gpsSerial.available()) {
    delay(1);
  }
  while (gpsSerial.available()) {
    (void)gpsSerial.read();
  }
  gpsSerial.updateBaudRate(115200);

  while (1) {
    attachInterrupt(UART_RX, incomingDataISR, FALLING);
    uint32_t notification = ulTaskNotifyTake(pdTRUE, pdMS_TO_TICKS(5000));

    if (1 == notification) {
      while (!gpsSerial.available()) {
        delay(1);
      }
      while (gpsSerial.available()) {
        char c = gpsSerial.read();
        if (nmea.process(c) && isValid) {
          DbDataGpsLock();

          DbDataGpsSetLocked(HIGH == digitalRead(GPS_3DFIX));
          long alt = 0;
          nmea.getAltitude(alt);
          DbDataGpsSetAltitude(alt);
          DbDataGpsSetCourse(nmea.getCourse());
          DbDataGpsSetLatitude(nmea.getLatitude());
          DbDataGpsSetLongitude(nmea.getLongitude());
          DbDataGpsSetNumOfSatellites(nmea.getNumSatellites());
          DbDataGpsSetSpeed(nmea.getSpeed());
          DbDataGpsSetYear(nmea.getYear());
          DbDataGpsSetMonth(nmea.getMonth());
          DbDataGpsSetDay(nmea.getDay());
          DbDataGpsSetHour(nmea.getHour());
          DbDataGpsSetMinute(nmea.getMinute());
          DbDataGpsSetSecond(nmea.getSecond());

          DbDataGpsUnlock();
        }

        isValid = true;
      }
    }
  }
}

/****************************** END OF FILE **********************************/
