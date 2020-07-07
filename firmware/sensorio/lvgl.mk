
LVGL_CSRC  = $(wildcard $(LVGL)/src/lv_core/*.c)
LVGL_CSRC += $(wildcard $(LVGL)/src/lv_draw/*.c)
LVGL_CSRC += $(wildcard $(LVGL)/src/lv_font/*.c)
LVGL_CSRC += $(wildcard $(LVGL)/src/lv_gpu/*.c)
LVGL_CSRC += $(wildcard $(LVGL)/src/lv_hal/*.c)
LVGL_CSRC += $(wildcard $(LVGL)/src/lv_misc/*.c)
LVGL_CSRC += $(wildcard $(LVGL)/src/lv_themes/*.c)
LVGL_CSRC += $(wildcard $(LVGL)/src/lv_widgets/*.c)

LVGL_INC   = $(LVGL)
LVGL_INC  += $(LVGL)/src/lv_core/
LVGL_INC  += $(LVGL)/src/lv_draw/
LVGL_INC  += $(LVGL)/src/lv_font/
LVGL_INC  += $(LVGL)/src/lv_gpu/
LVGL_INC  += $(LVGL)/src/lv_hald/
LVGL_INC  += $(LVGL)/src/lv_misc/
LVGL_INC  += $(LVGL)/src/lv_themes/
LVGL_INC  += $(LVGL)/src/lv_widgets/

ALLCSRC += $(LVGL_CSRC)
ALLINC  += $(LVGL_INC)
