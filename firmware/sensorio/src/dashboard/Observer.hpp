
#ifndef OBSERVER_H
#define OBSERVER_H

#include <platform/Assert.hpp>
#include <etl/mutex.h>
#include <etl/vector.h>
#include <functional>

using Flag = uint32_t;
using Mask = uint32_t;
using Notification = std::function<void(void)>;

class Observer {
  Mask aggregated{};
  Notification cb{};

public:
  Observer(Mask mask, Notification cb) : aggregated{mask}, cb{cb}
  {
    Platform::Assert::Assert(nullptr != cb);
  }

  bool match(Flag flag)
  {
    aggregated &= ~(static_cast<Mask>(flag));
    bool finished = (0U == aggregated);
    if (finished) {
      cb();
    }
    return finished;
  }
};

template<uint32_t SIZE>
class Observers {
  etl::mutex mutex;
  etl::vector<Observer, SIZE> observers{};

public:
  void add(Mask subjects, Notification cb)
  {
    mutex.lock();
    Platform::Assert::Assert(nullptr != cb);
    Platform::Assert::Assert(0U < subjects);
    observers.emplace_back(subjects, cb);
    mutex.unlock();
  }

  void notify(Flag flag)
  {
    mutex.lock();
    auto it = observers.begin();
    while (it != observers.end()) {
      if (it->match(flag)) {
        it = observers.erase(it);
      } else {
        ++it;
      }
    }
    mutex.unlock();
  }
};

#endif
