
#include <driver/uart.h>
#include <esp_log.h>

#include <core/DataLoggerThread.hpp>
#include <platform/Log.hpp>

#include <stdio.h>

namespace Platform {
  namespace Log {


    Error::~Error() {
      if (enabled) {
        ESP_LOGE(tag, "%s", msg.c_str());
      }
    }

    Info::~Info() {
      if (enabled) {
        ESP_LOGI(tag, "%s", msg.c_str());
      }
    }

    Debug::~Debug() {
      if (enabled) {
        ESP_LOGD(tag, "%s", msg.c_str());
      }
    }
  }
}

// Shared log-level
Platform::Log::Level Platform::Log::Base::level{Level::ERROR};
