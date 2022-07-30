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

#include <core/nmea/Message.hpp>
#include <etl/string_utilities.h>

namespace Nmea0183 {

  uint8_t Token::charToInt(char c) {
    c = toupper(c);

    auto value = static_cast<uint8_t>(0);
    if (isdigit(c)) {
      value = c - '0';
    } else if (isxdigit(c)) {
      value = c - 'A' + 10;
    } else {
      value = 0;
    }

    return value;
  }

  bool Token::asIntegral(double &num) const {
    auto tmp = static_cast<double>(0.0);
    auto error = (0 != str.length()) && asInt(tmp, 0, str.length());
    num = error ? 0.0 : tmp;
    return error;
  }

  bool Token::asFractional(double &num) const {
    // TODO do it backwards
    auto value{0.0};
    auto mul{0.1};
    bool valid{true};
    for (auto it = str.cbegin(); valid && (it != str.cend()); ++it) {
      auto c = *it;
      if (isdigit(c)) {
        value += charToInt(c) * mul;
        mul /= 10.0;
      } else {
        valid = false;
      }
    }

    if (valid) {
      num = value;
    }

    return !valid;
  }

  bool Token::combineDouble(double &result,
                            const Token &integral,
                            const Token &fractional) const {
    auto ival{0.0};
    auto fval{0.0};
    auto error{integral.isEmpty() && fractional.isEmpty()};
    error = error || integral.asIntegral(ival);
    error = error || fractional.asFractional(fval);

    if (!error) {
      result = ival + fval;
    }

    return error;
  }

  bool Token::getCrc8(uint8_t &crc) const {
    for (auto &c : str) {
      crc ^= c;
    }
    return (0 == str.length());
  }

  bool Token::asCrc8(uint8_t &crc) const {
    return (2 < str.length()) || asInt(crc, 0, str.length(), Format::HEX);
  }

  bool Token::get(Date &date, bool optional) const {
    auto error{false};

    if (6 == str.length()) {
      error = error || asInt(date.day, 0, 2);
      error = error || asInt(date.month, 2, 2);
      error = error || asInt(date.year, 4, 2);
    } else if (optional && (0 == str.length())) {
      date.year = 0;
      date.month = 0;
      date.day = 0;
    } else {
      error = true;
    }

    return error;
  }

  bool Token::get(Time &time, bool optional) const {
    auto error{false};

    if (10 == str.length()) {
      error = error || asInt(time.hour, 0, 2);
      error = error || asInt(time.minute, 2, 2);
      error = error || asInt(time.second, 4, 2);
      error = error || asInt(time.millisec, 7, 3);
    } else if (optional && (0 == str.length())) {
      time.hour = 0;
      time.minute = 0;
      time.second = 0;
      time.millisec = 0;
    } else {
      error = true;
    }

    return error;
  }

  bool Token::get(LatLon &latlon, bool optional) const {
    auto dot{str.find('.')};
    auto error{false};

    if ((4 == dot) || (5 == dot)) {
      auto dlen{dot - 2};
      auto min{dlen + 7};
      auto max{dlen + 9};
      auto length{str.length()};

      auto degrees{0};
      auto minutes = static_cast<double>(0);

      error = error || ((length < min) || (max < length));
      error = error || asInt(degrees, 0, dlen);
      error = error || Token{str.cbegin() + dlen, str.cend()}.get(minutes);

      if (!error) {
        latlon.degrees = degrees + minutes / 60.0;
      }
    } else if (optional && (0 == str.length())) {
      latlon.degrees = 0.0;
    } else {
      error = true;
    }

    return error;
  }

  bool Token::get(Direction &dir, bool optional) const {
    auto error{false};

    if (1 == str.length()) {
      switch (str[0]) {
      case 'S': {
        dir = Direction::SOUTH;
        break;
      }
      case 'N': {
        dir = Direction::NORTH;
        break;
      }
      case 'E': {
        dir = Direction::EAST;
        break;
      }
      case 'W': {
        dir = Direction::WEST;
        break;
      }
      default: {
        error = true;
      }
      }
    } else if (optional && (0 == str.length())) {
      dir = Direction::UNKNOWN;
    } else {
      error = true;
    }

    return error;
  }

  bool Token::get(Quality &quality, bool optional) const {
    auto error{false};

    if (0 < str.length()) {
      auto value{0};
      error = error || asInt(value, 0, str.length());
      error = error || (Quality::MAX <= static_cast<Quality>(value));

      if (!error) {
        quality = static_cast<Quality>(value);
      }
    } else if (optional && (0 == str.length())) {
      quality = Quality::UNKNOWN;
    } else {
      error = true;
    }

    return error;
  }

  bool Token::get(uint32_t &num, bool optional) const {
    auto error{false};
    if (0 < str.length()) {
      error = asInt(num, 0, str.length());
    } else if (optional && (0 == str.length())) {
      num = 0;
    } else {
      error = true;
    }
    return error;
  }

  bool Token::get(double &num, bool optional) const {
    auto error{false};

    if (0 < str.length()) {
      auto isNegative{(0 < str.length()) && ('-' == str[0])};
      auto offset = isNegative ? 1 : 0;
      etl::string_view view(str.cbegin() + offset, str.length() - offset);

      etl::optional<etl::string_view> token{};
      auto integral = token = etl::get_token(view, ".", token, false);
      auto fractional = token = etl::get_token(view, ".", token, false);

      double tmp{0.0};
      error = combineDouble(tmp, integral, fractional);

      if (!error) {
        auto sign = isNegative ? (-1) : 1;
        num = sign * tmp;
      }
    } else if (optional && (0 == str.length())) {
      num = 0.0;
    } else {
      error = true;
    }

    return error;
  }

  bool Message::isChecksumValid() const {
    auto refCrc = static_cast<uint8_t>(0);
    auto crc = static_cast<uint8_t>(0);

    auto error{Token(str.cbegin() + 1, str.cend() - 3).getCrc8(refCrc)};
    error = error || Token(etl::right_n_view(str, 2)).asCrc8(crc);

    return ((!error) && (refCrc == crc));
  }

  bool Message::isValid() const {
    static constexpr auto minLength{11};
    auto valid = (minLength <= str.length());

    valid = valid && ('$' == str[0]);
    for (auto i = 1; i < 6; ++i) {
      valid = valid && isalpha(str[i]);
    }

    valid = valid && (',' == str[6]);

    valid = valid && isChecksumValid();

    return valid;
  }

  TalkerID Message::talkerID() const {
    TalkerID talkerID{};
    talkerID[0] = str[1];
    talkerID[1] = str[2];
    talkerID[2] = '\0';
    return talkerID;
  }

  SentenceFormatter Message::sentenceFormat() const {
    SentenceFormatter type{};
    type[0] = str[3];
    type[1] = str[4];
    type[2] = str[5];
    type[3] = '\0';
    return type;
  }

  const Token Message::getNextToken() const {
    token = etl::get_token(str, ",*", token, false);
    return Token{token};
  }
}
