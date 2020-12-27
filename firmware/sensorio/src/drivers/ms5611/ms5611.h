/**
 * @file ms5611.h
 * @brief
 */

#ifndef MS5611_H
#define MS5611_H

/*****************************************************************************/
/* INCLUDES                                                                  */
/*****************************************************************************/
#include <Arduino.h>
#include <Wire.h>

/*****************************************************************************/
/* DEFINED CONSTANTS                                                         */
/*****************************************************************************/

/*****************************************************************************/
/* MACRO DEFINITIONS                                                         */
/*****************************************************************************/

/*****************************************************************************/
/* TYPE DEFINITIONS                                                          */
/*****************************************************************************/
class MS5611 {
public:
  typedef enum {
    CSB_HIGH = 0x76,
    CSB_LOW  = 0x77,
  } Address;

  typedef enum {
    OSR_256  = 0x00,
    OSR_512  = 0x01,
    OSR_1024 = 0x02,
    OSR_2048 = 0x03,
    OSR_4096 = 0x04,
  } Osr;

  typedef enum {
    RESET         = 0x1e,
    CONV_PRESSURE = 0x40,
    CONV_TEMP     = 0x50,
    READ_ADC      = 0x00,
    READ_PROM     = 0xa0,
  } Command;

  MS5611(TwoWire &twi) :
      twi(twi), addr(0), p_raw(0), p_comp(0), t_raw(0), t_comp(0)
  {
    memset(prom, 0, sizeof(prom));
  }
  ~MS5611(void)
  {
  }
  bool begin(int sda, int scl, int freq = 400000, int addr = CSB_LOW);
  bool convert(Osr osr = OSR_4096);

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
  bool doConversion(Command cmd, Osr osr, uint32_t &data);
};

#endif /* MS5611_H */

/****************************** END OF FILE **********************************/
