/**
 * @file GpsManagerThread.cpp
 * @brief
 */

/*****************************************************************************/
/* INCLUDES                                                                  */
/*****************************************************************************/
#include "GpsManagerThread.h"
#include <dashboard/Dashboard.h>

#include <MicroNMEA.h>

#include <driver/gpio.h>
#include <driver/uart.h>
#include <esp_log.h>
#include <freertos/FreeRTOS.h>
#include <freertos/queue.h>

#include <string.h>

/*****************************************************************************/
/* DEFINED CONSTANTS                                                         */
/*****************************************************************************/
#define GPS_3DFIX           GPIO_NUM_18
#define GPS_3DFIX_SEL       GPIO_SEL_18
#define GPS_UART            UART_NUM_1
#define GPS_RX              GPIO_NUM_26
#define GPS_TX              GPIO_NUM_27
#define GPS_TX_BUF_IN_BYTES 0U
#define GPS_RX_BUF_IN_BYTES 1024U

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
static bool isValid = true;
static const char *tag = "gps-manager";

/*****************************************************************************/
/* DECLARATION OF LOCAL FUNCTIONS                                            */
/*****************************************************************************/

/*****************************************************************************/
/* DEFINITION OF LOCAL FUNCTIONS                                             */
/*****************************************************************************/
static void badChecksumHandler(MicroNMEA &nmea)
{
  ESP_LOGE(tag, "bad checksum: %s", nmea.getSentence());
  isValid = false;
}

static void unknownSentenceHandler(MicroNMEA &nmea)
{
}

static void configure3dFixPin(void)
{
  gpio_config_t conf;
  conf.intr_type = GPIO_INTR_DISABLE;
  conf.mode = GPIO_MODE_INPUT;
  conf.pin_bit_mask = GPS_3DFIX_SEL;
  conf.pull_down_en = GPIO_PULLDOWN_DISABLE;
  conf.pull_up_en = GPIO_PULLUP_DISABLE;
  gpio_config(&conf);
}

/*****************************************************************************/
/* DEFINITION OF GLOBAL FUNCTIONS                                            */
/*****************************************************************************/
void GpsManagerThread(void *p)
{
  static QueueHandle_t uartEventQueue;
  uart_driver_install(GPS_UART, GPS_RX_BUF_IN_BYTES, 0, 20, &uartEventQueue, 0);

  uart_config_t gpsConfig = {
      .baud_rate = 115200,
      .data_bits = UART_DATA_8_BITS,
      .parity = UART_PARITY_DISABLE,
      .stop_bits = UART_STOP_BITS_1,
      .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
      .rx_flow_ctrl_thresh = 0,
      .source_clk = UART_SCLK_APB,
  };
  uart_param_config(GPS_UART, &gpsConfig);
  uart_set_pin(GPS_UART, UART_TX, UART_RX, GPIO_NUM_NC, GPIO_NUM_NC);

  configure3dFixPin();

  // Setup NMEA parser
  char buffer[100];
  MicroNMEA nmea(buffer, sizeof(buffer));
  nmea.setBadChecksumHandler(badChecksumHandler);
  nmea.setUnknownSentenceHandler(unknownSentenceHandler);
  nmea.clear();

  while (1) {
    uart_event_t event;
    int result = xQueueReceive(uartEventQueue, (void *)&event, portMAX_DELAY);

    if (pdTRUE == result) {
      if (UART_DATA == event.type) {
        uint8_t c;
        while (0 < uart_read_bytes(GPS_UART, &c, 1, pdMS_TO_TICKS(10))) {
          if (nmea.process(c) && isValid) {
            GpsData_t data;
            memset(&data, 0, sizeof(data));

            long alt = 0;
            nmea.getAltitude(alt);

            data.locked = 1 == gpio_get_level(GPS_3DFIX);
            data.altitude = (uint32_t)alt;
            data.course = nmea.getCourse();
            data.latitude = nmea.getLatitude();
            data.longitude = nmea.getLongitude();
            data.numOfSatellites = nmea.getNumSatellites();
            data.speed = nmea.getSpeed() * 0.001852;
            data.time.year = nmea.getYear();
            data.time.month = nmea.getMonth();
            data.time.day = nmea.getDay();
            data.time.hour = nmea.getHour();
            data.time.minute = nmea.getMinute();
            data.time.second = nmea.getSecond();

            DbDataGpsSet(&data);
            isValid = true;
          }
        }
      } else {
        ESP_LOGI(tag, "Discarded UART event %d", (int)event.type);
      }
    }
  }
}

/****************************** END OF FILE **********************************/
