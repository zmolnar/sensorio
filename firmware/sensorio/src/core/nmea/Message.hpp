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

#ifndef MESSAGE_HPP
#define MESSAGE_HPP

#include <core/nmea/Types.hpp>
#include <etl/optional.h>
#include <etl/string.h>
#include <etl/string_utilities.h>

#include <iostream>

namespace Nmea0183 {
  class Token {
    enum class Format { DEC, HEX };

    template <typename T>
    bool asInt(T &num,
               size_t start,
               size_t length,
               Format format = Format::DEC) const {
      auto error{str.length() < (start + length)};
      auto base = (Format::DEC == format) ? 10 : 16;
      auto matcher = (Format::DEC == format) ? isdigit : isxdigit;
      T value{0};

      for (auto i = start; i < (start + length) && (!error); ++i) {
        auto c = str[i];
        if (matcher(c)) {
          value *= base;
          value += charToInt(c);
        } else {
          error = true;
        }
      }

      if (!error) {
        num = value;
      }

      return error;
    }

    static uint8_t charToInt(char c);
    bool asIntegral(double &num) const;
    bool asFractional(double &num) const;
    bool combineDouble(double &result,
                       const Token &integral,
                       const Token &fractional) const;

  public:
    etl::string_view str;

    Token(etl::string_view str) : str{str} {
    }

    Token(etl::string_view::const_iterator begin,
          etl::string_view::const_iterator end) :
        str{begin, end} {
    }

    Token(etl::optional<etl::string_view> optstr) :
        str{optstr.value_or(etl::string_view{})} {
    }

    Token(const Token &) = default;
    Token &operator=(const Token &) = default;
    Token(Token &&) = default;
    Token &operator=(Token &&) = default;
    ~Token() = default;

    bool getCrc8(uint8_t &crc) const;
    bool asCrc8(uint8_t &crc) const;
    bool get(Date &date, bool optional = false) const;
    bool get(Time &time, bool optional = false) const;
    bool get(LatLon &latlon, bool optional = false) const;
    bool get(Direction &dir, bool optional = false) const;
    bool get(Quality &quality, bool optional = false) const;
    bool get(uint32_t &num, bool optional = false) const;
    bool get(double &num, bool optional = false) const;

    bool isEmpty() const {
      return str.empty();
    }
  };

  class Message {
    const etl::string_view str;
    mutable etl::optional<etl::string_view> token{};

    bool isChecksumValid() const;
    const Token getNextToken() const;

  public:
    Message(etl::string_view str) : str{str} {
    }

    Message(etl::string_view::const_iterator begin,
            etl::string_view::const_iterator end) :
        str{begin, end} {
    }

    bool isValid() const;
    bool isInvalid() const {
      return !isValid();
    }
    TalkerID talkerID() const;
    SentenceFormatter sentenceFormat() const;

    void ignoreNext() const {
      (void)getNextToken();
    }

    template <typename T>
    bool getNext(T &t) const {
      return getNextToken().get(t, false);
    }

    template <typename T>
    bool getNextOptional(T &t) const {
      return getNextToken().get(t, true);
    }
  };
}

#endif
