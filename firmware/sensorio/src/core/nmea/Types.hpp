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

#ifndef TYPES_HPP
#define TYPES_HPP

#include <etl/string.h>

namespace Nmea0183 {

  using TalkerID = etl::string<3>;
  using SentenceFormatter = etl::string<4>;

  struct Date {
    uint32_t year;
    uint32_t month;
    uint32_t day;
  };

  struct Time {
    uint32_t hour;
    uint32_t minute;
    uint32_t second;
    uint32_t millisec;
  };

  enum class Direction { SOUTH, NORTH, EAST, WEST, UNKNOWN };

  class LatLon {
  public:
    double degrees;
    Direction direction;

    bool isLatitudeOrUnknown() const {
      static constexpr auto min{-90.0};
      static constexpr auto max{90.0};
      auto error{(Direction::SOUTH == direction) ||
                 (Direction::NORTH == direction) ||
                 (Direction::UNKNOWN == direction)};
      error = error || ((degrees < min) || (max < degrees));
      return error;
    }

    bool isLongitudeOrUnknown() const {
      static constexpr auto min{-180.0};
      static constexpr auto max{180.0};
      auto error{(Direction::EAST == direction) ||
                 (Direction::WEST == direction ||
                 (Direction::UNKNOWN == direction))};
      error = error || ((degrees < min) || (max < degrees));
      return error;
    }
  };

  enum class Quality {
    INVALID = 0,
    SPS_VALID = 1,
    DGPS_SPS_SBAS_VALID = 2,
    PPS_VALID = 3,
    RTK_FIXED_INT = 4,
    RTK_FLOAT = 5,
    ESTIMATED = 6,
    MAX,
    UNKNOWN,
  };

  class Data {
  public:
    TalkerID talkerID;
    SentenceFormatter msgType;
    Date date;
    Time time;
    LatLon latitude;
    LatLon longitude;
    Quality quality;
    double altitude;
    double speed;
    double course;
    uint32_t numOfSats;
    double hdop;
    double separation;
  };
}

#endif
