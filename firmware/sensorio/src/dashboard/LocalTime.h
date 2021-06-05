
#ifndef LOCALTIME_H
#define LOCALTIME_H

#include <stdint.h>

#ifndef __cplusplus
#include <stdbool.h>
#endif

typedef struct LocalTime_s {
  int year;
  int month;
  int day;
  int hour;
  int minute;
  int second;
} LocalTime_t;


int LtDayOfWeek(int y, int m, int d);

bool LtIsDaylightSavingTime(int day, int month, int dow);

void LtAddOffsetToDateTime(LocalTime_t *p, int offset);

#endif