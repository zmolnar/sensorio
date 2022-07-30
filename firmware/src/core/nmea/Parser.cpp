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

#include <core/nmea/Parser.hpp>
#include <etl/string_utilities.h>

namespace Nmea0183 {

  bool GGA::parse(const Message &msg, Data &data) {

    msg.ignoreNext();
    auto error {msg.getNext(data.time)};
    error = error || msg.getNextOptional(data.latitude);
    error = error || msg.getNextOptional(data.latitude.direction);
    error = error || (!data.latitude.isLatitudeOrUnknown());
    error = error || msg.getNextOptional(data.longitude);
    error = error || msg.getNextOptional(data.longitude.direction);
    error = error || (!data.longitude.isLongitudeOrUnknown());
    error = error || msg.getNext(data.quality);
    error = error || msg.getNext(data.numOfSats);
    error = error || msg.getNextOptional(data.hdop);
    error = error || msg.getNextOptional(data.altitude);
    msg.ignoreNext();
    error = error || msg.getNextOptional(data.separation);
    msg.ignoreNext();

    return error;
  }

  bool RMC::parse(const Message &msg, Data &data) {

    msg.ignoreNext();
    auto error {msg.getNext(data.time)};
    msg.ignoreNext(); // validity
    error = error || msg.getNextOptional(data.latitude);
    error = error || msg.getNextOptional(data.latitude.direction);
    error = error || (!data.latitude.isLatitudeOrUnknown());
    error = error || msg.getNextOptional(data.longitude);
    error = error || msg.getNextOptional(data.longitude.direction);
    error = error || (!data.longitude.isLongitudeOrUnknown());
    error = error || msg.getNextOptional(data.speed);
    error = error || msg.getNextOptional(data.course);
    error = error || msg.getNextOptional(data.date);
    msg.ignoreNext(); // Magnetic variation not being output
    msg.ignoreNext(); // Magnetic variation E/W indicator not being output
    msg.ignoreNext(); // Positioning mode
    msg.ignoreNext(); // V=Navigational status not valid

    return error;
  }

}
