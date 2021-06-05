
#include "LocalTime.h"

int LtDayOfWeek(int y, int m, int d)
{
  int tmp = y -= (m < 3) ? 1 : 0;

  return (tmp + y / 4 - y / 100 + y / 400 + "-bed=pen+mad."[m] + d) % 7;
}

bool LtIsDaylightSavingTime(int day, int month, int dow)
{
  if (month < 3 || month > 10)
    return false;
  if (month > 3 && month < 10)
    return true;

  int previousSunday = day - dow;

  if (month == 3)
    return previousSunday >= 25;
  if (month == 10)
    return previousSunday < 25;

  return false; // this line never gonna happend
}

void LtAddOffsetToDateTime(LocalTime_t *p, int offset)
{
  static const int daysInMonth[] = {
      0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

  bool isOffsetValid = ((-12) <= offset) && (offset <= 12);

  if (isOffsetValid) {
    int hour = p->hour + offset;

    if (hour < 0) {
      p->hour = 24 + (hour % 24);
      p->day--;
    } else if ((0 <= hour) && (hour <= 23)) {
      p->hour = hour;
    } else {
      p->hour = hour % 24;
      p->day++;
    }

    if (p->day < 1) {
      int month = p->month - 1;
      if (month < 1) {
        month = 12 - month;
      }
      p->day = daysInMonth[month] + p->day;
      p->month--;
    } else if (daysInMonth[p->month] < p->day) {
      p->day = p->day - daysInMonth[p->month];
      p->month++;
    }

    if (p->month < 1) {
      p->month = 12 + (p->month % 12);
      p->year--;
    } else if (12 < p->month) {
      p->month = (p->month % 12);
      p->year++;
    }
  }
}
