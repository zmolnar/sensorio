/**
 * @file LvglThread.cpp
 * @brief
 */

/*****************************************************************************/
/* INCLUDES                                                                  */
/*****************************************************************************/
#include "LvglThread.h"

#include <drivers/encoder/Encoder.h>
#include <drivers/lcd/SharpLcd.h>
#include <esp_log.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <freertos/timers.h>
#include <gui/Sensorio.h>
#include <lvgl.h>


/*****************************************************************************/
/* DEFINED CONSTANTS                                                         */
/*****************************************************************************/
#define LVGL_TICK_IN_MS 20

/*****************************************************************************/
/* TYPE DEFINITIONS                                                          */
/*****************************************************************************/

/*****************************************************************************/
/* MACRO DEFINITIONS                                                         */
/*****************************************************************************/

/*****************************************************************************/
/* DEFINITION OF GLOBAL CONSTANTS AND VARIABLES                              */
/*****************************************************************************/
static TaskHandle_t lvglTask = NULL;
static TimerHandle_t timerHandle;
static bool shutdownRequested = false;
static const char *tag = "LVGL";

/*****************************************************************************/
/* DECLARATION OF LOCAL FUNCTIONS                                            */
/*****************************************************************************/

/*****************************************************************************/
/* DEFINITION OF LOCAL FUNCTIONS                                             */
/*****************************************************************************/
static void tick(TimerHandle_t xTimer)
{
  (void)xTimer;

  static uint32_t counter = 0;
  counter += LVGL_TICK_IN_MS;

  if (SHARP_LCD_VCOM_CHANGE_PERIOD_IN_MS <= counter) {
    SharpLcdToggleVcom();
    counter = 0;
  }

  BaseType_t xHigherPriorityTaskWoken = pdFALSE;
  vTaskNotifyGiveFromISR(lvglTask, &xHigherPriorityTaskWoken);
  if (pdTRUE == xHigherPriorityTaskWoken) {
    portYIELD_FROM_ISR();
  }
}

/*****************************************************************************/
/* DEFINITION OF GLOBAL FUNCTIONS                                            */
/*****************************************************************************/
void LvglThread(void *p)
{
  (void)p;

  ESP_LOGI(tag, "LVGL thread started");

  // Initialize LVGL
  lv_init();

  // Initialize display driver
  SharpLcdInit();
  SharpLcdRegisterDriver();

  // Start GUI application
  SensorioStart();

  lvglTask = xTaskGetCurrentTaskHandle();
  timerHandle = xTimerCreate(
      "lvgl tick timer", pdMS_TO_TICKS(LVGL_TICK_IN_MS), pdTRUE, 0, tick);
  configASSERT(timerHandle);

  xTimerStart(timerHandle, 0);

  while (1) {
    uint32_t notification = ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
    
    if (0 < notification) {
      lv_tick_inc(notification * LVGL_TICK_IN_MS);

      lv_task_handler();
      if (shutdownRequested) {
        SensorioConfirmExit();
        shutdownRequested = false;
      }
      SharpLcdSendVcomIfNeeded();
    } else {
      ESP_LOGE(tag, "lvgl timeout");
    }
  }
}

void LvglStartupFinished(void)
{
  EncoderInit();
  EncoderRegisterDriver(SensorioGetEncoderGroup());
}

void LvglShutdownRequested(void)
{
  shutdownRequested = true;
}

/****************************** END OF FILE **********************************/
