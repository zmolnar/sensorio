/**
 * @file Startup.cpp
 * @brief
 */

/*****************************************************************************/
/* INCLUDES                                                                  */
/*****************************************************************************/
#include "Startup.h"
#include "Ui.h"
#include "gui/Sensorio.h"

#include <stdint.h>
#if defined(SIMULATOR)
#include <stdlib.h>
#endif

/*****************************************************************************/
/* DEFINED CONSTANTS                                                         */
/*****************************************************************************/
#define STARTUP_TICK_IN_MS   200
#define STARTUP_LENGTH_IN_MS 5000

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
static lv_style_t title_style;
static lv_style_t subtitle_style;
static lv_style_t bar_bg_style;
static lv_style_t bar_indic_style;
static uint8_t    styles_initialized = 0;

/*****************************************************************************/
/* DECLARATION OF LOCAL FUNCTIONS                                            */
/*****************************************************************************/

/*****************************************************************************/
/* DEFINITION OF LOCAL FUNCTIONS                                             */
/*****************************************************************************/
static void bar_increment(lv_task_t *t)
{
  static uint32_t elapsed_ms = 0;

  uint32_t startup_length_ms = STARTUP_LENGTH_IN_MS;
#if defined(SIMULATOR)
  const char *startup_length_env = getenv("SENSORIO_SIM_STARTUP_MS");
  if (startup_length_env && startup_length_env[0]) {
    uint32_t value = (uint32_t)strtoul(startup_length_env, NULL, 10);
    if (0 < value) {
      startup_length_ms = value;
    }
  }
#endif
  elapsed_ms += STARTUP_TICK_IN_MS;
  if (startup_length_ms < elapsed_ms) {
    elapsed_ms = startup_length_ms;
  }

  int16_t percent = (int16_t)((elapsed_ms * 100U) / startup_length_ms);
  lv_bar_set_value(static_cast<lv_obj_t *>(t->user_data), percent, LV_ANIM_ON);

  if (startup_length_ms <= elapsed_ms) {
    elapsed_ms = 0;
    lv_task_del(t);
    SensorioStartupFinished();
  }
}

static void init_styles(void)
{
  if (styles_initialized) {
    return;
  }

  styles_initialized = 1;

  lv_style_init(&title_style);
  lv_style_set_bg_opa(&title_style, LV_STATE_DEFAULT, LV_OPA_TRANSP);
  lv_style_set_text_color(&title_style, LV_STATE_DEFAULT, LV_COLOR_BLACK);
  lv_style_set_text_font(&title_style, LV_STATE_DEFAULT, &lv_font_montserrat_36);

  lv_style_init(&subtitle_style);
  lv_style_set_bg_opa(&subtitle_style, LV_STATE_DEFAULT, LV_OPA_TRANSP);
  lv_style_set_text_color(&subtitle_style, LV_STATE_DEFAULT, LV_COLOR_BLACK);
  lv_style_set_text_font(
      &subtitle_style, LV_STATE_DEFAULT, &lv_font_montserrat_14);

  lv_style_init(&bar_bg_style);
  lv_style_set_bg_color(&bar_bg_style, LV_STATE_DEFAULT, LV_COLOR_WHITE);
  lv_style_set_bg_opa(&bar_bg_style, LV_STATE_DEFAULT, LV_OPA_COVER);
  lv_style_set_border_color(&bar_bg_style, LV_STATE_DEFAULT, LV_COLOR_BLACK);
  lv_style_set_border_width(&bar_bg_style, LV_STATE_DEFAULT, 1);
  lv_style_set_radius(&bar_bg_style, LV_STATE_DEFAULT, 0);

  lv_style_init(&bar_indic_style);
  lv_style_set_bg_color(&bar_indic_style, LV_STATE_DEFAULT, LV_COLOR_BLACK);
  lv_style_set_bg_opa(&bar_indic_style, LV_STATE_DEFAULT, LV_OPA_COVER);
  lv_style_set_radius(&bar_indic_style, LV_STATE_DEFAULT, 0);
}

/*****************************************************************************/
/* DEFINITION OF GLOBAL FUNCTIONS                                            */
/*****************************************************************************/
lv_obj_t *startup_screen_create(lv_style_t *style)
{
  using namespace Gui;

  init_styles();

  lv_obj_t *scr = Ui::screen(style);
  Ui::header(scr, "SENSORIO");

  lv_obj_t *title = lv_label_create(scr, NULL);
  lv_obj_add_style(title, LV_LABEL_PART_MAIN, &title_style);
  lv_label_set_text(title, "Sensorio");
  lv_obj_set_width(title, LV_HOR_RES_MAX);
  lv_label_set_align(title, LV_LABEL_ALIGN_CENTER);
  lv_obj_set_pos(title, 0, 58);

  lv_obj_t *subtitle = lv_label_create(scr, NULL);
  lv_obj_add_style(subtitle, LV_LABEL_PART_MAIN, &subtitle_style);
  lv_label_set_text(subtitle, "VARIOMETER");
  lv_obj_set_width(subtitle, LV_HOR_RES_MAX);
  lv_label_set_align(subtitle, LV_LABEL_ALIGN_CENTER);
  lv_obj_set_pos(subtitle, 0, 103);

  lv_obj_t *img = lv_img_create(scr, NULL);
  lv_img_set_src(img, &paraglider);
  lv_img_set_auto_size(img, true);
  lv_obj_align(img, scr, LV_ALIGN_IN_TOP_MID, 0, 142);

  lv_obj_t *bar = lv_bar_create(scr, NULL);
  lv_obj_add_style(bar, LV_BAR_PART_BG, &bar_bg_style);
  lv_obj_add_style(bar, LV_BAR_PART_INDIC, &bar_indic_style);
  lv_obj_set_size(bar, 200, 16);
  lv_obj_align(bar, scr, LV_ALIGN_IN_BOTTOM_MID, 0, -34);
  lv_bar_set_value(bar, 0, LV_ANIM_OFF);
  lv_task_create(bar_increment, STARTUP_TICK_IN_MS, LV_TASK_PRIO_LOWEST, bar);

  return scr;
}

/****************************** END OF FILE **********************************/
