
#include <Power.h>
#include <core/BatteryMonitorThread.h>
#include <core/BeepControlThread.h>
#include <core/DataFilterThread.h>
#include <core/DataLoggerThread.h>
#include <core/GpsManagerThread.h>
#include <core/LvglThread.h>
#include <core/PressureReaderThread.h>
#include <core/ImuManagerThread.h>
#include <dashboard/Dashboard.h>

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <esp_log.h>

typedef enum {
  PRIO_0_INVALID = 0,
  PRIO_0_BATTMON,
  PRIO_0_LOGGER,
  PRIO_0_BEEPER,
  PRIO_0_GPS,
  PRIO_0_FILTER,
  PRIO_0_IMU,
  PRIO_0_PRESS,
} ThreadPrio_Core0_t;

typedef enum {
  PRIO_1_INVALID = 0,
  PRIO_1_LVGL = 10,
} ThreadPrio_Core1_t;

static const char *tag = "MAIN";

void app_main(void)
{
  ESP_LOGI(tag, "Sensorio started");
  
  PowerStart();
  DbInit();

  PressureReaderThreadInit();
  ImuManagerThreadInit();
  GpsManagerThreadInit();
  DataFilterThreadInit();
  DataLoggerThreadInit();
  BeepControlThreadInit();
  BatteryMonitorInit();

#if 1
  xTaskCreatePinnedToCore(DataFilterThread,
                          "data filter",
                          8192,
                          NULL,
                          PRIO_0_FILTER,
                          NULL,
                          0);
#endif

#if 1
  xTaskCreatePinnedToCore(DataLoggerThread,
                          "data logger",
                          8192,
                          NULL,
                          PRIO_0_LOGGER,
                          NULL,
                          0);
#endif

#if 1
  xTaskCreatePinnedToCore(PressureReaderThread,
                          "pressure reader",
                          2048,
                          NULL,
                          PRIO_0_PRESS,
                          NULL,
                          0);
#endif

#if 1
  xTaskCreatePinnedToCore(BeepControlThread,
                          "beeper",
                          2048,
                          NULL,
                          PRIO_0_BEEPER,
                          NULL,
                          0);
#endif

#if 1
  xTaskCreatePinnedToCore(
      ImuManagerThread, "IMU manager", 4096, NULL, PRIO_0_IMU, NULL, 0);
#endif

#if 1
  xTaskCreatePinnedToCore(
      GpsManagerThread, "GPS thread", 4096, NULL, PRIO_0_GPS, NULL, 0);
#endif

#if 1
  xTaskCreatePinnedToCore(
      BatteryMonitorThread, "Battery thread", 2048, NULL, PRIO_0_BATTMON, NULL, 0);
#endif

#if 1
  xTaskCreatePinnedToCore(
      LvglThread, "LVGL thread", 16384, NULL, PRIO_1_LVGL, NULL, 1);
#endif

  while(1) {
    vTaskDelay(10000 / portTICK_RATE_MS);
  }
}
