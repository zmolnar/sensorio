
#include "lvgl.h"
#include "app_hal.h"

#include "Sensorio.h"

int main(void)
{
	lv_init();

	hal_setup();

	SensorioStart();

	hal_loop();
}
