/**
 * @file BpsData.c
 * @brief
 */

/*****************************************************************************/
/* INCLUDES                                                                  */
/*****************************************************************************/
#include "BpsData.h"
#include "Ui.h"
#include "dashboard/Dashboard.hpp"

#include <cstdint>

/*****************************************************************************/
/* DEFINITION OF GLOBAL CONSTANTS AND VARIABLES                              */
/*****************************************************************************/
static lv_obj_t *         temperature_value;
static lv_obj_t *         pressure_value;
static lv_obj_t *         chart;
static lv_chart_series_t *pseries;
static lv_task_t *        task;

static uint32_t   count  = 0;
static uint32_t   offset = 0;
static lv_coord_t ymax   = 0;
static lv_coord_t ymin   = INT16_MAX;

/*****************************************************************************/
/* DEFINITION OF LOCAL FUNCTIONS                                             */
/*****************************************************************************/
static void refresh_task(lv_task_t *p)
{
  (void)p;

  Dashboard::Bps bps {dashboard.bps.get()};

  lv_label_set_text_fmt(
      temperature_value, "%.1f", (float)bps.cooked.temp / 100.0);
  lv_label_set_text_fmt(
      pressure_value, "%.1f", (float)bps.cooked.pressure / 100.0);

  if (count < lv_chart_get_point_count(chart)) {
    count++;
  }

  if (0 == offset) {
    offset = bps.cooked.pressure - (INT16_MAX / 2);
  }

  lv_coord_t point = (lv_coord_t)(bps.cooked.pressure - offset);
  lv_chart_set_next(chart, pseries, point);

  lv_coord_t min = point;
  lv_coord_t max = point;
  for (uint16_t i = 0; i < count; ++i) {
    lv_coord_t pi = pseries->points[i];
    if (pi < min) {
      min = pi;
    }

    if (max < pi) {
      max = pi;
    }
  }

  if (((min - 2) != ymin) || ((max + 2) != ymax)) {
    ymin = min - 2;
    ymax = max + 2;
    lv_chart_set_range(chart, ymin, ymax);
  }

  lv_chart_refresh(chart);
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
    count  = 0;
    offset = 0;
    ymax   = 0;
    ymin   = INT16_MAX;
    lv_chart_clear_series(chart, pseries);
    lv_chart_refresh(chart);
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
lv_obj_t *bps_data_screen_create(lv_style_t *style)
{
  using namespace Gui;

  lv_obj_t *scr = Ui::screen(style);
  lv_obj_set_event_cb(scr, event_handler);

  Ui::header(scr, "BARO");

  lv_obj_t *temp_panel =
      Ui::panel(scr, Ui::Margin, 40, 108, 86, "TEMP");
  temperature_value = Ui::label(
      temp_panel, "0.0", Ui::large_value_style(), 0, 28, 108, LV_LABEL_ALIGN_CENTER);
  Ui::label(
      temp_panel, "deg C", Ui::unit_style(), 0, 61, 108, LV_LABEL_ALIGN_CENTER);

  lv_obj_t *pressure_panel = Ui::panel(scr, 124, 40, 108, 86, "PRESSURE");
  pressure_value = Ui::label(pressure_panel,
                             "0.0",
                             Ui::value_style(),
                             0,
                             32,
                             108,
                             LV_LABEL_ALIGN_CENTER);
  Ui::label(
      pressure_panel, "mbar", Ui::unit_style(), 0, 62, 108, LV_LABEL_ALIGN_CENTER);

  Ui::label(scr,
            LV_SYMBOL_REFRESH,
            Ui::icon_style(),
            Ui::Margin,
            142,
            Ui::ScreenWidth - 2 * Ui::Margin,
            LV_LABEL_ALIGN_CENTER);

  chart = lv_chart_create(scr, NULL);
  Ui::style_chart(chart);
  lv_obj_set_pos(chart, Ui::Margin, 162);
  lv_obj_set_size(chart, Ui::ScreenWidth - 2 * Ui::Margin, 228);
  lv_chart_set_type(chart, LV_CHART_TYPE_LINE);

  pseries = lv_chart_add_series(chart, LV_COLOR_BLACK);
  lv_chart_set_update_mode(chart, LV_CHART_UPDATE_MODE_SHIFT);
  lv_chart_set_point_count(chart, 100);
  lv_chart_set_div_line_count(chart, 4, 0);

  return scr;
}

/****************************** END OF FILE **********************************/
