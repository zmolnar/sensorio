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

/*****************************************************************************/
/* DEFINED CONSTANTS                                                         */
/*****************************************************************************/
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
static uint32_t counter = 0;

/*****************************************************************************/
/* DECLARATION OF LOCAL FUNCTIONS                                            */
/*****************************************************************************/

/*****************************************************************************/
/* DEFINITION OF LOCAL FUNCTIONS                                             */
/*****************************************************************************/
static void badChecksumHandler(MicroNMEA &nmea)
{
  Serial.println("GPS bad checksum");
  Serial.println(nmea.getSentence());
}

static void unknownSentenceHandler(MicroNMEA &nmea)
{
  Serial.println("GPS unknown sentence");
  Serial.println(nmea.getSentence());
}

static void incomingDataISR(void)
{
  detachInterrupt(UART_RX);
  counter++;

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

  // Setup NMEA parser
  char buffer[100];
  MicroNMEA nmea(buffer, sizeof(buffer));
  nmea.setBadChecksumHandler(badChecksumHandler);
  nmea.setUnknownSentenceHandler(unknownSentenceHandler);
  nmea.clear();

  // Setup the UART interface
  HardwareSerial &gpsSerial = Serial2;
  gpsSerial.begin(9600, SERIAL_8N1, UART_RX, UART_TX);
  gpsSerial.println("$PMTK251,115200*1F");
  gpsSerial.updateBaudRate(115200);

  while (1) {
    attachInterrupt(UART_RX, incomingDataISR, FALLING);

    uint32_t notification = ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
    if (1 == notification) {
      while (!gpsSerial.available()) {
        delay(1);
      }
      while (gpsSerial.available()) {
        char c = gpsSerial.read();
        if (nmea.process(c)) {
          // TODO update dashboard
        }
      }
    }
  }
}

/****************************** END OF FILE **********************************/
