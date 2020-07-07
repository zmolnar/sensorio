#include "unity.h"

#include "mock_epd.h"
#include "epd_1_9.h"

void setUp(void)
{
}

void tearDown(void)
{
}

void test_epd_EPD_setScanByte_S2(void)
{
    EPD_Line_t line = {0};
    size_t i = 1;

    EPD_setScanByte(&line, i);

    TEST_ASSERT_EQUAL(0xC0, line.line.scan_even[0]);
}

void test_epd_EPD_setScanByte_S4(void)
{
    EPD_Line_t line = {0};
    size_t i = 3;

    EPD_setScanByte(&line, i);

    TEST_ASSERT_EQUAL(0x30, line.line.scan_even[0]);
}

void test_epd_EPD_setScanByte_S6(void)
{
    EPD_Line_t line = {0};
    size_t i = 5;

    EPD_setScanByte(&line, i);

    TEST_ASSERT_EQUAL(0x0C, line.line.scan_even[0]);
}

void test_epd_EPD_setScanByte_S8(void)
{
    EPD_Line_t line = {0};
    size_t i = 7;

    EPD_setScanByte(&line, i);

    TEST_ASSERT_EQUAL(0x03, line.line.scan_even[0]);
}

void test_epd_EPD_setScanByte_S16(void)
{
    EPD_Line_t line = {0};
    size_t i = 15;

    EPD_setScanByte(&line, i);

    TEST_ASSERT_EQUAL(0x03, line.line.scan_even[1]);
}

void test_epd_EPD_setScanByte_S128(void)
{
    EPD_Line_t line = {0};
    size_t i = 127;

    EPD_setScanByte(&line, i);

    TEST_ASSERT_EQUAL(0x03, line.line.scan_even[15]);
}

void test_epd_EPD_setScanByte_S1(void)
{
    EPD_Line_t line = {0};
    size_t i = 0;

    EPD_setScanByte(&line, i);

    TEST_ASSERT_EQUAL(0x03, line.line.scan_odd[15]);
}

void test_epd_EPD_setScanByte_S3(void)
{
    EPD_Line_t line = {0};
    size_t i = 2;

    EPD_setScanByte(&line, i);

    TEST_ASSERT_EQUAL(0x0C, line.line.scan_odd[15]);
}

void test_epd_EPD_setScanByte_S127(void)
{
    EPD_Line_t line = {0};
    size_t i = 126;

    EPD_setScanByte(&line, i);

    TEST_ASSERT_EQUAL(0xC0, line.line.scan_odd[0]);
}

void test_EPD_CreateDataByte_Stage1(void)
{
    uint8_t byte = 0b01011000;
    uint8_t d1 = 0;
    uint8_t d2 = 0;

    EPD_CreateDataBytes(byte, EPD_STAGE_1, &d1, &d2);

    TEST_ASSERT_EQUAL(0b11111110, d1);
    TEST_ASSERT_EQUAL(0b10111011, d2);
}

void test_EPD_CreateDataByte_Stage2(void)
{
    uint8_t byte = 0b01011000;
    uint8_t d1 = 0;
    uint8_t d2 = 0;

    EPD_CreateDataBytes(byte, EPD_STAGE_2, &d1, &d2);

    TEST_ASSERT_EQUAL(0b10101000, d1);
    TEST_ASSERT_EQUAL(0b00100010, d2);
}

void test_EPD_CreateDataByte_Stage3(void)
{
    uint8_t byte = 0b01011000;
    uint8_t d1 = 0;
    uint8_t d2 = 0;

    EPD_CreateDataBytes(byte, EPD_STAGE_3, &d1, &d2);

    TEST_ASSERT_EQUAL(0b11111100, d1);
    TEST_ASSERT_EQUAL(0b00110011, d2);
}

void test_EPD_CreateDataByte_Stage4(void)
{
    uint8_t byte = 0b01011000;
    uint8_t d1 = 0;
    uint8_t d2 = 0;

    EPD_CreateDataBytes(byte, EPD_STAGE_4, &d1, &d2);

    TEST_ASSERT_EQUAL(0b10101011, d1);
    TEST_ASSERT_EQUAL(0b11101110, d2);
}