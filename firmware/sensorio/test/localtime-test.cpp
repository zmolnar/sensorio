
#include <catch2/catch_all.hpp>

#include <LocalTime.h>

SCENARIO("Offset added to given time")
{
  GIVEN("invalid offset")
  {
    int32_t offset = 20;

    WHEN("trying to add")
    {
      LocalTime_t lt = {
          .year = 2021,
          .month = 6,
          .day = 3,
          .hour = 6,
          .minute = 49,
          .second = 33,
      };
      LtAddOffsetToDateTime(&lt, offset);

      THEN("datetime is not modified")
      {
        REQUIRE(2021 == lt.year);
        REQUIRE(6 == lt.month);
        REQUIRE(3 == lt.day);
        REQUIRE(6 == lt.hour);
        REQUIRE(49 == lt.minute);
        REQUIRE(33 == lt.second);
      }
    }
  }

  GIVEN("valid positive offset")
  {
    int32_t offset = 4;

    WHEN("no overflow occurs")
    {
      LocalTime_t lt = {
          .year = 2021,
          .month = 6,
          .day = 3,
          .hour = 6,
          .minute = 49,
          .second = 33,
      };
      LtAddOffsetToDateTime(&lt, offset);

      THEN("hour counter is incremented")
      {
        REQUIRE(2021 == lt.year);
        REQUIRE(6 == lt.month);
        REQUIRE(3 == lt.day);
        REQUIRE(10 == lt.hour);
        REQUIRE(49 == lt.minute);
        REQUIRE(33 == lt.second);
      }
    }

    WHEN("hour counter overflows")
    {
      LocalTime_t lt = {
          .year = 2021,
          .month = 6,
          .day = 3,
          .hour = 23,
          .minute = 49,
          .second = 33,
      };
      LtAddOffsetToDateTime(&lt, offset);

      THEN("day counter is incremented too")
      {
        REQUIRE(2021 == lt.year);
        REQUIRE(6 == lt.month);
        REQUIRE(4 == lt.day);
        REQUIRE(3 == lt.hour);
        REQUIRE(49 == lt.minute);
        REQUIRE(33 == lt.second);
      }
    }

    WHEN("day counter overflows")
    {
      LocalTime_t lt = {
          .year = 2021,
          .month = 6,
          .day = 30,
          .hour = 23,
          .minute = 49,
          .second = 33,
      };

      LtAddOffsetToDateTime(&lt, offset);

      THEN("month counter is incremented too")
      {
        REQUIRE(2021 == lt.year);
        REQUIRE(7 == lt.month);
        REQUIRE(1 == lt.day);
        REQUIRE(3 == lt.hour);
        REQUIRE(49 == lt.minute);
        REQUIRE(33 == lt.second);
      }
    }

    WHEN("month counter overflows")
    {
      LocalTime_t lt = {
          .year = 2021,
          .month = 12,
          .day = 31,
          .hour = 23,
          .minute = 49,
          .second = 33,
      };

      LtAddOffsetToDateTime(&lt, offset);

      THEN("year counter is incremented too")
      {
        REQUIRE(2022 == lt.year);
        REQUIRE(1 == lt.month);
        REQUIRE(1 == lt.day);
        REQUIRE(3 == lt.hour);
        REQUIRE(49 == lt.minute);
        REQUIRE(33 == lt.second);
      }
    }
  }

  GIVEN("valid negative offset")
  {
    int32_t offset = -4;

    WHEN("no overflow occurs")
    {
      LocalTime_t lt = {
          .year = 2021,
          .month = 6,
          .day = 3,
          .hour = 6,
          .minute = 49,
          .second = 33,
      };
      LtAddOffsetToDateTime(&lt, offset);

      THEN("hour counter is decremented")
      {
        REQUIRE(2021 == lt.year);
        REQUIRE(6 == lt.month);
        REQUIRE(3 == lt.day);
        REQUIRE(2 == lt.hour);
        REQUIRE(49 == lt.minute);
        REQUIRE(33 == lt.second);
      }
    }

    WHEN("hour counter underflows")
    {
      LocalTime_t lt = {
          .year = 2021,
          .month = 6,
          .day = 3,
          .hour = 2,
          .minute = 49,
          .second = 33,
      };
      LtAddOffsetToDateTime(&lt, offset);

      THEN("day counter is decremented too")
      {
        REQUIRE(2021 == lt.year);
        REQUIRE(6 == lt.month);
        REQUIRE(2 == lt.day);
        REQUIRE(22 == lt.hour);
        REQUIRE(49 == lt.minute);
        REQUIRE(33 == lt.second);
      }
    }

    WHEN("day counter underflows")
    {
      LocalTime_t lt = {
          .year = 2021,
          .month = 6,
          .day = 1,
          .hour = 2,
          .minute = 49,
          .second = 33,
      };

      LtAddOffsetToDateTime(&lt, offset);

      THEN("month counter is decremented too")
      {
        REQUIRE(2021 == lt.year);
        REQUIRE(5 == lt.month);
        REQUIRE(31 == lt.day);
        REQUIRE(22 == lt.hour);
        REQUIRE(49 == lt.minute);
        REQUIRE(33 == lt.second);
      }
    }

    WHEN("month counter underflows")
    {
      LocalTime_t lt = {
          .year = 2021,
          .month = 1,
          .day = 1,
          .hour = 2,
          .minute = 49,
          .second = 33,
      };

      LtAddOffsetToDateTime(&lt, offset);

      THEN("year counter is decremented too")
      {
        REQUIRE(2020 == lt.year);
        REQUIRE(12 == lt.month);
        REQUIRE(31 == lt.day);
        REQUIRE(22 == lt.hour);
        REQUIRE(49 == lt.minute);
        REQUIRE(33 == lt.second);
      }
    }
  }
}