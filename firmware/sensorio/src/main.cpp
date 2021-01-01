#include <Arduino.h>

#include "core/GpsManagerThread.h"
#include "core/ImuManagerThread.h"
#include "core/LvglThread.h"
#include "core/PressureReaderThread.h"

void setup()
{
  Serial.begin(115200);
  Serial.println();
  Serial.println();
  Serial.print("Sensorio started ...\n");

#if 0
  xTaskCreatePinnedToCore(PressureReaderThread, "pressure reader", 2048, NULL, 1, NULL, 0);
#endif

#if 0
  xTaskCreatePinnedToCore(ImuManagerThread, "IMU manager", 2048, NULL, 1, NULL, 0);
#endif

#if 0
  xTaskCreatePinnedToCore(LvglThread, "LVGL thread", 2048, NULL, 1, NULL, 1);
#endif

#if 1
  xTaskCreatePinnedToCore(GpsManagerThread, "GPS thread", 2048, NULL, 1, NULL, 0);
#endif

}

void loop()
{
}