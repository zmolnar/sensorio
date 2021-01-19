
#include "lvgl.h"
#include "app_hal.h"

#include "gui/Sensorio.h"

int main(void)
{
	lv_init();

	hal_setup();

	SensorioStart();

	hal_loop();
}
