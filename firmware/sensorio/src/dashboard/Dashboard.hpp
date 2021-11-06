
#ifndef DASHBOARD_H
#define DASHBOARD_H

#include <dashboard/Subject.hpp>
#include <dashboard/Observer.hpp>
#include <platform/Assert.hpp>
namespace Dashboard {

  class Imu {
  public:
    enum class Status {
      IDLE = 0,
      ERROR = 1,
      PERIPHERAL_INIT = 2,
      INITIALIZING = 3,
      RUNNING_SELFTEST = 4,
      RUNNING_FUSION = 5,
      RUNNING_NO_FUSION = 6,
      UNKNOWN = 7,
    };

    enum class ClockSource {
      INTERNAL = 0,
      EXTERNAL = 1,
      UNKNOWN = 2,
    };

    struct {
      Status status{Status::UNKNOWN};
      ClockSource clock{ClockSource::UNKNOWN};
    } system;
    struct {
      uint32_t acc{0};
      uint32_t gyro{0};
      uint32_t mag{0};
      uint32_t sys{0};
    } calibration;
    struct {
      double yaw{0.0};
      double pitch{0.0};
      double roll{0.0};
    } euler;
    struct {
      double x{0.0};
      double y{0.0};
      double z{0.0};
    } gravity;
    struct {
      double x{0};
      double y{0};
      double z{0};
    } acceleration;
  };

  class Bps {
  public:
    struct {
      uint32_t pressure{0};
      uint32_t temp{0};
    } raw;
    struct {
      uint32_t pressure{0};
      uint32_t temp{0};
    } cooked;
  };

  class Gps {
  public:
    bool locked{0};
    uint32_t altitude{0};
    uint32_t course{0};
    uint32_t latitude{0};
    uint32_t longitude{0};
    uint32_t numOfSatellites{0};
    uint32_t speed{0};

    struct DateTime {
      uint32_t year{0};
      uint32_t month{0};
      uint32_t day{0};
      uint32_t hour{0};
      uint32_t minute{0};
      uint32_t second{0};
    };

    DateTime gmt;

    DateTime getLocalTime(int utcOffset)
    {
      int dstOffset = isDaylightSavingTime() ? 1 : 0;
      return addOffset(utcOffset + dstOffset);
    }

  private:
    uint32_t getDayOfWeek();
    bool isDaylightSavingTime();
    DateTime addOffset(int offset);
  };

  class Board {
  public:
    bool usbConnected{false};
  };

  class Battery {
  public:
    enum class Status {
      DISCHARGING,
      CHARGING,
      CHARGED,
      UNKNOWN,
    };

    Status status{Status::UNKNOWN};
    double voltage{0.0};
    uint32_t percentage{0};
    uint32_t adcValue{0};
  };

  class Filter {
  public:
    struct {
      double averaged{0.0};
      double instant{0.0};
    } vario;
    double height{0.0};
  };

  class Dashboard {
    Observers<10> observers{};

    void notify(Flag flag)
    {
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
        filter{[this]() { notify(static_cast<Flag>(Data::FILTER)); }}
    {
    }

    void subscribe(Mask subjects, Notification cb)
    {
      Platform::Assert::Assert(nullptr != cb);
      Platform::Assert::Assert(0U < subjects);
      observers.add(subjects, cb);
    }
  };

}

extern Dashboard::Dashboard dashboard;

#endif
