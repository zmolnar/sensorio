/**
 * @file SystemStatus.c
 * @brief
 */

/*****************************************************************************/
/* INCLUDES                                                                  */
/*****************************************************************************/
#include "SystemStatus.h"
#include "Ui.h"

#include <dashboard/Dashboard.hpp>

/*****************************************************************************/
/* DEFINITION OF GLOBAL CONSTANTS AND VARIABLES                              */
/*****************************************************************************/
static lv_obj_t * battery_percent_value;
static lv_obj_t * battery_icon;
static lv_obj_t * battery_bar;
static lv_obj_t * battery_voltage_value;
static lv_obj_t * battery_status_value;
static lv_obj_t * battery_adc_value;
static lv_obj_t * usb_value;
static lv_task_t *task;

/*****************************************************************************/
/* DEFINITION OF LOCAL FUNCTIONS                                             */
/*****************************************************************************/
static const char *battery_status_text(Dashboard::Battery::Status status)
{
  switch (status) {
  case Dashboard::Battery::Status::DISCHARGING:
    return "Discharging";
  case Dashboard::Battery::Status::CHARGING:
    return "Charging";
  case Dashboard::Battery::Status::CHARGED:
    return "Charged";
  default:
    return "Unknown";
  }
}

static void refresh_task(lv_task_t *t)
{
  (void)t;

  Dashboard::Battery batt {dashboard.battery.get()};

  lv_label_set_text(battery_icon, Gui::Ui::battery_symbol(batt.percentage));
  lv_label_set_text_fmt(battery_percent_value, "%d%%", batt.percentage);
  lv_bar_set_value(battery_bar, (int16_t)batt.percentage, LV_ANIM_OFF);
  lv_label_set_text_fmt(battery_voltage_value, "%3.02f V", batt.voltage);
  lv_label_set_text(battery_status_value, battery_status_text(batt.status));
  lv_label_set_text_fmt(battery_adc_value, "%d", (int)batt.adcValue);

  Dashboard::Board bd {dashboard.board.get()};

  lv_label_set_text(usb_value, bd.usbConnected ? "Connected" : "Disconnected");
}

static void event_handler(lv_obj_t *obj, lv_event_t event)
{
  switch (event) {
  case LV_EVENT_FOCUSED: {
    task = lv_task_create(refresh_task, 100, LV_TASK_PRIO_LOW, NULL);
    refresh_task(task);
    break;
  }
  case LV_EVENT_DEFOCUSED: {
    lv_task_del(task);
    break;
  }
  default: {
    break;
  }
  }
}

/*****************************************************************************/
/* DEFINITION OF GLOBAL FUNCTIONS                                            */
/*****************************************************************************/
lv_obj_t *system_status_screen_create(lv_style_t *style)
{
  using namespace Gui;

  lv_obj_t *scr = Ui::screen(style);
  lv_obj_set_event_cb(scr, event_handler);

  Ui::header(scr, "SYSTEM");

  lv_obj_t *battery_panel =
      Ui::panel(scr, Ui::Margin, 40, Ui::ScreenWidth - 2 * Ui::Margin, 138,
                nullptr);
  battery_icon = Ui::label(battery_panel,
                           LV_SYMBOL_BATTERY_EMPTY,
                           Ui::large_icon_style(),
                           10,
                           14,
                           42,
                           LV_LABEL_ALIGN_CENTER);
  battery_percent_value = Ui::label(battery_panel,
                                    "0%",
                                    Ui::hero_value_style(),
                                    0,
                                    26,
                                    Ui::ScreenWidth - 2 * Ui::Margin,
                                    LV_LABEL_ALIGN_CENTER);
  battery_bar = lv_bar_create(battery_panel, NULL);
  Ui::style_bar(battery_bar);
  lv_obj_set_pos(battery_bar, 18, 92);
  lv_obj_set_size(battery_bar, Ui::ScreenWidth - 2 * Ui::Margin - 36, 16);
  lv_bar_set_range(battery_bar, 0, 100);
  lv_bar_set_value(battery_bar, 0, LV_ANIM_OFF);
  battery_voltage_value =
      Ui::label(battery_panel, "0.00 V", Ui::body_style(), 12, 113, 90);
  battery_status_value = Ui::label(battery_panel,
                                   "Unknown",
                                   Ui::body_right_style(),
                                   102,
                                   113,
                                   110,
                                   LV_LABEL_ALIGN_RIGHT);

  lv_obj_t *board_panel =
      Ui::panel(scr, Ui::Margin, 188, Ui::ScreenWidth - 2 * Ui::Margin, 74,
                nullptr);
  Ui::label(board_panel, LV_SYMBOL_USB, Ui::icon_style(), 12, 28, 34,
            LV_LABEL_ALIGN_CENTER);
  usb_value = Ui::label(board_panel,
                        "Disconnected",
                        Ui::body_right_style(),
                        52,
                        30,
                        160,
                        LV_LABEL_ALIGN_RIGHT);

  lv_obj_t *debug_panel =
      Ui::panel(scr, Ui::Margin, 272, Ui::ScreenWidth - 2 * Ui::Margin, 118,
                nullptr);
  Ui::label(debug_panel, LV_SYMBOL_CHARGE, Ui::icon_style(), 12, 28, 34,
            LV_LABEL_ALIGN_CENTER);
  Ui::label(debug_panel, "ADC", Ui::body_style(), 52, 32, 90);
  battery_adc_value = Ui::label(debug_panel,
                                "0",
                                Ui::body_right_style(),
                                142,
                                32,
                                70,
                                LV_LABEL_ALIGN_RIGHT);
  return scr;
}

/****************************** END OF FILE **********************************/
