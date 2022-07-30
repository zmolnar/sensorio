/**
 * @file Sensorio.h
 * @brief
 */

#ifndef SENSORIO_H
#define SENSORIO_H

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
#if defined(SIMULATOR)
extern lv_group_t *encgroup;
#endif

/*****************************************************************************/
/* DECLARATION OF GLOBAL FUNCTIONS                                           */
/*****************************************************************************/
#ifdef __cplusplus
extern "C"
{
#endif

  void        SensorioStart(void);
  void        SensorioStop(void);
  void        SensorioStartupFinished(void);
  lv_group_t *SensorioGetEncoderGroup(void);
  void        SensorioClearEncoderGroup(void);
  void        SensorioLoadEncoderGroup(void);
  void        SensorioConfirmExit(void);

#ifdef __cplusplus
}
#endif

#endif /* SENSORIO_H */

/****************************** END OF FILE **********************************/
