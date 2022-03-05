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

#ifndef IMU_HPP
#define IMU_HPP

#include <cstdint>

namespace Dashboard {
  class Imu {
  public:
    enum class Status {
      IDLE = 0,
      ERROR = 1,
      PERIPHERAL_INIT = 2,
      INITIALIZING = 3,
      RUNNING_SELFTEST = 4,
      RUNNING_FUSION = 5,
      RUNNING_NO_FUSION = 6,
      UNKNOWN = 7,
    };

    enum class ClockSource {
      INTERNAL = 0,
      EXTERNAL = 1,
      UNKNOWN = 2,
    };

    struct {
      Status status{Status::UNKNOWN};
      ClockSource clock{ClockSource::UNKNOWN};
    } system;
    struct {
      uint32_t acc{0};
      uint32_t gyro{0};
      uint32_t mag{0};
      uint32_t sys{0};
    } calibration;
    struct {
      double yaw{0.0};
      double pitch{0.0};
      double roll{0.0};
    } euler;
    struct {
      double x{0.0};
      double y{0.0};
      double z{0.0};
    } gravity;
    struct {
      double x{0};
      double y{0};
      double z{0};
    } acceleration;

    void assign(const Imu &rhs) {
      *this = rhs;
    }
  };
}

namespace Config {
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

    static uint8_t crc8(const uint8_t data[], size_t length) {
      uint8_t crc = 0;
      for (size_t i = 0; i < length; ++i) {
        crc ^= data[i];
      }

      return crc;
    }

    bool isValid() {
      uint8_t *data = (uint8_t *)&offset;
      size_t length = sizeof(offset);
      uint8_t crc = crc8(data, length);

      return this->crc == crc;
    }

    void assign(const Imu &rhs) {
      *this = rhs;
      uint8_t *data = (uint8_t *)&offset;
      size_t length = sizeof(offset);
      auto crc = crc8(data, length);
      this->crc = (0x00 == crc) ? 0xFF : crc;
    }
  };
}

#endif
