/**
 * @file Startup.c
 * @brief
 */

/*****************************************************************************/
/* INCLUDES                                                                  */
/*****************************************************************************/
#include "Startup.h"
#include "gui/Sensorio.h"

#include <stdint.h>

/*****************************************************************************/
/* DEFINED CONSTANTS                                                         */
/*****************************************************************************/
#define STARTUP_TICK_IN_MS   200
#define STARTUP_LENGTH_IN_MS 5000
#define STARTUP_BAR_INCREMENT                                                  \
  (100 / (STARTUP_LENGTH_IN_MS / STARTUP_TICK_IN_MS))

/*****************************************************************************/
/* TYPE DEFINITIONS                                                          */
/*****************************************************************************/

/*****************************************************************************/
/* MACRO DEFINITIONS                                                         */
/*****************************************************************************/

/*****************************************************************************/
/* DEFINITION OF GLOBAL CONSTANTS AND VARIABLES                              */
/*****************************************************************************/
LV_IMG_DECLARE(paraglider);

/*****************************************************************************/
/* DECLARATION OF LOCAL FUNCTIONS                                            */
/*****************************************************************************/

/*****************************************************************************/
/* DEFINITION OF LOCAL FUNCTIONS                                             */
/*****************************************************************************/
static void bar_increment(lv_task_t *t)
{
  static int16_t a = 0;

  a += STARTUP_BAR_INCREMENT;
  lv_bar_set_value(t->user_data, a, LV_ANIM_ON);

  if (100 <= a) {
    a = 0;
    lv_task_del(t);
    SensorioStartupFinished();
  }
}

/*****************************************************************************/
/* DEFINITION OF GLOBAL FUNCTIONS                                            */
/*****************************************************************************/
lv_obj_t *startup_screen_create(lv_style_t *style)
{
  lv_obj_t *scr = lv_obj_create(NULL, NULL);
  lv_obj_add_style(scr, LV_STATE_DEFAULT, style);

  lv_obj_t *cont = lv_cont_create(scr, NULL);
  lv_obj_add_style(cont, LV_STATE_DEFAULT, style);
  lv_obj_set_auto_realign(cont, true);
  lv_obj_align_origo(cont, NULL, LV_ALIGN_CENTER, 0, 0);
  lv_cont_set_fit(cont, LV_FIT_PARENT);
  lv_cont_set_layout(cont, LV_LAYOUT_CENTER);

  lv_obj_t *img = lv_img_create(scr, NULL);
  lv_img_set_src(img, &paraglider);
  lv_img_set_auto_size(img, true);
  lv_obj_align(img, scr, LV_ALIGN_IN_TOP_MID, 0, 77);

  lv_obj_t *bar = lv_bar_create(scr, NULL);
  lv_obj_set_size(bar, 200, 15);
  lv_obj_align(bar, scr, LV_ALIGN_IN_BOTTOM_MID, 0, -40);
  lv_bar_set_value(bar, 0, LV_ANIM_OFF);
  lv_task_create(bar_increment, STARTUP_TICK_IN_MS, LV_TASK_PRIO_LOWEST, bar);

  return scr;
}

/****************************** END OF FILE **********************************/
