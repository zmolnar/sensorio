/**
 * @file ImuData.c
 * @brief
 */

/*****************************************************************************/
/* INCLUDES                                                                  */
/*****************************************************************************/
#include "ImuData.h"
#include "Ui.h"
#include "dashboard/Dashboard.hpp"

/*****************************************************************************/
/* DEFINITION OF GLOBAL CONSTANTS AND VARIABLES                              */
/*****************************************************************************/
static lv_obj_t * status_value;
static lv_obj_t * clock_value;
static lv_obj_t * acc_cal_value;
static lv_obj_t * mag_cal_value;
static lv_obj_t * gyro_cal_value;
static lv_obj_t * sys_cal_value;
static lv_obj_t * yaw_value;
static lv_obj_t * pitch_value;
static lv_obj_t * roll_value;
static lv_obj_t * gravity_x_value;
static lv_obj_t * gravity_y_value;
static lv_obj_t * gravity_z_value;
static lv_obj_t * acceleration_x_value;
static lv_obj_t * acceleration_y_value;
static lv_obj_t * acceleration_z_value;
static lv_task_t *task;

/*****************************************************************************/
/* DEFINITION OF LOCAL FUNCTIONS                                             */
/*****************************************************************************/
static const char *status_text(Dashboard::Imu::Status status)
{
  switch (status) {
  case Dashboard::Imu::Status::IDLE:
    return "Idle";
  case Dashboard::Imu::Status::ERROR:
    return "Error";
  case Dashboard::Imu::Status::PERIPHERAL_INIT:
    return "Periph init";
  case Dashboard::Imu::Status::INITIALIZING:
    return "Init";
  case Dashboard::Imu::Status::RUNNING_SELFTEST:
    return "Selftest";
  case Dashboard::Imu::Status::RUNNING_FUSION:
    return "Fusion";
  case Dashboard::Imu::Status::RUNNING_NO_FUSION:
    return "No fusion";
  default:
    return "Unknown";
  }
}

static const char *clock_text(Dashboard::Imu::ClockSource clock)
{
  switch (clock) {
  case Dashboard::Imu::ClockSource::INTERNAL:
    return "Internal";
  case Dashboard::Imu::ClockSource::EXTERNAL:
    return "External";
  default:
    return "Unknown";
  }
}

static void refresh_task(lv_task_t *p)
{
  (void)p;

  Dashboard::Imu imu {dashboard.imu.get()};

  lv_label_set_text(status_value, status_text(imu.system.status));
  lv_label_set_text(clock_value, clock_text(imu.system.clock));

  lv_label_set_text_fmt(acc_cal_value, "%d", imu.calibration.acc);
  lv_label_set_text_fmt(mag_cal_value, "%d", imu.calibration.mag);
  lv_label_set_text_fmt(gyro_cal_value, "%d", imu.calibration.gyro);
  lv_label_set_text_fmt(sys_cal_value, "%d", imu.calibration.sys);

  lv_label_set_text_fmt(yaw_value, "%3.1f", imu.euler.yaw);
  lv_label_set_text_fmt(pitch_value, "%3.1f", imu.euler.pitch);
  lv_label_set_text_fmt(roll_value, "%3.1f", imu.euler.roll);

  lv_label_set_text_fmt(gravity_x_value, "%3.1f", imu.gravity.x);
  lv_label_set_text_fmt(gravity_y_value, "%3.1f", imu.gravity.y);
  lv_label_set_text_fmt(gravity_z_value, "%3.1f", imu.gravity.z);

  lv_label_set_text_fmt(acceleration_x_value, "%3.1f", imu.acceleration.x);
  lv_label_set_text_fmt(acceleration_y_value, "%3.1f", imu.acceleration.y);
  lv_label_set_text_fmt(acceleration_z_value, "%3.1f", imu.acceleration.z);
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

static lv_obj_t *axis_value(lv_obj_t *parent, const char *axis, lv_coord_t x)
{
  using namespace Gui;

  Ui::label(parent, axis, Ui::caption_style(), x, 16, 52, LV_LABEL_ALIGN_CENTER);
  return Ui::label(
      parent, "0.0", Ui::body_style(), x, 31, 52, LV_LABEL_ALIGN_CENTER);
}

/*****************************************************************************/
/* DEFINITION OF GLOBAL FUNCTIONS                                            */
/*****************************************************************************/
lv_obj_t *imu_data_screen_create(lv_style_t *style)
{
  using namespace Gui;

  lv_obj_t *scr = Ui::screen(style);
  lv_obj_set_event_cb(scr, event_handler);

  Ui::header(scr, "IMU");

  lv_obj_t *status_panel =
      Ui::panel(scr, Ui::Margin, 40, Ui::ScreenWidth - 2 * Ui::Margin, 68,
                nullptr);
  Ui::label(status_panel, LV_SYMBOL_SETTINGS, Ui::icon_style(), 8, 5, 24,
            LV_LABEL_ALIGN_CENTER);
  Ui::label(status_panel, "Status", Ui::body_style(), 10, 25, 70);
  status_value = Ui::label(
      status_panel, "Unknown", Ui::body_right_style(), 82, 25, 130,
      LV_LABEL_ALIGN_RIGHT);
  Ui::label(status_panel, "Clock", Ui::caption_style(), 10, 46, 70);
  clock_value = Ui::label(status_panel,
                          "Unknown",
                          Ui::caption_style(),
                          82,
                          46,
                          130,
                          LV_LABEL_ALIGN_RIGHT);

  lv_obj_t *cal_panel =
      Ui::panel(scr, Ui::Margin, 118, Ui::ScreenWidth - 2 * Ui::Margin, 72,
                nullptr);
  Ui::label(cal_panel, LV_SYMBOL_OK, Ui::icon_style(), 8, 5, 24,
            LV_LABEL_ALIGN_CENTER);
  Ui::label(cal_panel, "ACC", Ui::caption_style(), 8, 24, 48, LV_LABEL_ALIGN_CENTER);
  Ui::label(cal_panel, "MAG", Ui::caption_style(), 62, 24, 48, LV_LABEL_ALIGN_CENTER);
  Ui::label(cal_panel, "GYR", Ui::caption_style(), 116, 24, 48, LV_LABEL_ALIGN_CENTER);
  Ui::label(cal_panel, "SYS", Ui::caption_style(), 168, 24, 48, LV_LABEL_ALIGN_CENTER);
  acc_cal_value =
      Ui::label(cal_panel, "0", Ui::value_style(), 8, 42, 48, LV_LABEL_ALIGN_CENTER);
  mag_cal_value =
      Ui::label(cal_panel, "0", Ui::value_style(), 62, 42, 48, LV_LABEL_ALIGN_CENTER);
  gyro_cal_value =
      Ui::label(cal_panel, "0", Ui::value_style(), 116, 42, 48, LV_LABEL_ALIGN_CENTER);
  sys_cal_value =
      Ui::label(cal_panel, "0", Ui::value_style(), 168, 42, 48, LV_LABEL_ALIGN_CENTER);

  lv_obj_t *euler_panel =
      Ui::panel(scr, Ui::Margin, 198, Ui::ScreenWidth - 2 * Ui::Margin, 64,
                nullptr);
  Ui::label(euler_panel, LV_SYMBOL_LOOP, Ui::icon_style(), 8, 5, 24,
            LV_LABEL_ALIGN_CENTER);
  yaw_value   = axis_value(euler_panel, "YAW", 12);
  pitch_value = axis_value(euler_panel, "PIT", 86);
  roll_value  = axis_value(euler_panel, "ROL", 160);

  lv_obj_t *gravity_panel =
      Ui::panel(scr, Ui::Margin, 272, Ui::ScreenWidth - 2 * Ui::Margin, 54,
                nullptr);
  Ui::label(gravity_panel, LV_SYMBOL_DOWN, Ui::icon_style(), 8, 4, 24,
            LV_LABEL_ALIGN_CENTER);
  gravity_x_value = axis_value(gravity_panel, "X", 12);
  gravity_y_value = axis_value(gravity_panel, "Y", 86);
  gravity_z_value = axis_value(gravity_panel, "Z", 160);

  lv_obj_t *accel_panel =
      Ui::panel(scr, Ui::Margin, 336, Ui::ScreenWidth - 2 * Ui::Margin, 54,
                nullptr);
  Ui::label(accel_panel, LV_SYMBOL_UP, Ui::icon_style(), 8, 4, 24,
            LV_LABEL_ALIGN_CENTER);
  acceleration_x_value = axis_value(accel_panel, "X", 12);
  acceleration_y_value = axis_value(accel_panel, "Y", 86);
  acceleration_z_value = axis_value(accel_panel, "Z", 160);

  return scr;
}

/****************************** END OF FILE **********************************/
