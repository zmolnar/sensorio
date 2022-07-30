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

#ifndef SUBJECT_HPP
#define SUBJECT_HPP

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
    this->value.assign(value);
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
