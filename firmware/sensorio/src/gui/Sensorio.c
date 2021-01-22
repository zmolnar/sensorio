/**
 * @file Sensorio.c
 * @brief
 */

/*****************************************************************************/
/* INCLUDES                                                                  */
/*****************************************************************************/
#include "Sensorio.h"
#include "Power.h"
#include "dashboard/Dashboard.h"

#if defined(SIMULATOR)
#include <stdio.h>
#else
#include "core/LvglThread.h"
#endif

#include "screens/PressureSensorData.h"
#include "screens/ImuData.h"
#include "screens/Startup.h"
#include "screens/Variometer.h"

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
#if defined(SIMULATOR)
// Publish it to the global namespace, because app_hal.c needs it.
lv_group_t *encgroup;
#else
static lv_group_t *encgroup;
#endif

// Screen objects
lv_obj_t *startup;
lv_obj_t *vario;
lv_obj_t *bpsdata;
lv_obj_t *imudata;

// Refresher task
lv_task_t *task;

// Default style for the screens
static lv_style_t no_border_style;

// Exit confirmation message box button labels
static const char *exitMsgButtons[] = {"Yes", "Cancel", ""};

/*****************************************************************************/
/* DECLARATION OF LOCAL FUNCTIONS                                            */
/*****************************************************************************/

/*****************************************************************************/
/* DEFINITION OF LOCAL FUNCTIONS                                             */
/*****************************************************************************/
static void refresh_task(lv_task_t *t)
{
  lv_event_send_refresh(lv_scr_act());
}

static void group_focus_cb(lv_group_t *g)
{
  lv_obj_t *obj = lv_group_get_focused(g);

  if (vario == obj) {
    lv_scr_load(vario);
  } else if (bpsdata == obj) {
    lv_scr_load(bpsdata);
  } else if (imudata == obj) {
    lv_scr_load(imudata);
  } else {
    ;
  }

  lv_event_send_refresh(lv_scr_act());
}

static void exit_msgbox_event_handler(lv_obj_t *obj, lv_event_t event)
{
  if (LV_EVENT_VALUE_CHANGED == event) {
    uint16_t id = lv_msgbox_get_active_btn(obj);

#if 0
    const char *btext = lv_msgbox_get_active_btn_text(obj);
    printf("id:%d btext:%x\n", id, (uint32_t)btext);
#endif

    if (0 == id) {
      lv_obj_clean(lv_scr_act());
      PowerStop();
    } else {
      lv_group_remove_all_objs(encgroup);
      lv_group_add_obj(encgroup, vario);
      lv_group_add_obj(encgroup, bpsdata);
      lv_group_add_obj(encgroup, imudata);
      lv_group_focus_obj(vario);
      task = lv_task_create(refresh_task, 100, LV_TASK_PRIO_LOW, NULL);
    }
    lv_obj_del(obj);
  }
}

/*****************************************************************************/
/* DEFINITION OF GLOBAL FUNCTIONS                                            */
/*****************************************************************************/
void SensorioStart(void)
{
  lv_style_init(&no_border_style);
  lv_style_set_border_width(&no_border_style, LV_STATE_DEFAULT, 0);

  startup = startup_screen_create(&no_border_style);

#if defined(SIMULATOR)
  // In the simulator app_hal.c will create the group.
#else
  encgroup = lv_group_create();
#endif

  lv_group_set_focus_cb(encgroup, group_focus_cb);

  lv_scr_load(startup);
}

void SensorioStartupFinished(void)
{
  lv_obj_del(startup);

  vario   = variometer_screen_create(&no_border_style);
  bpsdata = bps_data_screen_create(&no_border_style);
  imudata = imu_data_screen_create(&no_border_style);

  lv_group_add_obj(encgroup, vario);
  lv_group_add_obj(encgroup, bpsdata);
  lv_group_add_obj(encgroup, imudata);
  lv_group_focus_obj(vario);

  task = lv_task_create(refresh_task, 100, LV_TASK_PRIO_LOW, NULL);

  PowerStartupFinished();
}

lv_group_t *SensorioGetEncoderGroup(void)
{
  return encgroup;
}

void SensorioConfirmExit(void)
{
  lv_task_del(task);

  lv_obj_t *mbox = lv_msgbox_create(lv_scr_act(), NULL);
  lv_msgbox_set_text(mbox, "Do you want to exit?");
  lv_msgbox_add_btns(mbox, exitMsgButtons);
  lv_obj_set_width(mbox, 200);
  lv_obj_set_event_cb(mbox, exit_msgbox_event_handler);
  lv_obj_align(mbox, lv_scr_act(), LV_ALIGN_CENTER, 0, 0);

  lv_group_remove_all_objs(encgroup);
  lv_group_add_obj(encgroup, mbox);
}

/****************************** END OF FILE **********************************/