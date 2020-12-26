/**
 * @file ms5611.cpp
 * @brief
 */

/*****************************************************************************/
/* INCLUDES                                                                  */
/*****************************************************************************/
#include "ms5611.h"
#include "esp32-hal-i2c.h"

/*****************************************************************************/
/* DEFINED CONSTANTS                                                         */
/*****************************************************************************/
// Cx compensation parameter in the PROM
#define C(x) prom[(x)]

/*****************************************************************************/
/* TYPE DEFINITIONS                                                          */
/*****************************************************************************/

/*****************************************************************************/
/* MACRO DEFINITIONS                                                         */
/*****************************************************************************/

/*****************************************************************************/
/* DEFINITION OF GLOBAL CONSTANTS AND VARIABLES                              */
/*****************************************************************************/

/*****************************************************************************/
/* DEFINITION OF LOCAL FUNCTIONS                                             */
/*****************************************************************************/

/*****************************************************************************/
/* DEFINITION OF GLOBAL FUNCTIONS                                            */
/*****************************************************************************/
bool MS5611Class::begin(int sda, int scl, int freq, int addr)
{
  bool success = false;

  if (twi.begin(sda, scl, freq)) {
    this->addr = addr;

    if (reset()) {
      delay(250);

      if (readCalibration()) {
        success = true;
      } else {
        Serial.print("PROM read failed.");
      }
    } else {
      Serial.print("Reset failed.");
    }
  } else {
    Serial.print("TWI begin failed.");
  }

  return success;
}

bool MS5611Class::convert(ms5611_osr_t osr)
{
  bool success = doConversion(MS5611_CMD_CONV_PRESSURE, osr, p_raw);

  if (success) {
    success = doConversion(MS5611_CMD_CONV_TEMP, osr, t_raw);
  }

  if (success) {
    int64_t dT    = (int64_t)t_raw - ((uint64_t)C(5) << 8);
    int64_t temp  = 2000 + ((dT * (int64_t)C(6)) >> 23);
    int64_t off   = ((uint64_t)C(2) << 16) + (((int64_t)C(4) * dT) >> 7);
    int64_t sens  = ((int64_t)C(1) << 15) + ((dT * (int64_t)(C(3)) >> 8));
    int64_t temp2 = 0;
    int64_t off2  = 0;
    int64_t sens2 = 0;

    /* Second order temperature compensation. */
    if (temp < 2000) {
      temp2 = ((dT * dT) >> 31);
      off2  = (5 * (temp - 2000) * (temp - 2000)) >> 1;
      sens2 = (5 * (temp - 2000) * (temp - 2000)) >> 2;

      /* Very low temperature. */
      if (temp < (-15)) {
        off2  = off2 + (7 * (temp + 1500) * (temp + 1500));
        sens2 = sens2 + ((11 * (temp + 1500) * (temp + 1500)) >> 2);
      }
    } else {
      temp2 = 0;
      off2  = 0;
      sens2 = 0;
    }

    temp -= temp2;
    off -= off2;
    sens -= sens2;

    /* Calculate temperature and temperature compensated pressure */
    t_comp = (int32_t)temp;
    p_comp = (uint32_t)(((((int64_t)p_raw * sens) >> 21) - off) >> 15);
  }

  return success;
}

bool MS5611Class::reset(void)
{
  twi.beginTransmission(addr);
  twi.write(MS5611_CMD_RESET);
  uint8_t err = twi.endTransmission();

  return I2C_ERROR_OK == (i2c_err_t)err;
}

bool MS5611Class::readCalibration(void)
{
  bool success = true;

  for (size_t i = 0; (i < 8) && success; ++i) {
    success = false;

    uint32_t cmd = MS5611_CMD_READ_PROM + (i << 1);
    twi.beginTransmission(addr);
    twi.write(cmd);
    twi.endTransmission();

    twi.beginTransmission(addr);
    twi.requestFrom(addr, sizeof(uint16_t));
    if (twi.available()) {
      uint8_t msb = (uint8_t)twi.read();
      if (twi.available()) {
        uint8_t lsb = (uint8_t)twi.read();
        prom[i]     = ((uint16_t)msb << 8) + lsb;
        success     = true;
      }

      Serial.printf("prom[%d] = %d\n", (int)i, (int)prom[i]);
    }
    twi.endTransmission();
  }

  if (success) {
    success = validateProm(prom);
  }

  return success;
}

bool MS5611Class::validateProm(uint16_t prom[8])
{
  uint16_t p7_backup = prom[7];
  uint16_t crc_read  = prom[7] & 0x000f;
  uint16_t remainder = 0;

  prom[7] = (0xFF00 & (prom[7]));

  for (size_t cnt = 0; cnt < 16; cnt++) {
    if (cnt % 2 == 1)
      remainder ^= ((prom[cnt >> 1]) & 0x00FF);
    else
      remainder ^= (prom[cnt >> 1] >> 8);

    for (uint8_t n = 8; n > 0; n--) {
      if (remainder & (0x8000)) {
        remainder = (remainder << 1) ^ 0x3000;
      } else {
        remainder = (remainder << 1);
      }
    }
  }

  prom[7]   = p7_backup;
  remainder = (0x000F & (remainder >> 12));

  return (remainder == crc_read);
}

bool MS5611Class::doConversion(ms5611_command_t cmd, ms5611_osr_t osr, uint32_t &data)
{
  static const uint32_t conv_time_array[5] = {1, 2, 3, 5, 9};

  twi.beginTransmission(addr);
  twi.write(cmd | (osr << 1));
  i2c_err_t err = (i2c_err_t)twi.endTransmission();

  bool success = false;

  if (I2C_ERROR_OK == err) {
    delay(conv_time_array[osr]);

    twi.beginTransmission(addr);
    twi.write(MS5611_CMD_READ_ADC);
    twi.endTransmission();

    twi.beginTransmission(addr);
    twi.requestFrom(addr, (uint8_t)3);

    if (twi.available()) {
      uint8_t msb = (uint8_t)twi.read();
      if (twi.available()) {
        uint8_t xsb = (uint8_t)twi.read();
        if (twi.available()) {
          uint8_t lsb = (uint8_t)twi.read();
          data        = ((uint32_t)msb << 16);
          data += ((uint32_t)xsb << 8);
          data += (uint32_t)lsb;
          success = true;
        }
      }
    }

    twi.endTransmission();
  }

  return success;
}

/****************************** END OF FILE **********************************/
