
#include <freertos/FreeRTOS.h>
#include <platform/Assert.hpp>

namespace Platform {
  namespace Assert {
    void Assert(bool expression)
    {
      configASSERT(expression);
    }
  }
}
