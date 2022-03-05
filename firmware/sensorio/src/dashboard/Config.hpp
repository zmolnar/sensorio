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

#ifndef CONFIG_HPP
#define CONFIG_HPP

#include <dashboard/Observer.hpp>
#include <dashboard/Subject.hpp>

#include <dashboard/Gui.hpp>
#include <dashboard/Imu.hpp>
#include <dashboard/System.hpp>

#include <etl/vector.h>

#include <platform/Assert.hpp>
#include <platform/Log.hpp>

namespace Config {

  enum class Component {
    SYSTEM = 0x01 << 0,
    GUI = 0x01 << 1,
    IMU = 0x01 << 2,
  };

  class Config;
  class Plugin {
  protected:
    Config *config{};

  public:
    void bind(Config *config) {
      this->config = config;
    }
  };

  class ASerializer : public Plugin {
  public:
    virtual bool serialize(etl::vector_ext<uint8_t> &blob) = 0;
    virtual bool deserialize(const etl::vector_ext<uint8_t> &blob) = 0;
  };

  class AStorage : public Plugin {
  public:
    virtual bool load(etl::vector_ext<uint8_t> &blob) = 0;
    virtual bool save(const etl::vector_ext<uint8_t> &blob) = 0;
  };

  class Config {
    const char *tag{"config"};
    Observers<10> observers{};
    ASerializer &serializer;
    AStorage &storage;

    void notify(Flag flag) {
      observers.notify(flag);
    }

    void loadDefault();

  public:
    Subject<System> system;
    Subject<Gui> gui;
    Subject<Imu> imu;

    Config(ASerializer &serializer, AStorage &storage) :
        serializer{serializer},
        storage{storage},
        system{[this]() { notify(static_cast<Flag>(Component::SYSTEM)); }},
        gui{[this]() { notify(static_cast<Flag>(Component::GUI)); }},
        imu{[this]() { notify(static_cast<Flag>(Component::IMU)); }} {
      serializer.bind(this);
      storage.bind(this);
    }

    void load();

    void save();

    void subscribe(Mask subjects, Notification cb) {
      Platform::Assert::Assert(nullptr != cb);
      Platform::Assert::Assert(0U < subjects);
      observers.add(subjects, cb);
    }
  };

}

extern Config::Config config;

#endif
