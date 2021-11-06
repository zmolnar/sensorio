
#include <dashboard/Dashboard.hpp>

namespace Dashboard {

  uint32_t Gps::getDayOfWeek()
  {
    uint32_t y = gmt.year;
    uint32_t m = gmt.month;
    uint32_t d = gmt.day;
    const uint32_t tmp = (y -= ((m < 3) ? 1 : 0));
    return (tmp + y / 4 - y / 100 + y / 400 + "-bed=pen+mad."[m] + d) % 7;
  }

  bool Gps::isDaylightSavingTime()
  {
    uint32_t dow{getDayOfWeek()};

    if (gmt.month < 3 || gmt.month > 10)
      return false;
    if (gmt.month > 3 && gmt.month < 10)
      return true;

    int previousSunday = gmt.day - dow;

    if (gmt.month == 3)
      return previousSunday >= 25;
    if (gmt.month == 10)
      return previousSunday < 25;

    return false;
  }

  Gps::DateTime Gps::addOffset(int offset)
  {
    static const uint32_t daysInMonth[] = {
        0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

    bool isOffsetValid = ((-12) <= offset) && (offset <= 12);

    DateTime dt{gmt};

    if (isOffsetValid) {
      int hour = dt.hour + offset;

      if (hour < 0) {
        dt.hour = 24 + (hour % 24);
        dt.day--;
      } else if ((0 <= hour) && (hour <= 23)) {
        dt.hour = hour;
      } else {
        dt.hour = hour % 24;
        dt.day++;
      }

      if (dt.day < 1) {
        int month = dt.month - 1;
        if (month < 1) {
          month = 12 - month;
        }
        dt.day = daysInMonth[month] + dt.day;
        dt.month--;
      } else if (daysInMonth[dt.month] < dt.day) {
        dt.day = dt.day - daysInMonth[dt.month];
        dt.month++;
      }

      if (dt.month < 1) {
        dt.month = 12 + (dt.month % 12);
        dt.year--;
      } else if (12 < dt.month) {
        dt.month = (dt.month % 12);
        dt.year++;
      }
    }

    return dt;
  }
}
