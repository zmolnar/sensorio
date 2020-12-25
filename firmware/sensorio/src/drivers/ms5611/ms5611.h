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
#define MS5611_ADDRESS_CSB_LOW 0x77

/*****************************************************************************/
/* MACRO DEFINITIONS                                                         */
/*****************************************************************************/

/*****************************************************************************/
/* TYPE DEFINITIONS                                                          */
/*****************************************************************************/
typedef enum {
  MS5611_OSR_256  = 0x00,
  MS5611_OSR_512  = 0x02,
  MS5611_OSR_1024 = 0x04,
  MS5611_OSR_2048 = 0x06,
  MS5611_OSR_4096 = 0x08,
} ms5611_osr_t;

typedef enum {
  MS5611_OK = 0,
  MS5611_ERROR,
} ms5611_error_t;

class MS5611Class {
public:
  MS5611Class(TwoWire &twi) : twi(twi), addr(MS5611_ADDRESS_CSB_LOW), p_raw(0), p_comp(0), t(0)
  {
    memset(prom, 0, sizeof(prom));
  }
  ~MS5611Class(void)
  {
  }
  bool     begin(int sda, int scl, int freq = 400000, int addr = MS5611_ADDRESS_CSB_LOW);
  bool     convert(ms5611_osr_t osr);

private:
  TwoWire &twi;
  uint8_t  addr;
  uint16_t prom[8];
  uint32_t p_raw;
  uint32_t p_comp;
  uint32_t t;

  bool reset(void);
  bool readCalibration(void);
  bool validateProm(uint16_t prom[8]);
};

#endif /* MS_5611_H */

/****************************** END OF FILE **********************************/
