
#ifndef SUBJECT_H
#define SUBJECT_H

#include <functional>
#include <platform/Assert.hpp>
#include <etl/mutex.h>

template <typename T>
class Subject {
  using Notification = std::function<void(void)>;

  etl::mutex mutex {};
  Notification cb;
  T value{};

  void notify()
  {
    cb();
  }

public:
  Subject(Notification cb) : mutex{}, cb{cb}, value{}
  {
    Platform::Assert::Assert(nullptr != cb);
  }

  void set(const T &value)
  {
    mutex.lock();
    this->value = value;
    mutex.unlock();
    notify();
  }

  T get()
  {
    mutex.lock();
    T tmp{value};
    mutex.unlock();
    return tmp;
  }
};

#endif
