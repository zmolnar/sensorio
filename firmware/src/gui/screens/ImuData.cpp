/**
 * @file ImuData.c
 * @brief
 */

/*****************************************************************************/
/* INCLUDES                                                                  */
/*****************************************************************************/
#include "ImuData.h"
#include "dashboard/Dashboard.hpp"

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
static lv_obj_t * sensor;
static lv_obj_t * euler;
static lv_obj_t * gravity;
static lv_obj_t * acceleration;
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

  Dashboard::Imu imu {dashboard.imu.get()};

  static const char *status[] = {
      "Idle",
      "Error",
      "Periph. init.",
      "Initializing",
      "Selftest",
      "Fusion",
      "No fusion",
  };

  static const char *clkSource[] = {"Internal", "External"};

  std::size_t statusi = static_cast<std::size_t>(imu.system.status);
  std::size_t clki = static_cast<std::size_t>(imu.system.clock);

  // lv_table_set_cell_value(sensor, 0, 1, status[statusi]);
  // lv_table_set_cell_value(sensor, 1, 1, clkSource[clki]);
  lv_table_set_cell_value_fmt(sensor, 2, 1, "%d", imu.calibration.acc);
  lv_table_set_cell_value_fmt(sensor, 3, 1, "%d", imu.calibration.mag);
  lv_table_set_cell_value_fmt(sensor, 4, 1, "%d", imu.calibration.gyro);
  lv_table_set_cell_value_fmt(sensor, 5, 1, "%d", imu.calibration.sys);

  lv_table_set_cell_value_fmt(euler, 1, 0, "Y: %3.1f", imu.euler.yaw);
  lv_table_set_cell_value_fmt(euler, 1, 1, "P: %3.1f", imu.euler.pitch);
  lv_table_set_cell_value_fmt(euler, 1, 2, "R: %3.1f", imu.euler.roll);

  lv_table_set_cell_value_fmt(gravity, 1, 0, "X: %3.1f", imu.gravity.x);
  lv_table_set_cell_value_fmt(gravity, 1, 1, "Y: %3.1f", imu.gravity.y);
  lv_table_set_cell_value_fmt(gravity, 1, 2, "Z: %3.1f", imu.gravity.z);

  lv_table_set_cell_value_fmt(
      acceleration, 1, 0, "X: %3.1f", imu.acceleration.x);
  lv_table_set_cell_value_fmt(
      acceleration, 1, 1, "Y: %3.1f", imu.acceleration.y);
  lv_table_set_cell_value_fmt(
      acceleration, 1, 2, "Z: %3.1f", imu.acceleration.z);
}

static void event_handler(lv_obj_t *obj, lv_event_t event)
{
  switch (event) {
  case LV_EVENT_FOCUSED: {
    task = lv_task_create(refresh_task, 100, LV_TASK_PRIO_LOW, NULL);
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
lv_obj_t *imu_data_screen_create(lv_style_t *style)
{
  lv_obj_t *scr = lv_obj_create(NULL, NULL);
  lv_obj_add_style(scr, LV_STATE_DEFAULT, style);
  lv_obj_set_event_cb(scr, event_handler);

  static lv_style_t lstyle;
  lv_style_init(&lstyle);
  lv_style_set_text_font(&lstyle, LV_STATE_DEFAULT, &lv_font_montserrat_24);
  label = lv_label_create(scr, NULL);
  lv_label_set_text(label, "IMU");
  lv_obj_add_style(label, LV_STATE_DEFAULT, style);
  lv_obj_add_style(label, LV_STATE_DEFAULT, &lstyle);
  lv_obj_align(label, scr, LV_ALIGN_IN_TOP_MID, 0, 0);

  static lv_style_t hstyle;
  lv_style_init(&hstyle);
  lv_style_set_text_font(&hstyle, LV_STATE_DEFAULT, &lv_font_montserrat_18);

  static lv_style_t tstyle;
  lv_style_init(&tstyle);
  lv_style_set_pad_all(&tstyle, LV_STATE_DEFAULT, 2);

  sensor = lv_table_create(scr, NULL);
  lv_obj_add_style(sensor, LV_TABLE_PART_BG, style);
  lv_obj_add_style(sensor, LV_TABLE_PART_BG, &tstyle);
  lv_obj_add_style(sensor, LV_TABLE_PART_CELL1, style);
  lv_obj_add_style(sensor, LV_TABLE_PART_CELL1, &tstyle);
  lv_obj_add_style(sensor, LV_TABLE_PART_CELL2, style);
  lv_obj_add_style(sensor, LV_TABLE_PART_CELL2, &tstyle);
  lv_obj_add_style(sensor, LV_TABLE_PART_CELL2, &hstyle);

  lv_table_set_col_cnt(sensor, 2);
  lv_table_set_row_cnt(sensor, 6);

  lv_table_set_col_width(sensor, 0, lv_obj_get_width(scr) * 0.4);
  lv_table_set_col_width(sensor, 1, lv_obj_get_width(scr) * 0.6);

  lv_table_set_cell_type(sensor, 0, 0, 1);
  lv_table_set_cell_type(sensor, 0, 1, 1);
  lv_table_set_cell_type(sensor, 1, 0, 1);
  lv_table_set_cell_type(sensor, 1, 1, 1);
  lv_table_set_cell_type(sensor, 2, 0, 1);
  lv_table_set_cell_type(sensor, 2, 1, 1);
  lv_table_set_cell_type(sensor, 3, 0, 1);
  lv_table_set_cell_type(sensor, 3, 1, 1);
  lv_table_set_cell_type(sensor, 4, 0, 1);
  lv_table_set_cell_type(sensor, 4, 1, 1);
  lv_table_set_cell_type(sensor, 5, 0, 1);
  lv_table_set_cell_type(sensor, 5, 1, 1);

  lv_obj_align(sensor, label, LV_ALIGN_OUT_BOTTOM_MID, 0, 10);
  lv_table_set_cell_align(sensor, 0, 0, LV_LABEL_ALIGN_LEFT);
  lv_table_set_cell_align(sensor, 0, 1, LV_LABEL_ALIGN_LEFT);
  lv_table_set_cell_align(sensor, 1, 0, LV_LABEL_ALIGN_LEFT);
  lv_table_set_cell_align(sensor, 1, 1, LV_LABEL_ALIGN_LEFT);
  lv_table_set_cell_align(sensor, 2, 0, LV_LABEL_ALIGN_LEFT);
  lv_table_set_cell_align(sensor, 2, 1, LV_LABEL_ALIGN_LEFT);
  lv_table_set_cell_align(sensor, 3, 0, LV_LABEL_ALIGN_LEFT);
  lv_table_set_cell_align(sensor, 3, 1, LV_LABEL_ALIGN_LEFT);
  lv_table_set_cell_align(sensor, 4, 0, LV_LABEL_ALIGN_LEFT);
  lv_table_set_cell_align(sensor, 4, 1, LV_LABEL_ALIGN_LEFT);
  lv_table_set_cell_align(sensor, 5, 0, LV_LABEL_ALIGN_LEFT);
  lv_table_set_cell_align(sensor, 5, 1, LV_LABEL_ALIGN_LEFT);

  lv_table_set_cell_value(sensor, 0, 0, "Status");
  lv_table_set_cell_value(sensor, 1, 0, "Clock");
  lv_table_set_cell_value(sensor, 2, 0, "Acc");
  lv_table_set_cell_value(sensor, 3, 0, "Mag");
  lv_table_set_cell_value(sensor, 4, 0, "Gyro");
  lv_table_set_cell_value(sensor, 5, 0, "Sys");

  euler = lv_table_create(scr, NULL);
  lv_obj_add_style(euler, LV_TABLE_PART_BG, style);
  lv_obj_add_style(euler, LV_TABLE_PART_BG, &tstyle);
  lv_obj_add_style(euler, LV_TABLE_PART_CELL1, style);
  lv_obj_add_style(euler, LV_TABLE_PART_CELL1, &tstyle);
  lv_obj_add_style(euler, LV_TABLE_PART_CELL2, style);
  lv_obj_add_style(euler, LV_TABLE_PART_CELL2, &tstyle);
  lv_obj_add_style(euler, LV_TABLE_PART_CELL2, &hstyle);

  lv_table_set_col_cnt(euler, 3);
  lv_table_set_row_cnt(euler, 2);

  lv_table_set_col_width(euler, 0, lv_obj_get_width(scr) / 3);
  lv_table_set_col_width(euler, 1, lv_obj_get_width(scr) / 3);
  lv_table_set_col_width(euler, 2, lv_obj_get_width(scr) / 3);

  lv_table_set_cell_type(euler, 0, 0, 2);
  lv_table_set_cell_type(euler, 0, 1, 2);
  lv_table_set_cell_type(euler, 0, 2, 2);
  lv_table_set_cell_type(euler, 1, 0, 1);
  lv_table_set_cell_type(euler, 1, 1, 1);
  lv_table_set_cell_type(euler, 1, 2, 1);

  lv_table_set_cell_merge_right(euler, 0, 0, true);
  lv_table_set_cell_merge_right(euler, 0, 1, true);

  lv_obj_align(euler, sensor, LV_ALIGN_OUT_BOTTOM_MID, 0, 10);
  lv_table_set_cell_align(euler, 0, 0, LV_LABEL_ALIGN_CENTER);
  lv_table_set_cell_align(euler, 1, 0, LV_LABEL_ALIGN_LEFT);
  lv_table_set_cell_align(euler, 1, 1, LV_LABEL_ALIGN_LEFT);
  lv_table_set_cell_align(euler, 1, 2, LV_LABEL_ALIGN_LEFT);

  lv_table_set_cell_value(euler, 0, 0, "Euler");

  gravity = lv_table_create(scr, NULL);
  lv_obj_add_style(gravity, LV_TABLE_PART_BG, style);
  lv_obj_add_style(gravity, LV_TABLE_PART_BG, &tstyle);
  lv_obj_add_style(gravity, LV_TABLE_PART_CELL1, style);
  lv_obj_add_style(gravity, LV_TABLE_PART_CELL1, &tstyle);
  lv_obj_add_style(gravity, LV_TABLE_PART_CELL2, style);
  lv_obj_add_style(gravity, LV_TABLE_PART_CELL2, &tstyle);
  lv_obj_add_style(gravity, LV_TABLE_PART_CELL2, &hstyle);

  lv_table_set_col_cnt(gravity, 3);
  lv_table_set_row_cnt(gravity, 2);

  lv_table_set_col_width(gravity, 0, lv_obj_get_width(scr) / 3);
  lv_table_set_col_width(gravity, 1, lv_obj_get_width(scr) / 3);
  lv_table_set_col_width(gravity, 2, lv_obj_get_width(scr) / 3);

  lv_table_set_cell_type(gravity, 0, 0, 2);
  lv_table_set_cell_type(gravity, 0, 1, 2);
  lv_table_set_cell_type(gravity, 0, 2, 2);
  lv_table_set_cell_type(gravity, 1, 0, 1);
  lv_table_set_cell_type(gravity, 1, 1, 1);
  lv_table_set_cell_type(gravity, 1, 2, 1);

  lv_table_set_cell_merge_right(gravity, 0, 0, true);
  lv_table_set_cell_merge_right(gravity, 0, 1, true);

  lv_obj_align(gravity, euler, LV_ALIGN_OUT_BOTTOM_MID, 0, 10);
  lv_table_set_cell_align(gravity, 0, 0, LV_LABEL_ALIGN_CENTER);
  lv_table_set_cell_align(gravity, 1, 0, LV_LABEL_ALIGN_LEFT);
  lv_table_set_cell_align(gravity, 1, 1, LV_LABEL_ALIGN_LEFT);
  lv_table_set_cell_align(gravity, 1, 2, LV_LABEL_ALIGN_LEFT);

  lv_table_set_cell_value(gravity, 0, 0, "Gravity");

  acceleration = lv_table_create(scr, NULL);
  lv_obj_add_style(acceleration, LV_TABLE_PART_BG, style);
  lv_obj_add_style(acceleration, LV_TABLE_PART_BG, &tstyle);
  lv_obj_add_style(acceleration, LV_TABLE_PART_CELL1, style);
  lv_obj_add_style(acceleration, LV_TABLE_PART_CELL1, &tstyle);
  lv_obj_add_style(acceleration, LV_TABLE_PART_CELL2, style);
  lv_obj_add_style(acceleration, LV_TABLE_PART_CELL2, &tstyle);
  lv_obj_add_style(acceleration, LV_TABLE_PART_CELL2, &hstyle);

  lv_table_set_col_cnt(acceleration, 3);
  lv_table_set_row_cnt(acceleration, 2);

  lv_table_set_col_width(acceleration, 0, lv_obj_get_width(scr) / 3);
  lv_table_set_col_width(acceleration, 1, lv_obj_get_width(scr) / 3);
  lv_table_set_col_width(acceleration, 2, lv_obj_get_width(scr) / 3);

  lv_table_set_cell_type(acceleration, 0, 0, 2);
  lv_table_set_cell_type(acceleration, 0, 1, 2);
  lv_table_set_cell_type(acceleration, 0, 2, 2);
  lv_table_set_cell_type(acceleration, 1, 0, 1);
  lv_table_set_cell_type(acceleration, 1, 1, 1);
  lv_table_set_cell_type(acceleration, 1, 2, 1);

  lv_table_set_cell_merge_right(acceleration, 0, 0, true);
  lv_table_set_cell_merge_right(acceleration, 0, 1, true);

  lv_obj_align(acceleration, gravity, LV_ALIGN_OUT_BOTTOM_MID, 0, 10);
  lv_table_set_cell_align(acceleration, 0, 0, LV_LABEL_ALIGN_CENTER);
  lv_table_set_cell_align(acceleration, 1, 0, LV_LABEL_ALIGN_LEFT);
  lv_table_set_cell_align(acceleration, 1, 1, LV_LABEL_ALIGN_LEFT);
  lv_table_set_cell_align(acceleration, 1, 2, LV_LABEL_ALIGN_LEFT);

  lv_table_set_cell_value(acceleration, 0, 0, "Acceleration");

  return scr;
}

/****************************** END OF FILE **********************************/
