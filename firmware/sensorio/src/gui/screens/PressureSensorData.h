/**
 * @file PressureSensorData.h
 * @brief
 */

#ifndef PRESSURE_SENSOR_DATA_H
#define PRESSURE_SENSOR_DATA_H

/*****************************************************************************/
/* INCLUDES                                                                  */
/*****************************************************************************/
#include "lvgl.h"

/*****************************************************************************/
/* DEFINED CONSTANTS                                                         */
/*****************************************************************************/

/*****************************************************************************/
/* MACRO DEFINITIONS                                                         */
/*****************************************************************************/

/*****************************************************************************/
/* TYPE DEFINITIONS                                                          */
/*****************************************************************************/

/*****************************************************************************/
/* DECLARATION OF GLOBAL VARIABLES                                           */
/*****************************************************************************/

/*****************************************************************************/
/* DECLARATION OF GLOBAL FUNCTIONS                                           */
/*****************************************************************************/
#ifdef __cplusplus
extern "C"
{
#endif

  lv_obj_t *bps_data_screen_create(lv_style_t *style);

#ifdef __cplusplus
}
#endif 
#endif /* PRESSURE_SENSOR_DATA_H */

/****************************** END OF FILE **********************************/
