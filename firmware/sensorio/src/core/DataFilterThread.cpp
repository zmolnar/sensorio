/**
 * @file DataFilterThread.cpp
 * @brief
 */

/*****************************************************************************/
/* INCLUDES                                                                  */
/*****************************************************************************/
#include "DataFilterThread.h"
#include "dashboard/Dashboard.h"

#include <Arduino.h>

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
TaskHandle_t filterTask;

/*****************************************************************************/
/* DECLARATION OF LOCAL FUNCTIONS                                            */
/*****************************************************************************/

/*****************************************************************************/
/* DEFINITION OF LOCAL FUNCTIONS                                             */
/*****************************************************************************/

/*****************************************************************************/
/* DEFINITION OF GLOBAL FUNCTIONS                                            */
/*****************************************************************************/
void DataFilterThread(void *p)
{
  filterTask = xTaskGetCurrentTaskHandle();

  while (1) {
    uint32_t notification = ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
    if (0 < notification) {
      BpsData_t bps;
      DbDataBpsGet(&bps);

      ImuData_t imu;
      DbDataImuGet(&imu);

      TickType_t time = xTaskGetTickCount();

      Serial.printf(
        "%d %d %d %d %d %d %.3f %.3f %.3f %.3f %.3f %.3f\r",
        time, 
        imu.calibration.acc, imu.calibration.gyro, imu.calibration.mag, 
        imu.calibration.sys,
        bps.cooked.pressure,
        imu.gravity.x, imu.gravity.y, imu.gravity.z,
        imu.acceleration.x, imu.acceleration.y, imu.acceleration.z);
    }
  }
}

/****************************** END OF FILE **********************************/
