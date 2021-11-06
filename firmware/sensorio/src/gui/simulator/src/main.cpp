
#include "lvgl.h"
#include "app_hal.h"

#include <gui/Sensorio.h>
#include <dashboard/Dashboard.hpp>
#include <dashboard/RawSerializer.hpp>

#include "FileStorage.hpp"

Config::RawSerializer serializer{};
Config::FileStorage storage{};
Config::Config config(serializer, storage);
Dashboard::Dashboard dashboard {};

int main(void)
{
	using namespace Platform;
	Log::Base::setLevel(Log::Level::DEBUG);
	lv_init();

	hal_setup();

    storage.init();
	config.load();

	SensorioStart();

	hal_loop();
}

