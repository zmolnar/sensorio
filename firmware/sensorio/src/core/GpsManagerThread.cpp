//
//  This file is part of Sensorio.
//
//  Sensorio is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  Sensorio is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with Sensorio.  If not, see <https://www.gnu.org/licenses/>.
//

#include <core/GpsManagerThread.hpp>
#include <core/nmea/Processor.hpp>
#include <dashboard/Dashboard.hpp>
#include <platform/Log.hpp>

#include <driver/gpio.h>
#include <driver/uart.h>
#include <esp_log.h>
#include <freertos/FreeRTOS.h>
#include <freertos/queue.h>

#include <string.h>

static constexpr auto GPS_3DFIX{GPIO_NUM_18};
static constexpr auto GPS_3DFIX_SEL{GPIO_SEL_18};
static constexpr auto GPS_UART{UART_NUM_1};
static constexpr auto GPS_RX{GPIO_NUM_26};
static constexpr auto GPS_TX{GPIO_NUM_27};

static constexpr auto UART_TX{GPS_RX};
static constexpr auto UART_RX{GPS_TX};

static const char *tag = "gps-thread";

static void configure3dFixPin(void) {
  gpio_config_t conf;
  conf.intr_type = GPIO_INTR_DISABLE;
  conf.mode = GPIO_MODE_INPUT;
  conf.pin_bit_mask = GPS_3DFIX_SEL;
  conf.pull_down_en = GPIO_PULLDOWN_DISABLE;
  conf.pull_up_en = GPIO_PULLUP_DISABLE;
  gpio_config(&conf);
}

void GpsManagerThread(void *p) {
  static QueueHandle_t uartEventQueue;
  static constexpr int GPS_RX_BUF_IN_BYTES{1024U};
  uart_driver_install(GPS_UART, GPS_RX_BUF_IN_BYTES, 0, 20, &uartEventQueue, 0);

  uart_config_t gpsConfig = {
      .baud_rate = 9600,
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
  bool clearNeeded{false};
  using namespace Nmea0183;
  auto receiveCb = [&clearNeeded](const Data &data, const char *str) {
    Dashboard::Gps gps{};

    gps.locked = 1 == gpio_get_level(GPS_3DFIX);
    gps.altitude = data.altitude;
    gps.course = data.course;
    gps.latitude = data.latitude.degrees;
    gps.longitude = data.longitude.degrees;
    gps.numOfSatellites = data.numOfSats;
    gps.speed = data.speed * 1.852;
    gps.gmt.year = data.date.year;
    gps.gmt.month = data.date.month;
    gps.gmt.day = data.date.day;
    gps.gmt.hour = data.time.hour;
    gps.gmt.minute = data.time.minute;
    gps.gmt.second = data.time.second;

    dashboard.gps.set(gps);

    clearNeeded = true;
  };

  auto errorCb = [&clearNeeded](Processor::ErrorCode code, const char *str) {
    using namespace Platform;
    if (Processor::ErrorCode::UNKNOWN_SENTENCE != code) {
      Log::Error(tag) << "NMEA error: " << (int)code << " " << str;
    }
    clearNeeded = true;
  };

  Nmea0183::Processor nmea(receiveCb, errorCb);

  while (1) {
    uart_event_t event;
    int result = xQueueReceive(uartEventQueue, (void *)&event, portMAX_DELAY);

    if (pdTRUE == result) {
      if (UART_DATA == event.type) {
        uint8_t c;
        while (0 < uart_read_bytes(GPS_UART, &c, 1, pdMS_TO_TICKS(10))) {
          nmea.process(c);
          if (clearNeeded) {
            nmea.clear();
            clearNeeded = false;
          }
        }
      } else {
        ESP_LOGI(tag, "Discarded UART event %d", (int)event.type);
        vTaskDelay(pdMS_TO_TICKS(1000));
      }
    }
  }
}

void GpsManagerThreadInit(void) {
}
