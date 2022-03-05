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

#ifndef SYSTEM_HPP
#define SYSTEM_HPP

namespace Config {
  class System {
  public:
    enum class Volume {
      ZERO,
      LOW,
      MED,
      HIGH,
    };
    struct {
      int32_t utcOffset{0};
    } location;
    struct {
      Volume volume{Volume::ZERO};
      void set(int16_t value) {
        int16_t off = static_cast<int16_t>(Volume::ZERO);
        int16_t high = static_cast<int16_t>(Volume::HIGH);
        if ((off <= value) && (value <= high)) {
          volume = static_cast<Volume>(value);
        }
      }
    } beep;

    void assign(const System &rhs) {
      *this = rhs;
    }
  };
}

#endif
