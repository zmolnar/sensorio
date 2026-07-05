/**
 * @file Variometer.c
 * @brief
 */

/*****************************************************************************/
/* INCLUDES                                                                  */
/*****************************************************************************/
#include "Variometer.h"
#include "Ui.h"
#include "VariometerSettings.h"

#include <dashboard/Config.hpp>
#include <dashboard/Dashboard.hpp>
#include "gui/Sensorio.h"

#if defined(SIMULATOR)
#include <stdlib.h>
#include <string.h>
#endif

/*****************************************************************************/
/* DEFINITION OF GLOBAL CONSTANTS AND VARIABLES                              */
/*****************************************************************************/
static lv_obj_t *         vario_value;
static lv_obj_t *         vario_symbol;
static lv_obj_t *         speed_value;
static lv_obj_t *         altitude_value;
static lv_obj_t *         height_chart;
static lv_chart_series_t *height_history;
static uint32_t           chart_refresh_period;

static lv_task_t *task;
static lv_task_t *chart_task;

static lv_obj_t *settings;

/*****************************************************************************/
/* DEFINITION OF LOCAL FUNCTIONS                                             */
/*****************************************************************************/
static void chart_refresh(lv_task_t *p)
{
  (void)p;

  Dashboard::Filter filter {dashboard.filter.get()};

  lv_coord_t point = (lv_coord_t)filter.height;
  lv_chart_set_next(height_chart, height_history, point);

  static uint32_t point_count = 0;
  if (point_count < lv_chart_get_point_count(height_chart)) {
    ++point_count;
  }

  lv_coord_t min = point;
  lv_coord_t max = point;
  for (uint16_t i = 0; i < point_count; ++i) {
    lv_coord_t pi = height_history->points[i];
    if (pi < min) {
      min = pi;
    }

    if (max < pi) {
      max = pi;
    }
  }

  static lv_coord_t height_min;
  static lv_coord_t height_max;

  if (((min - 2) != height_min) || ((max + 2) != height_max)) {
    height_min = min / 100 * 100;
    height_max = (max / 100 + 1) * 100;
    lv_chart_set_range(height_chart, height_min, height_max);

    static char labels[200];
    char *      end    = labels;
    size_t      length = sizeof(labels);

    for (uint32_t tick = height_max; tick > height_min; tick -= 100) {
      size_t offset = lv_snprintf(end, length, "%d\n", tick);
      end += offset;
      length -= offset;
    }

    lv_snprintf(end, length, "%d", height_min);
    lv_chart_set_y_tick_texts(
        height_chart, labels, 0, LV_CHART_AXIS_DRAW_LAST_TICK);
  }

  lv_chart_refresh(height_chart);
}

static void refresh_task(lv_task_t *p)
{
  (void)p;

  Dashboard::Gps gps {dashboard.gps.get()};
  Dashboard::Filter filter {dashboard.filter.get()};

  double vario = filter.vario.instant;
  vario        = ((int32_t)(vario * 10.0)) / 10.0;

  if (vario < -0.2) {
    lv_label_set_text(vario_symbol, LV_SYMBOL_DOWN);
  } else if (0.2 < vario) {
    lv_label_set_text(vario_symbol, LV_SYMBOL_UP);
  } else {
    lv_label_set_text(vario_symbol, LV_SYMBOL_MINUS);
  }

  lv_label_set_text_fmt(vario_value, "%+.1f", vario);
  lv_label_set_text_fmt(speed_value, "%d", (int)gps.speed);
  lv_label_set_text_fmt(altitude_value, "%d", (int)filter.height);
}

static void reschedule_chart_refresh_task(void)
{
  Config::Gui gui {config.gui.get()};

  if (chart_refresh_period != gui.screens.variometer.chartRefreshPeriod) {
    chart_refresh_period = gui.screens.variometer.chartRefreshPeriod;
    if (chart_task) {
      lv_task_del(chart_task);
    }
    chart_task = lv_task_create(
        chart_refresh, chart_refresh_period, LV_TASK_PRIO_LOW, NULL);
  }
}

static void event_handler(lv_obj_t *obj, lv_event_t event)
{
  switch (event) {
  case LV_EVENT_FOCUSED: {
    reschedule_chart_refresh_task();
    task = lv_task_create(refresh_task, 100, LV_TASK_PRIO_LOW, NULL);
    refresh_task(task);
    break;
  }
  case LV_EVENT_DEFOCUSED: {
    lv_task_del(task);
    break;
  }
  case LV_EVENT_SHORT_CLICKED: {
    SensorioClearEncoderGroup();
    lv_group_add_obj(SensorioGetEncoderGroup(), settings);
    lv_scr_load(settings);

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
lv_obj_t *variometer_screen_create(lv_style_t *style)
{
  using namespace Gui;

  settings = variometer_settings_screen_create(style);
#if defined(SIMULATOR)
  const char *screen = getenv("SENSORIO_SIM_SCREEN");
  if (screen && 0 == strcmp(screen, "settings")) {
    return settings;
  }
#endif

  lv_obj_t *scr = Ui::screen(style);
  lv_obj_set_event_cb(scr, event_handler);

  Ui::header(scr, "VARIO");

  lv_obj_t *main_panel =
      Ui::panel(scr, Ui::Margin, 40, Ui::ScreenWidth - 2 * Ui::Margin, 104,
                nullptr);
  Ui::label(main_panel, LV_SYMBOL_UP, Ui::icon_style(), 8, 5, 26,
            LV_LABEL_ALIGN_CENTER);
  vario_value = Ui::label(
      main_panel, "+0.0", Ui::hero_value_style(), 14, 27, 146, LV_LABEL_ALIGN_CENTER);
  vario_symbol = Ui::label(
      main_panel, LV_SYMBOL_MINUS, Ui::large_value_style(), 160, 28, 50,
      LV_LABEL_ALIGN_CENTER);
  Ui::label(main_panel, "meters / second", Ui::unit_style(), 14, 78, 146,
            LV_LABEL_ALIGN_CENTER);

  lv_obj_t *speed_panel = Ui::panel(scr, Ui::Margin, 154, 108, 68, nullptr);
  Ui::label(speed_panel, LV_SYMBOL_RIGHT, Ui::icon_style(), 8, 4, 24,
            LV_LABEL_ALIGN_CENTER);
  speed_value = Ui::label(
      speed_panel, "0", Ui::value_style(), 0, 28, 108, LV_LABEL_ALIGN_CENTER);
  Ui::label(speed_panel, "km/h", Ui::unit_style(), 0, 50, 108, LV_LABEL_ALIGN_CENTER);

  lv_obj_t *alt_panel =
      Ui::panel(scr, 124, 154, 108, 68, nullptr);
  Ui::label(alt_panel, LV_SYMBOL_UPLOAD, Ui::icon_style(), 8, 4, 24,
            LV_LABEL_ALIGN_CENTER);
  altitude_value = Ui::label(
      alt_panel, "0", Ui::value_style(), 0, 28, 108, LV_LABEL_ALIGN_CENTER);
  Ui::label(alt_panel, "m AMSL", Ui::unit_style(), 0, 50, 108, LV_LABEL_ALIGN_CENTER);

  Ui::label(scr,
            LV_SYMBOL_REFRESH,
            Ui::icon_style(),
            Ui::Margin,
            230,
            Ui::ScreenWidth - 2 * Ui::Margin,
            LV_LABEL_ALIGN_CENTER);

  height_chart = lv_chart_create(scr, NULL);
  Ui::style_chart(height_chart);
  lv_obj_set_pos(height_chart, Ui::Margin, 250);
  lv_obj_set_size(height_chart, Ui::ScreenWidth - 2 * Ui::Margin, 140);
  lv_chart_set_type(height_chart, LV_CHART_TYPE_LINE);

  height_history = lv_chart_add_series(height_chart, LV_COLOR_BLACK);
  lv_chart_set_update_mode(height_chart, LV_CHART_UPDATE_MODE_SHIFT);
  lv_chart_set_point_count(height_chart, 80);
  lv_chart_set_div_line_count(height_chart, 3, 0);

  return scr;
}

/****************************** END OF FILE **********************************/
