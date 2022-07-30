//
//  This file is part of Sensorio.
//
//  Sensorio is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  Sensorio is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with Sensorio.  If not, see <https://www.gnu.org/licenses/>.
//

#ifndef MS5611_HPP
#define MS5611_HPP

#include <cstdint>
#include <cstring>

using init_fn = bool (*)(void);
using write_fn = size_t (*)(uint8_t addr, uint8_t buf[], size_t length);
using read_fn = size_t (*)(uint8_t addr, uint8_t buf[], size_t length);
using delay_fn = void (*)(uint32_t ms);

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
      t_comp(0) {
    memset(prom, 0, sizeof(prom));
  }

  ~MS5611(void) = default;

  bool start();

  bool update(Osr osr = OSR_4096);

  uint32_t getRawTemp(void) {
    return t_raw;
  }

  uint32_t getCompensatedTemp(void) {
    return t_comp;
  }

  uint32_t getRawPressure(void) {
    return p_raw;
  }

  uint32_t getCompensatedPressure(void) {
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

  uint16_t C(int x) {
    return prom[x];
  }
  bool checkRange(void);
  bool sendCmd(uint8_t buf[], size_t length);
  bool receiveData(uint8_t buf[], size_t length);
  bool reset(void);
  bool readCalibration(void);
  bool validateProm(uint16_t prom[8]);
  bool doConversion(Command cmd, Osr osr, uint32_t &data);
  bool processData(void);
};

#endif
