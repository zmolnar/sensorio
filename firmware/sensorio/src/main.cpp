#include <Arduino.h>

#include "core/GpsManagerThread.h"
#include "core/ImuManagerThread.h"
#include "core/LvglThread.h"
#include "core/PressureReaderThread.h"
#include "dashboard/Dashboard.h"

typedef enum {
  PRIO_0_INVALID = 0,
  PRIO_0_GPS,
  PRIO_0_IMU,
  PRIO_0_PRESS,
} ThreadPrio_Core0_t;

typedef enum {
  PRIO_1_INVALID = 0,
  PRIO_1_LVGL,
} ThreadPrio_Core1_t;

void setup()
{
  Serial.begin(115200);
  Serial.println();
  Serial.println();
  Serial.println("Sensorio started ...");

  DbInit();

  pinMode(4, OUTPUT);
  digitalWrite(4, HIGH);

  Serial.println("power-on");

  // Threads assigned to Core 0
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
  xTaskCreatePinnedToCore(ImuManagerThread, "IMU manager", 2048, NULL, PRIO_0_IMU, NULL, 0);
#endif

#if 1
  xTaskCreatePinnedToCore(GpsManagerThread, 
                          "GPS thread", 
                          2048, 
                          NULL, 
                          PRIO_0_GPS, 
                          NULL, 
                          0);
#endif

  // Threads assigned to Core 1
#if 0
  xTaskCreatePinnedToCore(LvglThread, "LVGL thread", 2048, NULL, PRIO_1_LVGL, NULL, 1);
#endif
}

void loop()
{
  delay(1000);
}