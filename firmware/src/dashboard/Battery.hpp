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

#ifndef BATTERY_HPP
#define BATTERY_HPP

namespace Dashboard {
  class Battery {
  public:
    enum class Status {
      DISCHARGING,
      CHARGING,
      CHARGED,
      UNKNOWN,
    };

    Status status{Status::UNKNOWN};
    double voltage{0.0};
    uint32_t percentage{0};
    uint32_t adcValue{0};

    void assign(const Battery &rhs) {
      *this = rhs;
    }
  };
}

#endif
