
#include "lvgl.h"
#include "app_hal.h"

#include "gui/Sensorio.h"
#include "dashboard/Dashboard.h"

int main(void)
{
	lv_init();

	hal_setup();

    DbInit();
	SensorioStart();

	hal_loop();
}
