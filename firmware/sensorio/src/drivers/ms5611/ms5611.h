/**
 * @file ms5611.h
 * @brief
 */

#ifndef MS_5611_H
#define MS_5611_H

/*****************************************************************************/
/* INCLUDES                                                                  */
/*****************************************************************************/
#include <Arduino.h>
#include <Wire.h>

/*****************************************************************************/
/* DEFINED CONSTANTS                                                         */
/*****************************************************************************/
#define MS5611_ADDRESS_CSB_LOW  0x77
#define MS5611_ADDRESS_CSB_HIGH 0x76

/*****************************************************************************/
/* MACRO DEFINITIONS                                                         */
/*****************************************************************************/

/*****************************************************************************/
/* TYPE DEFINITIONS                                                          */
/*****************************************************************************/
typedef enum {
  MS5611_OSR_256  = 0x00,
  MS5611_OSR_512  = 0x01,
  MS5611_OSR_1024 = 0x02,
  MS5611_OSR_2048 = 0x03,
  MS5611_OSR_4096 = 0x04,
} ms5611_osr_t;

typedef enum {
  MS5611_CMD_RESET         = 0x1e,
  MS5611_CMD_CONV_PRESSURE = 0x40,
  MS5611_CMD_CONV_TEMP     = 0x50,
  MS5611_CMD_READ_ADC      = 0x00, 
  MS5611_CMD_READ_PROM     = 0xa0, 
} ms5611_command_t;

class MS5611Class {
public:
  MS5611Class(TwoWire &twi) :
      twi(twi), addr(MS5611_ADDRESS_CSB_LOW), p_raw(0), p_comp(0), t_raw(0), t_comp(0)
  {
    memset(prom, 0, sizeof(prom));
  }
  ~MS5611Class(void)
  {
  }
  bool     begin(int sda, int scl, int freq = 400000, int addr = MS5611_ADDRESS_CSB_LOW);
  bool     convert(ms5611_osr_t osr = MS5611_OSR_4096);

  uint32_t getRawTemp(void)
  {
    return t_raw;
  }
  uint32_t getCompensatedTemp(void)
  {
    return t_comp;
  }
  uint32_t getRawPressure(void)
  {
    return p_raw;
  }
  uint32_t getCompensatedPressure(void)
  {
    return p_comp;
  }

private:
  TwoWire &twi;
  uint8_t  addr;
  uint16_t prom[8];
  uint32_t p_raw;
  uint32_t p_comp;
  uint32_t t_raw;
  uint32_t t_comp;

  bool reset(void);
  bool readCalibration(void);
  bool validateProm(uint16_t prom[8]);
  bool doConversion(ms5611_command_t cmd, ms5611_osr_t osr, uint32_t &data);
};

#endif /* MS_5611_H */

/****************************** END OF FILE **********************************/
