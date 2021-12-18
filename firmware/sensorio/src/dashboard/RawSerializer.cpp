
#include <dashboard/RawSerializer.hpp>
#include <etl/span.h>
#include <platform/Log.hpp>

using namespace Config;
using namespace Platform;

const char *RawSerializer::tag{"raw-serializer"};

static const uint32_t MAGIC{0xacbb7876};

using RawSystem = struct __attribute__((packed)) {
  struct {
    int32_t utcOffset;
  } location;
  struct {
    uint32_t volume;
  } beep;
};

using RawGui = struct __attribute__((packed)) {
  struct {
    struct {
      uint32_t chartRefreshPeriod;
    } variometer;
  } screens;
};

using RawImu = struct __attribute__((packed)) {
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

struct RawData {
  struct __attribute__((packed)) {
    uint32_t magic;
    RawSystem system;
    RawGui gui;
    RawImu imu;
    uint8_t crc;
  } raw;

  uint8_t calculateCrc8() const
  {
    const uint8_t *buf = reinterpret_cast<const uint8_t *>(&this->raw);
    size_t length = sizeof(*this) - sizeof(this->raw.crc);
    etl::span<const uint8_t> data{buf, length};

    uint8_t crc{0};
    for (const auto &c : data) {
      crc ^= c;
    }

    return crc;
  }

  bool isValid() const
  {
    return (MAGIC == raw.magic) && (raw.crc == calculateCrc8());
  }
};

bool RawSerializer::serialize(etl::vector_ext<uint8_t> &blob)
{
  bool error{true};

  if (sizeof(RawData) <= blob.max_size()) {
    RawData *data = reinterpret_cast<RawData *>(blob.data());
    blob.resize(sizeof(*data));

    System system{config->system.get()};
    Gui gui{config->gui.get()};
    Imu imu{config->imu.get()};

    data->raw.magic = MAGIC;
    data->raw.system.location.utcOffset = system.location.utcOffset;
    data->raw.system.beep.volume = static_cast<uint32_t>(system.beep.volume);
    data->raw.gui.screens.variometer.chartRefreshPeriod =
        gui.screens.variometer.chartRefreshPeriod;
    data->raw.imu.acc.x = imu.offset.acc.x;
    data->raw.imu.acc.y = imu.offset.acc.y;
    data->raw.imu.acc.z = imu.offset.acc.z;
    data->raw.imu.acc.r = imu.offset.acc.r;
    data->raw.imu.gyro.x = imu.offset.gyro.x;
    data->raw.imu.gyro.y = imu.offset.gyro.y;
    data->raw.imu.gyro.z = imu.offset.gyro.z;
    data->raw.imu.mag.x = imu.offset.mag.x;
    data->raw.imu.mag.y = imu.offset.mag.y;
    data->raw.imu.mag.z = imu.offset.mag.z;
    data->raw.imu.mag.r = imu.offset.mag.r;
    data->raw.crc = data->calculateCrc8();

    error = false;

    Log::Info(tag) << "config serialized on " << blob.size() << "bytes";

  } else {
    Log::Error(tag) << "buffer too small (" << blob.max_size() << " < "
                    << sizeof(RawData) << ")";
  }

  return error;
}

bool RawSerializer::deserialize(const etl::vector_ext<uint8_t> &blob)
{
  bool error{true};

  if (sizeof(RawData) == blob.size()) {
    const RawData *data = reinterpret_cast<const RawData *>(blob.data());

    if (data->isValid()) {
      System system{};
      system.location.utcOffset = data->raw.system.location.utcOffset;
      system.beep.set(data->raw.system.beep.volume);
      config->system.set(system);

      Gui gui{};
      gui.screens.variometer.chartRefreshPeriod =
          data->raw.gui.screens.variometer.chartRefreshPeriod;
      config->gui.set(gui);

      Imu imu{};
      imu.offset.acc.x = data->raw.imu.acc.x;
      imu.offset.acc.y = data->raw.imu.acc.y;
      imu.offset.acc.z = data->raw.imu.acc.z;
      imu.offset.acc.r = data->raw.imu.acc.r;
      imu.offset.gyro.x = data->raw.imu.gyro.x;
      imu.offset.gyro.y = data->raw.imu.gyro.y;
      imu.offset.gyro.z = data->raw.imu.gyro.z;
      imu.offset.mag.x = data->raw.imu.mag.x;
      imu.offset.mag.y = data->raw.imu.mag.y;
      imu.offset.mag.z = data->raw.imu.mag.z;
      imu.offset.mag.r = data->raw.imu.mag.r;
      config->imu.set(imu);

      error = false;

      Log::Info(tag) << "config deserialized from " << blob.size() << " bytes";
    } else {
      Log::Error(tag) << "invalid data in blob";
    }
  } else {
    Log::Error(tag) << "invalid size (" << blob.size()
                    << " != " << sizeof(RawData) << ")";
  }

  return error;
}
