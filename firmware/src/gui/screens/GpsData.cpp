/**
 * @file GpsData.c
 * @brief
 */

/*****************************************************************************/
/* INCLUDES                                                                  */
/*****************************************************************************/
#include "GpsData.h"
#include "Ui.h"
#include "dashboard/Dashboard.hpp"

/*****************************************************************************/
/* DEFINITION OF GLOBAL CONSTANTS AND VARIABLES                              */
/*****************************************************************************/
static lv_obj_t * lock_value;
static lv_obj_t * speed_value;
static lv_obj_t * altitude_value;
static lv_obj_t * course_value;
static lv_obj_t * satellites_value;
static lv_obj_t * latitude_value;
static lv_obj_t * longitude_value;
static lv_obj_t * date_value;
static lv_obj_t * time_value;
static lv_task_t *task;

/*****************************************************************************/
/* DEFINITION OF LOCAL FUNCTIONS                                             */
/*****************************************************************************/
static void refresh_task(lv_task_t *p)
{
  (void)p;

  Dashboard::Gps gps {dashboard.gps.get()};

  lv_label_set_text(lock_value, gps.locked ? "LOCKED" : "NO FIX");
  lv_label_set_text_fmt(speed_value, "%d", (int)gps.speed);
  lv_label_set_text_fmt(altitude_value, "%.0f", gps.altitude);
  lv_label_set_text_fmt(course_value, "%03d", (int)gps.course);
  lv_label_set_text_fmt(satellites_value, "%d", gps.numOfSatellites);
  lv_label_set_text_fmt(latitude_value, "%.5f", gps.latitude);
  lv_label_set_text_fmt(longitude_value, "%.5f", gps.longitude);
  lv_label_set_text_fmt(date_value,
                        "%02d/%02d/%02d",
                        gps.gmt.day,
                        gps.gmt.month,
                        gps.gmt.year);
  lv_label_set_text_fmt(time_value,
                        "%02d:%02d:%02d",
                        gps.gmt.hour,
                        gps.gmt.minute,
                        gps.gmt.second);
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
  using namespace Gui;

  lv_obj_t *scr = Ui::screen(style);
  lv_obj_set_event_cb(scr, event_handler);

  Ui::header(scr, "GPS");

  lv_obj_t *speed_panel = Ui::panel(scr, Ui::Margin, 40, 108, 84, nullptr);
  Ui::label(speed_panel, LV_SYMBOL_RIGHT, Ui::icon_style(), 8, 5, 24,
            LV_LABEL_ALIGN_CENTER);
  speed_value = Ui::label(
      speed_panel, "0", Ui::large_value_style(), 0, 26, 108, LV_LABEL_ALIGN_CENTER);
  Ui::label(speed_panel, "km/h", Ui::unit_style(), 0, 58, 108, LV_LABEL_ALIGN_CENTER);

  lv_obj_t *alt_panel = Ui::panel(scr, 124, 40, 108, 84, nullptr);
  Ui::label(alt_panel, LV_SYMBOL_UPLOAD, Ui::icon_style(), 8, 5, 24,
            LV_LABEL_ALIGN_CENTER);
  altitude_value = Ui::label(
      alt_panel, "0", Ui::large_value_style(), 0, 26, 108, LV_LABEL_ALIGN_CENTER);
  Ui::label(alt_panel, "m", Ui::unit_style(), 0, 58, 108, LV_LABEL_ALIGN_CENTER);

  lv_obj_t *fix_panel =
      Ui::panel(scr, Ui::Margin, 134, Ui::ScreenWidth - 2 * Ui::Margin, 78,
                nullptr);
  Ui::label(fix_panel, LV_SYMBOL_GPS, Ui::icon_style(), 8, 5, 24,
            LV_LABEL_ALIGN_CENTER);
  Ui::label(fix_panel, "Status", Ui::body_style(), 10, 26, 74);
  lock_value = Ui::label(
      fix_panel, "NO FIX", Ui::body_right_style(), 102, 26, 112,
      LV_LABEL_ALIGN_RIGHT);
  Ui::label(fix_panel, "Satellites", Ui::body_style(), 10, 50, 90);
  satellites_value = Ui::label(
      fix_panel, "0", Ui::body_right_style(), 102, 50, 112, LV_LABEL_ALIGN_RIGHT);

  lv_obj_t *course_panel =
      Ui::panel(scr, Ui::Margin, 222, Ui::ScreenWidth - 2 * Ui::Margin, 46,
                nullptr);
  Ui::label(course_panel, LV_SYMBOL_RIGHT, Ui::icon_style(), 8, 13, 24,
            LV_LABEL_ALIGN_CENTER);
  course_value = Ui::label(course_panel,
                           "000",
                           Ui::value_style(),
                           0,
                           17,
                           Ui::ScreenWidth - 2 * Ui::Margin,
                           LV_LABEL_ALIGN_CENTER);
  Ui::label(course_panel,
            "deg",
            Ui::unit_style(),
            150,
            25,
            60,
            LV_LABEL_ALIGN_LEFT);

  lv_obj_t *position_panel =
      Ui::panel(scr, Ui::Margin, 278, Ui::ScreenWidth - 2 * Ui::Margin, 68,
                nullptr);
  Ui::label(position_panel, LV_SYMBOL_GPS, Ui::icon_style(), 8, 5, 24,
            LV_LABEL_ALIGN_CENTER);
  Ui::label(position_panel, "LAT", Ui::caption_style(), 10, 25, 42);
  latitude_value = Ui::label(position_panel,
                             "0.00000",
                             Ui::body_right_style(),
                             62,
                             23,
                             150,
                             LV_LABEL_ALIGN_RIGHT);
  Ui::label(position_panel, "LON", Ui::caption_style(), 10, 47, 42);
  longitude_value = Ui::label(position_panel,
                              "0.00000",
                              Ui::body_right_style(),
                              62,
                              45,
                              150,
                              LV_LABEL_ALIGN_RIGHT);

  lv_obj_t *time_panel =
      Ui::panel(scr, Ui::Margin, 356, Ui::ScreenWidth - 2 * Ui::Margin, 34,
                nullptr);
  date_value =
      Ui::label(time_panel, "--/--/--", Ui::caption_style(), 8, 9, 96);
  time_value = Ui::label(
      time_panel, "--:--:--", Ui::body_right_style(), 112, 6, 100,
      LV_LABEL_ALIGN_RIGHT);

  return scr;
}

/****************************** END OF FILE **********************************/
