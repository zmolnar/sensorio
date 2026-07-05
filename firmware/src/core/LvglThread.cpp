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

#include "LvglThread.hpp"

#include <drivers/encoder/Encoder.hpp>
#include <drivers/lcd/SharpLcd.hpp>
#include <gui/Sensorio.h>
#include <platform/Log.hpp>

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <freertos/timers.h>

#include <lvgl.h>

static constexpr uint32_t LVGL_TICK_IN_MS{20U};
static constexpr auto *tag = "lvgl-thread";

static TaskHandle_t lvglTask = NULL;
static TimerHandle_t timerHandle;
static volatile bool startupFinishedRequested = false;
static bool shutdownRequested = false;

static void tick(TimerHandle_t xTimer) {
  (void)xTimer;

  if (nullptr != lvglTask) {
    xTaskNotifyGive(lvglTask);
  }
}

void LvglThread(void *p) {
  (void)p;

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

  uint32_t vcomCounter = 0;

  while (1) {
    uint32_t notification = ulTaskNotifyTake(pdTRUE, portMAX_DELAY);

    if (0 < notification) {
      lv_tick_inc(notification * LVGL_TICK_IN_MS);

      vcomCounter += notification * LVGL_TICK_IN_MS;
      if (SHARP_LCD_VCOM_CHANGE_PERIOD_IN_MS <= vcomCounter) {
        SharpLcdToggleVcom();
        vcomCounter = 0;
      }

      lv_task_handler();
      if (startupFinishedRequested) {
        EncoderInit();
        EncoderRegisterDriver(SensorioGetEncoderGroup());
        startupFinishedRequested = false;
      }
      if (shutdownRequested) {
        SensorioConfirmExit();
        shutdownRequested = false;
      }
      SharpLcdSendVcomIfNeeded();
    } else {
      Platform::Log::Error(tag) << "lvgl notification timeout";
    }
  }
}

void LvglStartupFinished(void) {
  startupFinishedRequested = true;
}

void LvglShutdownRequested(void) {
  shutdownRequested = true;
}
