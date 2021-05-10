#include <Arduino.h>

#include "core/BatteryMonitorThread.h"
#include "core/BeepControlThread.h"
#include "core/GpsManagerThread.h"
#include "core/DataFilterThread.h"
#include "core/DataLoggerThread.h"
#include "core/ImuManagerThread.h"
#include "core/LvglThread.h"
#include "core/PressureReaderThread.h"
#include "dashboard/Dashboard.h"
#include "Power.h"

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

void setup()
{
  Serial.begin(460800);
  Serial.println();
  Serial.println();
  Serial.println("Sensorio started ...");

  PowerStart();
  
  DbInit();
  
  BeepControlThreadInit();
  ImuManagerInit();

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
      GpsManagerThread, "GPS thread", 2048, NULL, PRIO_0_GPS, NULL, 0);
#endif

#if 1
  xTaskCreatePinnedToCore(
      BatteryMonitorThread, "Battery thread", 2048, NULL, PRIO_0_BATTMON, NULL, 0);
#endif

#if 1
  xTaskCreatePinnedToCore(
      LvglThread, "LVGL thread", 16384, NULL, PRIO_1_LVGL, NULL, 1);
#endif
}

void loop()
{
  delay(1000);
}