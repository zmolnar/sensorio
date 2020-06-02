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

#ifndef _BOARD_H_
#define _BOARD_H_

/*
 * Setup for the Olimex STM32-103STK proto board.
 */

/*
 * Board identifier.
 */
#define BOARD_OLIMEX_STM32_103STK
#define BOARD_NAME              "Olimex STM32-103STK"

/*
 * Board frequencies.
 */
#define STM32_LSECLK            32768
#define STM32_HSECLK            8000000

/*
 * MCU type, supported types are defined in ./os/hal/platforms/hal_lld.h.
 */
#define STM32F103xB

/*
 * IO pins assignments.
 */
#define GPIOA_BUTTON_WAKEUP     0
#define GPIOC_BUTTON_TAMPER     13
#define GPIOC_JOY               5
#define GPIOC_JOY_CENTER_BUT    6

#define GPIOA_SPI1NSS           4
#define GPIOB_SPI2NSS           12

#define GPIOC_MMCWP             2
#define GPIOC_MMCCP             1

#define GPIOC_USB_P             4
#define GPIOC_LCD_RES           7
#define GPIOC_NRF_CE            8
#define GPIOC_NRF_IRQ           9
#define GPIOC_LCD_E             10

#define GPIOC_USB_DISC          11
#define GPIOC_LED               12

#define GPIOB_ACCEL_IRQ         5


#define GPIOA_EPD_BUSY          2   // TX2 PA2
#define GPIOA_EPD_RESET         3   // RX2 PA3

#define GPIOB_EPD_VCC_EN        10  // SCL2 PB10
#define GPIOB_EPD_DISCHARGE     11  // SDA2 PB11
#define GPIOB_SPI2_NSS          12  // SPI2_NSS PB12
#define GPIOB_SPI2_SCK          13  // SPI2_SCK PB13
#define GPIOB_SPI2_MISO         14  // SPI2 MISO PB14
#define GPIOB_SPI2_MOSI         15  // SPI2 MOSI PB15
        
/*
 * I/O ports initial setup, this configuration is established soon after reset
 * in the initialization code.
 *
 * The digits have the following meaning:
 *   0 - Analog input.
 *   1 - Push Pull output 10MHz.
 *   2 - Push Pull output 2MHz.
 *   3 - Push Pull output 50MHz.
 *   4 - Digital input.
 *   5 - Open Drain output 10MHz.
 *   6 - Open Drain output 2MHz.
 *   7 - Open Drain output 50MHz.
 *   8 - Digital input with PullUp or PullDown resistor depending on ODR.
 *   9 - Alternate Push Pull output 10MHz.
 *   A - Alternate Push Pull output 2MHz.
 *   B - Alternate Push Pull output 50MHz.
 *   C - Reserved.
 *   D - Alternate Open Drain output 10MHz.
 *   E - Alternate Open Drain output 2MHz.
 *   F - Alternate Open Drain output 50MHz.
 * Please refer to the STM32 Reference Manual for details.
 */

/*
 * Port A setup.
 */
#define VAL_GPIOACRL            0x88883488      /*  PA7...PA0 */
#define VAL_GPIOACRH            0x88888888      /* PA15...PA8 */
#define VAL_GPIOAODR            0xFFFFFFFF

/*
 * Port B setup.
 */
#define VAL_GPIOBCRL            0xEE888888      /*  PB7...PB0 */
#define VAL_GPIOBCRH            0xBBB33388      /* PB15...PB8 */
#define VAL_GPIOBODR            0xFFFF07FF

/*
 * Port C setup.
 */
#define VAL_GPIOCCRL            0x44048888      /*  PC7...PC0 */
#define VAL_GPIOCCRH            0x88863388      /* PC15...PC8 */
#define VAL_GPIOCODR            0xFFFFFFFF

/*
 * Port D setup.
 * Everything input with pull-up except:
 * PD0  - Normal input (XTAL).
 * PD1  - Normal input (XTAL).
 */
#define VAL_GPIODCRL            0x88888844      /*  PD7...PD0 */
#define VAL_GPIODCRH            0x88888888      /* PD15...PD8 */
#define VAL_GPIODODR            0xFFFFFFFF

/*
 * Port E setup.
 * Everything input with pull-up except:
 */
#define VAL_GPIOECRL            0x88888888      /*  PE7...PE0 */
#define VAL_GPIOECRH            0x88888888      /* PE15...PE8 */
#define VAL_GPIOEODR            0xFFFFFFFF

/*
 * USB bus activation macro, required by the USB driver.
 */
#define usb_lld_connect_bus(usbp) palClearPad(GPIOC, GPIOC_USB_DISC)

/*
 * USB bus de-activation macro, required by the USB driver.
 */
#define usb_lld_disconnect_bus(usbp) palSetPad(GPIOC, GPIOC_USB_DISC)

#if !defined(_FROM_ASM_)
#ifdef __cplusplus
extern "C" {
#endif
  void boardInit(void);
#ifdef __cplusplus
}
#endif
#endif /* _FROM_ASM_ */

#endif /* _BOARD_H_ */
