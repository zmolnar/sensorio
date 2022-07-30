//
//  This file is part of Sensorio.
//
//  Sensorio is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  Sensorio is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with Sensorio.  If not, see <https://www.gnu.org/licenses/>.
//

#include <dashboard/Config.hpp>
#include <platform/Log.hpp>

using namespace Platform;

void Config::Config::loadDefault() {
  Log::Info(tag) << "loading default config";
  System system {};
  system.location.utcOffset = 1;
  system.beep.volume = System::Volume::HIGH;
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
