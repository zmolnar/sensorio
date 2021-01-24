/**
 * @file SystemStatus.c
 * @brief
 */

/*****************************************************************************/
/* INCLUDES                                                                  */
/*****************************************************************************/
#include "SystemStatus.h"

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
static lv_obj_t *label;
static lv_obj_t *battery;
static lv_obj_t *board;

/*****************************************************************************/
/* DECLARATION OF LOCAL FUNCTIONS                                            */
/*****************************************************************************/

/*****************************************************************************/
/* DEFINITION OF LOCAL FUNCTIONS                                             */
/*****************************************************************************/
static void event_handler(lv_obj_t *obj, lv_event_t event)
{
  if (LV_EVENT_REFRESH == event) {
    Battery_t data;
    DbDataBatteryGet(&data);

    char buf[10];
    lv_snprintf(buf, sizeof(buf), "%3.02f V", data.voltage);
    lv_table_set_cell_value(battery, 1, 1, buf);
    lv_snprintf(buf, sizeof(buf), "%d", data.percentage);
    lv_table_set_cell_value(battery, 2, 1, buf);

    const char *batStatus[] = {"Discharging", "Charging", "Charged", "Invalid"};
    lv_table_set_cell_value(battery, 3, 1, batStatus[data.status]);

    lv_snprintf(buf, sizeof(buf), "%d", data.value);
    lv_table_set_cell_value(battery, 4, 1, buf);

    Board_t bdata;
    DbDataBoardGet(&bdata);

    lv_table_set_cell_value(board, 1, 1, bdata.usbConnected ? "Yes" : "No");
  }
}

/*****************************************************************************/
/* DEFINITION OF GLOBAL FUNCTIONS                                            */
/*****************************************************************************/
lv_obj_t *system_status_screen_create(lv_style_t *style)
{
  lv_obj_t *scr = lv_obj_create(NULL, NULL);
  lv_obj_add_style(scr, LV_STATE_DEFAULT, style);
  lv_obj_set_event_cb(scr, event_handler);

  static lv_style_t lstyle;
  lv_style_init(&lstyle);
  lv_style_set_text_font(&lstyle, LV_STATE_DEFAULT, &lv_font_montserrat_24);
  label = lv_label_create(scr, NULL);
  lv_label_set_text(label, "System");
  lv_obj_add_style(label, LV_STATE_DEFAULT, style);
  lv_obj_add_style(label, LV_STATE_DEFAULT, &lstyle);
  lv_obj_align(label, scr, LV_ALIGN_IN_TOP_MID, 0, 0);

  static lv_style_t hstyle;
  lv_style_init(&hstyle);
  lv_style_set_text_font(&hstyle, LV_STATE_DEFAULT, &lv_font_montserrat_18);

  static lv_style_t tstyle;
  lv_style_init(&tstyle);
  lv_style_set_pad_all(&tstyle, LV_STATE_DEFAULT, 2);

  battery = lv_table_create(scr, NULL);
  lv_obj_add_style(battery, LV_TABLE_PART_BG, style);
  lv_obj_add_style(battery, LV_TABLE_PART_BG, &tstyle);
  lv_obj_add_style(battery, LV_TABLE_PART_CELL1, style);
  lv_obj_add_style(battery, LV_TABLE_PART_CELL1, &tstyle);
  lv_obj_add_style(battery, LV_TABLE_PART_CELL2, style);
  lv_obj_add_style(battery, LV_TABLE_PART_CELL2, &tstyle);
  lv_obj_add_style(battery, LV_TABLE_PART_CELL2, &hstyle);

  lv_table_set_col_cnt(battery, 2);
  lv_table_set_row_cnt(battery, 5);

  lv_table_set_col_width(battery, 0, lv_obj_get_width(scr) * 0.55);
  lv_table_set_col_width(battery, 1, lv_obj_get_width(scr) * 0.45);

  lv_table_set_cell_type(battery, 0, 0, 2);
  lv_table_set_cell_type(battery, 0, 1, 2);
  lv_table_set_cell_type(battery, 1, 0, 1);
  lv_table_set_cell_type(battery, 1, 1, 1);
  lv_table_set_cell_type(battery, 2, 0, 1);
  lv_table_set_cell_type(battery, 2, 1, 1);
  lv_table_set_cell_type(battery, 3, 0, 1);
  lv_table_set_cell_type(battery, 3, 1, 1);
  lv_table_set_cell_type(battery, 4, 0, 1);
  lv_table_set_cell_type(battery, 4, 1, 1);

  lv_table_set_cell_merge_right(battery, 0, 0, true);
  lv_table_set_cell_merge_right(battery, 0, 1, true);

  lv_obj_align(battery, label, LV_ALIGN_OUT_BOTTOM_MID, 0, 10);
  lv_table_set_cell_align(battery, 0, 0, LV_LABEL_ALIGN_LEFT);
  lv_table_set_cell_align(battery, 0, 1, LV_LABEL_ALIGN_LEFT);
  lv_table_set_cell_align(battery, 1, 0, LV_LABEL_ALIGN_LEFT);
  lv_table_set_cell_align(battery, 1, 1, LV_LABEL_ALIGN_LEFT);
  lv_table_set_cell_align(battery, 2, 0, LV_LABEL_ALIGN_LEFT);
  lv_table_set_cell_align(battery, 2, 1, LV_LABEL_ALIGN_LEFT);
  lv_table_set_cell_align(battery, 3, 0, LV_LABEL_ALIGN_LEFT);
  lv_table_set_cell_align(battery, 3, 1, LV_LABEL_ALIGN_LEFT);
  lv_table_set_cell_align(battery, 4, 0, LV_LABEL_ALIGN_LEFT);
  lv_table_set_cell_align(battery, 4, 1, LV_LABEL_ALIGN_LEFT);

  lv_table_set_cell_value(battery, 0, 0, "Battery");
  lv_table_set_cell_value(battery, 1, 0, "Voltage");
  lv_table_set_cell_value(battery, 2, 0, "Level");
  lv_table_set_cell_value(battery, 3, 0, "Status");
  lv_table_set_cell_value(battery, 4, 0, "ADC value");

  board = lv_table_create(scr, NULL);
  lv_obj_add_style(board, LV_TABLE_PART_BG, style);
  lv_obj_add_style(board, LV_TABLE_PART_BG, &tstyle);
  lv_obj_add_style(board, LV_TABLE_PART_CELL1, style);
  lv_obj_add_style(board, LV_TABLE_PART_CELL1, &tstyle);
  lv_obj_add_style(board, LV_TABLE_PART_CELL2, style);
  lv_obj_add_style(board, LV_TABLE_PART_CELL2, &tstyle);
  lv_obj_add_style(board, LV_TABLE_PART_CELL2, &hstyle);

  lv_table_set_col_cnt(board, 2);
  lv_table_set_row_cnt(board, 2);

  lv_table_set_col_width(board, 0, lv_obj_get_width(scr) * 0.55);
  lv_table_set_col_width(board, 1, lv_obj_get_width(scr) * 0.45);

  lv_table_set_cell_type(board, 0, 0, 2);
  lv_table_set_cell_type(board, 0, 1, 2);
  lv_table_set_cell_type(board, 1, 0, 1);
  lv_table_set_cell_type(board, 1, 1, 1);

  lv_table_set_cell_merge_right(board, 0, 0, true);
  lv_table_set_cell_merge_right(board, 0, 1, true);

  lv_obj_align(board, battery, LV_ALIGN_OUT_BOTTOM_MID, 0, 10);
  lv_table_set_cell_align(board, 0, 0, LV_LABEL_ALIGN_LEFT);
  lv_table_set_cell_align(board, 0, 1, LV_LABEL_ALIGN_LEFT);
  lv_table_set_cell_align(board, 1, 0, LV_LABEL_ALIGN_LEFT);
  lv_table_set_cell_align(board, 1, 1, LV_LABEL_ALIGN_LEFT);

  lv_table_set_cell_value(board, 0, 0, "Board");
  lv_table_set_cell_value(board, 1, 0, "USB connected");

  return scr;
}

/****************************** END OF FILE **********************************/
