/**
 * @file ms5611.h
 * @brief
 */

#ifndef MS5611_H
#define MS5611_H

/*****************************************************************************/
/* INCLUDES                                                                  */
/*****************************************************************************/
#include <stdint.h>
#include <string.h>

/*****************************************************************************/
/* DEFINED CONSTANTS                                                         */
/*****************************************************************************/

/*****************************************************************************/
/* MACRO DEFINITIONS                                                         */
/*****************************************************************************/

/*****************************************************************************/
/* TYPE DEFINITIONS                                                          */
/*****************************************************************************/
typedef bool (*init_fn)(void);
typedef size_t (*write_fn)(uint8_t addr, uint8_t buf[], size_t length);
typedef size_t (*read_fn)(uint8_t addr, uint8_t buf[], size_t length);
typedef void (*delay_fn)(uint32_t ms);

class MS5611 {
public:
  typedef enum {
    CSB_HIGH = 0x76,
    CSB_LOW = 0x77,
  } Address;

  typedef enum {
    OSR_256 = 0x00,
    OSR_512 = 0x01,
    OSR_1024 = 0x02,
    OSR_2048 = 0x03,
    OSR_4096 = 0x04,
  } Osr;

  typedef enum {
    RESET = 0x1e,
    CONV_PRESSURE = 0x40,
    CONV_TEMP = 0x50,
    READ_ADC = 0x00,
    READ_PROM = 0xa0,
  } Command;

  MS5611(Address addr, init_fn init, write_fn wr, read_fn rd, delay_fn delay) :
      addr(addr),
      init(init),
      write(wr),
      read(rd),
      delay(delay),
      p_raw(0),
      p_comp(0),
      t_raw(0),
      t_comp(0)
  {
    memset(prom, 0, sizeof(prom));
  }
  ~MS5611(void)
  {
  }
  bool start();
  bool update(Osr osr = OSR_4096);

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
  Address addr;
  init_fn init;
  write_fn write;
  read_fn read;
  delay_fn delay;
  uint16_t prom[8];
  uint32_t p_raw;
  uint32_t p_comp;
  uint32_t t_raw;
  uint32_t t_comp;

  bool sendCmd(uint8_t buf[], size_t length);
  bool receiveData(uint8_t buf[], size_t length);
  bool reset(void);
  bool readCalibration(void);
  bool validateProm(uint16_t prom[8]);
  bool doConversion(Command cmd, Osr osr, uint32_t &data);
  bool processData(void);
};

#endif /* MS5611_H */

/****************************** END OF FILE **********************************/
