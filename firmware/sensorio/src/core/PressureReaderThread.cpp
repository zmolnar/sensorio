/**
 * @file PressureReaderThread.cpp
 * @brief
 */

/*****************************************************************************/
/* INCLUDES                                                                  */
/*****************************************************************************/
#include "PressureReaderThread.h"

#include <Arduino.h>

#include "drivers/ms5611/ms5611.h"

/*****************************************************************************/
/* DEFINED CONSTANTS                                                         */
/*****************************************************************************/

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

/*****************************************************************************/
/* DEFINITION OF GLOBAL FUNCTIONS                                            */
/*****************************************************************************/
static float convertPressureToAltitude(float pressure) {
    return 44330 * (1 - pow ((pressure / 101325.0), 0.1902));
}

void PressureReaderThread(void *p)
{
  TwoWire     ms5611_twi = TwoWire(0);
  MS5611Class ms5611     = MS5611Class(ms5611_twi);

  if (!ms5611.begin(13, 14, 400000)) {
    Serial.print("Failed to initialize\n");
  } else {
    Serial.print("Succeeded to initialize\n");
  }

  while (1) {
    delay(1000);

    bool result = ms5611.convert(MS5611_OSR_4096);

    if (result) {
      uint32_t p_comp = ms5611.getCompensatedPressure();
      float alt = convertPressureToAltitude((float)p_comp);

      Serial.print("t_raw : ");
      Serial.println(ms5611.getRawTemp());
      Serial.print("t_comp: ");
      Serial.println(ms5611.getCompensatedTemp());
      Serial.print("p_raw : ");
      Serial.println(ms5611.getRawPressure());
      Serial.print("p_comp : ");
      Serial.println(p_comp);
      Serial.println();
      Serial.print("Altitude: ");
      Serial.print(alt, 4);
      Serial.println();
      Serial.println();
    } else {
      Serial.println("Convert failed");
    }
  }
}

/****************************** END OF FILE **********************************/
