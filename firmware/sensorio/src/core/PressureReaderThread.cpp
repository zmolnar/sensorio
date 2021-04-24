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

#define SAMPLE_PERIOD_IN_MS 20

/*****************************************************************************/
/* TYPE DEFINITIONS                                                          */
/*****************************************************************************/

/*****************************************************************************/
/* MACRO DEFINITIONS                                                         */
/*****************************************************************************/

/*****************************************************************************/
/* DEFINITION OF GLOBAL CONSTANTS AND VARIABLES                              */
/*****************************************************************************/
static TaskHandle_t      bpsTask = NULL;
static SemaphoreHandle_t semaphore;

/*****************************************************************************/
/* DECLARATION OF LOCAL FUNCTIONS                                            */
/*****************************************************************************/

/*****************************************************************************/
/* DEFINITION OF LOCAL FUNCTIONS                                             */
/*****************************************************************************/
static void tick(TimerHandle_t xTimer)
{
  (void)xTimer;

  BaseType_t xHigherPriorityTaskWoken = pdFALSE;
  xSemaphoreGiveFromISR(semaphore, &xHigherPriorityTaskWoken);
  if (pdTRUE == xHigherPriorityTaskWoken) {
    portYIELD_FROM_ISR();
  }

  vTaskNotifyGiveFromISR(imuTask, &xHigherPriorityTaskWoken);
  if (pdTRUE == xHigherPriorityTaskWoken) {
    portYIELD_FROM_ISR();
  }
}

/*****************************************************************************/
/* DEFINITION OF GLOBAL FUNCTIONS                                            */
/*****************************************************************************/
void PressureReaderThread(void *p)
{
  bpsTask   = xTaskGetCurrentTaskHandle();
  semaphore = xSemaphoreCreateBinary();

  TwoWire ms5611_twi = TwoWire(0);
  MS5611  ms5611     = MS5611(ms5611_twi);

  while (!ms5611.begin(BPS_SDA, BPS_SCL, BPS_FREQ)) {
    Serial.print("MS5611 startup error\n");
    delay(1000);
  }

  Serial.print("MS5611 is ready\n");

  TimerHandle_t timerHandle;
  timerHandle = xTimerCreate("data sampling timer",
                             pdMS_TO_TICKS(SAMPLE_PERIOD_IN_MS),
                             pdTRUE,
                             0,
                             tick);
  if (pdPASS != xTimerStart(timerHandle, 0)) {
    Serial.println("Failed to start data sampling timer");
  }

  while (1) {
    xSemaphoreTake(semaphore, portMAX_DELAY);

    bool result = ms5611.convert(MS5611::Osr::OSR_4096);

    if (result) {
      BpsData_t data;
      memset(&data, 0, sizeof(data));

      data.raw.temp        = ms5611.getRawTemp();
      data.raw.pressure    = ms5611.getRawPressure();
      data.cooked.temp     = ms5611.getCompensatedTemp();
      data.cooked.pressure = ms5611.getCompensatedPressure();

      DbDataBpsSet(&data);

      xTaskNotifyGive(filterTask);

    } else {
      Serial.println("MS5611 conversion error");
    }
  }
}

/****************************** END OF FILE **********************************/
