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

#include <core/nmea/Processor.hpp>
#include <etl/string_utilities.h>

#include <iostream>

namespace Nmea0183 {

  void Processor::saveChar(char c) {
    if (input.length() < MAX_MESSAGE_LENGTH) {
      input.push_back(c);
    } else {
      errorCb(ErrorCode::BUFFER_OVERFLOW, input.c_str());
    }
  }

  bool Processor::isComplete() {
    return ('\n' == input[input.length() - 1]);
  }

  void Processor::parseIfComplete() {
    if (isComplete()) {
      Message msg(input.begin(), input.end() - 2);
      parse(msg);
    }
  }

  void Processor::parse(const Message &msg) {
    Parser *parser{};

    auto error{testMessage(msg)};
    error = error || findParser(msg, parser);
    error = error || runParser(msg, parser);

    if (!error) {
      receiveCb(data, input.c_str());
    }
  }

  bool Processor::testMessage(const Message &msg) {
    auto error {msg.isInvalid()};

    if(error) {
      errorCb(ErrorCode::INVALID_MESSAGE, input.c_str());
    }

    return error;
  }

  bool Processor::findParser(const Message &msg, Parser *&parser) {
    auto format = msg.sentenceFormat();
    auto res = parsers.find(format.c_str());
    auto error = (parsers.end() == res);

    if (error) {
      errorCb(ErrorCode::UNKNOWN_SENTENCE, input.c_str());
    } else {
      parser = res->second;
    }

    return error;
  }

  bool Processor::runParser(const Message &msg, Parser *parser) {
    assert(parser);
    auto error{parser->parse(msg, data)};
    if (error) {
      errorCb(ErrorCode::PARSE_ERROR, input.c_str());
    }
    return error;
  }

  void Processor::process(char c) {
    saveChar(c);
    parseIfComplete();
  }
}
