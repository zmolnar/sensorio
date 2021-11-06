
#include <cassert>
#include <platform/Assert.hpp>

namespace Platform {
  namespace Assert {
    void Assert(bool expression)
    {
      assert(expression);
    }
  }
}
