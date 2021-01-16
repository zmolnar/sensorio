/**
 * @file LvglThread.cpp
 * @brief
 */

/*****************************************************************************/
/* INCLUDES                                                                  */
/*****************************************************************************/
#include "LvglThread.h"

#include <Arduino.h>

#include "drivers/encoder/Encoder.h"
#include "drivers/lcd/SharpLcd.h"
#include "gui/startup.h"
#include "lvgl.h"

/*****************************************************************************/
/* DEFINED CONSTANTS                                                         */
/*****************************************************************************/
#define LVGL_TICK_IN_MS 5

/*****************************************************************************/
/* TYPE DEFINITIONS                                                          */
/*****************************************************************************/

/*****************************************************************************/
/* MACRO DEFINITIONS                                                         */
/*****************************************************************************/

/*****************************************************************************/
/* DEFINITION OF GLOBAL CONSTANTS AND VARIABLES                              */
/*****************************************************************************/
static TaskHandle_t  lvglTask = NULL;
static TimerHandle_t timerHandle;

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

  // Initialize LVGL
  lv_init();

  // Initialize display driver
  SharpLcdInit();
  SharpLcdRegisterDriver();

  // // Initialize input device
  // EncoderInit();
  // EncoderRegisterDriver();

  // Draw initial screen
  lv_obj_t *scr = startup_screen_create();
  lv_scr_load(scr);

  lvglTask = xTaskGetCurrentTaskHandle();
  timerHandle = xTimerCreate(
      "lvgl tick timer", pdMS_TO_TICKS(LVGL_TICK_IN_MS), pdTRUE, 0, tick);
  if (pdPASS != xTimerStart(timerHandle, 0)) {
    Serial.println("timer start failed");
  }

  while (1) {
    // TODO measure the time needed for updating the frame and check scheduling. 
    uint32_t notification = ulTaskNotifyTake(pdTRUE, portMAX_DELAY);

    if (0 < notification) {
      lv_tick_inc(LVGL_TICK_IN_MS);
      lv_task_handler();
      SharpLcdSendVcomIfNeeded();
    } else {
      Serial.println("lvgl timeout");
    }
  }
}

/****************************** END OF FILE **********************************/
