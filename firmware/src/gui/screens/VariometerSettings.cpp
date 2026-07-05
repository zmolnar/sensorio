/**
 * @file VariometerSettings.c
 * @brief
 */

/*****************************************************************************/
/* INCLUDES                                                                  */
/*****************************************************************************/
#include "VariometerSettings.h"
#include "Ui.h"

#include <dashboard/Config.hpp>
#include "gui/Sensorio.h"

/*****************************************************************************/
/* DEFINITION OF GLOBAL CONSTANTS AND VARIABLES                              */
/*****************************************************************************/
static lv_group_t *group;

static lv_obj_t *scr;
static lv_obj_t *period_slider;
static lv_obj_t *period_text;
static lv_obj_t *volume_list;
static lv_obj_t *utcoffset_spinbox;
static lv_obj_t *save_button;
static lv_obj_t *cancel_button;

/*****************************************************************************/
/* DEFINITION OF LOCAL FUNCTIONS                                             */
/*****************************************************************************/
static void event_handler(lv_obj_t *obj, lv_event_t event)
{
  switch (event) {
  case LV_EVENT_FOCUSED: {
    lv_group_add_obj(group, period_slider);
    lv_group_add_obj(group, volume_list);
    lv_group_add_obj(group, utcoffset_spinbox);
    lv_group_add_obj(group, save_button);
    lv_group_add_obj(group, cancel_button);
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
    lv_label_set_text_fmt(period_text, "%u s", lv_slider_get_value(slider));
  }
}

static void utc_increment_event_cb(lv_obj_t *btn, lv_event_t e)
{
  (void)btn;

  if (e == LV_EVENT_SHORT_CLICKED || e == LV_EVENT_LONG_PRESSED_REPEAT) {
    lv_spinbox_increment(utcoffset_spinbox);
  }
}

static void utc_decrement_event_cb(lv_obj_t *btn, lv_event_t e)
{
  (void)btn;

  if (e == LV_EVENT_SHORT_CLICKED || e == LV_EVENT_LONG_PRESSED_REPEAT) {
    lv_spinbox_decrement(utcoffset_spinbox);
  }
}

static void save_button_handler(lv_obj_t *btn, lv_event_t e)
{
  (void)btn;

  if (e == LV_EVENT_SHORT_CLICKED || e == LV_EVENT_LONG_PRESSED_REPEAT) {
    uint32_t period = (uint32_t)lv_slider_get_value(period_slider);
    int32_t  offset = (int32_t)lv_spinbox_get_value(utcoffset_spinbox);
    int16_t  option = lv_dropdown_get_selected(volume_list);

    Config::System system {config.system.get()};
    system.location.utcOffset = offset;
    system.beep.set(option);
    config.system.set(system);

    Config::Gui gui {config.gui.get()};
    gui.screens.variometer.chartRefreshPeriod = period * 1000;
    config.gui.set(gui);

    config.save();

    SensorioLoadEncoderGroup();
  }
}

static void cancel_button_handler(lv_obj_t *btn, lv_event_t e)
{
  (void)btn;

  if (e == LV_EVENT_SHORT_CLICKED || e == LV_EVENT_LONG_PRESSED_REPEAT) {
    SensorioLoadEncoderGroup();
  }
}

/*****************************************************************************/
/* DEFINITION OF GLOBAL FUNCTIONS                                            */
/*****************************************************************************/
lv_obj_t *variometer_settings_screen_create(lv_style_t *style)
{
  using namespace Gui;

  scr = Ui::screen(style);
  lv_obj_set_event_cb(scr, event_handler);

  Ui::header(scr, "SETTINGS");

  Config::Gui gui_params {config.gui.get()};
  Config::System sys_params {config.system.get()};

  lv_obj_t *chart_panel =
      Ui::panel(scr, Ui::Margin, 40, Ui::ScreenWidth - 2 * Ui::Margin, 94,
                nullptr);
  Ui::label(chart_panel, LV_SYMBOL_REFRESH, Ui::icon_style(), 10, 25, 34,
            LV_LABEL_ALIGN_CENTER);
  period_text = Ui::label(chart_panel,
                          "1 s",
                          Ui::body_right_style(),
                          122,
                          26,
                          90,
                          LV_LABEL_ALIGN_RIGHT);
  period_slider = lv_slider_create(chart_panel, NULL);
  lv_obj_set_event_cb(period_slider, slider_event_handler);
  lv_obj_set_pos(period_slider, 12, 60);
  lv_obj_set_width(period_slider, Ui::ScreenWidth - 2 * Ui::Margin - 24);
  lv_slider_set_range(period_slider, 1, 20);
  uint32_t period = gui_params.screens.variometer.chartRefreshPeriod / 1000;
  lv_slider_set_value(period_slider, (int16_t)period, LV_ANIM_OFF);
  lv_label_set_text_fmt(period_text, "%u s", period);

  lv_obj_t *audio_panel =
      Ui::panel(scr, Ui::Margin, 146, Ui::ScreenWidth - 2 * Ui::Margin, 72,
                nullptr);
  Ui::label(audio_panel, LV_SYMBOL_BELL, Ui::icon_style(), 10, 27, 34,
            LV_LABEL_ALIGN_CENTER);
  volume_list = lv_dropdown_create(audio_panel, NULL);
  lv_dropdown_set_options(volume_list,
                          "OFF\n"
                          "Low\n"
                          "Medium\n"
                          "High");
  lv_dropdown_set_selected(volume_list,
                           static_cast<int16_t>(sys_params.beep.volume));
  lv_obj_set_pos(volume_list, 92, 22);
  lv_obj_set_width(volume_list, 120);

  lv_obj_t *time_panel =
      Ui::panel(scr, Ui::Margin, 230, Ui::ScreenWidth - 2 * Ui::Margin, 84,
                nullptr);
  Ui::label(time_panel, "UTC", Ui::body_style(), 10, 31, 52);
  utcoffset_spinbox = lv_spinbox_create(time_panel, NULL);
  lv_spinbox_set_range(utcoffset_spinbox, -12, 12);
  lv_spinbox_set_digit_format(utcoffset_spinbox, 2, 0);
  lv_spinbox_step_prev(utcoffset_spinbox);
  lv_spinbox_set_value(utcoffset_spinbox, sys_params.location.utcOffset);
  lv_obj_set_width(utcoffset_spinbox, 58);
  lv_obj_set_pos(utcoffset_spinbox, 112, 26);

  lv_obj_t *btn = lv_btn_create(time_panel, NULL);
  Ui::style_button(btn, false);
  lv_obj_set_size(btn, 32, 32);
  lv_obj_set_pos(btn, 178, 25);
  lv_obj_set_style_local_value_str(
      btn, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LV_SYMBOL_PLUS);
  lv_obj_set_event_cb(btn, utc_increment_event_cb);

  btn = lv_btn_create(time_panel, NULL);
  Ui::style_button(btn, false);
  lv_obj_set_size(btn, 32, 32);
  lv_obj_set_pos(btn, 74, 25);
  lv_obj_set_style_local_value_str(
      btn, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LV_SYMBOL_MINUS);
  lv_obj_set_event_cb(btn, utc_decrement_event_cb);

  save_button = lv_btn_create(scr, NULL);
  Ui::style_button(save_button, true);
  lv_obj_set_style_local_value_str(
      save_button, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LV_SYMBOL_SAVE);
  lv_obj_set_size(save_button, 104, 40);
  lv_obj_set_pos(save_button, Ui::Margin, 350);
  lv_obj_set_event_cb(save_button, save_button_handler);

  cancel_button = lv_btn_create(scr, NULL);
  Ui::style_button(cancel_button, false);
  lv_obj_set_style_local_value_str(
      cancel_button, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LV_SYMBOL_CLOSE);
  lv_obj_set_size(cancel_button, 104, 40);
  lv_obj_set_pos(cancel_button, 128, 350);
  lv_obj_set_event_cb(cancel_button, cancel_button_handler);

  group = SensorioGetEncoderGroup();

  return scr;
}

/****************************** END OF FILE **********************************/
