/**
 * @file Ui.h
 * @brief Shared LVGL helpers for the Sensorio monochrome UI.
 */

#ifndef GUI_SCREENS_UI_H
#define GUI_SCREENS_UI_H

#include "lvgl.h"

namespace Gui {
namespace Ui {

  constexpr lv_coord_t ScreenWidth  = LV_HOR_RES_MAX;
  constexpr lv_coord_t ScreenHeight = LV_VER_RES_MAX;
  constexpr lv_coord_t Margin       = 8;
  constexpr lv_coord_t HeaderHeight = 30;

  lv_obj_t *screen(lv_style_t *base_style);
  const char *battery_symbol(uint32_t percentage);

  lv_obj_t *header(lv_obj_t *parent, const char *title);
  lv_obj_t *panel(lv_obj_t *parent,
                  lv_coord_t x,
                  lv_coord_t y,
                  lv_coord_t w,
                  lv_coord_t h,
                  const char *title = nullptr);
  lv_obj_t *label(lv_obj_t *parent,
                  const char *text,
                  lv_style_t *style,
                  lv_coord_t x,
                  lv_coord_t y,
                  lv_coord_t w,
                  lv_label_align_t align = LV_LABEL_ALIGN_LEFT);
  lv_obj_t *rule(lv_obj_t *parent, lv_coord_t x, lv_coord_t y, lv_coord_t w);

  void style_chart(lv_obj_t *chart);
  void style_bar(lv_obj_t *bar);
  void style_button(lv_obj_t *button, bool primary);
  void style_control(lv_obj_t *control);

  lv_style_t *icon_style();
  lv_style_t *large_icon_style();
  lv_style_t *caption_style();
  lv_style_t *body_style();
  lv_style_t *body_right_style();
  lv_style_t *value_style();
  lv_style_t *large_value_style();
  lv_style_t *hero_value_style();
  lv_style_t *unit_style();

} // namespace Ui
} // namespace Gui

#endif /* GUI_SCREENS_UI_H */
