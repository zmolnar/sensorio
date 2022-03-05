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

#ifndef DASHBOARD_HPP
#define DASHBOARD_HPP

#include <dashboard/Observer.hpp>
#include <dashboard/Subject.hpp>

#include <dashboard/Battery.hpp>
#include <dashboard/Board.hpp>
#include <dashboard/Bps.hpp>
#include <dashboard/Filter.hpp>
#include <dashboard/Gps.hpp>
#include <dashboard/Imu.hpp>
#include <platform/Assert.hpp>

namespace Dashboard {

  class Dashboard {
    Observers<10> observers{};

    void notify(Flag flag) {
      observers.notify(flag);
    }

  public:
    Subject<Imu> imu;
    Subject<Bps> bps;
    Subject<Gps> gps;
    Subject<Board> board;
    Subject<Battery> battery;
    Subject<Filter> filter;

    enum class Data : uint32_t {
      IMU = 0x01 << 0,
      BPS = 0x01 << 1,
      GPS = 0x01 << 2,
      BOARD = 0x01 << 3,
      BATTERY = 0x01 << 4,
      FILTER = 0x01 << 5,
    };

    Dashboard() :
        imu{[this]() { notify(static_cast<Flag>(Data::IMU)); }},
        bps{[this]() { notify(static_cast<Flag>(Data::BPS)); }},
        gps{[this]() { notify(static_cast<Flag>(Data::GPS)); }},
        board{[this]() { notify(static_cast<Flag>(Data::BOARD)); }},
        battery{[this]() { notify(static_cast<Flag>(Data::BATTERY)); }},
        filter{[this]() { notify(static_cast<Flag>(Data::FILTER)); }} {
    }

    void subscribe(Mask subjects, Notification cb) {
      Platform::Assert::Assert(nullptr != cb);
      Platform::Assert::Assert(0U < subjects);
      observers.add(subjects, cb);
    }
  };

}

extern Dashboard::Dashboard dashboard;

#endif
