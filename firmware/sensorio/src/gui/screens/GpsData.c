/**
 * @file GpsData.c
 * @brief
 */

/*****************************************************************************/
/* INCLUDES                                                                  */
/*****************************************************************************/
#include "GpsData.h"
#include "dashboard/Dashboard.h"

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
static lv_obj_t * label;
static lv_obj_t * gpsdata;
static lv_task_t *task;

/*****************************************************************************/
/* DECLARATION OF LOCAL FUNCTIONS                                            */
/*****************************************************************************/

/*****************************************************************************/
/* DEFINITION OF LOCAL FUNCTIONS                                             */
/*****************************************************************************/
static void refresh_task(lv_task_t *p)
{
  (void)p;

  GpsData_t data;
  DbDataGpsGet(&data);

  lv_table_set_cell_value_fmt(gpsdata, 0, 1, "%s", data.locked ? "Yes" : "No");
  lv_table_set_cell_value_fmt(gpsdata, 1, 1, "%.03f m", data.altitude / 1000.0);
  lv_table_set_cell_value_fmt(gpsdata, 2, 1, "%.03f", data.course / 1000.0);
  lv_table_set_cell_value_fmt(gpsdata, 3, 1, "%f", data.latitude / 1000000.0);
  lv_table_set_cell_value_fmt(gpsdata, 4, 1, "%f", data.longitude / 1000000.0);
  lv_table_set_cell_value_fmt(gpsdata, 5, 1, "%d", data.numOfSatellites);
  lv_table_set_cell_value_fmt(gpsdata, 6, 1, "%d km/h", (int)data.speed);
  lv_table_set_cell_value_fmt(gpsdata,
                              7,
                              1,
                              "%02d/%02d/%04d",
                              data.time.day,
                              data.time.month,
                              data.time.year);
  lv_table_set_cell_value_fmt(gpsdata,
                              8,
                              1,
                              "%02d:%02d:%02d",
                              data.time.hour,
                              data.time.minute,
                              data.time.second);
}

static void event_handler(lv_obj_t *obj, lv_event_t event)
{
  switch (event) {
  case LV_EVENT_FOCUSED: {
    task = lv_task_create(refresh_task, 1000, LV_TASK_PRIO_LOW, NULL);
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
lv_obj_t *gps_data_screen_create(lv_style_t *style)
{
  lv_obj_t *scr = lv_obj_create(NULL, NULL);
  lv_obj_add_style(scr, LV_STATE_DEFAULT, style);
  lv_obj_set_event_cb(scr, event_handler);

  static lv_style_t lstyle;
  lv_style_init(&lstyle);
  lv_style_set_text_font(&lstyle, LV_STATE_DEFAULT, &lv_font_montserrat_24);
  label = lv_label_create(scr, NULL);
  lv_label_set_text(label, "GPS");
  lv_obj_add_style(label, LV_STATE_DEFAULT, style);
  lv_obj_add_style(label, LV_STATE_DEFAULT, &lstyle);
  lv_obj_align(label, scr, LV_ALIGN_IN_TOP_MID, 0, 0);

  static lv_style_t hstyle;
  lv_style_init(&hstyle);
  lv_style_set_text_font(&hstyle, LV_STATE_DEFAULT, &lv_font_montserrat_18);

  static lv_style_t tstyle;
  lv_style_init(&tstyle);
  lv_style_set_pad_all(&tstyle, LV_STATE_DEFAULT, 2);

  gpsdata = lv_table_create(scr, NULL);
  lv_obj_add_style(gpsdata, LV_TABLE_PART_BG, style);
  lv_obj_add_style(gpsdata, LV_TABLE_PART_BG, &tstyle);
  lv_obj_add_style(gpsdata, LV_TABLE_PART_CELL1, style);
  lv_obj_add_style(gpsdata, LV_TABLE_PART_CELL1, &tstyle);
  lv_obj_add_style(gpsdata, LV_TABLE_PART_CELL2, style);
  lv_obj_add_style(gpsdata, LV_TABLE_PART_CELL2, &tstyle);
  lv_obj_add_style(gpsdata, LV_TABLE_PART_CELL2, &hstyle);

  lv_table_set_col_cnt(gpsdata, 2);
  lv_table_set_row_cnt(gpsdata, 9);

  lv_table_set_col_width(gpsdata, 0, lv_obj_get_width(scr) * 0.4);
  lv_table_set_col_width(gpsdata, 1, lv_obj_get_width(scr) * 0.6);

  lv_table_set_cell_type(gpsdata, 0, 0, 1);
  lv_table_set_cell_type(gpsdata, 0, 1, 1);
  lv_table_set_cell_type(gpsdata, 1, 0, 1);
  lv_table_set_cell_type(gpsdata, 1, 1, 1);
  lv_table_set_cell_type(gpsdata, 2, 0, 1);
  lv_table_set_cell_type(gpsdata, 2, 1, 1);
  lv_table_set_cell_type(gpsdata, 3, 0, 1);
  lv_table_set_cell_type(gpsdata, 3, 1, 1);
  lv_table_set_cell_type(gpsdata, 4, 0, 1);
  lv_table_set_cell_type(gpsdata, 4, 1, 1);
  lv_table_set_cell_type(gpsdata, 5, 0, 1);
  lv_table_set_cell_type(gpsdata, 5, 1, 1);
  lv_table_set_cell_type(gpsdata, 6, 0, 1);
  lv_table_set_cell_type(gpsdata, 6, 1, 1);
  lv_table_set_cell_type(gpsdata, 7, 0, 1);
  lv_table_set_cell_type(gpsdata, 7, 1, 1);
  lv_table_set_cell_type(gpsdata, 8, 0, 1);
  lv_table_set_cell_type(gpsdata, 8, 1, 1);

  lv_obj_align(gpsdata, label, LV_ALIGN_OUT_BOTTOM_MID, 0, 10);
  lv_table_set_cell_align(gpsdata, 0, 0, LV_LABEL_ALIGN_LEFT);
  lv_table_set_cell_align(gpsdata, 0, 1, LV_LABEL_ALIGN_LEFT);
  lv_table_set_cell_align(gpsdata, 1, 0, LV_LABEL_ALIGN_LEFT);
  lv_table_set_cell_align(gpsdata, 1, 1, LV_LABEL_ALIGN_LEFT);
  lv_table_set_cell_align(gpsdata, 2, 0, LV_LABEL_ALIGN_LEFT);
  lv_table_set_cell_align(gpsdata, 2, 1, LV_LABEL_ALIGN_LEFT);
  lv_table_set_cell_align(gpsdata, 3, 0, LV_LABEL_ALIGN_LEFT);
  lv_table_set_cell_align(gpsdata, 3, 1, LV_LABEL_ALIGN_LEFT);
  lv_table_set_cell_align(gpsdata, 4, 0, LV_LABEL_ALIGN_LEFT);
  lv_table_set_cell_align(gpsdata, 4, 1, LV_LABEL_ALIGN_LEFT);
  lv_table_set_cell_align(gpsdata, 5, 0, LV_LABEL_ALIGN_LEFT);
  lv_table_set_cell_align(gpsdata, 5, 1, LV_LABEL_ALIGN_LEFT);
  lv_table_set_cell_align(gpsdata, 6, 0, LV_LABEL_ALIGN_LEFT);
  lv_table_set_cell_align(gpsdata, 6, 1, LV_LABEL_ALIGN_LEFT);
  lv_table_set_cell_align(gpsdata, 7, 0, LV_LABEL_ALIGN_LEFT);
  lv_table_set_cell_align(gpsdata, 7, 1, LV_LABEL_ALIGN_LEFT);
  lv_table_set_cell_align(gpsdata, 8, 0, LV_LABEL_ALIGN_LEFT);
  lv_table_set_cell_align(gpsdata, 8, 1, LV_LABEL_ALIGN_LEFT);

  lv_table_set_cell_value(gpsdata, 0, 0, "Locked");
  lv_table_set_cell_value(gpsdata, 1, 0, "Altitude");
  lv_table_set_cell_value(gpsdata, 2, 0, "Course");
  lv_table_set_cell_value(gpsdata, 3, 0, "Latitude");
  lv_table_set_cell_value(gpsdata, 4, 0, "Longitude");
  lv_table_set_cell_value(gpsdata, 5, 0, "Num. of sat.");
  lv_table_set_cell_value(gpsdata, 6, 0, "Speed");
  lv_table_set_cell_value(gpsdata, 7, 0, "Date");
  lv_table_set_cell_value(gpsdata, 8, 0, "Time");

  return scr;
}

/****************************** END OF FILE **********************************/
