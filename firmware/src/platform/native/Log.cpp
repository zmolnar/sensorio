
#include <iostream>
#include <platform/Log.hpp>

namespace Platform {

  Log::Level Log::Base::level{Log::Level::ERROR};

  Log::Error::~Error()
  {
    if (enabled) {
      std::cout << "[ERR][" << tag << "] " << msg.c_str() << std::endl;
    }
  }

  Log::Info::~Info()
  {
    if (enabled) {
      std::cout << "[INF][" << tag << "] " << msg.c_str() << std::endl;
    }
  }

  Log::Debug::~Debug()
  {
    if (enabled) {
      std::cout << "[DBG][" << tag << "] " << msg.c_str() << std::endl;
    }
  }
}
