/**
 * @file Encoder.h
 * @brief
 */

#ifndef ENCODER_H
#define ENCODER_H

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
extern "C"
{
  void EncoderInit(void);
  void EncoderRegisterDriver(lv_group_t *group);
}

#endif /* ENCODER_H */

/****************************** END OF FILE **********************************/
