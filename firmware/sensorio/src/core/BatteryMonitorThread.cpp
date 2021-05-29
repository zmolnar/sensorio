/**
 * @file BatteryMonitorThread.cpp
 * @brief
 */

/*****************************************************************************/
/* INCLUDES                                                                  */
/*****************************************************************************/
#include "BatteryMonitorThread.h"

#include <driver/adc.h>
#include <driver/gpio.h>
#include <esp_adc_cal.h>
#include <esp_log.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

#include <dashboard/Dashboard.h>

/*****************************************************************************/
/* DEFINED CONSTANTS                                                         */
/*****************************************************************************/
#define BAT_ADC_CHANNEL      ADC1_CHANNEL_4
#define BAT_ADC_WIDTH        ADC_WIDTH_BIT_12
#define BAT_ADC_ATTEN        ADC_ATTEN_DB_11
#define BAT_AVG_SAMPLE_COUNT 10

#define USB_PRESENT     GPIO_NUM_34
#define USB_PRESENT_SEL GPIO_SEL_34
#define BAT_STATUS      GPIO_NUM_39
#define BAT_STATUS_SEL  GPIO_SEL_39

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
static inline bool detectUSB(void)
{
  return (1 == gpio_get_level(USB_PRESENT));
}

static inline bool detectSTAT(void)
{
  return (1 == gpio_get_level(BAT_STATUS));
}

static BatteryStatus_t decodeStatus(void)
{
  BatteryStatus_t status;

  bool usbPresent = detectUSB();
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

static void configurePins(void)
{
  gpio_config_t conf;
  conf.pin_bit_mask = USB_PRESENT_SEL | BAT_STATUS_SEL;
  conf.intr_type = GPIO_INTR_DISABLE;
  conf.mode = GPIO_MODE_INPUT;
  conf.pull_down_en = GPIO_PULLDOWN_DISABLE;
  conf.pull_up_en = GPIO_PULLUP_DISABLE;
  gpio_config(&conf);
}

/*****************************************************************************/
/* DEFINITION OF GLOBAL FUNCTIONS                                            */
/*****************************************************************************/
void BatteryMonitorThread(void *p)
{
  configurePins();

  adc1_config_width(BAT_ADC_WIDTH);
  adc1_config_channel_atten(BAT_ADC_CHANNEL, BAT_ADC_ATTEN);

  esp_adc_cal_characteristics_t adc_chars;
  esp_adc_cal_characterize(
      ADC_UNIT_1, BAT_ADC_ATTEN, BAT_ADC_WIDTH, 1100, &adc_chars);

  while (1) {
    int raw = 0;
    for (size_t i = 0; i < BAT_AVG_SAMPLE_COUNT; ++i) {
      raw += adc1_get_raw(BAT_ADC_CHANNEL);
    }

    raw = raw / BAT_AVG_SAMPLE_COUNT;

    uint32_t raw_voltage = esp_adc_cal_raw_to_voltage(raw, &adc_chars);
    double voltage = ((double)raw_voltage) / 1000.0 * (330.0 / 220.0);

    Battery_t battery;
    battery.status = decodeStatus();
    battery.voltage = voltage;
    battery.percentage = voltage2percentage(voltage);
    battery.value = (uint32_t)raw;
    DbDataBatterySet(&battery);

    Board_t board;
    board.usbConnected = detectUSB();
    DbDataBoardSet(&board);

    vTaskDelay(pdMS_TO_TICKS(1000));
  }
}

void BatteryMonitorInit(void)
{
}

/****************************** END OF FILE **********************************/
