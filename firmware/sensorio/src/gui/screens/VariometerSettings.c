/**
 * @file VariometerSettings.c
 * @brief
 */

/*****************************************************************************/
/* INCLUDES                                                                  */
/*****************************************************************************/
#include "VariometerSettings.h"

#include "dashboard/Dashboard.h"
#include "gui/Sensorio.h"

#include <stdio.h>

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
static lv_group_t *group;

static lv_obj_t *scr;
static lv_obj_t *label;
static lv_obj_t *period_label;
static lv_obj_t *period_slider;
static lv_obj_t *period_text;

/*****************************************************************************/
/* DECLARATION OF LOCAL FUNCTIONS                                            */
/*****************************************************************************/

/*****************************************************************************/
/* DEFINITION OF LOCAL FUNCTIONS                                             */
/*****************************************************************************/
static void mbox_event_handler(lv_obj_t *obj, lv_event_t event)
{
  if (LV_EVENT_VALUE_CHANGED == event) {
    uint16_t id = lv_msgbox_get_active_btn(obj);

    if (0 == id) {
      uint32_t period = (uint32_t)lv_slider_get_value(period_slider);

      DbParamsLock();
      SysParams_t params;
      DbParamsGet(&params);
      params.screens.vario.chart_refresh_period = period * 1000;
      DbParamsSet(&params);
      DbParamsUnlock();
    } else {
      ;
    }

    SensorioLoadEncoderGroup();
    lv_obj_del(obj);
  }
}

static void event_handler(lv_obj_t *obj, lv_event_t event)
{
  switch (event) {
  case LV_EVENT_FOCUSED: {
    lv_group_add_obj(group, period_slider);
    break;
  }
  case LV_EVENT_DEFOCUSED: {
    break;
  }
  case LV_EVENT_LONG_PRESSED: {
    lv_obj_t *mbox = lv_msgbox_create(lv_scr_act(), NULL);
    lv_msgbox_set_text(mbox, "Save?");
    static const char *buttons[] = {"Yes", "No", ""};
    lv_msgbox_add_btns(mbox, buttons);
    lv_obj_set_width(mbox, 200);
    lv_obj_set_event_cb(mbox, mbox_event_handler);
    lv_obj_align(mbox, lv_scr_act(), LV_ALIGN_CENTER, 0, 0);

    lv_group_remove_all_objs(group);
    lv_group_add_obj(group, mbox);
    lv_group_set_editing(group, true);
    break;
  }
  default: {
    break;
  }
  }
}

static void slider_event_handler(lv_obj_t *slider, lv_event_t event)
{
  if (event == LV_EVENT_VALUE_CHANGED) {
    lv_label_set_text_fmt(period_text, "%u", lv_slider_get_value(slider));
  }
}

/*****************************************************************************/
/* DEFINITION OF GLOBAL FUNCTIONS                                            */
/*****************************************************************************/
lv_obj_t *variometer_settings_screen_create(lv_style_t *style)
{
  scr = lv_obj_create(NULL, NULL);
  lv_obj_add_style(scr, LV_STATE_DEFAULT, style);
  lv_obj_set_event_cb(scr, event_handler);

  static lv_style_t lstyle;
  lv_style_init(&lstyle);
  lv_style_set_text_font(&lstyle, LV_STATE_DEFAULT, &lv_font_montserrat_24);
  label = lv_label_create(scr, NULL);
  lv_label_set_text(label, "Settings");
  lv_obj_add_style(label, LV_STATE_DEFAULT, style);
  lv_obj_add_style(label, LV_STATE_DEFAULT, &lstyle);
  lv_obj_align(label, scr, LV_ALIGN_IN_TOP_MID, 0, 0);

  period_label = lv_label_create(scr, NULL);
  lv_label_set_text(period_label, "Height chart refresh period");
  lv_obj_align(period_label, scr, LV_ALIGN_IN_TOP_MID, 0, 50);

  period_slider = lv_slider_create(scr, NULL);
  lv_obj_set_event_cb(period_slider, slider_event_handler);
  lv_obj_set_width(period_slider, lv_obj_get_width(scr) * 0.8);
  lv_slider_set_range(period_slider, 1, 20);
  lv_obj_align(period_slider, period_label, LV_ALIGN_OUT_BOTTOM_MID, 0, 10);

  SysParams_t params;
  DbParamsLock();
  DbParamsGet(&params);
  DbParamsUnlock();

  uint32_t period = params.screens.vario.chart_refresh_period / 1000;
  lv_slider_set_value(period_slider, (int16_t)period, LV_ANIM_OFF);

  period_text = lv_label_create(scr, NULL);
  lv_label_set_text_fmt(period_text, "%d", period);
  lv_obj_set_auto_realign(period_text, true);
  lv_obj_align(period_text, period_slider, LV_ALIGN_OUT_BOTTOM_MID, 0, 10);

  group = SensorioGetEncoderGroup();

  return scr;
}

/****************************** END OF FILE **********************************/
