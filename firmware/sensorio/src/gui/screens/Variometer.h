/**
 * @file Variometer.h
 * @brief
 */

#ifndef VARIOMETER_H
#define VARIOMETER_H

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

  lv_obj_t *variometer_screen_create(lv_style_t *style);

#ifdef __cplusplus
}
#endif  

#endif /* VARIOMETER_H */

/****************************** END OF FILE **********************************/
