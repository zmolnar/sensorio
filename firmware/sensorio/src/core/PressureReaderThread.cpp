/**
 * @file PressureReaderThread.cpp
 * @brief
 */

/*****************************************************************************/
/* INCLUDES                                                                  */
/*****************************************************************************/
#include "PressureReaderThread.h"

#include <Arduino.h>

#include "dashboard/Dashboard.h"
#include "drivers/ms5611/ms5611.h"

/*****************************************************************************/
/* DEFINED CONSTANTS                                                         */
/*****************************************************************************/
#define BPS_SDA  13
#define BPS_SCL  14
#define BPS_FREQ 400000

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
void PressureReaderThread(void *p)
{
  TwoWire ms5611_twi = TwoWire(0);
  MS5611  ms5611     = MS5611(ms5611_twi);

  while (!ms5611.begin(BPS_SDA, BPS_SCL, BPS_FREQ)) {
    Serial.print("MS5611 startup error\n");
    delay(1000);
  }

  Serial.print("MS5611 is ready\n");

  while (1) {
    // TODO sync with IMU interrupt
    delay(1000);

    bool result = ms5611.convert(MS5611::Osr::OSR_4096);

    if (result) {
      BpsData_t data;
      memset(&data, 0, sizeof(data));

      data.raw.temp        = ms5611.getRawTemp();
      data.raw.pressure    = ms5611.getRawPressure();
      data.cooked.temp     = ms5611.getCompensatedTemp();
      data.cooked.pressure = ms5611.getCompensatedPressure();

      DbDataBpsSet(&data);

#if 0
      Serial.print("t_raw : ");
      Serial.println(ms5611.getRawTemp());
      Serial.print("t_comp: ");
      Serial.println(ms5611.getCompensatedTemp());
      Serial.print("p_raw : ");
      Serial.println(ms5611.getRawPressure());
      Serial.print("p_comp : ");
      Serial.println(ms5611.getCompensatedPressure());
      Serial.println();
#endif

    } else {
      Serial.println("MS5611 conversion error");
    }
  }
}

/****************************** END OF FILE **********************************/
