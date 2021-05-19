/**
 * @file ms5611.cpp
 * @brief
 */

/*****************************************************************************/
/* INCLUDES                                                                  */
/*****************************************************************************/
#include "ms5611.h"
#include <esp_log.h>

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
static const char *tag = "MS5611";

/*****************************************************************************/
/* DEFINITION OF LOCAL FUNCTIONS                                             */
/*****************************************************************************/

/*****************************************************************************/
/* DEFINITION OF GLOBAL FUNCTIONS                                            */
/*****************************************************************************/
bool MS5611::start(void)
{
  bool error = init();
  error = error || reset();
  error = error || readCalibration();
  return error;
}

bool MS5611::update(Osr osr)
{
  bool error = doConversion(CONV_PRESSURE, osr, p_raw);
  error = error || doConversion(CONV_TEMP, osr, t_raw);
  error = error || processData();
  return error;
}

bool MS5611::sendCmd(uint8_t buf[], size_t length)
{
  return length != write(addr, buf, length);
}

bool MS5611::receiveData(uint8_t buf[], size_t length)
{
  return length != read(addr, buf, length);
}

bool MS5611::reset(void)
{
  uint8_t cmd = RESET;
  bool error = sendCmd(&cmd, 1);

  if (!error) {
    delay(250);
  } else {
    ESP_LOGE(tag, "Failed to reset MS5611");
  }

  return error;
}

bool MS5611::readCalibration(void)
{
  bool error = false;
  for (size_t i = 0; (i < 8) && !error; ++i) {
    uint8_t cmd = READ_PROM + (i << 1);
    error = sendCmd(&cmd, 1);

    uint8_t data[2];
    error = error || receiveData(data, 2);

    if (!error) {
      prom[i] = ((uint16_t)data[0] << 8) + data[1];
    } else {
      ESP_LOGE(tag, "Read PROM failed");
    }

    ESP_LOGI(tag, "PROM[%d] = %x", (int)i, (int)prom[i]);
  }

  error = error || validateProm(prom);

  return error;
}

bool MS5611::validateProm(uint16_t prom[8])
{
  uint16_t p7_backup = prom[7];
  uint16_t crc_read = prom[7] & 0x000f;
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

  prom[7] = p7_backup;
  remainder = (0x000F & (remainder >> 12));

  bool isValid = remainder == crc_read;
  ESP_LOGI(tag, "Calibration constants are %svalid", isValid ? "" : "in");

  return !isValid;
}

bool MS5611::doConversion(Command cmd, Osr osr, uint32_t &data)
{
  static const uint32_t conv_time_array[5] = {
      [OSR_256] = 1,
      [OSR_512] = 2,
      [OSR_1024] = 3,
      [OSR_2048] = 5,
      [OSR_4096] = 9,
  };

  uint8_t txdata = cmd | (osr << 1);
  bool error = sendCmd(&txdata, 1);

  if (!error) {
    delay(conv_time_array[osr]);
  }

  txdata = READ_ADC;
  error = error || sendCmd(&txdata, 1);

  uint8_t rxdata[3];
  error = error || receiveData(rxdata, 3);

  if (!error) {
    data = ((uint32_t)rxdata[0]) << 16;
    data += ((uint32_t)rxdata[1]) << 8;
    data += (uint32_t)rxdata[2];
  }

  return error;
}

bool MS5611::processData(void)
{
  int64_t dT = (int64_t)t_raw - ((uint64_t)C(5) << 8);
  int64_t temp = 2000 + ((dT * (int64_t)C(6)) >> 23);
  int64_t off = ((uint64_t)C(2) << 16) + (((int64_t)C(4) * dT) >> 7);
  int64_t sens = ((int64_t)C(1) << 15) + ((dT * (int64_t)(C(3)) >> 8));
  int64_t temp2 = 0;
  int64_t off2 = 0;
  int64_t sens2 = 0;

  /* Second order temperature compensation. */
  if (temp < 2000) {
    temp2 = ((dT * dT) >> 31);
    off2 = (5 * (temp - 2000) * (temp - 2000)) >> 1;
    sens2 = (5 * (temp - 2000) * (temp - 2000)) >> 2;

    /* Very low temperature. */
    if (temp < (-15)) {
      off2 = off2 + (7 * (temp + 1500) * (temp + 1500));
      sens2 = sens2 + ((11 * (temp + 1500) * (temp + 1500)) >> 2);
    }
  } else {
    temp2 = 0;
    off2 = 0;
    sens2 = 0;
  }

  temp -= temp2;
  off -= off2;
  sens -= sens2;

  /* Calculate temperature and temperature compensated pressure */
  t_comp = (int32_t)temp;
  p_comp = (uint32_t)(((((int64_t)p_raw * sens) >> 21) - off) >> 15);

  return false;
}

/****************************** END OF FILE **********************************/
