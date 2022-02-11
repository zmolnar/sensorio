
#include <core/nmea/Processor.hpp>

#include <gtest/gtest.h>

using namespace Nmea0183;

class ProcessorTest : public ::testing::Test {
public:
  virtual void SetUp() {
  }

  virtual void TearDown() {
  }
};

namespace {

  using namespace Nmea0183;
  class Result {
  public:
    Data data{};
    Processor::ErrorCode ecode{Processor::ErrorCode::NO_ERROR};

    bool received{false};
    bool errorDetected{false};
  };

}

TEST_F(ProcessorTest, ProcessGGA) {
  static constexpr auto str{
      "$GNGGA,080237.000,3149.333190,N,11706.911552,E,2,15,0.74,53.489,M,-0."
      "337,M,,*5F\r\n"};

  Result result{};

  auto receiveCb = [&result](const Data &data, const char *str) {
    result.data = data;
    result.received = true;
  };

  auto errorCb = [&result](Processor::ErrorCode ecode, const char *str) {
    result.ecode = ecode;
    result.errorDetected = true;
  };

  Processor proc(receiveCb, errorCb);

  for (auto i = 0; i < strlen(str); ++i) {
    proc.process(str[i]);
  }

  EXPECT_TRUE(result.received);
  EXPECT_FALSE(result.errorDetected);
  EXPECT_EQ(Processor::ErrorCode::NO_ERROR, result.ecode);
}

TEST_F(ProcessorTest, ProcessRMC) {
  static constexpr auto str{
      "$GPRMC,080325.000,A,3149.332234,N,11706.911645,E,0.07,33.95,221121,,,A,"
      "V*27\r\n"};

  Result result{};

  auto receiveCb = [&result](const Data &data, const char *str) {
    result.data = data;
    result.received = true;
  };

  auto errorCb = [&result](Processor::ErrorCode ecode, const char *str) {
    result.ecode = ecode;
    result.errorDetected = true;
  };

  Processor proc(receiveCb, errorCb);

  for (auto i = 0; i < strlen(str); ++i) {
    proc.process(str[i]);
  }

  EXPECT_TRUE(result.received);
  EXPECT_FALSE(result.errorDetected);
  EXPECT_EQ(Processor::ErrorCode::NO_ERROR, result.ecode);
}

TEST_F(ProcessorTest, ProcessUnknownSentence) {
  static constexpr auto str{
      "$GPGSV,4,1,13,194,72,074,43,26,61,222,45,31,61,352,43,32,60,116,47,1*"
      "5A\r\n"};

  Result result{};

  auto receiveCb = [&result](const Data &data, const char *str) {
    result.data = data;
    result.received = true;
  };

  auto errorCb = [&result](Processor::ErrorCode ecode, const char *str) {
    result.ecode = ecode;
    result.errorDetected = true;
  };

  Processor proc(receiveCb, errorCb);

  for (auto i = 0; i < strlen(str); ++i) {
    proc.process(str[i]);
  }

  EXPECT_FALSE(result.received);
  EXPECT_TRUE(result.errorDetected);
  EXPECT_EQ(Processor::ErrorCode::UNKNOWN_SENTENCE, result.ecode);
}

TEST_F(ProcessorTest, ProcessGGAWithInvalidChecksum) {
  static constexpr auto str{
      "$GNGGA,080237.000,3149.333190,N,11706.911552,E,2,15,0.74,53.489,M,-0."
      "337,M,,*5A\r\n"};

  Result result{};

  auto receiveCb = [&result](const Data &data, const char *str) {
    result.data = data;
    result.received = true;
  };

  auto errorCb = [&result](Processor::ErrorCode ecode, const char *str) {
    result.ecode = ecode;
    result.errorDetected = true;
  };

  Processor proc(receiveCb, errorCb);

  for (auto i = 0; i < strlen(str); ++i) {
    proc.process(str[i]);
  }

  EXPECT_FALSE(result.received);
  EXPECT_TRUE(result.errorDetected);
  EXPECT_EQ(Processor::ErrorCode::INVALID_MESSAGE, result.ecode);
}

TEST_F(ProcessorTest, ProcessTooLongMessage) {
  Result result{};

  auto receiveCb = [&result](const Data &data, const char *str) {
    result.data = data;
    result.received = true;
  };

  auto errorCb = [&result](Processor::ErrorCode ecode, const char *str) {
    result.ecode = ecode;
    result.errorDetected = true;
  };

  Processor proc(receiveCb, errorCb);

  for (auto i = 0; i < Processor::MAX_MESSAGE_LENGTH; ++i) {
    proc.process('c');
  }

  EXPECT_FALSE(result.received);
  EXPECT_FALSE(result.errorDetected);
  EXPECT_EQ(Processor::ErrorCode::NO_ERROR, result.ecode);

  proc.process('c');

  EXPECT_FALSE(result.received);
  EXPECT_TRUE(result.errorDetected);
  EXPECT_EQ(Processor::ErrorCode::BUFFER_OVERFLOW, result.ecode);
}

TEST_F(ProcessorTest, ProcessEmptyGGA) {
  static constexpr auto str{"$GPGGA,051638.920,,,,,0,0,,,M,,M,,*4A\r\n"};

  Result result{};

  auto receiveCb = [&result](const Data &data, const char *str) {
    result.data = data;
    result.received = true;
  };

  auto errorCb = [&result](Processor::ErrorCode ecode, const char *str) {
    result.ecode = ecode;
    result.errorDetected = true;
  };

  Processor proc(receiveCb, errorCb);

  for (auto i = 0; i < strlen(str); ++i) {
    proc.process(str[i]);
  }

  EXPECT_TRUE(result.received);
  EXPECT_FALSE(result.errorDetected);
  EXPECT_EQ(Processor::ErrorCode::NO_ERROR, result.ecode);
}
