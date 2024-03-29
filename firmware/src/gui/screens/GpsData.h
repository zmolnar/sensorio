/**
 * @file GpsData.h
 * @brief
 */

#ifndef GPS_DATA_H
#define GPS_DATA_H

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

  lv_obj_t *gps_data_screen_create(lv_style_t *style);

#ifdef __cplusplus
}
#endif  

#endif /* GPS_DATA_H */

/****************************** END OF FILE **********************************/
