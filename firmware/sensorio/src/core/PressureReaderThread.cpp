/**
 * @file PressureReaderThread.cpp
 * @brief
 */

/*****************************************************************************/
/* INCLUDES                                                                  */
/*****************************************************************************/
#include "PressureReaderThread.h"
#include "DataFilterThread.h"
#include "ImuManagerThread.h"

#include "dashboard/Dashboard.h"
#include "drivers/ms5611/ms5611.h"

/*****************************************************************************/
/* DEFINED CONSTANTS                                                         */
/*****************************************************************************/
#define BPS_SDA  13
#define BPS_SCL  14
#define BPS_FREQ 400000

#define SAMPLE_PERIOD_IN_US 25000

/*****************************************************************************/
/* TYPE DEFINITIONS                                                          */
/*****************************************************************************/

/*****************************************************************************/
/* MACRO DEFINITIONS                                                         */
/*****************************************************************************/

/*****************************************************************************/
/* DEFINITION OF GLOBAL CONSTANTS AND VARIABLES                              */
/*****************************************************************************/
static SemaphoreHandle_t readBps;

static hw_timer_t *timer;

/*****************************************************************************/
/* DECLARATION OF LOCAL FUNCTIONS                                            */
/*****************************************************************************/

/*****************************************************************************/
/* DEFINITION OF LOCAL FUNCTIONS                                             */
/*****************************************************************************/
static void tick(void)
{
  BaseType_t xHigherPriorityTaskWoken = pdFALSE;
  xSemaphoreGiveFromISR(readBps, &xHigherPriorityTaskWoken);
  if (pdTRUE == xHigherPriorityTaskWoken) {
    portYIELD_FROM_ISR();
  }

  xSemaphoreGiveFromISR(readImu, &xHigherPriorityTaskWoken);
  if (pdTRUE == xHigherPriorityTaskWoken) {
    portYIELD_FROM_ISR();
  }
}

/*****************************************************************************/
/* DEFINITION OF GLOBAL FUNCTIONS                                            */
/*****************************************************************************/
void PressureReaderThread(void *p)
{
  readBps = xSemaphoreCreateBinary();

  TwoWire ms5611_twi = TwoWire(0);
  MS5611  ms5611     = MS5611(ms5611_twi);

  while (!ms5611.begin(BPS_SDA, BPS_SCL, BPS_FREQ)) {
    Serial.print("MS5611 startup error\n");
    delay(1000);
  }

  Serial.print("MS5611 is ready\n");

  timer = timerBegin(3, 80, true);
  timerAttachInterrupt(timer, tick, true);
  timerAlarmWrite(timer, SAMPLE_PERIOD_IN_US, true);
  timerAlarmEnable(timer);


  while (1) {

    xSemaphoreTake(readBps, portMAX_DELAY);

    bool result = ms5611.convert(MS5611::Osr::OSR_4096);

    if (result) {
      BpsData_t data;
      memset(&data, 0, sizeof(data));

      data.raw.temp        = ms5611.getRawTemp();
      data.raw.pressure    = ms5611.getRawPressure();
      data.cooked.temp     = ms5611.getCompensatedTemp();
      data.cooked.pressure = ms5611.getCompensatedPressure();

      DbDataBpsSet(&data);

      xSemaphoreGive(filterDataReady);

    } else {
      Serial.println("MS5611 conversion error");
    }
  }
}

/****************************** END OF FILE **********************************/
