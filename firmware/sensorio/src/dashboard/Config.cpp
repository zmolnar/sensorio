
#include <dashboard/Config.hpp>
#include <platform/Log.hpp>

using namespace Platform;

void Config::Config::loadDefault() {
  Log::Info(tag) << "loading default config";
  System system {};
  system.location.utcOffset = 1;
  system.beep.level = System::Level::HIGH;
  this->system.set(system);

  Gui gui {};
  gui.screens.variometer.chartRefreshPeriod = 1000;
  this->gui.set(gui);
}

void Config::Config::load() {

  uint8_t buf[256] = {0};
  etl::vector_ext<uint8_t> blob {buf, sizeof(buf)};

  if(storage.load(blob) || serializer.deserialize(blob)) {
    Log::Error(tag) << "failed to load config";
    loadDefault();
  } else {
    Log::Info(tag) << "config loaded";
  }
}

void Config::Config::save() {

  uint8_t buf[256] = {0};
  etl::vector_ext<uint8_t> blob {buf, sizeof(buf)};

  if(serializer.serialize(blob) || storage.save(blob)) {
    Log::Error(tag) << "failed to save config";
  } else {
    Log::Info(tag) << "config saved";
  }
}
