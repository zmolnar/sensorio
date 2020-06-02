

/*
    ChibiOS - Copyright (C) 2006..2018 Giovanni Di Sirio

    Licensed under the Apache License, Version 2.0 (the "License");
    you may not use this file except in compliance with the License.
    You may obtain a copy of the License at

        http://www.apache.org/licenses/LICENSE-2.0

    Unless required by applicable law or agreed to in writing, software
    distributed under the License is distributed on an "AS IS" BASIS,
    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
    See the License for the specific language governing permissions and
    limitations under the License.
*/
#include "ch.h"
#include "hal.h"

#include "epd.h"
#include <string.h>

#include "paraglider.h"
#include "paraglider2.h"

/*
 * Green LED blinker thread, times are in milliseconds.
 */
static THD_WORKING_AREA(waThread1, 128);

static THD_FUNCTION(Thread1, arg)
{
  (void)arg;
  chRegSetThreadName("blinker");

  while (true) {
    palClearPad(GPIOC, GPIOC_LED);
    chThdSleepMilliseconds(100);
    palSetPad(GPIOC, GPIOC_LED);
    chThdSleepMilliseconds(100);
  }
}

const SPIConfig epdSpiConfig = {
    .end_cb = NULL,
    .ssport = GPIOB,
    .sspad  = GPIOB_SPI2_NSS,
    .cr1    = (((0x2 << 3) & SPI_CR1_BR) | SPI_CR1_MSTR),
};

uint8_t bufA[128][144 / 8];
uint8_t bufB[128][144 / 8];

/*
 * Application entry point.
 */
int main(void)
{
  /*
   * System initializations.
   * - HAL initialization, this also initializes the configured device drivers
   *	 and performs the board-specific initializations.
   * - Kernel initialization, the main() function becomes a thread and the
   *	 RTOS is active.
   */
  halInit();
  chSysInit();

  /*
   * Creates the blinker thread.
   */
  chThdCreateStatic(waThread1, sizeof(waThread1), NORMALPRIO, Thread1, NULL);

  spiObjectInit(&SPID2);
  spiStart(&SPID2, &epdSpiConfig);

  EPD_PowerOn();
  EPD_Initialize();

  memset(bufA, 0, sizeof(bufA));

  systime_t start = chVTGetSystemTime();
  EPD_UpdateDisplay(bufA, paraglider);
  systime_t end = chVTGetSystemTime();

  size_t n = 0;
  while (n < 15) {
    if (n % 2) {
      EPD_PartialUpdate(paraglider, paraglider2);
    } else {
      EPD_PartialUpdate(paraglider2, paraglider);
    }

    n++;
  }

  EPD_UpdateDisplay(paraglider, paraglider);
  EPD_UpdateDisplay(paraglider, paraglider);

  EPD_PowerOff();


  /*
   * Normal main() thread activity, in this demo it does nothing except
   * sleeping in a loop and check the button state.
   */
  while (true) {
    chThdSleepMilliseconds(500);
  }
}
