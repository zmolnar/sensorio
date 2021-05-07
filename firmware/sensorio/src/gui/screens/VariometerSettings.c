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
static lv_obj_t *qnh_label;
static lv_obj_t *qnh_spinbox;
static lv_obj_t *volume_label;
static lv_obj_t *volume_list;

/*****************************************************************************/
/* DECLARATION OF LOCAL FUNCTIONS                                            */
/*****************************************************************************/

/*****************************************************************************/
/* DEFINITION OF LOCAL FUNCTIONS                                             */
/*****************************************************************************/
static VolumeLevel_t convert_volume_option_to_enum(int16_t option)
{
  static const VolumeLevel_t options[4] = {
    [0] = VOL_ZERO,
    [1] = VOL_LOW,
    [2] = VOL_MEDIUM,
    [3] = VOL_HIGH,
  };

  VolumeLevel_t vol = VOL_ZERO;
  if ((0 <= option) && (option <= 3)) {
    vol = options[option];
  }

  return vol;
}

static void mbox_event_handler(lv_obj_t *obj, lv_event_t event)
{
  if (LV_EVENT_VALUE_CHANGED == event) {
    uint16_t id = lv_msgbox_get_active_btn(obj);

    if (0 == id) {
      uint32_t period = (uint32_t)lv_slider_get_value(period_slider);

      SysParams_t params;
      DbCfgSysParamsGet(&params);
      params.screens.vario.chart_refresh_period = period * 1000;
      DbCfgSysParamsSet(&params);

      BeepSettings_t beep;
      DbCfgBeepSettingsGet(&beep);
      int16_t option = lv_dropdown_get_selected(volume_list);
      beep.volume = convert_volume_option_to_enum(option);
      DbCfgBeepSettingsSet(&beep);
      
      DbCfgSaveToEeprom();
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
    lv_group_add_obj(group, qnh_spinbox);
    lv_group_add_obj(group, volume_list);
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

static void lv_spinbox_increment_event_cb(lv_obj_t *btn, lv_event_t e)
{
  if (e == LV_EVENT_SHORT_CLICKED || e == LV_EVENT_LONG_PRESSED_REPEAT) {
    lv_spinbox_increment(qnh_spinbox);
  }
}

static void lv_spinbox_decrement_event_cb(lv_obj_t *btn, lv_event_t e)
{
  if (e == LV_EVENT_SHORT_CLICKED || e == LV_EVENT_LONG_PRESSED_REPEAT) {
    lv_spinbox_decrement(qnh_spinbox);
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
  DbCfgSysParamsGet(&params);

  uint32_t period = params.screens.vario.chart_refresh_period / 1000;
  lv_slider_set_value(period_slider, (int16_t)period, LV_ANIM_OFF);

  period_text = lv_label_create(scr, NULL);
  lv_label_set_text_fmt(period_text, "%d", period);
  lv_obj_set_auto_realign(period_text, true);
  lv_obj_align(period_text, period_slider, LV_ALIGN_OUT_BOTTOM_MID, 0, 10);

  qnh_label = lv_label_create(scr, NULL);
  lv_label_set_text(qnh_label, "Pressure at sea level");
  lv_obj_align(qnh_label, period_slider, LV_ALIGN_OUT_BOTTOM_MID, 0, 50);

  qnh_spinbox = lv_spinbox_create(scr, NULL);
  lv_spinbox_set_range(qnh_spinbox, 95000, 105000);
  lv_spinbox_set_digit_format(qnh_spinbox, 6, 4);
  lv_spinbox_step_prev(qnh_spinbox);
  lv_obj_set_width(qnh_spinbox, 100);
  lv_obj_align(qnh_spinbox, qnh_label, LV_ALIGN_OUT_BOTTOM_MID, 0, 0);

  lv_coord_t h   = lv_obj_get_height(qnh_spinbox);
  lv_obj_t * btn = lv_btn_create(scr, NULL);
  lv_obj_set_size(btn, h, h);
  lv_obj_align(btn, qnh_spinbox, LV_ALIGN_OUT_RIGHT_MID, 5, 0);
  lv_theme_apply(btn, LV_THEME_SPINBOX_BTN);
  lv_obj_set_style_local_value_str(
      btn, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LV_SYMBOL_PLUS);
  lv_obj_set_event_cb(btn, lv_spinbox_increment_event_cb);

  btn = lv_btn_create(scr, btn);
  lv_obj_align(btn, qnh_spinbox, LV_ALIGN_OUT_LEFT_MID, -5, 0);
  lv_obj_set_event_cb(btn, lv_spinbox_decrement_event_cb);
  lv_obj_set_style_local_value_str(
      btn, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LV_SYMBOL_MINUS);

  volume_label = lv_label_create(scr, NULL);
  lv_label_set_text(volume_label, "Volume");
  lv_obj_align(volume_label, qnh_spinbox, LV_ALIGN_OUT_BOTTOM_MID, 0, 50);

  BeepSettings_t beep;
  DbCfgBeepSettingsGet(&beep);

  volume_list = lv_dropdown_create(scr, NULL);
  lv_dropdown_set_options(volume_list,
                          "OFF\n"
                          "Low\n"
                          "Medium\n"
                          "High");
  lv_dropdown_set_selected(volume_list, (int16_t)beep.volume);
  lv_obj_align(volume_list, volume_label, LV_ALIGN_OUT_BOTTOM_MID, 0, 0);

  group = SensorioGetEncoderGroup();

  return scr;
}

/****************************** END OF FILE **********************************/
