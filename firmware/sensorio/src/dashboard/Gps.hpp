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

#ifndef GPS_HPP
#define GPS_HPP

#include <cstdint>

namespace Dashboard {
  class Gps {
  public:
    bool locked{0};
    double altitude{0};
    double course{0};
    double latitude{0};
    double longitude{0};
    uint32_t numOfSatellites{0};
    double speed{0};

    struct DateTime {
      uint32_t year{0};
      uint32_t month{0};
      uint32_t day{0};
      uint32_t hour{0};
      uint32_t minute{0};
      uint32_t second{0};
    };

    DateTime gmt;

    DateTime getLocalTime(int utcOffset) {
      int dstOffset = isDaylightSavingTime() ? 1 : 0;
      return addOffset(utcOffset + dstOffset);
    }

    void assign(const Gps &rhs) {
      *this = rhs;
    }

  private:
    uint32_t getDayOfWeek();
    bool isDaylightSavingTime();
    DateTime addOffset(int offset);
  };
}

#endif
