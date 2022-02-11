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

#ifndef PROCESSOR_HPP
#define PROCESSOR_HPP

#include <etl/map.h>
#include <etl/string.h>

#include <functional>

#include <core/nmea/Message.hpp>
#include <core/nmea/Parser.hpp>
#include <core/nmea/Types.hpp>

namespace Nmea0183 {

  class Processor {
  public:
    enum class ErrorCode {
      NO_ERROR,
      BUFFER_OVERFLOW,
      UNKNOWN_SENTENCE,
      PARSE_ERROR,
      INVALID_MESSAGE,
    };

    static constexpr auto MAX_MESSAGE_LENGTH{82};
    using Input = etl::string<MAX_MESSAGE_LENGTH + 1>;
    using Parsers = etl::map<SentenceFormatter, Parser *, 2>;
    using ReceiveCallback = std::function<void(const Data &, const char *str)>;
    using ErrorCallback = std::function<void(ErrorCode error, const char *str)>;

    Input input{};
    Parsers parsers{};
    GGA gga{};
    RMC rmc{};

    const ReceiveCallback receiveCb;
    const ErrorCallback errorCb;

    Data data;

  public:
    Processor(ReceiveCallback rcb, ErrorCallback ecb) :
        receiveCb{rcb}, errorCb{ecb} {
      parsers.insert(etl::pair<SentenceFormatter, Parser *>{gga.type(), &gga});
      parsers.insert(etl::pair<SentenceFormatter, Parser *>{rmc.type(), &rmc});
    }

    void process(char c);
    void clear() {
      input.clear();
    }

  private:
    void saveChar(char c);
    void parseIfComplete();
    bool isComplete();
    void parse(const Message &msg);
    bool testMessage(const Message &msg);
    bool findParser(const Message &msg, Parser *&parser);
    bool runParser(const Message &msg, Parser *parser);
    bool parseMessage(const Message &msg);
  };

}

#endif
