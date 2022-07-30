
#include <core/nmea/Message.hpp>

#include <gtest/gtest.h>

using namespace Nmea0183;

class TokenTest : public ::testing::Test {
public:
  virtual void SetUp() {
  }

  virtual void TearDown() {
  }
};

TEST_F(TokenTest, GetCrcOfToken) {
  const etl::string_view str{
      "$GPGGA,092750.000,5321.6802,N,00630.3372,W,1,8,1.03,61.7,M,55.2,M,,*76"};

  auto refCrc = static_cast<uint8_t>(0);
  EXPECT_FALSE(Token(str.cbegin() + 1, str.cend() - 3).getCrc8(refCrc));
  EXPECT_EQ(refCrc, 0x76);
}

TEST_F(TokenTest, GetDate) {
  const etl::string_view str{"100122"};

  Date date{};
  EXPECT_FALSE(Token{str}.get(date));
  EXPECT_EQ(10, date.day);
  EXPECT_EQ(01, date.month);
  EXPECT_EQ(22, date.year);
}

TEST_F(TokenTest, GetDateInvalidLength) {
  const etl::string_view str{"1001221111"};

  Date date{};
  EXPECT_TRUE(Token{str}.get(date));
}

TEST_F(TokenTest, GetDateInvalidChar) {
  const etl::string_view str{"1A0122"};

  Date date{};
  EXPECT_TRUE(Token{str}.get(date));
}

TEST_F(TokenTest, GetTime) {
  const etl::string_view str{"080237.321"};

  Time time{};
  EXPECT_FALSE(Token{str}.get(time));
  EXPECT_EQ(8, time.hour);
  EXPECT_EQ(2, time.minute);
  EXPECT_EQ(37, time.second);
  EXPECT_EQ(321, time.millisec);
}

TEST_F(TokenTest, GetTimeInvalidLength) {
  const etl::string_view str{"080237.3213"};

  Time time{};
  EXPECT_TRUE(Token{str}.get(time));
}

TEST_F(TokenTest, GetTimeInvalidChar) {
  const etl::string_view str{"080237.A21"};

  Time time{};
  EXPECT_TRUE(Token{str}.get(time));
}

TEST_F(TokenTest, GetLatLonLength4) {
  const etl::string_view str{"5132.0097"};

  LatLon latitude{};
  EXPECT_FALSE(Token{str}.get(latitude));
  EXPECT_NEAR(51.533495, latitude.degrees, 0.0000001);
}

TEST_F(TokenTest, GetLatLonLength5) {
  const etl::string_view str{"5132.00976"};

  LatLon latitude{};
  EXPECT_FALSE(Token{str}.get(latitude));
  EXPECT_NEAR(51.533496, latitude.degrees, 0.0000001);
}

TEST_F(TokenTest, GetLatLonLength6) {
  const etl::string_view str{"5132.009765"};

  LatLon latitude{};
  EXPECT_FALSE(Token{str}.get(latitude));
  EXPECT_NEAR(51.533496, latitude.degrees, 0.0000001);
}

TEST_F(TokenTest, GetLatLonLengthTooShort) {
  const etl::string_view str{"5132.00"};

  LatLon latitude{};
  EXPECT_TRUE(Token{str}.get(latitude));
}

TEST_F(TokenTest, GetLatLonLengthTooLong) {
  const etl::string_view str{"5132.0011111"};

  LatLon latitude{};
  EXPECT_TRUE(Token{str}.get(latitude));
}

TEST_F(TokenTest, GetLatLonDegreesTooShort) {
  const etl::string_view str{"532.001111"};

  LatLon latitude{};
  EXPECT_TRUE(Token{str}.get(latitude));
}

TEST_F(TokenTest, GetLatLonDegreesTooLong) {
  const etl::string_view str{"132222.001111"};

  LatLon latitude{};
  EXPECT_TRUE(Token{str}.get(latitude));
}

TEST_F(TokenTest, GetDirectionSouth) {
  const etl::string_view str{"S"};

  auto dir{Direction::UNKNOWN};
  EXPECT_FALSE(Token{str}.get(dir));
  EXPECT_EQ(Direction::SOUTH, dir);
}

TEST_F(TokenTest, GetDirectionNorth) {
  const etl::string_view str{"N"};

  auto dir{Direction::UNKNOWN};
  EXPECT_FALSE(Token{str}.get(dir));
  EXPECT_EQ(Direction::NORTH, dir);
}

TEST_F(TokenTest, GetDirectionEast) {
  const etl::string_view str{"E"};

  auto dir{Direction::UNKNOWN};
  EXPECT_FALSE(Token{str}.get(dir));
  EXPECT_EQ(Direction::EAST, dir);
}

TEST_F(TokenTest, GetDirectionWest) {
  const etl::string_view str{"W"};

  auto dir{Direction::UNKNOWN};
  EXPECT_FALSE(Token{str}.get(dir));
  EXPECT_EQ(Direction::WEST, dir);
}

TEST_F(TokenTest, GetDirectionUnknown) {
  const etl::string_view str{"X"};

  auto dir{Direction::UNKNOWN};
  EXPECT_TRUE(Token{str}.get(dir));
  EXPECT_EQ(Direction::UNKNOWN, dir);
}

TEST_F(TokenTest, GetDirectionInvalid) {
  const etl::string_view str{"XXXXXXX"};

  auto dir{Direction::UNKNOWN};
  EXPECT_TRUE(Token{str}.get(dir));
  EXPECT_EQ(Direction::UNKNOWN, dir);
}

TEST_F(TokenTest, GetQualityInvalid) {
  const etl::string_view str{"0"};

  auto quality{Quality::UNKNOWN};
  EXPECT_FALSE(Token{str}.get(quality));
  EXPECT_EQ(Quality::INVALID, quality);
}

TEST_F(TokenTest, GetQualitySpsValid) {
  const etl::string_view str{"1"};

  auto quality{Quality::UNKNOWN};
  EXPECT_FALSE(Token{str}.get(quality));
  EXPECT_EQ(Quality::SPS_VALID, quality);
}

TEST_F(TokenTest, GetQualityDgpsSpsSbasValid) {
  const etl::string_view str{"2"};

  auto quality{Quality::UNKNOWN};
  EXPECT_FALSE(Token{str}.get(quality));
  EXPECT_EQ(Quality::DGPS_SPS_SBAS_VALID, quality);
}

TEST_F(TokenTest, GetQualityPpsValid) {
  const etl::string_view str{"3"};

  auto quality{Quality::UNKNOWN};
  EXPECT_FALSE(Token{str}.get(quality));
  EXPECT_EQ(Quality::PPS_VALID, quality);
}

TEST_F(TokenTest, GetQualityRtkFixedInt) {
  const etl::string_view str{"4"};

  auto quality{Quality::UNKNOWN};
  EXPECT_FALSE(Token{str}.get(quality));
  EXPECT_EQ(Quality::RTK_FIXED_INT, quality);
}

TEST_F(TokenTest, GetQualityRtkFloat) {
  const etl::string_view str{"5"};

  auto quality{Quality::UNKNOWN};
  EXPECT_FALSE(Token{str}.get(quality));
  EXPECT_EQ(Quality::RTK_FLOAT, quality);
}

TEST_F(TokenTest, GetQualityEstimated) {
  const etl::string_view str{"6"};

  auto quality{Quality::UNKNOWN};
  EXPECT_FALSE(Token{str}.get(quality));
  EXPECT_EQ(Quality::ESTIMATED, quality);
}

TEST_F(TokenTest, GetQualityInvalidLength) {
  const etl::string_view str{"0X"};

  auto quality{Quality::UNKNOWN};
  EXPECT_TRUE(Token{str}.get(quality));
  EXPECT_EQ(Quality::UNKNOWN, quality);
}

TEST_F(TokenTest, GetQualityInvalidValue) {
  const etl::string_view str{"9"};

  auto quality{Quality::UNKNOWN};
  EXPECT_TRUE(Token{str}.get(quality));
  EXPECT_EQ(Quality::UNKNOWN, quality);
}

TEST_F(TokenTest, GetUint32) {
  static const auto str{"1234567"};
  uint32_t num{0};

  Token{str}.get(num);
  EXPECT_EQ(num, 1234567);
}

TEST_F(TokenTest, GetInvalidUint32) {
  static const auto str{"abcdef"};
  uint32_t num{0};

  Token{str}.get(num);
  EXPECT_EQ(num, 0x00);
}

TEST_F(TokenTest, GetDouble) {
  static const auto str{"3.1415"};
  auto num = static_cast<double>(0.0);

  EXPECT_FALSE(Token{str}.get(num));
  EXPECT_DOUBLE_EQ(num, 3.1415);
}

TEST_F(TokenTest, GetNegativeDouble) {
  static const auto str{"-3.1415"};
  auto num = static_cast<double>(0.0);

  EXPECT_FALSE(Token{str}.get(num));
  EXPECT_DOUBLE_EQ(num, -3.1415);
}

TEST_F(TokenTest, GetDoubleNoIntegralPart) {
  static const auto str{".1415"};
  auto num = static_cast<double>(0.0);

  EXPECT_FALSE(Token{str}.get(num));
  EXPECT_DOUBLE_EQ(num, 0.1415);
}

TEST_F(TokenTest, GetDoubleNoFractionalPartWithDot) {
  static const auto str{"3."};
  auto num = static_cast<double>(0.0);

  EXPECT_FALSE(Token{str}.get(num));
  EXPECT_DOUBLE_EQ(num, 3.0);
}

TEST_F(TokenTest, GetDoubleNoFractionalPartWithoutDot) {
  static const auto str{"1415"};
  auto num = static_cast<double>(0.0);

  EXPECT_FALSE(Token{str}.get(num));
  EXPECT_DOUBLE_EQ(num, 1415);
}

TEST_F(TokenTest, GetDoubleInvalidIntegralPart) {
  static const auto str{"XX.1415"};
  auto num = static_cast<double>(0.0);

  EXPECT_TRUE(Token{str}.get(num));
  EXPECT_DOUBLE_EQ(num, 0);
}

TEST_F(TokenTest, GetDoubleInvalidFractionalPart) {
  static const auto str{"3.XX1415"};
  auto num = static_cast<double>(0.0);

  EXPECT_TRUE(Token{str}.get(num));
  EXPECT_DOUBLE_EQ(num, 0);
}

class MessageTest : public ::testing::Test {
public:
  virtual void SetUp() {
  }

  virtual void TearDown() {
  }
};

TEST_F(MessageTest, Validation) {
  static constexpr auto str{
      "$GPGGA,092750.000,5321.6802,N,00630.3372,W,1,8,1.03,61.7,M,55.2,M,,*76"};

  Message msg{str};
  EXPECT_TRUE(msg.isValid());
}

TEST_F(MessageTest, GetTalkerID) {
  static constexpr auto str{
      "$GPGGA,092750.000,5321.6802,N,00630.3372,W,1,8,1.03,61.7,M,55.2,M,,*76"};

  Message msg{str};
  EXPECT_STREQ("GP", msg.talkerID().c_str());
}

TEST_F(MessageTest, getSentenceFormatter) {
  static constexpr auto str{
      "$GPGGA,092750.000,5321.6802,N,00630.3372,W,1,8,1.03,61.7,M,55.2,M,,*76"};

  Message msg{str};
  EXPECT_STREQ(msg.sentenceFormat().c_str(), "GGA");
}

TEST_F(MessageTest, getNext) {
  static constexpr auto str{
      "$GPGGA,092750.000,5321.6802,N,00630.3372,W,1,8,1.03,61.7,M,55.2,M,,*76"};

  Message msg{str};

  double dval{0};
  msg.ignoreNext();
  EXPECT_FALSE(msg.getNext(dval));
  EXPECT_NEAR(92750.0, dval, 0.0001);

  EXPECT_FALSE(msg.getNext(dval));
  EXPECT_NEAR(5321.6802, dval, 0.0001);

  Direction dir{Direction::UNKNOWN};
  EXPECT_FALSE(msg.getNext(dir));
  EXPECT_EQ(Direction::NORTH, dir);

  EXPECT_FALSE(msg.getNext(dval));
  EXPECT_NEAR(630.3372, dval, 0.0001);

  EXPECT_FALSE(msg.getNext(dir));
  EXPECT_EQ(Direction::WEST, dir);

  EXPECT_FALSE(msg.getNext(dval));
  EXPECT_NEAR(1, dval, 0.0001);

  EXPECT_FALSE(msg.getNext(dval));
  EXPECT_NEAR(8, dval, 0.0001);

  EXPECT_FALSE(msg.getNext(dval));
  EXPECT_NEAR(1.03, dval, 0.0001);

  EXPECT_FALSE(msg.getNext(dval));
  EXPECT_NEAR(61.7, dval, 0.0001);

  msg.ignoreNext();

  EXPECT_FALSE(msg.getNext(dval));
  EXPECT_NEAR(55.2, dval, 0.0001);

  msg.ignoreNext();
}
