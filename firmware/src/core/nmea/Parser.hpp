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

#ifndef PARSER_HPP
#define PARSER_HPP

#include <etl/string.h>

#include <core/nmea/Message.hpp>

namespace Nmea0183 {
  class Parser {
  public:
    virtual const char *type() = 0;
    virtual bool parse(const Message &msg, Data &data) = 0;
  };

  class GGA : public Parser {
  public:
    const char *type() override final {
      return "GGA";
    }

    bool parse(const Message &msg, Data &data) override final;
  };

  class RMC : public Parser {
  public:
    const char *type() override final {
      return "RMC";
    }

    bool parse(const Message &msg, Data &data) override final;
  };
}

#endif
