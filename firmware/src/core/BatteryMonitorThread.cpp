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

#include <core/BatteryMonitorThread.hpp>

#include <driver/adc.h>
#include <driver/gpio.h>
#include <esp_adc_cal.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

#include <dashboard/Dashboard.hpp>

static constexpr adc1_channel_t BAT_ADC_CHANNEL{ADC1_CHANNEL_4};
static constexpr adc_bits_width_t BAT_ADC_WIDTH{ADC_WIDTH_BIT_12};
static constexpr adc_atten_t BAT_ADC_ATTEN{ADC_ATTEN_DB_11};

static constexpr gpio_num_t USB_PRESENT{GPIO_NUM_34};
static constexpr uint64_t USB_PRESENT_SEL{GPIO_SEL_34};
static constexpr gpio_num_t BAT_STATUS{GPIO_NUM_39};
static constexpr uint64_t BAT_STATUS_SEL{GPIO_SEL_39};

static inline bool detectUSB(void) {
  return (1 == gpio_get_level(USB_PRESENT));
}

static inline bool detectSTAT(void) {
  return (1 == gpio_get_level(BAT_STATUS));
}

static Dashboard::Battery::Status decodeStatus(void) {
  Dashboard::Battery::Status status;

  bool usbPresent = detectUSB();
  bool chargeFinished = detectSTAT();

  if (usbPresent) {
    if (chargeFinished) {
      status = Dashboard::Battery::Status::CHARGED;
    } else {
      status = Dashboard::Battery::Status::CHARGING;
    }
  } else {
    status = Dashboard::Battery::Status::DISCHARGING;
  }

  return status;
}

static uint32_t voltage2percentage(double voltage) {
  uint32_t p = 0;

  if (4.15 <= voltage) {
    p = 100;
  } else if (4.13 <= voltage) {
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

static void configurePins(void) {
  gpio_config_t conf;
  conf.pin_bit_mask = USB_PRESENT_SEL | BAT_STATUS_SEL;
  conf.intr_type = GPIO_INTR_DISABLE;
  conf.mode = GPIO_MODE_INPUT;
  conf.pull_down_en = GPIO_PULLDOWN_DISABLE;
  conf.pull_up_en = GPIO_PULLUP_DISABLE;
  gpio_config(&conf);
}

void BatteryMonitorThread(void *p) {
  configurePins();

  adc1_config_width(BAT_ADC_WIDTH);
  adc1_config_channel_atten(BAT_ADC_CHANNEL, BAT_ADC_ATTEN);

  esp_adc_cal_characteristics_t adc_chars;
  esp_adc_cal_characterize(
      ADC_UNIT_1, BAT_ADC_ATTEN, BAT_ADC_WIDTH, 1100, &adc_chars);

  while (1) {
    int raw = 0;
    static constexpr size_t BAT_AVG_SAMPLE_COUNT{10};
    for (size_t i = 0; i < BAT_AVG_SAMPLE_COUNT; ++i) {
      raw += adc1_get_raw(BAT_ADC_CHANNEL);
    }

    raw = raw / BAT_AVG_SAMPLE_COUNT;

    uint32_t raw_voltage = esp_adc_cal_raw_to_voltage(raw, &adc_chars);
    double voltage = ((double)raw_voltage) / 1000.0 * (330.0 / 220.0);

    Dashboard::Battery battery{};
    battery.status = decodeStatus();
    battery.voltage = voltage;
    battery.percentage = voltage2percentage(voltage);
    battery.adcValue = raw_voltage;
    dashboard.battery.set(battery);

    Dashboard::Board board{};
    board.usbConnected = detectUSB();
    dashboard.board.set(board);

    vTaskDelay(pdMS_TO_TICKS(1000));
  }
}

void BatteryMonitorInit(void) {
}
