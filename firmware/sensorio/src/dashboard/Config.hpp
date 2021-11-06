
#ifndef CONFIG_H
#define CONFIG_H

#include <dashboard/Observer.hpp>
#include <dashboard/Subject.hpp>
#include <etl/vector.h>
#include <platform/Assert.hpp>

namespace Config {

  enum class Component {
    SYSTEM = 0x01 << 0,
    GUI = 0x01 << 1,
    IMU = 0x01 << 2,
  };

  class System {
  public:
    enum class Level {
      OFF,
      LOW,
      MED,
      HIGH,
    };
    struct {
      int32_t utcOffset{0};
    } location;
    struct {
      Level level{Level::OFF};
      void set(int16_t value)
      {
        int16_t off = static_cast<int16_t>(Level::OFF);
        int16_t high = static_cast<int16_t>(Level::HIGH);
        if ((off <= value) && (value <= high)) {
          level = static_cast<Level>(value);
        }
      }
    } beep;
  };

  class Gui {
  public:
    struct {
      struct {
        uint32_t chartRefreshPeriod;
      } variometer;
    } screens;
  };

  class Imu {
  public:
    struct Offset {
      struct {
        int16_t x;
        int16_t y;
        int16_t z;
        int16_t r;
      } acc;
      struct {
        int16_t x;
        int16_t y;
        int16_t z;
      } gyro;
      struct {
        int16_t x;
        int16_t y;
        int16_t z;
        int16_t r;
      } mag;
    };

    Offset offset{};
    uint8_t crc{};

    static uint8_t crc8(const uint8_t data[], size_t length)
    {
      uint8_t crc = 0;
      for (size_t i = 0; i < length; ++i) {
        crc ^= data[i];
      }

      return crc;
    }

    bool isValid()
    {
      uint8_t *data = (uint8_t *)&offset;
      size_t length = sizeof(offset);
      uint8_t crc = crc8(data, length);

      return this->crc == crc;
    }
  };

  class Config;
  class Plugin {
  protected:
    Config *config{};

  public:
    void bind(Config *config)
    {
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

    void notify(Flag flag)
    {
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
        imu{[this]() { notify(static_cast<Flag>(Component::IMU)); }}
    {
      serializer.bind(this);
      storage.bind(this);
    }

    void load();

    void save();

    void subscribe(Mask subjects, Notification cb)
    {
      Platform::Assert::Assert(nullptr != cb);
      Platform::Assert::Assert(0U < subjects);
      observers.add(subjects, cb);
    }
  };

}

extern Config::Config config;

#endif
