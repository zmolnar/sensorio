CSRCS += $(LVGL_DIR)/$(LVGL_DIR_NAME)/src/lv_core/lv_group.c
CSRCS += $(LVGL_DIR)/$(LVGL_DIR_NAME)/src/lv_core/lv_indev.c
CSRCS += $(LVGL_DIR)/$(LVGL_DIR_NAME)/src/lv_core/lv_disp.c
CSRCS += $(LVGL_DIR)/$(LVGL_DIR_NAME)/src/lv_core/lv_obj.c
CSRCS += $(LVGL_DIR)/$(LVGL_DIR_NAME)/src/lv_core/lv_refr.c
CSRCS += $(LVGL_DIR)/$(LVGL_DIR_NAME)/src/lv_core/lv_style.c
CSRCS += $(LVGL_DIR)/$(LVGL_DIR_NAME)/src/lv_core/lv_debug.c

DEPPATH += --dep-path $(LVGL_DIR)/$(LVGL_DIR_NAME)/src/lv_core
VPATH += :$(LVGL_DIR)/$(LVGL_DIR_NAME)/src/lv_core

CFLAGS += "-I$(LVGL_DIR)/$(LVGL_DIR_NAME)/src/lv_core"
