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
#define C(i) prom[(i)]

/*****************************************************************************/
/* TYPE DEFINITIONS                                                          */
/*****************************************************************************/
/**
 * MS5611 command set.
 * @defgroup MS5611 pressure sensor command set.
 * @{
 */
typedef enum {
  MS5611_CMD_RESET         = 0x1e, /**< Reset command. */
  MS5611_CMD_CONV_PRESSURE = 0x40,
  MS5611_CMD_CONV_TEMP     = 0x50,
  MS5611_CMD_ADC_READ      = 0x00, /**< Read conversation result. */
  MS5611_CMD_PROM_READ     = 0xa0, /**< Read PROM register. */
} ms5611_command_t;

/*****************************************************************************/
/* MACRO DEFINITIONS                                                         */
/*****************************************************************************/
#define MS5611_CMD_CONV_PRESSURE_OSR(osr)  (MS5611_CMD_CONV_PRESSURE + (osr))
#define MS5611_CMD_CONV_TEMP_OSR(osr)      (MS5611_CMD_CONV_TEMP + (osr))
#define MS5611_CMD_READ_PROM_REGISTER(reg) (MS5611_CMD_PROM_READ + ((reg) << 1))

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
  bool result = false;

  if (twi.begin(sda, scl, freq)) {
    this->addr = addr;

    if (reset()) {
      delay(250);

      result = readCalibration();
    } else {
      Serial.print("Reset failed.");
    }
  } else {
    Serial.print("TWI begin failed.");
  }

  return result;
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

    uint32_t cmd = MS5611_CMD_READ_PROM_REGISTER(i);
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

/****************************** END OF FILE **********************************/
