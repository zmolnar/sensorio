
#include <core/nmea/Parser.hpp>

#include <gtest/gtest.h>

using namespace Nmea0183;

class ParserTest : public ::testing::Test {
public:
  virtual void SetUp() {
  }

  virtual void TearDown() {
  }
};

TEST_F(ParserTest, ParseGGA) {
  const auto str{
      "$GPGGA,092750.129,5321.6802,N,00630.3372,W,1,8,1.03,61.7,M,55.2,M,,*76"};

  Message msg{str};
  Data data{};
  GGA gga{};
  EXPECT_FALSE(gga.parse(msg, data));
  EXPECT_EQ(9, data.time.hour);
  EXPECT_EQ(27, data.time.minute);
  EXPECT_EQ(50, data.time.second);
  EXPECT_EQ(129, data.time.millisec);
  EXPECT_NEAR(53.3613, data.latitude.degrees, 0.0001);
  EXPECT_EQ(Direction::NORTH, data.latitude.direction);
  EXPECT_NEAR(6.505620, data.longitude.degrees, 0.0001);
  EXPECT_EQ(Direction::WEST, data.longitude.direction);
}

TEST_F(ParserTest, ParseGGAWithOptionalField) {
  const auto str{
      "$GPGGA,092750.000,,,,,1,8,1.03,61.7,M,-55.2,M,,*76"};

  Message msg{str};
  Data data{};
  GGA gga{};
  EXPECT_FALSE(gga.parse(msg, data));
}

TEST_F(ParserTest, ParseGGAWithMissingFields) {
  const auto str{
      "$GPGGA,,5321.6802,N,00630.3372,W,1,8,1.03,61.7,M,55.2,M,,*76"};

  Message msg{str};
  Data data{};
  GGA gga{};
  EXPECT_TRUE(gga.parse(msg, data));
}

TEST_F(ParserTest, ParseInvalidGGATimestampTooLong) {
  const auto str{
      "$GPGGA,092750.0009,5321.6802,N,00630.3372,W,1,8,1.03,61.7,M,55.2,M,,*76"};

  Message msg{str};
  Data data{};
  GGA gga{};
  EXPECT_TRUE(gga.parse(msg, data));
}

TEST_F(ParserTest, ParseRMC) {
  const auto str{
      "$GNRMC,084629.000,A,3150.7822,N,11711.9323,E,3.14,119.00,240715,,,D*7C<"};

  Message msg{str};
  Data data{};
  RMC rmc{};
  EXPECT_FALSE(rmc.parse(msg, data));
  EXPECT_EQ(8, data.time.hour);
  EXPECT_EQ(46, data.time.minute);
  EXPECT_EQ(29, data.time.second);
  EXPECT_NEAR(31.846370, data.latitude.degrees, 0.0001);
  EXPECT_EQ(Direction::NORTH, data.latitude.direction);
  EXPECT_NEAR(117.198871, data.longitude.degrees, 0.0001);
  EXPECT_EQ(Direction::EAST, data.longitude.direction);
  EXPECT_NEAR(3.14, data.speed, 0.0001);
  EXPECT_NEAR(119.0, data.course, 0.0001);
  EXPECT_EQ(24, data.date.day);
  EXPECT_EQ(7, data.date.month);
  EXPECT_EQ(15, data.date.year);
}
