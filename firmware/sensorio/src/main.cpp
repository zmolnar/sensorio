#include <Arduino.h>

#include "core/PressureReaderThread.h"

void setup() {
  Serial.begin(115200);
  Serial.println();
  Serial.println();
  Serial.print("Sensorio started ...\n");

  xTaskCreatePinnedToCore(
    PressureReaderThread,
    "pressure reader",
    2048,
    NULL,
    1,
    NULL,
    0
  );
}

void loop() {
  // put your main code here, to run repeatedly:
}