/**
 * @file PressureSensorData.c
 * @brief
 */

/*****************************************************************************/
/* INCLUDES                                                                  */
/*****************************************************************************/
#include "PressureSensorData.h"
#include "dashboard/Dashboard.h"

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
static lv_obj_t *         label;
static lv_obj_t *         table;
static lv_obj_t *         chart;
static lv_chart_series_t *pseries;
static lv_coord_t         ymax = 0;
static lv_coord_t         ymin = INT16_MAX;

/*****************************************************************************/
/* DECLARATION OF LOCAL FUNCTIONS                                            */
/*****************************************************************************/

/*****************************************************************************/
/* DEFINITION OF LOCAL FUNCTIONS                                             */
/*****************************************************************************/
static void event_handler(lv_obj_t *obj, lv_event_t event)
{
  static uint32_t count  = 0;
  static uint32_t offset = 0;

  if (LV_EVENT_REFRESH == event) {
    BpsData_t data;
    DbDataBpsGet(&data);

    // Update table cells
    char value[20];
    lv_snprintf(
        value, sizeof(value), "%04.02f C", (float)data.cooked.temp / 100.0);
    lv_table_set_cell_value(table, 0, 1, value);

    lv_snprintf(value,
                sizeof(value),
                "%04.02f mb",
                (float)data.cooked.pressure / 100.0);
    lv_table_set_cell_value(table, 1, 1, value);

    // Update static variables
    if (count < lv_chart_get_point_count(chart)) {
      count++;
    }

    if (0 == offset) {
      offset = data.cooked.pressure - (INT16_MAX / 2);
    }

    // Add point to the data series
    lv_coord_t point = (lv_coord_t)(data.cooked.pressure - offset);
    lv_chart_set_next(chart, pseries, point);

    // Search the minimum and maximum of the data series
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

    // Update the chart range if needed
    if (((min - 2) != ymin) || ((max + 2) != ymax)) {
      ymin = min - 2;
      ymax = max + 2;
      lv_chart_set_range(chart, ymin, ymax);
    }

    lv_chart_refresh(chart);

  } else if (LV_EVENT_FOCUSED == event) {
    count  = 0;
    offset = 0;
    ymax   = 0;
    ymin   = INT16_MAX;
    lv_chart_clear_series(chart, pseries);
    lv_chart_refresh(chart);
  }
}

/*****************************************************************************/
/* DEFINITION OF GLOBAL FUNCTIONS                                            */
/*****************************************************************************/
lv_obj_t *bps_data_screen_create(lv_style_t *style)
{
  lv_obj_t *scr = lv_obj_create(NULL, NULL);
  lv_obj_add_style(scr, LV_STATE_DEFAULT, style);
  lv_obj_set_event_cb(scr, event_handler);

  static lv_style_t lstyle;
  lv_style_init(&lstyle);
  lv_style_set_text_font(&lstyle, LV_STATE_DEFAULT, &lv_font_montserrat_24);
  label = lv_label_create(scr, NULL);
  lv_label_set_text(label, "Pressure Sensor");
  lv_obj_add_style(label, LV_STATE_DEFAULT, style);
  lv_obj_add_style(label, LV_STATE_DEFAULT, &lstyle);
  lv_obj_align(label, scr, LV_ALIGN_IN_TOP_MID, 0, 0);

  table = lv_table_create(scr, NULL);
  lv_obj_add_style(table, LV_TABLE_PART_BG, style);
  lv_obj_add_style(table, LV_TABLE_PART_CELL1, style);
  
  lv_table_set_col_cnt(table, 2);
  lv_table_set_row_cnt(table, 2);
  
  lv_table_set_col_width(table, 0, lv_obj_get_width(scr) * 0.55);
  lv_table_set_col_width(table, 1, lv_obj_get_width(scr) * 0.45);

  lv_table_set_cell_type(table, 0, 0, 1);
  lv_table_set_cell_type(table, 0, 1, 1);
  lv_table_set_cell_type(table, 1, 0, 1);
  lv_table_set_cell_type(table, 1, 1, 1);

  lv_obj_align(table, label, LV_ALIGN_OUT_BOTTOM_MID, 0, 0);
  lv_table_set_cell_align(table, 0, 0, LV_LABEL_ALIGN_LEFT);
  lv_table_set_cell_align(table, 0, 1, LV_LABEL_ALIGN_LEFT);
  lv_table_set_cell_align(table, 1, 0, LV_LABEL_ALIGN_LEFT);
  lv_table_set_cell_align(table, 1, 1, LV_LABEL_ALIGN_LEFT);

  lv_table_set_cell_value(table, 0, 0, "Temperature");
  lv_table_set_cell_value(table, 0, 1, "");
  lv_table_set_cell_value(table, 1, 0, "Pressure");
  lv_table_set_cell_value(table, 1, 1, "");

  chart = lv_chart_create(scr, NULL);
  lv_obj_add_style(chart, LV_CHART_PART_BG, style);
  lv_obj_set_width(chart, lv_obj_get_width(scr));
  lv_obj_align(chart, table, LV_ALIGN_OUT_BOTTOM_MID, 0, 0);
  lv_chart_set_type(chart, LV_CHART_TYPE_LINE);

  pseries = lv_chart_add_series(chart, LV_COLOR_BLACK);
  lv_chart_set_update_mode(chart, LV_CHART_UPDATE_MODE_SHIFT);
  lv_chart_set_point_count(chart, 100);
  lv_chart_set_div_line_count(chart, 0, 0);

  return scr;
}

/****************************** END OF FILE **********************************/
