#ifdef LV_CONF_INCLUDE_SIMPLE
#include <lvgl.h>
#else
#include "../../../lvgl/lvgl.h"
#endif

#ifdef __cplusplus
extern "C"
{
#endif

  lv_obj_t *startup_screen_create(void);

#ifdef __cplusplus
}
#endif  
