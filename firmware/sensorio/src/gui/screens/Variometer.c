//#include "sensorio.h"
#include "Variometer.h"

static void variometer_event_cb(lv_obj_t *ta, lv_event_t e)
{
    //printf("%s %d\n", __func__, e);
}

lv_obj_t * variometer_screen_create(lv_style_t *style)
{
    lv_obj_t *scr = lv_obj_create(NULL, NULL);
    lv_obj_add_style(scr, LV_STATE_DEFAULT, style);
    lv_obj_set_event_cb(scr, variometer_event_cb);

    // Vario
    lv_obj_t *act_vario = lv_label_create(scr, NULL);
    static lv_style_t act_vario_style;
    lv_style_init(&act_vario_style);
    //lv_style_set_text_font(&act_vario_style, LV_STATE_DEFAULT, &lv_font_montserrat_48);
    lv_obj_add_style(act_vario, LV_LABEL_PART_MAIN, &act_vario_style);
    lv_label_set_text(act_vario, "+ 3.1");
    lv_obj_align(act_vario, scr, LV_ALIGN_IN_TOP_LEFT, 0, 0);

    // C2 for maxline and label
    // lv_obj_t *c2 = lv_cont_create(scr, NULL);
    //lv_obj_add_style(c2, LV_STATE_DEFAULT, &no_border_style);

    // static lv_point_t line_points[] = { {5,0}, {230, 0} };

    // static lv_style_t minmax_line_style;
    // lv_style_init(&minmax_line_style);
    // lv_style_set_line_width(&minmax_line_style, LV_STATE_DEFAULT, 1);
    // lv_style_set_line_color(&minmax_line_style, LV_STATE_DEFAULT, LV_COLOR_BLACK);
    // lv_style_set_line_dash_gap(&minmax_line_style, LV_STATE_DEFAULT, 6);
    // lv_style_set_line_dash_width(&minmax_line_style, LV_STATE_DEFAULT, 3);

    // lv_obj_t *max_line = lv_line_create(c2, NULL);
    // lv_obj_add_style(max_line, LV_LINE_PART_MAIN, &minmax_line_style);
    // lv_line_set_points(max_line, line_points, 2);

    // lv_obj_t *max = lv_label_create(c2, NULL);
    // lv_label_set_text(max, "2239 m");
    // lv_obj_align(max, max_line, LV_ALIGN_CENTER, 0, 0);

    // lv_cont_set_fit(c2, LV_FIT_TIGHT);

    // Chart
    lv_obj_t *chart = lv_chart_create(scr, NULL);
    lv_obj_add_style(chart, LV_CHART_PART_BG, style);
    lv_obj_set_size(chart, lv_obj_get_width(scr), lv_obj_get_height(scr)*0.5);
    lv_obj_align(chart, scr, LV_ALIGN_IN_BOTTOM_MID, 0, 0);
    lv_chart_set_type(chart, LV_CHART_TYPE_LINE);
    lv_chart_set_range(chart, 1100, 2100);
    lv_chart_set_point_count(chart, 100);
    lv_chart_set_div_line_count(chart, 0, 0);

    lv_chart_series_t *ser = lv_chart_add_series(chart, LV_COLOR_BLACK);
    size_t i;
    for (i = 1100; i < 2100; i += 10)
        lv_chart_set_next(chart, ser, i);

    lv_chart_refresh(chart);

    // C3 for maxline and label
    // lv_obj_t *c3 = lv_cont_create(scr, NULL);
    // lv_obj_add_style(c3, LV_STATE_DEFAULT, &no_border_style);
    // lv_cont_set_fit(c3, LV_FIT_TIGHT);
    // lv_obj_t *min_line = lv_line_create(c3, NULL);
    // lv_obj_add_style(min_line, LV_LINE_PART_MAIN, &minmax_line_style);
    // lv_line_set_points(min_line, line_points, 2);

    // lv_obj_t *min = lv_label_create(c3, NULL);
    // lv_label_set_text(min, "1320 m");
    // lv_obj_align(min, min_line, LV_ALIGN_CENTER, 0, 0);

    lv_obj_align(chart, scr, LV_ALIGN_IN_BOTTOM_MID, 0, 0);
    // lv_obj_align(c3, scr, LV_ALIGN_IN_BOTTOM_MID, 0, 0);
    // lv_obj_align(chart, c3, LV_ALIGN_OUT_TOP_MID, 0, 0);
    // lv_obj_align(c2, chart, LV_ALIGN_OUT_TOP_MID, 0, 0);

    return scr;
}