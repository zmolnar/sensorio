#include "startup.h"
#include "Power.h"

#include <stdint.h>

LV_IMG_DECLARE(paraglider);

static void bar_increment(lv_task_t *t)
{
  static int16_t a = 0;

  a += 2;
  lv_bar_set_value(t->user_data, a, LV_ANIM_ON);

  if (100 <= a) {
    a = 0;
    lv_task_del(t);
    PowerStartupFinished();
  }
}

lv_obj_t *startup_screen_create(void)
{
  lv_obj_t *scr = lv_obj_create(NULL, NULL);

  lv_obj_t *cont = lv_cont_create(scr, NULL);
  lv_obj_set_auto_realign(cont, true);
  lv_obj_align_origo(cont, NULL, LV_ALIGN_CENTER, 0, 0);
  lv_cont_set_fit(cont, LV_FIT_PARENT);
  lv_cont_set_layout(cont, LV_LAYOUT_CENTER);

  lv_obj_t *img = lv_img_create(scr, NULL);
  lv_img_set_src(img, &paraglider);
  lv_img_set_auto_size(img, true);
  lv_obj_align(img, scr, LV_ALIGN_IN_TOP_MID, 0, 77);

  lv_obj_t *bar = lv_bar_create(scr, NULL);
  lv_obj_set_size(bar, 200, 15);
  lv_obj_align(bar, scr, LV_ALIGN_IN_BOTTOM_MID, 0, -40);
  lv_bar_set_value(bar, 0, LV_ANIM_OFF);
  lv_task_create(bar_increment, 60, LV_TASK_PRIO_LOWEST, bar);

  return scr;
}