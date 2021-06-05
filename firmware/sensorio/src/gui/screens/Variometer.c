/**
 * @file Variometer.c
 * @brief
 */

/*****************************************************************************/
/* INCLUDES                                                                  */
/*****************************************************************************/
#include "Variometer.h"
#include "VariometerSettings.h"

#include "dashboard/Dashboard.h"
#include "gui/Sensorio.h"

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
static lv_obj_t *         data_table;
static lv_obj_t *         height_chart;
static lv_chart_series_t *height_history;
static uint32_t           chart_refresh_period;

static lv_task_t *task;
static lv_task_t *chart_task;

static lv_obj_t *settings;

/*****************************************************************************/
/* DECLARATION OF LOCAL FUNCTIONS                                            */
/*****************************************************************************/

/*****************************************************************************/
/* DEFINITION OF LOCAL FUNCTIONS                                             */
/*****************************************************************************/
static void chart_refresh(lv_task_t *p)
{
  (void)p;

  FilterOutput_t filter_data;
  DbDataFilterOutputGet(&filter_data);

  lv_coord_t point = (lv_coord_t)filter_data.height;
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

  // Update the scale if needed
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

  // Set time
  LocalTime_t time;
  DbDataLocalTimeGet(&time);

  lv_table_set_cell_value_fmt(data_table,
                              0,
                              0,
                              "%02d:%02d:%02d",
                              time.hour,
                              time.minute,
                              time.second);

  FilterOutput_t filter_data;
  DbDataFilterOutputGet(&filter_data);

  // Set vario
  double vario = filter_data.vario.instant;
  vario        = ((int32_t)(vario * 10.0)) / 10.0;
  const char *symbol;

  if (vario < -0.2) {
    symbol = LV_SYMBOL_DOWN;
  } else if (0.2 < vario) {
    symbol = LV_SYMBOL_UP;
  } else {
    symbol = LV_SYMBOL_MINUS;
  }

  GpsData_t gps_data;
  DbDataGpsGet(&gps_data);

  lv_table_set_cell_value_fmt(data_table, 1, 0, "%2.1f %s", vario, symbol);
  lv_table_set_cell_value_fmt(data_table, 3, 0, "%d", (int)gps_data.speed);
  lv_table_set_cell_value_fmt(data_table, 3, 1, "%d", (int)filter_data.height);
}

static void reschedule_chart_refresh_task(void)
{
  SysParams_t params;
  DbCfgSysParamsGet(&params);

  if (chart_refresh_period != params.screens.vario.chart_refresh_period) {
    chart_refresh_period = params.screens.vario.chart_refresh_period;
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
/* DEFINITION OF GLOBAL FUNCTIONS */
/*****************************************************************************/
lv_obj_t *variometer_screen_create(lv_style_t *style)
{
  settings = variometer_settings_screen_create(style);

  lv_obj_t *scr = lv_obj_create(NULL, NULL);
  lv_obj_add_style(scr, LV_STATE_DEFAULT, style);
  lv_obj_set_event_cb(scr, event_handler);

  static lv_style_t tstyle;
  lv_style_init(&tstyle);
  lv_style_set_pad_all(&tstyle, LV_STATE_DEFAULT, 2);

  static lv_style_t small_style;
  lv_style_init(&small_style);
  lv_style_set_text_font(
      &small_style, LV_STATE_DEFAULT, &lv_font_montserrat_18);

  static lv_style_t big_style;
  lv_style_init(&big_style);
  lv_style_set_text_font(&big_style, LV_STATE_DEFAULT, &lv_font_montserrat_48);
  lv_style_set_pad_top(&big_style, LV_STATE_DEFAULT, 10);
  lv_style_set_pad_bottom(&big_style, LV_STATE_DEFAULT, 10);

  static lv_style_t medium_style;
  lv_style_init(&medium_style);
  lv_style_set_text_font(
      &medium_style, LV_STATE_DEFAULT, &lv_font_montserrat_36);

  data_table = lv_table_create(scr, NULL);
  lv_obj_add_style(data_table, LV_TABLE_PART_BG, style);
  lv_obj_add_style(data_table, LV_TABLE_PART_BG, &tstyle);
  lv_obj_add_style(data_table, LV_TABLE_PART_CELL1, style);
  lv_obj_add_style(data_table, LV_TABLE_PART_CELL1, &tstyle);
  lv_obj_add_style(data_table, LV_TABLE_PART_CELL1, &small_style);
  lv_obj_add_style(data_table, LV_TABLE_PART_CELL2, style);
  lv_obj_add_style(data_table, LV_TABLE_PART_CELL2, &tstyle);
  lv_obj_add_style(data_table, LV_TABLE_PART_CELL2, &medium_style);
  lv_obj_add_style(data_table, LV_TABLE_PART_CELL3, style);
  lv_obj_add_style(data_table, LV_TABLE_PART_CELL3, &tstyle);
  lv_obj_add_style(data_table, LV_TABLE_PART_CELL3, &big_style);

  lv_obj_set_width(data_table, lv_obj_get_width(scr));
  lv_obj_set_height(data_table, lv_obj_get_height(scr) * 0.4);

  lv_table_set_col_cnt(data_table, 2);
  lv_table_set_row_cnt(data_table, 4);

  lv_table_set_col_width(data_table, 0, lv_obj_get_width(scr) * 0.54);
  lv_table_set_col_width(data_table, 1, lv_obj_get_width(scr) * 0.45);

  lv_table_set_cell_type(data_table, 0, 0, 2);
  lv_table_set_cell_type(data_table, 0, 1, 2);
  lv_table_set_cell_type(data_table, 1, 0, 3);
  lv_table_set_cell_type(data_table, 1, 1, 3);
  lv_table_set_cell_type(data_table, 2, 0, 1);
  lv_table_set_cell_type(data_table, 2, 1, 1);
  lv_table_set_cell_type(data_table, 3, 0, 2);
  lv_table_set_cell_type(data_table, 3, 1, 2);

  lv_table_set_cell_merge_right(data_table, 0, 0, true);
  lv_table_set_cell_merge_right(data_table, 1, 0, true);

  lv_obj_align(data_table, scr, LV_ALIGN_IN_TOP_MID, 0, 0);
  lv_table_set_cell_align(data_table, 0, 0, LV_LABEL_ALIGN_CENTER);
  lv_table_set_cell_align(data_table, 1, 0, LV_LABEL_ALIGN_CENTER);
  lv_table_set_cell_align(data_table, 1, 1, LV_LABEL_ALIGN_CENTER);
  lv_table_set_cell_align(data_table, 2, 0, LV_LABEL_ALIGN_CENTER);
  lv_table_set_cell_align(data_table, 2, 1, LV_LABEL_ALIGN_CENTER);
  lv_table_set_cell_align(data_table, 3, 0, LV_LABEL_ALIGN_CENTER);
  lv_table_set_cell_align(data_table, 3, 1, LV_LABEL_ALIGN_CENTER);

  lv_table_set_cell_value(data_table, 2, 0, "Speed (km/h)");
  lv_table_set_cell_value(data_table, 2, 1, "AMSL (m)");

  static lv_style_t chart_style;
  lv_style_init(&chart_style);
  lv_style_set_pad_left(&chart_style, LV_STATE_DEFAULT, 65);

  height_chart = lv_chart_create(scr, NULL);
  lv_obj_add_style(height_chart, LV_CHART_PART_BG, style);
  lv_obj_add_style(height_chart, LV_CHART_PART_BG, &chart_style);
  lv_obj_set_width(height_chart, lv_obj_get_width(scr));
  lv_obj_set_height(height_chart, lv_obj_get_height(scr) * 0.5);
  lv_obj_align(height_chart, scr, LV_ALIGN_IN_BOTTOM_MID, 0, 0);
  lv_chart_set_type(height_chart, LV_CHART_TYPE_LINE);

  height_history = lv_chart_add_series(height_chart, LV_COLOR_BLACK);
  lv_chart_set_update_mode(height_chart, LV_CHART_UPDATE_MODE_SHIFT);
  lv_chart_set_point_count(height_chart, 100);
  lv_chart_set_div_line_count(height_chart, 0, 2);

  return scr;
}

/****************************** END OF FILE **********************************/
