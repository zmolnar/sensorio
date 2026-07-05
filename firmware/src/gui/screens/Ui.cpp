/**
 * @file Ui.cpp
 * @brief Shared LVGL helpers for the Sensorio monochrome UI.
 */

#include "Ui.h"

#include "dashboard/Dashboard.hpp"

namespace Gui {
namespace Ui {
namespace {

  bool initialized{false};

  lv_style_t screen_style_obj;
  lv_style_t header_style_obj;
  lv_style_t header_text_style_obj;
  lv_style_t panel_style_obj;
  lv_style_t rule_style_obj;
  lv_style_t caption_style_obj;
  lv_style_t icon_style_obj;
  lv_style_t large_icon_style_obj;
  lv_style_t body_style_obj;
  lv_style_t body_right_style_obj;
  lv_style_t value_style_obj;
  lv_style_t large_value_style_obj;
  lv_style_t hero_value_style_obj;
  lv_style_t unit_style_obj;
  lv_style_t chart_bg_style_obj;
  lv_style_t chart_series_style_obj;
  lv_style_t chart_series_bg_style_obj;
  lv_style_t bar_bg_style_obj;
  lv_style_t bar_indic_style_obj;
  lv_style_t button_style_obj;
  lv_style_t primary_button_style_obj;
  lv_style_t control_style_obj;

  struct HeaderBattery {
    lv_obj_t * label{nullptr};
    lv_task_t *task{nullptr};
  };

  HeaderBattery header_batteries[12];

  const char *battery_symbol_for_percentage(uint32_t percentage)
  {
    if (percentage <= 25) {
      return LV_SYMBOL_BATTERY_1;
    }
    if (percentage <= 50) {
      return LV_SYMBOL_BATTERY_2;
    }
    if (percentage <= 75) {
      return LV_SYMBOL_BATTERY_3;
    }
    return LV_SYMBOL_BATTERY_FULL;
  }

  void header_battery_refresh(lv_task_t *task)
  {
    Dashboard::Battery battery{dashboard.battery.get()};

    lv_label_set_text(static_cast<lv_obj_t *>(task->user_data),
                      battery_symbol_for_percentage(battery.percentage));
  }

  void unregister_header_battery(lv_obj_t *label)
  {
    for (HeaderBattery &battery : header_batteries) {
      if (battery.label == label) {
        if (battery.task) {
          lv_task_del(battery.task);
        }
        battery = {};
        return;
      }
    }
  }

  void header_battery_event(lv_obj_t *obj, lv_event_t event)
  {
    if (event == LV_EVENT_DELETE) {
      unregister_header_battery(obj);
    }
  }

  void register_header_battery(lv_obj_t *label)
  {
    lv_task_t *task =
        lv_task_create(header_battery_refresh, 1000, LV_TASK_PRIO_LOW, label);
    if (!task) {
      return;
    }

    for (HeaderBattery &battery : header_batteries) {
      if (!battery.label) {
        battery.label = label;
        battery.task  = task;
        lv_obj_set_event_cb(label, header_battery_event);
        header_battery_refresh(task);
        return;
      }
    }

    lv_task_del(task);
  }

  void init_text_style(lv_style_t *style, const lv_font_t *font)
  {
    lv_style_init(style);
    lv_style_set_bg_opa(style, LV_STATE_DEFAULT, LV_OPA_TRANSP);
    lv_style_set_border_width(style, LV_STATE_DEFAULT, 0);
    lv_style_set_text_color(style, LV_STATE_DEFAULT, LV_COLOR_BLACK);
    lv_style_set_text_font(style, LV_STATE_DEFAULT, font);
    lv_style_set_text_letter_space(style, LV_STATE_DEFAULT, 0);
  }

  void init_styles()
  {
    if (initialized) {
      return;
    }

    initialized = true;

    lv_style_init(&screen_style_obj);
    lv_style_set_bg_color(&screen_style_obj, LV_STATE_DEFAULT, LV_COLOR_WHITE);
    lv_style_set_bg_opa(&screen_style_obj, LV_STATE_DEFAULT, LV_OPA_COVER);
    lv_style_set_border_width(&screen_style_obj, LV_STATE_DEFAULT, 0);
    lv_style_set_pad_all(&screen_style_obj, LV_STATE_DEFAULT, 0);
    lv_style_set_text_color(&screen_style_obj, LV_STATE_DEFAULT, LV_COLOR_BLACK);
    lv_style_set_text_font(
        &screen_style_obj, LV_STATE_DEFAULT, &lv_font_montserrat_14);

    lv_style_init(&header_style_obj);
    lv_style_set_bg_color(&header_style_obj, LV_STATE_DEFAULT, LV_COLOR_BLACK);
    lv_style_set_bg_opa(&header_style_obj, LV_STATE_DEFAULT, LV_OPA_COVER);
    lv_style_set_border_width(&header_style_obj, LV_STATE_DEFAULT, 0);
    lv_style_set_radius(&header_style_obj, LV_STATE_DEFAULT, 0);
    lv_style_set_pad_all(&header_style_obj, LV_STATE_DEFAULT, 0);

    init_text_style(&header_text_style_obj, &lv_font_montserrat_18);
    lv_style_set_text_color(
        &header_text_style_obj, LV_STATE_DEFAULT, LV_COLOR_WHITE);

    lv_style_init(&panel_style_obj);
    lv_style_set_bg_color(&panel_style_obj, LV_STATE_DEFAULT, LV_COLOR_WHITE);
    lv_style_set_bg_opa(&panel_style_obj, LV_STATE_DEFAULT, LV_OPA_COVER);
    lv_style_set_border_color(&panel_style_obj, LV_STATE_DEFAULT, LV_COLOR_BLACK);
    lv_style_set_border_opa(&panel_style_obj, LV_STATE_DEFAULT, LV_OPA_COVER);
    lv_style_set_border_width(&panel_style_obj, LV_STATE_DEFAULT, 1);
    lv_style_set_radius(&panel_style_obj, LV_STATE_DEFAULT, 0);
    lv_style_set_pad_all(&panel_style_obj, LV_STATE_DEFAULT, 0);

    lv_style_init(&rule_style_obj);
    lv_style_set_bg_color(&rule_style_obj, LV_STATE_DEFAULT, LV_COLOR_BLACK);
    lv_style_set_bg_opa(&rule_style_obj, LV_STATE_DEFAULT, LV_OPA_COVER);
    lv_style_set_border_width(&rule_style_obj, LV_STATE_DEFAULT, 0);

    init_text_style(&caption_style_obj, &lv_font_montserrat_14);
    init_text_style(&icon_style_obj, &lv_font_montserrat_18);
    init_text_style(&large_icon_style_obj, &lv_font_montserrat_24);
    init_text_style(&body_style_obj, &lv_font_montserrat_18);
    init_text_style(&body_right_style_obj, &lv_font_montserrat_18);
    init_text_style(&value_style_obj, &lv_font_montserrat_24);
    init_text_style(&large_value_style_obj, &lv_font_montserrat_36);
    init_text_style(&hero_value_style_obj, &lv_font_montserrat_48);
    init_text_style(&unit_style_obj, &lv_font_montserrat_14);

    lv_style_init(&chart_bg_style_obj);
    lv_style_set_bg_color(&chart_bg_style_obj, LV_STATE_DEFAULT, LV_COLOR_WHITE);
    lv_style_set_bg_opa(&chart_bg_style_obj, LV_STATE_DEFAULT, LV_OPA_COVER);
    lv_style_set_border_color(
        &chart_bg_style_obj, LV_STATE_DEFAULT, LV_COLOR_BLACK);
    lv_style_set_border_width(&chart_bg_style_obj, LV_STATE_DEFAULT, 1);
    lv_style_set_pad_left(&chart_bg_style_obj, LV_STATE_DEFAULT, 36);
    lv_style_set_pad_right(&chart_bg_style_obj, LV_STATE_DEFAULT, 6);
    lv_style_set_pad_top(&chart_bg_style_obj, LV_STATE_DEFAULT, 8);
    lv_style_set_pad_bottom(&chart_bg_style_obj, LV_STATE_DEFAULT, 8);

    lv_style_init(&chart_series_style_obj);
    lv_style_set_line_color(
        &chart_series_style_obj, LV_STATE_DEFAULT, LV_COLOR_BLACK);
    lv_style_set_line_width(&chart_series_style_obj, LV_STATE_DEFAULT, 2);
    lv_style_set_size(&chart_series_style_obj, LV_STATE_DEFAULT, 0);

    lv_style_init(&chart_series_bg_style_obj);
    lv_style_set_line_color(
        &chart_series_bg_style_obj, LV_STATE_DEFAULT, LV_COLOR_BLACK);
    lv_style_set_line_width(&chart_series_bg_style_obj, LV_STATE_DEFAULT, 1);
    lv_style_set_line_dash_width(&chart_series_bg_style_obj, LV_STATE_DEFAULT, 2);
    lv_style_set_line_dash_gap(&chart_series_bg_style_obj, LV_STATE_DEFAULT, 3);

    lv_style_init(&bar_bg_style_obj);
    lv_style_set_bg_color(&bar_bg_style_obj, LV_STATE_DEFAULT, LV_COLOR_WHITE);
    lv_style_set_bg_opa(&bar_bg_style_obj, LV_STATE_DEFAULT, LV_OPA_COVER);
    lv_style_set_border_color(&bar_bg_style_obj, LV_STATE_DEFAULT, LV_COLOR_BLACK);
    lv_style_set_border_width(&bar_bg_style_obj, LV_STATE_DEFAULT, 1);
    lv_style_set_radius(&bar_bg_style_obj, LV_STATE_DEFAULT, 0);
    lv_style_set_pad_all(&bar_bg_style_obj, LV_STATE_DEFAULT, 0);

    lv_style_init(&bar_indic_style_obj);
    lv_style_set_bg_color(&bar_indic_style_obj, LV_STATE_DEFAULT, LV_COLOR_BLACK);
    lv_style_set_bg_opa(&bar_indic_style_obj, LV_STATE_DEFAULT, LV_OPA_COVER);
    lv_style_set_radius(&bar_indic_style_obj, LV_STATE_DEFAULT, 0);

    lv_style_init(&button_style_obj);
    lv_style_set_bg_color(&button_style_obj, LV_STATE_DEFAULT, LV_COLOR_WHITE);
    lv_style_set_bg_opa(&button_style_obj, LV_STATE_DEFAULT, LV_OPA_COVER);
    lv_style_set_border_color(&button_style_obj, LV_STATE_DEFAULT, LV_COLOR_BLACK);
    lv_style_set_border_width(&button_style_obj, LV_STATE_DEFAULT, 1);
    lv_style_set_radius(&button_style_obj, LV_STATE_DEFAULT, 0);
    lv_style_set_value_color(&button_style_obj, LV_STATE_DEFAULT, LV_COLOR_BLACK);
    lv_style_set_value_font(
        &button_style_obj, LV_STATE_DEFAULT, &lv_font_montserrat_18);

    lv_style_init(&primary_button_style_obj);
    lv_style_set_bg_color(
        &primary_button_style_obj, LV_STATE_DEFAULT, LV_COLOR_BLACK);
    lv_style_set_bg_opa(
        &primary_button_style_obj, LV_STATE_DEFAULT, LV_OPA_COVER);
    lv_style_set_border_width(&primary_button_style_obj, LV_STATE_DEFAULT, 0);
    lv_style_set_radius(&primary_button_style_obj, LV_STATE_DEFAULT, 0);
    lv_style_set_value_color(
        &primary_button_style_obj, LV_STATE_DEFAULT, LV_COLOR_WHITE);
    lv_style_set_value_font(
        &primary_button_style_obj, LV_STATE_DEFAULT, &lv_font_montserrat_18);

    lv_style_init(&control_style_obj);
    lv_style_set_bg_color(&control_style_obj, LV_STATE_DEFAULT, LV_COLOR_WHITE);
    lv_style_set_bg_opa(&control_style_obj, LV_STATE_DEFAULT, LV_OPA_COVER);
    lv_style_set_border_color(&control_style_obj, LV_STATE_DEFAULT, LV_COLOR_BLACK);
    lv_style_set_border_width(&control_style_obj, LV_STATE_DEFAULT, 1);
    lv_style_set_radius(&control_style_obj, LV_STATE_DEFAULT, 0);
    lv_style_set_pad_all(&control_style_obj, LV_STATE_DEFAULT, 4);
  }

} // namespace

lv_obj_t *screen(lv_style_t *base_style)
{
  init_styles();

  lv_obj_t *scr = lv_obj_create(NULL, NULL);
  if (base_style) {
    lv_obj_add_style(scr, LV_OBJ_PART_MAIN, base_style);
  }
  lv_obj_add_style(scr, LV_OBJ_PART_MAIN, &screen_style_obj);
  lv_obj_set_size(scr, ScreenWidth, ScreenHeight);

  return scr;
}

const char *battery_symbol(uint32_t percentage)
{
  return battery_symbol_for_percentage(percentage);
}

lv_obj_t *header(lv_obj_t *parent, const char *title)
{
  init_styles();

  lv_obj_t *bar = lv_cont_create(parent, NULL);
  lv_cont_set_fit(bar, LV_FIT_NONE);
  lv_cont_set_layout(bar, LV_LAYOUT_OFF);
  lv_obj_add_style(bar, LV_CONT_PART_MAIN, &header_style_obj);
  lv_obj_set_pos(bar, 0, 0);
  lv_obj_set_size(bar, ScreenWidth, HeaderHeight);

  lv_obj_t *title_label =
      label(bar, title, &header_text_style_obj, 8, 5, 168, LV_LABEL_ALIGN_LEFT);

  (void)title_label;

  lv_obj_t *battery_label = label(bar,
                                  LV_SYMBOL_BATTERY_1,
                                  &header_text_style_obj,
                                  ScreenWidth - 40,
                                  5,
                                  32,
                                  LV_LABEL_ALIGN_RIGHT);
  register_header_battery(battery_label);

  return bar;
}

lv_obj_t *panel(lv_obj_t *parent,
                lv_coord_t x,
                lv_coord_t y,
                lv_coord_t w,
                lv_coord_t h,
                const char *title)
{
  init_styles();

  lv_obj_t *box = lv_cont_create(parent, NULL);
  lv_cont_set_fit(box, LV_FIT_NONE);
  lv_cont_set_layout(box, LV_LAYOUT_OFF);
  lv_obj_add_style(box, LV_CONT_PART_MAIN, &panel_style_obj);
  lv_obj_set_pos(box, x, y);
  lv_obj_set_size(box, w, h);

  if (title) {
    label(box, title, &caption_style_obj, 6, 4, w - 12, LV_LABEL_ALIGN_LEFT);
  }

  return box;
}

lv_obj_t *label(lv_obj_t *parent,
                const char *text,
                lv_style_t *style,
                lv_coord_t x,
                lv_coord_t y,
                lv_coord_t w,
                lv_label_align_t align)
{
  init_styles();

  lv_obj_t *obj = lv_label_create(parent, NULL);
  lv_obj_add_style(obj, LV_LABEL_PART_MAIN, style);
  lv_label_set_long_mode(obj, LV_LABEL_LONG_CROP);
  lv_label_set_align(obj, align);
  lv_obj_set_width(obj, w);
  lv_obj_set_pos(obj, x, y);
  lv_label_set_text(obj, text);

  return obj;
}

lv_obj_t *rule(lv_obj_t *parent, lv_coord_t x, lv_coord_t y, lv_coord_t w)
{
  init_styles();

  lv_obj_t *obj = lv_obj_create(parent, NULL);
  lv_obj_add_style(obj, LV_OBJ_PART_MAIN, &rule_style_obj);
  lv_obj_set_pos(obj, x, y);
  lv_obj_set_size(obj, w, 1);

  return obj;
}

void style_chart(lv_obj_t *chart)
{
  init_styles();

  lv_obj_add_style(chart, LV_CHART_PART_BG, &chart_bg_style_obj);
  lv_obj_add_style(chart, LV_CHART_PART_SERIES, &chart_series_style_obj);
  lv_obj_add_style(chart, LV_CHART_PART_SERIES_BG, &chart_series_bg_style_obj);
}

void style_bar(lv_obj_t *bar)
{
  init_styles();

  lv_obj_add_style(bar, LV_BAR_PART_BG, &bar_bg_style_obj);
  lv_obj_add_style(bar, LV_BAR_PART_INDIC, &bar_indic_style_obj);
}

void style_button(lv_obj_t *button, bool primary)
{
  init_styles();

  lv_obj_add_style(
      button, LV_BTN_PART_MAIN, primary ? &primary_button_style_obj
                                        : &button_style_obj);
}

void style_control(lv_obj_t *control)
{
  init_styles();

  lv_obj_add_style(control, LV_OBJ_PART_MAIN, &control_style_obj);
}

lv_style_t *icon_style()
{
  init_styles();
  return &icon_style_obj;
}

lv_style_t *large_icon_style()
{
  init_styles();
  return &large_icon_style_obj;
}

lv_style_t *caption_style()
{
  init_styles();
  return &caption_style_obj;
}

lv_style_t *body_style()
{
  init_styles();
  return &body_style_obj;
}

lv_style_t *body_right_style()
{
  init_styles();
  return &body_right_style_obj;
}

lv_style_t *value_style()
{
  init_styles();
  return &value_style_obj;
}

lv_style_t *large_value_style()
{
  init_styles();
  return &large_value_style_obj;
}

lv_style_t *hero_value_style()
{
  init_styles();
  return &hero_value_style_obj;
}

lv_style_t *unit_style()
{
  init_styles();
  return &unit_style_obj;
}

} // namespace Ui
} // namespace Gui
