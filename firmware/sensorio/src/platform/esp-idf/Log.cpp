
#include <esp_log.h>
#include <platform/Log.hpp>

namespace Platform {

  Log::Level Log::Base::level{Log::Level::ERROR};

  Log::Error::~Error()
  {
    if (enabled) {
      ESP_LOGE(tag, "%s", msg.c_str());
    }
  }

  Log::Info::~Info()
  {
    if (enabled) {
      ESP_LOGI(tag, "%s", msg.c_str());
    }
  }

  Log::Debug::~Debug()
  {
    if (enabled) {
      ESP_LOGD(tag, "%s", msg.c_str());
    }
  }
}
