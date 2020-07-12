/*
    ChibiOS - Copyright (C) 2006..2020 Giovanni Di Sirio

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

/*
 * This file has been automatically generated using ChibiStudio board
 * generator plugin. Do not edit manually.
 */

#ifndef BOARD_H
#define BOARD_H

/*===========================================================================*/
/* Driver constants.                                                         */
/*===========================================================================*/

/*
 * Setup for STMicroelectronics STM32L4_IOT_NODE board.
 */

/*
 * Board identifier.
 */
#define BOARD_ST_STM32L4_IOT_NODE
#define BOARD_NAME                  "STMicroelectronics STM32L4_IOT_NODE"

/*
 * Board oscillators-related settings.
 * NOTE: HSE not fitted.
 */
#if !defined(STM32_LSECLK)
#define STM32_LSECLK                32768U
#endif

#define STM32_LSEDRV                (3U << 3U)

#if !defined(STM32_HSECLK)
#define STM32_HSECLK                0U
#endif

/*
 * Board voltages.
 * Required for performance limits calculation.
 */
#define STM32_VDD                   300U

/*
 * MCU type as defined in the ST header.
 */
#define STM32L476xx

/*
 * IO pins assignments.
 */
#define GPIOA_UART4_TX              0U
#define GPIOA_UART4_RX              1U
#define GPIOA_TIM2_CH3              2U
#define GPIOA_PIN3                  3U
#define GPIOA_PIN4                  4U
#define GPIOA_SPI1_SCK              5U
#define GPIOA_SPI1_MISO             6U
#define GPIOA_SPI1_MOSI             7U
#define GPIOA_SPBTLE_RF_RST         8U
#define GPIOA_USB_OTG_FS_VBUS       9U
#define GPIOA_USB_OTG_FS_ID         10U
#define GPIOA_USB_OTG_FS_DM         11U
#define GPIOA_USB_OTG_FS_DP         12U
#define GPIOA_SWDIO                 13U
#define GPIOA_SWCLK                 14U
#define GPIOA_TIM2_CH1              15U

#define GPIOB_TIM3_CH3              0U
#define GPIOB_TIM3_CH4              1U
#define GPIOB_PIN2                  2U
#define GPIOB_SWO                   3U
#define GPIOB_TIM3_CH1              4U
#define GPIOB_PIN5                  5U
#define GPIOB_STLINK_USART1_TX      6U
#define GPIOB_STLINK_USART1_RX      7U
#define GPIOB_I2C1_SCL              8U
#define GPIOB_I2C1_SDA              9U
#define GPIOB_I2C2_SCL              10U
#define GPIOB_I2C2_SDA              11U
#define GPIOB_PIN12                 12U
#define GPIOB_PIN13                 13U
#define GPIOB_LED2                  14U
#define GPIOB_PIN15                 15U

#define GPIOC_ADC1_IN1              0U
#define GPIOC_ADC1_IN2              1U
#define GPIOC_ADC1_IN3              2U
#define GPIOC_ADC1_IN4              3U
#define GPIOC_ADC1_IN13             4U
#define GPIOC_ADC1_IN14             5U
#define GPIOC_VL53L0X_XSHUT         6U
#define GPIOC_VL53L0X_GPIO1_EXTI7   7U
#define GPIOC_LIS3MDL_DRDY_EXTI8    8U
#define GPIOC_LED3_LED4             9U
#define GPIOC_SPI3_SCK              10U
#define GPIOC_SPI3_MISO             11U
#define GPIOC_SPI3_MOSI             12U
#define GPIOC_BUTTON_EXTI13         13U
#define GPIOC_OSC32_IN              14U
#define GPIOC_OSC32_OUT             15U

#define GPIOD_PMOD_RESET            0U
#define GPIOD_PMOD_SPI2_SCK         1U
#define GPIOD_PMOD_IRQ_EXTI2        2U
#define GPIOD_USART2_CTS            3U
#define GPIOD_USART2_RTS            4U
#define GPIOD_USART2_TX             5U
#define GPIOD_USART2_RX             6U
#define GPIOD_STSAFE_A100_RESET     7U
#define GPIOD_USART3_TX             8U
#define GPIOD_USART3_RX             9U
#define GPIOD_LPS22HB_INT_DRDY_EXTI10 10U
#define GPIOD_LSM6DSL_INT1_EXTI11   11U
#define GPIOD_USB_OTG_FS_PWR_EN     12U
#define GPIOD_SPBTLE_RF_SPI3_CSN    13U
#define GPIOD_PIN14                 14U
#define GPIOD_HTS221_DRDY_EXTI15    15U

#define GPIOE_ISM43362_SPI3_CSN     0U
#define GPIOE_ISM43362_DRDY_EXTI1   1U
#define GPIOE_M24SR64_Y_RF_DISABLE  2U
#define GPIOE_USB_OTG_OVRCR_EXTI3   3U
#define GPIOE_M24SR64_Y_GPO         4U
#define GPIOE_SPSGRF_915_GPIO3_EXTI5 5U
#define GPIOE_SPBTLE_RF_IRQ_EXTI6   6U
#define GPIOE_DFSDM1_DATIN2         7U
#define GPIOE_ISM43362_RST          8U
#define GPIOE_DFSDM1_CKOUT          9U
#define GPIOE_QUADSPI_CLK           10U
#define GPIOE_QUADSPI_NCS           11U
#define GPIOE_QUADSPI_BK1_IO0       12U
#define GPIOE_QUADSPI_BK1_IO1       13U
#define GPIOE_QUADSPI_BK1_IO2       14U
#define GPIOE_QUADSPI_BK1_IO3       15U

#define GPIOF_PIN0                  0U
#define GPIOF_PIN1                  1U
#define GPIOF_PIN2                  2U
#define GPIOF_PIN3                  3U
#define GPIOF_PIN4                  4U
#define GPIOF_PIN5                  5U
#define GPIOF_PIN6                  6U
#define GPIOF_PIN7                  7U
#define GPIOF_PIN8                  8U
#define GPIOF_PIN9                  9U
#define GPIOF_PIN10                 10U
#define GPIOF_PIN11                 11U
#define GPIOF_PIN12                 12U
#define GPIOF_PIN13                 13U
#define GPIOF_PIN14                 14U
#define GPIOF_PIN15                 15U

#define GPIOG_PIN0                  0U
#define GPIOG_PIN1                  1U
#define GPIOG_PIN2                  2U
#define GPIOG_PIN3                  3U
#define GPIOG_PIN4                  4U
#define GPIOG_PIN5                  5U
#define GPIOG_PIN6                  6U
#define GPIOG_PIN7                  7U
#define GPIOG_PIN8                  8U
#define GPIOG_PIN9                  9U
#define GPIOG_PIN10                 10U
#define GPIOG_PIN11                 11U
#define GPIOG_PIN12                 12U
#define GPIOG_PIN13                 13U
#define GPIOG_PIN14                 14U
#define GPIOG_PIN15                 15U

#define GPIOH_OSC_IN                0U
#define GPIOH_OSC_OUT               1U
#define GPIOH_PIN2                  2U
#define GPIOH_PIN3                  3U
#define GPIOH_PIN4                  4U
#define GPIOH_PIN5                  5U
#define GPIOH_PIN6                  6U
#define GPIOH_PIN7                  7U
#define GPIOH_PIN8                  8U
#define GPIOH_PIN9                  9U
#define GPIOH_PIN10                 10U
#define GPIOH_PIN11                 11U
#define GPIOH_PIN12                 12U
#define GPIOH_PIN13                 13U
#define GPIOH_PIN14                 14U
#define GPIOH_PIN15                 15U

/*
 * IO lines assignments.
 */
#define LINE_UART4_TX               PAL_LINE(GPIOA, 0U)
#define LINE_UART4_RX               PAL_LINE(GPIOA, 1U)
#define LINE_TIM2_CH3               PAL_LINE(GPIOA, 2U)
#define LINE_PIN3                   PAL_LINE(GPIOA, 3U)
#define LINE_PIN4                   PAL_LINE(GPIOA, 4U)
#define LINE_SPI1_SCK               PAL_LINE(GPIOA, 5U)
#define LINE_SPI1_MISO              PAL_LINE(GPIOA, 6U)
#define LINE_SPI1_MOSI              PAL_LINE(GPIOA, 7U)
#define LINE_SPBTLE_RF_RST          PAL_LINE(GPIOA, 8U)
#define LINE_USB_OTG_FS_VBUS        PAL_LINE(GPIOA, 9U)
#define LINE_USB_OTG_FS_ID          PAL_LINE(GPIOA, 10U)
#define LINE_USB_OTG_FS_DM          PAL_LINE(GPIOA, 11U)
#define LINE_USB_OTG_FS_DP          PAL_LINE(GPIOA, 12U)
#define LINE_SWDIO                  PAL_LINE(GPIOA, 13U)
#define LINE_SWCLK                  PAL_LINE(GPIOA, 14U)
#define LINE_TIM2_CH1               PAL_LINE(GPIOA, 15U)
#define LINE_TIM3_CH3               PAL_LINE(GPIOB, 0U)
#define LINE_TIM3_CH4               PAL_LINE(GPIOB, 1U)
#define LINE_PIN2                   PAL_LINE(GPIOB, 2U)
#define LINE_SWO                    PAL_LINE(GPIOB, 3U)
#define LINE_TIM3_CH1               PAL_LINE(GPIOB, 4U)
#define LINE_PIN5                   PAL_LINE(GPIOB, 5U)
#define LINE_STLINK_USART1_TX       PAL_LINE(GPIOB, 6U)
#define LINE_STLINK_USART1_RX       PAL_LINE(GPIOB, 7U)
#define LINE_I2C1_SCL               PAL_LINE(GPIOB, 8U)
#define LINE_I2C1_SDA               PAL_LINE(GPIOB, 9U)
#define LINE_I2C2_SCL               PAL_LINE(GPIOB, 10U)
#define LINE_I2C2_SDA               PAL_LINE(GPIOB, 11U)
#define LINE_PIN12                  PAL_LINE(GPIOB, 12U)
#define LINE_PIN13                  PAL_LINE(GPIOB, 13U)
#define LINE_LED2                   PAL_LINE(GPIOB, 14U)
#define LINE_PIN15                  PAL_LINE(GPIOB, 15U)
#define LINE_ADC1_IN1               PAL_LINE(GPIOC, 0U)
#define LINE_ADC1_IN2               PAL_LINE(GPIOC, 1U)
#define LINE_ADC1_IN3               PAL_LINE(GPIOC, 2U)
#define LINE_ADC1_IN4               PAL_LINE(GPIOC, 3U)
#define LINE_ADC1_IN13              PAL_LINE(GPIOC, 4U)
#define LINE_ADC1_IN14              PAL_LINE(GPIOC, 5U)
#define LINE_VL53L0X_XSHUT          PAL_LINE(GPIOC, 6U)
#define LINE_VL53L0X_GPIO1_EXTI7    PAL_LINE(GPIOC, 7U)
#define LINE_LIS3MDL_DRDY_EXTI8     PAL_LINE(GPIOC, 8U)
#define LINE_LED3_LED4              PAL_LINE(GPIOC, 9U)
#define LINE_SPI3_SCK               PAL_LINE(GPIOC, 10U)
#define LINE_SPI3_MISO              PAL_LINE(GPIOC, 11U)
#define LINE_SPI3_MOSI              PAL_LINE(GPIOC, 12U)
#define LINE_BUTTON_EXTI13          PAL_LINE(GPIOC, 13U)
#define LINE_OSC32_IN               PAL_LINE(GPIOC, 14U)
#define LINE_OSC32_OUT              PAL_LINE(GPIOC, 15U)
#define LINE_PMOD_RESET             PAL_LINE(GPIOD, 0U)
#define LINE_PMOD_SPI2_SCK          PAL_LINE(GPIOD, 1U)
#define LINE_PMOD_IRQ_EXTI2         PAL_LINE(GPIOD, 2U)
#define LINE_USART2_CTS             PAL_LINE(GPIOD, 3U)
#define LINE_USART2_RTS             PAL_LINE(GPIOD, 4U)
#define LINE_USART2_TX              PAL_LINE(GPIOD, 5U)
#define LINE_USART2_RX              PAL_LINE(GPIOD, 6U)
#define LINE_STSAFE_A100_RESET      PAL_LINE(GPIOD, 7U)
#define LINE_USART3_TX              PAL_LINE(GPIOD, 8U)
#define LINE_USART3_RX              PAL_LINE(GPIOD, 9U)
#define LINE_LPS22HB_INT_DRDY_EXTI10 PAL_LINE(GPIOD, 10U)
#define LINE_LSM6DSL_INT1_EXTI11    PAL_LINE(GPIOD, 11U)
#define LINE_USB_OTG_FS_PWR_EN      PAL_LINE(GPIOD, 12U)
#define LINE_SPBTLE_RF_SPI3_CSN     PAL_LINE(GPIOD, 13U)
#define LINE_PIN14                  PAL_LINE(GPIOD, 14U)
#define LINE_HTS221_DRDY_EXTI15     PAL_LINE(GPIOD, 15U)
#define LINE_ISM43362_SPI3_CSN      PAL_LINE(GPIOE, 0U)
#define LINE_ISM43362_DRDY_EXTI1    PAL_LINE(GPIOE, 1U)
#define LINE_M24SR64_Y_RF_DISABLE   PAL_LINE(GPIOE, 2U)
#define LINE_USB_OTG_OVRCR_EXTI3    PAL_LINE(GPIOE, 3U)
#define LINE_M24SR64_Y_GPO          PAL_LINE(GPIOE, 4U)
#define LINE_SPSGRF_915_GPIO3_EXTI5 PAL_LINE(GPIOE, 5U)
#define LINE_SPBTLE_RF_IRQ_EXTI6    PAL_LINE(GPIOE, 6U)
#define LINE_DFSDM1_DATIN2          PAL_LINE(GPIOE, 7U)
#define LINE_ISM43362_RST           PAL_LINE(GPIOE, 8U)
#define LINE_DFSDM1_CKOUT           PAL_LINE(GPIOE, 9U)
#define LINE_QUADSPI_CLK            PAL_LINE(GPIOE, 10U)
#define LINE_QUADSPI_NCS            PAL_LINE(GPIOE, 11U)
#define LINE_QUADSPI_BK1_IO0        PAL_LINE(GPIOE, 12U)
#define LINE_QUADSPI_BK1_IO1        PAL_LINE(GPIOE, 13U)
#define LINE_QUADSPI_BK1_IO2        PAL_LINE(GPIOE, 14U)
#define LINE_QUADSPI_BK1_IO3        PAL_LINE(GPIOE, 15U)
#define LINE_OSC_IN                 PAL_LINE(GPIOH, 0U)
#define LINE_OSC_OUT                PAL_LINE(GPIOH, 1U)

/*===========================================================================*/
/* Driver pre-compile time settings.                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/

/*===========================================================================*/
/* Driver data structures and types.                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Driver macros.                                                            */
/*===========================================================================*/

/*
 * I/O ports initial setup, this configuration is established soon after reset
 * in the initialization code.
 * Please refer to the STM32 Reference Manual for details.
 */
#define PIN_MODE_INPUT(n)           (0U << ((n) * 2U))
#define PIN_MODE_OUTPUT(n)          (1U << ((n) * 2U))
#define PIN_MODE_ALTERNATE(n)       (2U << ((n) * 2U))
#define PIN_MODE_ANALOG(n)          (3U << ((n) * 2U))
#define PIN_ODR_LOW(n)              (0U << (n))
#define PIN_ODR_HIGH(n)             (1U << (n))
#define PIN_OTYPE_PUSHPULL(n)       (0U << (n))
#define PIN_OTYPE_OPENDRAIN(n)      (1U << (n))
#define PIN_OSPEED_VERYLOW(n)       (0U << ((n) * 2U))
#define PIN_OSPEED_LOW(n)           (1U << ((n) * 2U))
#define PIN_OSPEED_MEDIUM(n)        (2U << ((n) * 2U))
#define PIN_OSPEED_HIGH(n)          (3U << ((n) * 2U))
#define PIN_PUPDR_FLOATING(n)       (0U << ((n) * 2U))
#define PIN_PUPDR_PULLUP(n)         (1U << ((n) * 2U))
#define PIN_PUPDR_PULLDOWN(n)       (2U << ((n) * 2U))
#define PIN_AFIO_AF(n, v)           ((v) << (((n) % 8U) * 4U))
#define PIN_ASCR_DISABLED(n)        (0U << (n))
#define PIN_ASCR_ENABLED(n)         (1U << (n))
#define PIN_LOCKR_DISABLED(n)       (0U << (n))
#define PIN_LOCKR_ENABLED(n)        (1U << (n))

/*
 * GPIOA setup:
 *
 * PA0  - UART4_TX                  (analog).
 * PA1  - UART4_RX                  (analog).
 * PA2  - TIM2_CH3                  (analog).
 * PA3  - PIN3                      (analog).
 * PA4  - PIN4                      (analog).
 * PA5  - SPI1_SCK                  (analog).
 * PA6  - SPI1_MISO                 (analog).
 * PA7  - SPI1_MOSI                 (analog).
 * PA8  - SPBTLE_RF_RST             (analog).
 * PA9  - USB_OTG_FS_VBUS           (analog).
 * PA10 - USB_OTG_FS_ID             (analog).
 * PA11 - USB_OTG_FS_DM             (analog).
 * PA12 - USB_OTG_FS_DP             (analog).
 * PA13 - SWDIO                     (alternate 0).
 * PA14 - SWCLK                     (alternate 0).
 * PA15 - TIM2_CH1                  (analog).
 */
#define VAL_GPIOA_MODER             (PIN_MODE_ANALOG(GPIOA_UART4_TX) |      \
                                     PIN_MODE_ANALOG(GPIOA_UART4_RX) |      \
                                     PIN_MODE_ANALOG(GPIOA_TIM2_CH3) |      \
                                     PIN_MODE_ANALOG(GPIOA_PIN3) |          \
                                     PIN_MODE_ANALOG(GPIOA_PIN4) |          \
                                     PIN_MODE_ANALOG(GPIOA_SPI1_SCK) |      \
                                     PIN_MODE_ANALOG(GPIOA_SPI1_MISO) |     \
                                     PIN_MODE_ANALOG(GPIOA_SPI1_MOSI) |     \
                                     PIN_MODE_ANALOG(GPIOA_SPBTLE_RF_RST) | \
                                     PIN_MODE_ANALOG(GPIOA_USB_OTG_FS_VBUS) |\
                                     PIN_MODE_ANALOG(GPIOA_USB_OTG_FS_ID) | \
                                     PIN_MODE_ANALOG(GPIOA_USB_OTG_FS_DM) | \
                                     PIN_MODE_ANALOG(GPIOA_USB_OTG_FS_DP) | \
                                     PIN_MODE_ALTERNATE(GPIOA_SWDIO) |      \
                                     PIN_MODE_ALTERNATE(GPIOA_SWCLK) |      \
                                     PIN_MODE_ANALOG(GPIOA_TIM2_CH1))
#define VAL_GPIOA_OTYPER            (PIN_OTYPE_PUSHPULL(GPIOA_UART4_TX) |   \
                                     PIN_OTYPE_PUSHPULL(GPIOA_UART4_RX) |   \
                                     PIN_OTYPE_PUSHPULL(GPIOA_TIM2_CH3) |   \
                                     PIN_OTYPE_PUSHPULL(GPIOA_PIN3) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOA_PIN4) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOA_SPI1_SCK) |   \
                                     PIN_OTYPE_PUSHPULL(GPIOA_SPI1_MISO) |  \
                                     PIN_OTYPE_PUSHPULL(GPIOA_SPI1_MOSI) |  \
                                     PIN_OTYPE_PUSHPULL(GPIOA_SPBTLE_RF_RST) |\
                                     PIN_OTYPE_PUSHPULL(GPIOA_USB_OTG_FS_VBUS) |\
                                     PIN_OTYPE_PUSHPULL(GPIOA_USB_OTG_FS_ID) |\
                                     PIN_OTYPE_PUSHPULL(GPIOA_USB_OTG_FS_DM) |\
                                     PIN_OTYPE_PUSHPULL(GPIOA_USB_OTG_FS_DP) |\
                                     PIN_OTYPE_PUSHPULL(GPIOA_SWDIO) |      \
                                     PIN_OTYPE_PUSHPULL(GPIOA_SWCLK) |      \
                                     PIN_OTYPE_PUSHPULL(GPIOA_TIM2_CH1))
#define VAL_GPIOA_OSPEEDR           (PIN_OSPEED_HIGH(GPIOA_UART4_TX) |      \
                                     PIN_OSPEED_HIGH(GPIOA_UART4_RX) |      \
                                     PIN_OSPEED_HIGH(GPIOA_TIM2_CH3) |      \
                                     PIN_OSPEED_HIGH(GPIOA_PIN3) |          \
                                     PIN_OSPEED_HIGH(GPIOA_PIN4) |          \
                                     PIN_OSPEED_HIGH(GPIOA_SPI1_SCK) |      \
                                     PIN_OSPEED_HIGH(GPIOA_SPI1_MISO) |     \
                                     PIN_OSPEED_HIGH(GPIOA_SPI1_MOSI) |     \
                                     PIN_OSPEED_HIGH(GPIOA_SPBTLE_RF_RST) | \
                                     PIN_OSPEED_HIGH(GPIOA_USB_OTG_FS_VBUS) |\
                                     PIN_OSPEED_HIGH(GPIOA_USB_OTG_FS_ID) | \
                                     PIN_OSPEED_HIGH(GPIOA_USB_OTG_FS_DM) | \
                                     PIN_OSPEED_HIGH(GPIOA_USB_OTG_FS_DP) | \
                                     PIN_OSPEED_HIGH(GPIOA_SWDIO) |         \
                                     PIN_OSPEED_HIGH(GPIOA_SWCLK) |         \
                                     PIN_OSPEED_HIGH(GPIOA_TIM2_CH1))
#define VAL_GPIOA_PUPDR             (PIN_PUPDR_FLOATING(GPIOA_UART4_TX) |   \
                                     PIN_PUPDR_FLOATING(GPIOA_UART4_RX) |   \
                                     PIN_PUPDR_FLOATING(GPIOA_TIM2_CH3) |   \
                                     PIN_PUPDR_FLOATING(GPIOA_PIN3) |       \
                                     PIN_PUPDR_FLOATING(GPIOA_PIN4) |       \
                                     PIN_PUPDR_FLOATING(GPIOA_SPI1_SCK) |   \
                                     PIN_PUPDR_FLOATING(GPIOA_SPI1_MISO) |  \
                                     PIN_PUPDR_FLOATING(GPIOA_SPI1_MOSI) |  \
                                     PIN_PUPDR_FLOATING(GPIOA_SPBTLE_RF_RST) |\
                                     PIN_PUPDR_FLOATING(GPIOA_USB_OTG_FS_VBUS) |\
                                     PIN_PUPDR_FLOATING(GPIOA_USB_OTG_FS_ID) |\
                                     PIN_PUPDR_FLOATING(GPIOA_USB_OTG_FS_DM) |\
                                     PIN_PUPDR_FLOATING(GPIOA_USB_OTG_FS_DP) |\
                                     PIN_PUPDR_PULLUP(GPIOA_SWDIO) |        \
                                     PIN_PUPDR_PULLDOWN(GPIOA_SWCLK) |      \
                                     PIN_PUPDR_FLOATING(GPIOA_TIM2_CH1))
#define VAL_GPIOA_ODR               (PIN_ODR_HIGH(GPIOA_UART4_TX) |         \
                                     PIN_ODR_HIGH(GPIOA_UART4_RX) |         \
                                     PIN_ODR_HIGH(GPIOA_TIM2_CH3) |         \
                                     PIN_ODR_HIGH(GPIOA_PIN3) |             \
                                     PIN_ODR_HIGH(GPIOA_PIN4) |             \
                                     PIN_ODR_HIGH(GPIOA_SPI1_SCK) |         \
                                     PIN_ODR_HIGH(GPIOA_SPI1_MISO) |        \
                                     PIN_ODR_HIGH(GPIOA_SPI1_MOSI) |        \
                                     PIN_ODR_HIGH(GPIOA_SPBTLE_RF_RST) |    \
                                     PIN_ODR_HIGH(GPIOA_USB_OTG_FS_VBUS) |  \
                                     PIN_ODR_HIGH(GPIOA_USB_OTG_FS_ID) |    \
                                     PIN_ODR_HIGH(GPIOA_USB_OTG_FS_DM) |    \
                                     PIN_ODR_HIGH(GPIOA_USB_OTG_FS_DP) |    \
                                     PIN_ODR_HIGH(GPIOA_SWDIO) |            \
                                     PIN_ODR_HIGH(GPIOA_SWCLK) |            \
                                     PIN_ODR_HIGH(GPIOA_TIM2_CH1))
#define VAL_GPIOA_AFRL              (PIN_AFIO_AF(GPIOA_UART4_TX, 0U) |      \
                                     PIN_AFIO_AF(GPIOA_UART4_RX, 0U) |      \
                                     PIN_AFIO_AF(GPIOA_TIM2_CH3, 0U) |      \
                                     PIN_AFIO_AF(GPIOA_PIN3, 0U) |          \
                                     PIN_AFIO_AF(GPIOA_PIN4, 0U) |          \
                                     PIN_AFIO_AF(GPIOA_SPI1_SCK, 0U) |      \
                                     PIN_AFIO_AF(GPIOA_SPI1_MISO, 0U) |     \
                                     PIN_AFIO_AF(GPIOA_SPI1_MOSI, 0U))
#define VAL_GPIOA_AFRH              (PIN_AFIO_AF(GPIOA_SPBTLE_RF_RST, 0U) | \
                                     PIN_AFIO_AF(GPIOA_USB_OTG_FS_VBUS, 0U) |\
                                     PIN_AFIO_AF(GPIOA_USB_OTG_FS_ID, 0U) | \
                                     PIN_AFIO_AF(GPIOA_USB_OTG_FS_DM, 0U) | \
                                     PIN_AFIO_AF(GPIOA_USB_OTG_FS_DP, 0U) | \
                                     PIN_AFIO_AF(GPIOA_SWDIO, 0U) |         \
                                     PIN_AFIO_AF(GPIOA_SWCLK, 0U) |         \
                                     PIN_AFIO_AF(GPIOA_TIM2_CH1, 0U))
#define VAL_GPIOA_ASCR              (PIN_ASCR_DISABLED(GPIOA_UART4_TX) |    \
                                     PIN_ASCR_DISABLED(GPIOA_UART4_RX) |    \
                                     PIN_ASCR_DISABLED(GPIOA_TIM2_CH3) |    \
                                     PIN_ASCR_DISABLED(GPIOA_PIN3) |        \
                                     PIN_ASCR_DISABLED(GPIOA_PIN4) |        \
                                     PIN_ASCR_DISABLED(GPIOA_SPI1_SCK) |    \
                                     PIN_ASCR_DISABLED(GPIOA_SPI1_MISO) |   \
                                     PIN_ASCR_DISABLED(GPIOA_SPI1_MOSI) |   \
                                     PIN_ASCR_DISABLED(GPIOA_SPBTLE_RF_RST) |\
                                     PIN_ASCR_DISABLED(GPIOA_USB_OTG_FS_VBUS) |\
                                     PIN_ASCR_DISABLED(GPIOA_USB_OTG_FS_ID) |\
                                     PIN_ASCR_DISABLED(GPIOA_USB_OTG_FS_DM) |\
                                     PIN_ASCR_DISABLED(GPIOA_USB_OTG_FS_DP) |\
                                     PIN_ASCR_DISABLED(GPIOA_SWDIO) |       \
                                     PIN_ASCR_DISABLED(GPIOA_SWCLK) |       \
                                     PIN_ASCR_DISABLED(GPIOA_TIM2_CH1))
#define VAL_GPIOA_LOCKR             (PIN_LOCKR_DISABLED(GPIOA_UART4_TX) |   \
                                     PIN_LOCKR_DISABLED(GPIOA_UART4_RX) |   \
                                     PIN_LOCKR_DISABLED(GPIOA_TIM2_CH3) |   \
                                     PIN_LOCKR_DISABLED(GPIOA_PIN3) |       \
                                     PIN_LOCKR_DISABLED(GPIOA_PIN4) |       \
                                     PIN_LOCKR_DISABLED(GPIOA_SPI1_SCK) |   \
                                     PIN_LOCKR_DISABLED(GPIOA_SPI1_MISO) |  \
                                     PIN_LOCKR_DISABLED(GPIOA_SPI1_MOSI) |  \
                                     PIN_LOCKR_DISABLED(GPIOA_SPBTLE_RF_RST) |\
                                     PIN_LOCKR_DISABLED(GPIOA_USB_OTG_FS_VBUS) |\
                                     PIN_LOCKR_DISABLED(GPIOA_USB_OTG_FS_ID) |\
                                     PIN_LOCKR_DISABLED(GPIOA_USB_OTG_FS_DM) |\
                                     PIN_LOCKR_DISABLED(GPIOA_USB_OTG_FS_DP) |\
                                     PIN_LOCKR_DISABLED(GPIOA_SWDIO) |      \
                                     PIN_LOCKR_DISABLED(GPIOA_SWCLK) |      \
                                     PIN_LOCKR_DISABLED(GPIOA_TIM2_CH1))

/*
 * GPIOB setup:
 *
 * PB0  - TIM3_CH3                  (analog).
 * PB1  - TIM3_CH4                  (analog).
 * PB2  - PIN2                      (analog).
 * PB3  - SWO                       (analog).
 * PB4  - TIM3_CH1                  (analog).
 * PB5  - PIN5                      (analog).
 * PB6  - STLINK_USART1_TX          (analog).
 * PB7  - STLINK_USART1_RX          (analog).
 * PB8  - I2C1_SCL                  (analog).
 * PB9  - I2C1_SDA                  (analog).
 * PB10 - I2C2_SCL                  (analog).
 * PB11 - I2C2_SDA                  (analog).
 * PB12 - PIN12                     (analog).
 * PB13 - PIN13                     (analog).
 * PB14 - LED2                      (output pushpull maximum).
 * PB15 - PIN15                     (analog).
 */
#define VAL_GPIOB_MODER             (PIN_MODE_ANALOG(GPIOB_TIM3_CH3) |      \
                                     PIN_MODE_ANALOG(GPIOB_TIM3_CH4) |      \
                                     PIN_MODE_ANALOG(GPIOB_PIN2) |          \
                                     PIN_MODE_ANALOG(GPIOB_SWO) |           \
                                     PIN_MODE_ANALOG(GPIOB_TIM3_CH1) |      \
                                     PIN_MODE_ANALOG(GPIOB_PIN5) |          \
                                     PIN_MODE_ANALOG(GPIOB_STLINK_USART1_TX) |\
                                     PIN_MODE_ANALOG(GPIOB_STLINK_USART1_RX) |\
                                     PIN_MODE_ANALOG(GPIOB_I2C1_SCL) |      \
                                     PIN_MODE_ANALOG(GPIOB_I2C1_SDA) |      \
                                     PIN_MODE_ANALOG(GPIOB_I2C2_SCL) |      \
                                     PIN_MODE_ANALOG(GPIOB_I2C2_SDA) |      \
                                     PIN_MODE_ANALOG(GPIOB_PIN12) |         \
                                     PIN_MODE_ANALOG(GPIOB_PIN13) |         \
                                     PIN_MODE_OUTPUT(GPIOB_LED2) |          \
                                     PIN_MODE_ANALOG(GPIOB_PIN15))
#define VAL_GPIOB_OTYPER            (PIN_OTYPE_PUSHPULL(GPIOB_TIM3_CH3) |   \
                                     PIN_OTYPE_PUSHPULL(GPIOB_TIM3_CH4) |   \
                                     PIN_OTYPE_PUSHPULL(GPIOB_PIN2) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOB_SWO) |        \
                                     PIN_OTYPE_PUSHPULL(GPIOB_TIM3_CH1) |   \
                                     PIN_OTYPE_PUSHPULL(GPIOB_PIN5) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOB_STLINK_USART1_TX) |\
                                     PIN_OTYPE_PUSHPULL(GPIOB_STLINK_USART1_RX) |\
                                     PIN_OTYPE_PUSHPULL(GPIOB_I2C1_SCL) |   \
                                     PIN_OTYPE_PUSHPULL(GPIOB_I2C1_SDA) |   \
                                     PIN_OTYPE_PUSHPULL(GPIOB_I2C2_SCL) |   \
                                     PIN_OTYPE_PUSHPULL(GPIOB_I2C2_SDA) |   \
                                     PIN_OTYPE_PUSHPULL(GPIOB_PIN12) |      \
                                     PIN_OTYPE_PUSHPULL(GPIOB_PIN13) |      \
                                     PIN_OTYPE_PUSHPULL(GPIOB_LED2) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOB_PIN15))
#define VAL_GPIOB_OSPEEDR           (PIN_OSPEED_HIGH(GPIOB_TIM3_CH3) |      \
                                     PIN_OSPEED_HIGH(GPIOB_TIM3_CH4) |      \
                                     PIN_OSPEED_HIGH(GPIOB_PIN2) |          \
                                     PIN_OSPEED_HIGH(GPIOB_SWO) |           \
                                     PIN_OSPEED_HIGH(GPIOB_TIM3_CH1) |      \
                                     PIN_OSPEED_HIGH(GPIOB_PIN5) |          \
                                     PIN_OSPEED_HIGH(GPIOB_STLINK_USART1_TX) |\
                                     PIN_OSPEED_HIGH(GPIOB_STLINK_USART1_RX) |\
                                     PIN_OSPEED_HIGH(GPIOB_I2C1_SCL) |      \
                                     PIN_OSPEED_HIGH(GPIOB_I2C1_SDA) |      \
                                     PIN_OSPEED_HIGH(GPIOB_I2C2_SCL) |      \
                                     PIN_OSPEED_HIGH(GPIOB_I2C2_SDA) |      \
                                     PIN_OSPEED_HIGH(GPIOB_PIN12) |         \
                                     PIN_OSPEED_HIGH(GPIOB_PIN13) |         \
                                     PIN_OSPEED_HIGH(GPIOB_LED2) |          \
                                     PIN_OSPEED_HIGH(GPIOB_PIN15))
#define VAL_GPIOB_PUPDR             (PIN_PUPDR_FLOATING(GPIOB_TIM3_CH3) |   \
                                     PIN_PUPDR_FLOATING(GPIOB_TIM3_CH4) |   \
                                     PIN_PUPDR_FLOATING(GPIOB_PIN2) |       \
                                     PIN_PUPDR_FLOATING(GPIOB_SWO) |        \
                                     PIN_PUPDR_FLOATING(GPIOB_TIM3_CH1) |   \
                                     PIN_PUPDR_FLOATING(GPIOB_PIN5) |       \
                                     PIN_PUPDR_FLOATING(GPIOB_STLINK_USART1_TX) |\
                                     PIN_PUPDR_FLOATING(GPIOB_STLINK_USART1_RX) |\
                                     PIN_PUPDR_FLOATING(GPIOB_I2C1_SCL) |   \
                                     PIN_PUPDR_FLOATING(GPIOB_I2C1_SDA) |   \
                                     PIN_PUPDR_FLOATING(GPIOB_I2C2_SCL) |   \
                                     PIN_PUPDR_FLOATING(GPIOB_I2C2_SDA) |   \
                                     PIN_PUPDR_FLOATING(GPIOB_PIN12) |      \
                                     PIN_PUPDR_FLOATING(GPIOB_PIN13) |      \
                                     PIN_PUPDR_PULLDOWN(GPIOB_LED2) |       \
                                     PIN_PUPDR_FLOATING(GPIOB_PIN15))
#define VAL_GPIOB_ODR               (PIN_ODR_HIGH(GPIOB_TIM3_CH3) |         \
                                     PIN_ODR_HIGH(GPIOB_TIM3_CH4) |         \
                                     PIN_ODR_HIGH(GPIOB_PIN2) |             \
                                     PIN_ODR_HIGH(GPIOB_SWO) |              \
                                     PIN_ODR_HIGH(GPIOB_TIM3_CH1) |         \
                                     PIN_ODR_HIGH(GPIOB_PIN5) |             \
                                     PIN_ODR_HIGH(GPIOB_STLINK_USART1_TX) | \
                                     PIN_ODR_HIGH(GPIOB_STLINK_USART1_RX) | \
                                     PIN_ODR_HIGH(GPIOB_I2C1_SCL) |         \
                                     PIN_ODR_HIGH(GPIOB_I2C1_SDA) |         \
                                     PIN_ODR_HIGH(GPIOB_I2C2_SCL) |         \
                                     PIN_ODR_HIGH(GPIOB_I2C2_SDA) |         \
                                     PIN_ODR_HIGH(GPIOB_PIN12) |            \
                                     PIN_ODR_HIGH(GPIOB_PIN13) |            \
                                     PIN_ODR_HIGH(GPIOB_LED2) |             \
                                     PIN_ODR_HIGH(GPIOB_PIN15))
#define VAL_GPIOB_AFRL              (PIN_AFIO_AF(GPIOB_TIM3_CH3, 0U) |      \
                                     PIN_AFIO_AF(GPIOB_TIM3_CH4, 0U) |      \
                                     PIN_AFIO_AF(GPIOB_PIN2, 0U) |          \
                                     PIN_AFIO_AF(GPIOB_SWO, 0U) |           \
                                     PIN_AFIO_AF(GPIOB_TIM3_CH1, 0U) |      \
                                     PIN_AFIO_AF(GPIOB_PIN5, 0U) |          \
                                     PIN_AFIO_AF(GPIOB_STLINK_USART1_TX, 0U) |\
                                     PIN_AFIO_AF(GPIOB_STLINK_USART1_RX, 0U))
#define VAL_GPIOB_AFRH              (PIN_AFIO_AF(GPIOB_I2C1_SCL, 0U) |      \
                                     PIN_AFIO_AF(GPIOB_I2C1_SDA, 0U) |      \
                                     PIN_AFIO_AF(GPIOB_I2C2_SCL, 0U) |      \
                                     PIN_AFIO_AF(GPIOB_I2C2_SDA, 0U) |      \
                                     PIN_AFIO_AF(GPIOB_PIN12, 0U) |         \
                                     PIN_AFIO_AF(GPIOB_PIN13, 0U) |         \
                                     PIN_AFIO_AF(GPIOB_LED2, 0U) |          \
                                     PIN_AFIO_AF(GPIOB_PIN15, 0U))
#define VAL_GPIOB_ASCR              (PIN_ASCR_DISABLED(GPIOB_TIM3_CH3) |    \
                                     PIN_ASCR_DISABLED(GPIOB_TIM3_CH4) |    \
                                     PIN_ASCR_DISABLED(GPIOB_PIN2) |        \
                                     PIN_ASCR_ENABLED(GPIOB_SWO) |          \
                                     PIN_ASCR_DISABLED(GPIOB_TIM3_CH1) |    \
                                     PIN_ASCR_DISABLED(GPIOB_PIN5) |        \
                                     PIN_ASCR_DISABLED(GPIOB_STLINK_USART1_TX) |\
                                     PIN_ASCR_DISABLED(GPIOB_STLINK_USART1_RX) |\
                                     PIN_ASCR_DISABLED(GPIOB_I2C1_SCL) |    \
                                     PIN_ASCR_DISABLED(GPIOB_I2C1_SDA) |    \
                                     PIN_ASCR_DISABLED(GPIOB_I2C2_SCL) |    \
                                     PIN_ASCR_DISABLED(GPIOB_I2C2_SDA) |    \
                                     PIN_ASCR_DISABLED(GPIOB_PIN12) |       \
                                     PIN_ASCR_DISABLED(GPIOB_PIN13) |       \
                                     PIN_ASCR_DISABLED(GPIOB_LED2) |        \
                                     PIN_ASCR_DISABLED(GPIOB_PIN15))
#define VAL_GPIOB_LOCKR             (PIN_LOCKR_DISABLED(GPIOB_TIM3_CH3) |   \
                                     PIN_LOCKR_DISABLED(GPIOB_TIM3_CH4) |   \
                                     PIN_LOCKR_DISABLED(GPIOB_PIN2) |       \
                                     PIN_LOCKR_DISABLED(GPIOB_SWO) |        \
                                     PIN_LOCKR_DISABLED(GPIOB_TIM3_CH1) |   \
                                     PIN_LOCKR_DISABLED(GPIOB_PIN5) |       \
                                     PIN_LOCKR_DISABLED(GPIOB_STLINK_USART1_TX) |\
                                     PIN_LOCKR_DISABLED(GPIOB_STLINK_USART1_RX) |\
                                     PIN_LOCKR_DISABLED(GPIOB_I2C1_SCL) |   \
                                     PIN_LOCKR_DISABLED(GPIOB_I2C1_SDA) |   \
                                     PIN_LOCKR_DISABLED(GPIOB_I2C2_SCL) |   \
                                     PIN_LOCKR_DISABLED(GPIOB_I2C2_SDA) |   \
                                     PIN_LOCKR_DISABLED(GPIOB_PIN12) |      \
                                     PIN_LOCKR_DISABLED(GPIOB_PIN13) |      \
                                     PIN_LOCKR_DISABLED(GPIOB_LED2) |       \
                                     PIN_LOCKR_DISABLED(GPIOB_PIN15))

/*
 * GPIOC setup:
 *
 * PC0  - ADC1_IN1                  (analog).
 * PC1  - ADC1_IN2                  (analog).
 * PC2  - ADC1_IN3                  (analog).
 * PC3  - ADC1_IN4                  (analog).
 * PC4  - ADC1_IN13                 (analog).
 * PC5  - ADC1_IN14                 (analog).
 * PC6  - VL53L0X_XSHUT             (analog).
 * PC7  - VL53L0X_GPIO1_EXTI7       (analog).
 * PC8  - LIS3MDL_DRDY_EXTI8        (analog).
 * PC9  - LED3_LED4                 (analog).
 * PC10 - SPI3_SCK                  (analog).
 * PC11 - SPI3_MISO                 (analog).
 * PC12 - SPI3_MOSI                 (analog).
 * PC13 - BUTTON_EXTI13             (analog).
 * PC14 - OSC32_IN                  (input floating).
 * PC15 - OSC32_OUT                 (input floating).
 */
#define VAL_GPIOC_MODER             (PIN_MODE_ANALOG(GPIOC_ADC1_IN1) |      \
                                     PIN_MODE_ANALOG(GPIOC_ADC1_IN2) |      \
                                     PIN_MODE_ANALOG(GPIOC_ADC1_IN3) |      \
                                     PIN_MODE_ANALOG(GPIOC_ADC1_IN4) |      \
                                     PIN_MODE_ANALOG(GPIOC_ADC1_IN13) |     \
                                     PIN_MODE_ANALOG(GPIOC_ADC1_IN14) |     \
                                     PIN_MODE_ANALOG(GPIOC_VL53L0X_XSHUT) | \
                                     PIN_MODE_ANALOG(GPIOC_VL53L0X_GPIO1_EXTI7) |\
                                     PIN_MODE_ANALOG(GPIOC_LIS3MDL_DRDY_EXTI8) |\
                                     PIN_MODE_ANALOG(GPIOC_LED3_LED4) |     \
                                     PIN_MODE_ANALOG(GPIOC_SPI3_SCK) |      \
                                     PIN_MODE_ANALOG(GPIOC_SPI3_MISO) |     \
                                     PIN_MODE_ANALOG(GPIOC_SPI3_MOSI) |     \
                                     PIN_MODE_ANALOG(GPIOC_BUTTON_EXTI13) | \
                                     PIN_MODE_INPUT(GPIOC_OSC32_IN) |       \
                                     PIN_MODE_INPUT(GPIOC_OSC32_OUT))
#define VAL_GPIOC_OTYPER            (PIN_OTYPE_PUSHPULL(GPIOC_ADC1_IN1) |   \
                                     PIN_OTYPE_PUSHPULL(GPIOC_ADC1_IN2) |   \
                                     PIN_OTYPE_PUSHPULL(GPIOC_ADC1_IN3) |   \
                                     PIN_OTYPE_PUSHPULL(GPIOC_ADC1_IN4) |   \
                                     PIN_OTYPE_PUSHPULL(GPIOC_ADC1_IN13) |  \
                                     PIN_OTYPE_PUSHPULL(GPIOC_ADC1_IN14) |  \
                                     PIN_OTYPE_PUSHPULL(GPIOC_VL53L0X_XSHUT) |\
                                     PIN_OTYPE_PUSHPULL(GPIOC_VL53L0X_GPIO1_EXTI7) |\
                                     PIN_OTYPE_PUSHPULL(GPIOC_LIS3MDL_DRDY_EXTI8) |\
                                     PIN_OTYPE_PUSHPULL(GPIOC_LED3_LED4) |  \
                                     PIN_OTYPE_PUSHPULL(GPIOC_SPI3_SCK) |   \
                                     PIN_OTYPE_PUSHPULL(GPIOC_SPI3_MISO) |  \
                                     PIN_OTYPE_PUSHPULL(GPIOC_SPI3_MOSI) |  \
                                     PIN_OTYPE_PUSHPULL(GPIOC_BUTTON_EXTI13) |\
                                     PIN_OTYPE_PUSHPULL(GPIOC_OSC32_IN) |   \
                                     PIN_OTYPE_PUSHPULL(GPIOC_OSC32_OUT))
#define VAL_GPIOC_OSPEEDR           (PIN_OSPEED_HIGH(GPIOC_ADC1_IN1) |      \
                                     PIN_OSPEED_HIGH(GPIOC_ADC1_IN2) |      \
                                     PIN_OSPEED_HIGH(GPIOC_ADC1_IN3) |      \
                                     PIN_OSPEED_HIGH(GPIOC_ADC1_IN4) |      \
                                     PIN_OSPEED_HIGH(GPIOC_ADC1_IN13) |     \
                                     PIN_OSPEED_HIGH(GPIOC_ADC1_IN14) |     \
                                     PIN_OSPEED_HIGH(GPIOC_VL53L0X_XSHUT) | \
                                     PIN_OSPEED_HIGH(GPIOC_VL53L0X_GPIO1_EXTI7) |\
                                     PIN_OSPEED_HIGH(GPIOC_LIS3MDL_DRDY_EXTI8) |\
                                     PIN_OSPEED_HIGH(GPIOC_LED3_LED4) |     \
                                     PIN_OSPEED_HIGH(GPIOC_SPI3_SCK) |      \
                                     PIN_OSPEED_HIGH(GPIOC_SPI3_MISO) |     \
                                     PIN_OSPEED_HIGH(GPIOC_SPI3_MOSI) |     \
                                     PIN_OSPEED_HIGH(GPIOC_BUTTON_EXTI13) | \
                                     PIN_OSPEED_VERYLOW(GPIOC_OSC32_IN) |   \
                                     PIN_OSPEED_VERYLOW(GPIOC_OSC32_OUT))
#define VAL_GPIOC_PUPDR             (PIN_PUPDR_FLOATING(GPIOC_ADC1_IN1) |   \
                                     PIN_PUPDR_FLOATING(GPIOC_ADC1_IN2) |   \
                                     PIN_PUPDR_FLOATING(GPIOC_ADC1_IN3) |   \
                                     PIN_PUPDR_FLOATING(GPIOC_ADC1_IN4) |   \
                                     PIN_PUPDR_FLOATING(GPIOC_ADC1_IN13) |  \
                                     PIN_PUPDR_FLOATING(GPIOC_ADC1_IN14) |  \
                                     PIN_PUPDR_FLOATING(GPIOC_VL53L0X_XSHUT) |\
                                     PIN_PUPDR_FLOATING(GPIOC_VL53L0X_GPIO1_EXTI7) |\
                                     PIN_PUPDR_FLOATING(GPIOC_LIS3MDL_DRDY_EXTI8) |\
                                     PIN_PUPDR_FLOATING(GPIOC_LED3_LED4) |  \
                                     PIN_PUPDR_FLOATING(GPIOC_SPI3_SCK) |   \
                                     PIN_PUPDR_FLOATING(GPIOC_SPI3_MISO) |  \
                                     PIN_PUPDR_FLOATING(GPIOC_SPI3_MOSI) |  \
                                     PIN_PUPDR_PULLDOWN(GPIOC_BUTTON_EXTI13) |\
                                     PIN_PUPDR_FLOATING(GPIOC_OSC32_IN) |   \
                                     PIN_PUPDR_FLOATING(GPIOC_OSC32_OUT))
#define VAL_GPIOC_ODR               (PIN_ODR_HIGH(GPIOC_ADC1_IN1) |         \
                                     PIN_ODR_HIGH(GPIOC_ADC1_IN2) |         \
                                     PIN_ODR_HIGH(GPIOC_ADC1_IN3) |         \
                                     PIN_ODR_HIGH(GPIOC_ADC1_IN4) |         \
                                     PIN_ODR_HIGH(GPIOC_ADC1_IN13) |        \
                                     PIN_ODR_HIGH(GPIOC_ADC1_IN14) |        \
                                     PIN_ODR_HIGH(GPIOC_VL53L0X_XSHUT) |    \
                                     PIN_ODR_HIGH(GPIOC_VL53L0X_GPIO1_EXTI7) |\
                                     PIN_ODR_HIGH(GPIOC_LIS3MDL_DRDY_EXTI8) |\
                                     PIN_ODR_HIGH(GPIOC_LED3_LED4) |        \
                                     PIN_ODR_HIGH(GPIOC_SPI3_SCK) |         \
                                     PIN_ODR_HIGH(GPIOC_SPI3_MISO) |        \
                                     PIN_ODR_HIGH(GPIOC_SPI3_MOSI) |        \
                                     PIN_ODR_HIGH(GPIOC_BUTTON_EXTI13) |    \
                                     PIN_ODR_HIGH(GPIOC_OSC32_IN) |         \
                                     PIN_ODR_HIGH(GPIOC_OSC32_OUT))
#define VAL_GPIOC_AFRL              (PIN_AFIO_AF(GPIOC_ADC1_IN1, 0U) |      \
                                     PIN_AFIO_AF(GPIOC_ADC1_IN2, 0U) |      \
                                     PIN_AFIO_AF(GPIOC_ADC1_IN3, 0U) |      \
                                     PIN_AFIO_AF(GPIOC_ADC1_IN4, 0U) |      \
                                     PIN_AFIO_AF(GPIOC_ADC1_IN13, 0U) |     \
                                     PIN_AFIO_AF(GPIOC_ADC1_IN14, 0U) |     \
                                     PIN_AFIO_AF(GPIOC_VL53L0X_XSHUT, 0U) | \
                                     PIN_AFIO_AF(GPIOC_VL53L0X_GPIO1_EXTI7, 0U))
#define VAL_GPIOC_AFRH              (PIN_AFIO_AF(GPIOC_LIS3MDL_DRDY_EXTI8, 0U) |\
                                     PIN_AFIO_AF(GPIOC_LED3_LED4, 0U) |     \
                                     PIN_AFIO_AF(GPIOC_SPI3_SCK, 0U) |      \
                                     PIN_AFIO_AF(GPIOC_SPI3_MISO, 0U) |     \
                                     PIN_AFIO_AF(GPIOC_SPI3_MOSI, 0U) |     \
                                     PIN_AFIO_AF(GPIOC_BUTTON_EXTI13, 0U) | \
                                     PIN_AFIO_AF(GPIOC_OSC32_IN, 0U) |      \
                                     PIN_AFIO_AF(GPIOC_OSC32_OUT, 0U))
#define VAL_GPIOC_ASCR              (PIN_ASCR_DISABLED(GPIOC_ADC1_IN1) |    \
                                     PIN_ASCR_DISABLED(GPIOC_ADC1_IN2) |    \
                                     PIN_ASCR_DISABLED(GPIOC_ADC1_IN3) |    \
                                     PIN_ASCR_DISABLED(GPIOC_ADC1_IN4) |    \
                                     PIN_ASCR_DISABLED(GPIOC_ADC1_IN13) |   \
                                     PIN_ASCR_DISABLED(GPIOC_ADC1_IN14) |   \
                                     PIN_ASCR_DISABLED(GPIOC_VL53L0X_XSHUT) |\
                                     PIN_ASCR_DISABLED(GPIOC_VL53L0X_GPIO1_EXTI7) |\
                                     PIN_ASCR_DISABLED(GPIOC_LIS3MDL_DRDY_EXTI8) |\
                                     PIN_ASCR_DISABLED(GPIOC_LED3_LED4) |   \
                                     PIN_ASCR_DISABLED(GPIOC_SPI3_SCK) |    \
                                     PIN_ASCR_DISABLED(GPIOC_SPI3_MISO) |   \
                                     PIN_ASCR_DISABLED(GPIOC_SPI3_MOSI) |   \
                                     PIN_ASCR_DISABLED(GPIOC_BUTTON_EXTI13) |\
                                     PIN_ASCR_DISABLED(GPIOC_OSC32_IN) |    \
                                     PIN_ASCR_DISABLED(GPIOC_OSC32_OUT))
#define VAL_GPIOC_LOCKR             (PIN_LOCKR_DISABLED(GPIOC_ADC1_IN1) |   \
                                     PIN_LOCKR_DISABLED(GPIOC_ADC1_IN2) |   \
                                     PIN_LOCKR_DISABLED(GPIOC_ADC1_IN3) |   \
                                     PIN_LOCKR_DISABLED(GPIOC_ADC1_IN4) |   \
                                     PIN_LOCKR_DISABLED(GPIOC_ADC1_IN13) |  \
                                     PIN_LOCKR_DISABLED(GPIOC_ADC1_IN14) |  \
                                     PIN_LOCKR_DISABLED(GPIOC_VL53L0X_XSHUT) |\
                                     PIN_LOCKR_DISABLED(GPIOC_VL53L0X_GPIO1_EXTI7) |\
                                     PIN_LOCKR_DISABLED(GPIOC_LIS3MDL_DRDY_EXTI8) |\
                                     PIN_LOCKR_DISABLED(GPIOC_LED3_LED4) |  \
                                     PIN_LOCKR_DISABLED(GPIOC_SPI3_SCK) |   \
                                     PIN_LOCKR_DISABLED(GPIOC_SPI3_MISO) |  \
                                     PIN_LOCKR_DISABLED(GPIOC_SPI3_MOSI) |  \
                                     PIN_LOCKR_DISABLED(GPIOC_BUTTON_EXTI13) |\
                                     PIN_LOCKR_DISABLED(GPIOC_OSC32_IN) |   \
                                     PIN_LOCKR_DISABLED(GPIOC_OSC32_OUT))

/*
 * GPIOD setup:
 *
 * PD0  - PMOD_RESET                (analog).
 * PD1  - PMOD_SPI2_SCK             (analog).
 * PD2  - PMOD_IRQ_EXTI2            (analog).
 * PD3  - USART2_CTS                (analog).
 * PD4  - USART2_RTS                (analog).
 * PD5  - USART2_TX                 (analog).
 * PD6  - USART2_RX                 (analog).
 * PD7  - STSAFE_A100_RESET         (analog).
 * PD8  - USART3_TX                 (analog).
 * PD9  - USART3_RX                 (analog).
 * PD10 - LPS22HB_INT_DRDY_EXTI10   (analog).
 * PD11 - LSM6DSL_INT1_EXTI11       (analog).
 * PD12 - USB_OTG_FS_PWR_EN         (analog).
 * PD13 - SPBTLE_RF_SPI3_CSN        (analog).
 * PD14 - PIN14                     (analog).
 * PD15 - HTS221_DRDY_EXTI15        (analog).
 */
#define VAL_GPIOD_MODER             (PIN_MODE_ANALOG(GPIOD_PMOD_RESET) |    \
                                     PIN_MODE_ANALOG(GPIOD_PMOD_SPI2_SCK) | \
                                     PIN_MODE_ANALOG(GPIOD_PMOD_IRQ_EXTI2) |\
                                     PIN_MODE_ANALOG(GPIOD_USART2_CTS) |    \
                                     PIN_MODE_ANALOG(GPIOD_USART2_RTS) |    \
                                     PIN_MODE_ANALOG(GPIOD_USART2_TX) |     \
                                     PIN_MODE_ANALOG(GPIOD_USART2_RX) |     \
                                     PIN_MODE_ANALOG(GPIOD_STSAFE_A100_RESET) |\
                                     PIN_MODE_ANALOG(GPIOD_USART3_TX) |     \
                                     PIN_MODE_ANALOG(GPIOD_USART3_RX) |     \
                                     PIN_MODE_ANALOG(GPIOD_LPS22HB_INT_DRDY_EXTI10) |\
                                     PIN_MODE_ANALOG(GPIOD_LSM6DSL_INT1_EXTI11) |\
                                     PIN_MODE_ANALOG(GPIOD_USB_OTG_FS_PWR_EN) |\
                                     PIN_MODE_ANALOG(GPIOD_SPBTLE_RF_SPI3_CSN) |\
                                     PIN_MODE_ANALOG(GPIOD_PIN14) |         \
                                     PIN_MODE_ANALOG(GPIOD_HTS221_DRDY_EXTI15))
#define VAL_GPIOD_OTYPER            (PIN_OTYPE_PUSHPULL(GPIOD_PMOD_RESET) | \
                                     PIN_OTYPE_PUSHPULL(GPIOD_PMOD_SPI2_SCK) |\
                                     PIN_OTYPE_PUSHPULL(GPIOD_PMOD_IRQ_EXTI2) |\
                                     PIN_OTYPE_PUSHPULL(GPIOD_USART2_CTS) | \
                                     PIN_OTYPE_PUSHPULL(GPIOD_USART2_RTS) | \
                                     PIN_OTYPE_PUSHPULL(GPIOD_USART2_TX) |  \
                                     PIN_OTYPE_PUSHPULL(GPIOD_USART2_RX) |  \
                                     PIN_OTYPE_PUSHPULL(GPIOD_STSAFE_A100_RESET) |\
                                     PIN_OTYPE_PUSHPULL(GPIOD_USART3_TX) |  \
                                     PIN_OTYPE_PUSHPULL(GPIOD_USART3_RX) |  \
                                     PIN_OTYPE_PUSHPULL(GPIOD_LPS22HB_INT_DRDY_EXTI10) |\
                                     PIN_OTYPE_PUSHPULL(GPIOD_LSM6DSL_INT1_EXTI11) |\
                                     PIN_OTYPE_PUSHPULL(GPIOD_USB_OTG_FS_PWR_EN) |\
                                     PIN_OTYPE_PUSHPULL(GPIOD_SPBTLE_RF_SPI3_CSN) |\
                                     PIN_OTYPE_PUSHPULL(GPIOD_PIN14) |      \
                                     PIN_OTYPE_PUSHPULL(GPIOD_HTS221_DRDY_EXTI15))
#define VAL_GPIOD_OSPEEDR           (PIN_OSPEED_HIGH(GPIOD_PMOD_RESET) |    \
                                     PIN_OSPEED_HIGH(GPIOD_PMOD_SPI2_SCK) | \
                                     PIN_OSPEED_HIGH(GPIOD_PMOD_IRQ_EXTI2) |\
                                     PIN_OSPEED_HIGH(GPIOD_USART2_CTS) |    \
                                     PIN_OSPEED_HIGH(GPIOD_USART2_RTS) |    \
                                     PIN_OSPEED_HIGH(GPIOD_USART2_TX) |     \
                                     PIN_OSPEED_HIGH(GPIOD_USART2_RX) |     \
                                     PIN_OSPEED_HIGH(GPIOD_STSAFE_A100_RESET) |\
                                     PIN_OSPEED_HIGH(GPIOD_USART3_TX) |     \
                                     PIN_OSPEED_HIGH(GPIOD_USART3_RX) |     \
                                     PIN_OSPEED_HIGH(GPIOD_LPS22HB_INT_DRDY_EXTI10) |\
                                     PIN_OSPEED_HIGH(GPIOD_LSM6DSL_INT1_EXTI11) |\
                                     PIN_OSPEED_HIGH(GPIOD_USB_OTG_FS_PWR_EN) |\
                                     PIN_OSPEED_HIGH(GPIOD_SPBTLE_RF_SPI3_CSN) |\
                                     PIN_OSPEED_HIGH(GPIOD_PIN14) |         \
                                     PIN_OSPEED_HIGH(GPIOD_HTS221_DRDY_EXTI15))
#define VAL_GPIOD_PUPDR             (PIN_PUPDR_PULLDOWN(GPIOD_PMOD_RESET) | \
                                     PIN_PUPDR_PULLUP(GPIOD_PMOD_SPI2_SCK) |\
                                     PIN_PUPDR_PULLDOWN(GPIOD_PMOD_IRQ_EXTI2) |\
                                     PIN_PUPDR_PULLUP(GPIOD_USART2_CTS) |   \
                                     PIN_PUPDR_PULLUP(GPIOD_USART2_RTS) |   \
                                     PIN_PUPDR_FLOATING(GPIOD_USART2_TX) |  \
                                     PIN_PUPDR_FLOATING(GPIOD_USART2_RX) |  \
                                     PIN_PUPDR_PULLUP(GPIOD_STSAFE_A100_RESET) |\
                                     PIN_PUPDR_FLOATING(GPIOD_USART3_TX) |  \
                                     PIN_PUPDR_FLOATING(GPIOD_USART3_RX) |  \
                                     PIN_PUPDR_FLOATING(GPIOD_LPS22HB_INT_DRDY_EXTI10) |\
                                     PIN_PUPDR_FLOATING(GPIOD_LSM6DSL_INT1_EXTI11) |\
                                     PIN_PUPDR_FLOATING(GPIOD_USB_OTG_FS_PWR_EN) |\
                                     PIN_PUPDR_FLOATING(GPIOD_SPBTLE_RF_SPI3_CSN) |\
                                     PIN_PUPDR_FLOATING(GPIOD_PIN14) |      \
                                     PIN_PUPDR_FLOATING(GPIOD_HTS221_DRDY_EXTI15))
#define VAL_GPIOD_ODR               (PIN_ODR_HIGH(GPIOD_PMOD_RESET) |       \
                                     PIN_ODR_HIGH(GPIOD_PMOD_SPI2_SCK) |    \
                                     PIN_ODR_HIGH(GPIOD_PMOD_IRQ_EXTI2) |   \
                                     PIN_ODR_HIGH(GPIOD_USART2_CTS) |       \
                                     PIN_ODR_HIGH(GPIOD_USART2_RTS) |       \
                                     PIN_ODR_HIGH(GPIOD_USART2_TX) |        \
                                     PIN_ODR_HIGH(GPIOD_USART2_RX) |        \
                                     PIN_ODR_HIGH(GPIOD_STSAFE_A100_RESET) |\
                                     PIN_ODR_HIGH(GPIOD_USART3_TX) |        \
                                     PIN_ODR_HIGH(GPIOD_USART3_RX) |        \
                                     PIN_ODR_HIGH(GPIOD_LPS22HB_INT_DRDY_EXTI10) |\
                                     PIN_ODR_HIGH(GPIOD_LSM6DSL_INT1_EXTI11) |\
                                     PIN_ODR_HIGH(GPIOD_USB_OTG_FS_PWR_EN) |\
                                     PIN_ODR_HIGH(GPIOD_SPBTLE_RF_SPI3_CSN) |\
                                     PIN_ODR_HIGH(GPIOD_PIN14) |            \
                                     PIN_ODR_HIGH(GPIOD_HTS221_DRDY_EXTI15))
#define VAL_GPIOD_AFRL              (PIN_AFIO_AF(GPIOD_PMOD_RESET, 0U) |    \
                                     PIN_AFIO_AF(GPIOD_PMOD_SPI2_SCK, 0U) | \
                                     PIN_AFIO_AF(GPIOD_PMOD_IRQ_EXTI2, 0U) |\
                                     PIN_AFIO_AF(GPIOD_USART2_CTS, 0U) |    \
                                     PIN_AFIO_AF(GPIOD_USART2_RTS, 0U) |    \
                                     PIN_AFIO_AF(GPIOD_USART2_TX, 0U) |     \
                                     PIN_AFIO_AF(GPIOD_USART2_RX, 0U) |     \
                                     PIN_AFIO_AF(GPIOD_STSAFE_A100_RESET, 0U))
#define VAL_GPIOD_AFRH              (PIN_AFIO_AF(GPIOD_USART3_TX, 0U) |     \
                                     PIN_AFIO_AF(GPIOD_USART3_RX, 0U) |     \
                                     PIN_AFIO_AF(GPIOD_LPS22HB_INT_DRDY_EXTI10, 0U) |\
                                     PIN_AFIO_AF(GPIOD_LSM6DSL_INT1_EXTI11, 0U) |\
                                     PIN_AFIO_AF(GPIOD_USB_OTG_FS_PWR_EN, 0U) |\
                                     PIN_AFIO_AF(GPIOD_SPBTLE_RF_SPI3_CSN, 0U) |\
                                     PIN_AFIO_AF(GPIOD_PIN14, 0U) |         \
                                     PIN_AFIO_AF(GPIOD_HTS221_DRDY_EXTI15, 0U))
#define VAL_GPIOD_ASCR              (PIN_ASCR_DISABLED(GPIOD_PMOD_RESET) |  \
                                     PIN_ASCR_DISABLED(GPIOD_PMOD_SPI2_SCK) |\
                                     PIN_ASCR_DISABLED(GPIOD_PMOD_IRQ_EXTI2) |\
                                     PIN_ASCR_DISABLED(GPIOD_USART2_CTS) |  \
                                     PIN_ASCR_DISABLED(GPIOD_USART2_RTS) |  \
                                     PIN_ASCR_DISABLED(GPIOD_USART2_TX) |   \
                                     PIN_ASCR_DISABLED(GPIOD_USART2_RX) |   \
                                     PIN_ASCR_DISABLED(GPIOD_STSAFE_A100_RESET) |\
                                     PIN_ASCR_DISABLED(GPIOD_USART3_TX) |   \
                                     PIN_ASCR_DISABLED(GPIOD_USART3_RX) |   \
                                     PIN_ASCR_DISABLED(GPIOD_LPS22HB_INT_DRDY_EXTI10) |\
                                     PIN_ASCR_DISABLED(GPIOD_LSM6DSL_INT1_EXTI11) |\
                                     PIN_ASCR_DISABLED(GPIOD_USB_OTG_FS_PWR_EN) |\
                                     PIN_ASCR_DISABLED(GPIOD_SPBTLE_RF_SPI3_CSN) |\
                                     PIN_ASCR_DISABLED(GPIOD_PIN14) |       \
                                     PIN_ASCR_DISABLED(GPIOD_HTS221_DRDY_EXTI15))
#define VAL_GPIOD_LOCKR             (PIN_LOCKR_DISABLED(GPIOD_PMOD_RESET) | \
                                     PIN_LOCKR_DISABLED(GPIOD_PMOD_SPI2_SCK) |\
                                     PIN_LOCKR_DISABLED(GPIOD_PMOD_IRQ_EXTI2) |\
                                     PIN_LOCKR_DISABLED(GPIOD_USART2_CTS) | \
                                     PIN_LOCKR_DISABLED(GPIOD_USART2_RTS) | \
                                     PIN_LOCKR_DISABLED(GPIOD_USART2_TX) |  \
                                     PIN_LOCKR_DISABLED(GPIOD_USART2_RX) |  \
                                     PIN_LOCKR_DISABLED(GPIOD_STSAFE_A100_RESET) |\
                                     PIN_LOCKR_DISABLED(GPIOD_USART3_TX) |  \
                                     PIN_LOCKR_DISABLED(GPIOD_USART3_RX) |  \
                                     PIN_LOCKR_DISABLED(GPIOD_LPS22HB_INT_DRDY_EXTI10) |\
                                     PIN_LOCKR_DISABLED(GPIOD_LSM6DSL_INT1_EXTI11) |\
                                     PIN_LOCKR_DISABLED(GPIOD_USB_OTG_FS_PWR_EN) |\
                                     PIN_LOCKR_DISABLED(GPIOD_SPBTLE_RF_SPI3_CSN) |\
                                     PIN_LOCKR_DISABLED(GPIOD_PIN14) |      \
                                     PIN_LOCKR_DISABLED(GPIOD_HTS221_DRDY_EXTI15))

/*
 * GPIOE setup:
 *
 * PE0  - ISM43362_SPI3_CSN         (analog).
 * PE1  - ISM43362_DRDY_EXTI1       (analog).
 * PE2  - M24SR64_Y_RF_DISABLE      (analog).
 * PE3  - USB_OTG_OVRCR_EXTI3       (analog).
 * PE4  - M24SR64_Y_GPO             (analog).
 * PE5  - SPSGRF_915_GPIO3_EXTI5    (analog).
 * PE6  - SPBTLE_RF_IRQ_EXTI6       (analog).
 * PE7  - DFSDM1_DATIN2             (analog).
 * PE8  - ISM43362_RST              (analog).
 * PE9  - DFSDM1_CKOUT              (analog).
 * PE10 - QUADSPI_CLK               (analog).
 * PE11 - QUADSPI_NCS               (analog).
 * PE12 - QUADSPI_BK1_IO0           (analog).
 * PE13 - QUADSPI_BK1_IO1           (analog).
 * PE14 - QUADSPI_BK1_IO2           (analog).
 * PE15 - QUADSPI_BK1_IO3           (analog).
 */
#define VAL_GPIOE_MODER             (PIN_MODE_ANALOG(GPIOE_ISM43362_SPI3_CSN) |\
                                     PIN_MODE_ANALOG(GPIOE_ISM43362_DRDY_EXTI1) |\
                                     PIN_MODE_ANALOG(GPIOE_M24SR64_Y_RF_DISABLE) |\
                                     PIN_MODE_ANALOG(GPIOE_USB_OTG_OVRCR_EXTI3) |\
                                     PIN_MODE_ANALOG(GPIOE_M24SR64_Y_GPO) | \
                                     PIN_MODE_ANALOG(GPIOE_SPSGRF_915_GPIO3_EXTI5) |\
                                     PIN_MODE_ANALOG(GPIOE_SPBTLE_RF_IRQ_EXTI6) |\
                                     PIN_MODE_ANALOG(GPIOE_DFSDM1_DATIN2) | \
                                     PIN_MODE_ANALOG(GPIOE_ISM43362_RST) |  \
                                     PIN_MODE_ANALOG(GPIOE_DFSDM1_CKOUT) |  \
                                     PIN_MODE_ANALOG(GPIOE_QUADSPI_CLK) |   \
                                     PIN_MODE_ANALOG(GPIOE_QUADSPI_NCS) |   \
                                     PIN_MODE_ANALOG(GPIOE_QUADSPI_BK1_IO0) |\
                                     PIN_MODE_ANALOG(GPIOE_QUADSPI_BK1_IO1) |\
                                     PIN_MODE_ANALOG(GPIOE_QUADSPI_BK1_IO2) |\
                                     PIN_MODE_ANALOG(GPIOE_QUADSPI_BK1_IO3))
#define VAL_GPIOE_OTYPER            (PIN_OTYPE_PUSHPULL(GPIOE_ISM43362_SPI3_CSN) |\
                                     PIN_OTYPE_PUSHPULL(GPIOE_ISM43362_DRDY_EXTI1) |\
                                     PIN_OTYPE_PUSHPULL(GPIOE_M24SR64_Y_RF_DISABLE) |\
                                     PIN_OTYPE_PUSHPULL(GPIOE_USB_OTG_OVRCR_EXTI3) |\
                                     PIN_OTYPE_PUSHPULL(GPIOE_M24SR64_Y_GPO) |\
                                     PIN_OTYPE_PUSHPULL(GPIOE_SPSGRF_915_GPIO3_EXTI5) |\
                                     PIN_OTYPE_PUSHPULL(GPIOE_SPBTLE_RF_IRQ_EXTI6) |\
                                     PIN_OTYPE_PUSHPULL(GPIOE_DFSDM1_DATIN2) |\
                                     PIN_OTYPE_PUSHPULL(GPIOE_ISM43362_RST) |\
                                     PIN_OTYPE_PUSHPULL(GPIOE_DFSDM1_CKOUT) |\
                                     PIN_OTYPE_PUSHPULL(GPIOE_QUADSPI_CLK) |\
                                     PIN_OTYPE_PUSHPULL(GPIOE_QUADSPI_NCS) |\
                                     PIN_OTYPE_PUSHPULL(GPIOE_QUADSPI_BK1_IO0) |\
                                     PIN_OTYPE_PUSHPULL(GPIOE_QUADSPI_BK1_IO1) |\
                                     PIN_OTYPE_PUSHPULL(GPIOE_QUADSPI_BK1_IO2) |\
                                     PIN_OTYPE_PUSHPULL(GPIOE_QUADSPI_BK1_IO3))
#define VAL_GPIOE_OSPEEDR           (PIN_OSPEED_HIGH(GPIOE_ISM43362_SPI3_CSN) |\
                                     PIN_OSPEED_HIGH(GPIOE_ISM43362_DRDY_EXTI1) |\
                                     PIN_OSPEED_HIGH(GPIOE_M24SR64_Y_RF_DISABLE) |\
                                     PIN_OSPEED_HIGH(GPIOE_USB_OTG_OVRCR_EXTI3) |\
                                     PIN_OSPEED_HIGH(GPIOE_M24SR64_Y_GPO) | \
                                     PIN_OSPEED_HIGH(GPIOE_SPSGRF_915_GPIO3_EXTI5) |\
                                     PIN_OSPEED_HIGH(GPIOE_SPBTLE_RF_IRQ_EXTI6) |\
                                     PIN_OSPEED_HIGH(GPIOE_DFSDM1_DATIN2) | \
                                     PIN_OSPEED_HIGH(GPIOE_ISM43362_RST) |  \
                                     PIN_OSPEED_HIGH(GPIOE_DFSDM1_CKOUT) |  \
                                     PIN_OSPEED_HIGH(GPIOE_QUADSPI_CLK) |   \
                                     PIN_OSPEED_HIGH(GPIOE_QUADSPI_NCS) |   \
                                     PIN_OSPEED_HIGH(GPIOE_QUADSPI_BK1_IO0) |\
                                     PIN_OSPEED_HIGH(GPIOE_QUADSPI_BK1_IO1) |\
                                     PIN_OSPEED_HIGH(GPIOE_QUADSPI_BK1_IO2) |\
                                     PIN_OSPEED_HIGH(GPIOE_QUADSPI_BK1_IO3))
#define VAL_GPIOE_PUPDR             (PIN_PUPDR_PULLUP(GPIOE_ISM43362_SPI3_CSN) |\
                                     PIN_PUPDR_PULLDOWN(GPIOE_ISM43362_DRDY_EXTI1) |\
                                     PIN_PUPDR_FLOATING(GPIOE_M24SR64_Y_RF_DISABLE) |\
                                     PIN_PUPDR_FLOATING(GPIOE_USB_OTG_OVRCR_EXTI3) |\
                                     PIN_PUPDR_FLOATING(GPIOE_M24SR64_Y_GPO) |\
                                     PIN_PUPDR_FLOATING(GPIOE_SPSGRF_915_GPIO3_EXTI5) |\
                                     PIN_PUPDR_FLOATING(GPIOE_SPBTLE_RF_IRQ_EXTI6) |\
                                     PIN_PUPDR_FLOATING(GPIOE_DFSDM1_DATIN2) |\
                                     PIN_PUPDR_FLOATING(GPIOE_ISM43362_RST) |\
                                     PIN_PUPDR_FLOATING(GPIOE_DFSDM1_CKOUT) |\
                                     PIN_PUPDR_PULLUP(GPIOE_QUADSPI_CLK) |  \
                                     PIN_PUPDR_PULLUP(GPIOE_QUADSPI_NCS) |  \
                                     PIN_PUPDR_PULLUP(GPIOE_QUADSPI_BK1_IO0) |\
                                     PIN_PUPDR_PULLUP(GPIOE_QUADSPI_BK1_IO1) |\
                                     PIN_PUPDR_PULLUP(GPIOE_QUADSPI_BK1_IO2) |\
                                     PIN_PUPDR_PULLUP(GPIOE_QUADSPI_BK1_IO3))
#define VAL_GPIOE_ODR               (PIN_ODR_HIGH(GPIOE_ISM43362_SPI3_CSN) |\
                                     PIN_ODR_HIGH(GPIOE_ISM43362_DRDY_EXTI1) |\
                                     PIN_ODR_HIGH(GPIOE_M24SR64_Y_RF_DISABLE) |\
                                     PIN_ODR_HIGH(GPIOE_USB_OTG_OVRCR_EXTI3) |\
                                     PIN_ODR_HIGH(GPIOE_M24SR64_Y_GPO) |    \
                                     PIN_ODR_HIGH(GPIOE_SPSGRF_915_GPIO3_EXTI5) |\
                                     PIN_ODR_HIGH(GPIOE_SPBTLE_RF_IRQ_EXTI6) |\
                                     PIN_ODR_HIGH(GPIOE_DFSDM1_DATIN2) |    \
                                     PIN_ODR_LOW(GPIOE_ISM43362_RST) |      \
                                     PIN_ODR_HIGH(GPIOE_DFSDM1_CKOUT) |     \
                                     PIN_ODR_HIGH(GPIOE_QUADSPI_CLK) |      \
                                     PIN_ODR_HIGH(GPIOE_QUADSPI_NCS) |      \
                                     PIN_ODR_HIGH(GPIOE_QUADSPI_BK1_IO0) |  \
                                     PIN_ODR_HIGH(GPIOE_QUADSPI_BK1_IO1) |  \
                                     PIN_ODR_HIGH(GPIOE_QUADSPI_BK1_IO2) |  \
                                     PIN_ODR_HIGH(GPIOE_QUADSPI_BK1_IO3))
#define VAL_GPIOE_AFRL              (PIN_AFIO_AF(GPIOE_ISM43362_SPI3_CSN, 0U) |\
                                     PIN_AFIO_AF(GPIOE_ISM43362_DRDY_EXTI1, 0U) |\
                                     PIN_AFIO_AF(GPIOE_M24SR64_Y_RF_DISABLE, 0U) |\
                                     PIN_AFIO_AF(GPIOE_USB_OTG_OVRCR_EXTI3, 0U) |\
                                     PIN_AFIO_AF(GPIOE_M24SR64_Y_GPO, 0U) | \
                                     PIN_AFIO_AF(GPIOE_SPSGRF_915_GPIO3_EXTI5, 0U) |\
                                     PIN_AFIO_AF(GPIOE_SPBTLE_RF_IRQ_EXTI6, 0U) |\
                                     PIN_AFIO_AF(GPIOE_DFSDM1_DATIN2, 0U))
#define VAL_GPIOE_AFRH              (PIN_AFIO_AF(GPIOE_ISM43362_RST, 0U) |  \
                                     PIN_AFIO_AF(GPIOE_DFSDM1_CKOUT, 0U) |  \
                                     PIN_AFIO_AF(GPIOE_QUADSPI_CLK, 0U) |   \
                                     PIN_AFIO_AF(GPIOE_QUADSPI_NCS, 0U) |   \
                                     PIN_AFIO_AF(GPIOE_QUADSPI_BK1_IO0, 0U) |\
                                     PIN_AFIO_AF(GPIOE_QUADSPI_BK1_IO1, 0U) |\
                                     PIN_AFIO_AF(GPIOE_QUADSPI_BK1_IO2, 0U) |\
                                     PIN_AFIO_AF(GPIOE_QUADSPI_BK1_IO3, 0U))
#define VAL_GPIOE_ASCR              (PIN_ASCR_DISABLED(GPIOE_ISM43362_SPI3_CSN) |\
                                     PIN_ASCR_DISABLED(GPIOE_ISM43362_DRDY_EXTI1) |\
                                     PIN_ASCR_DISABLED(GPIOE_M24SR64_Y_RF_DISABLE) |\
                                     PIN_ASCR_DISABLED(GPIOE_USB_OTG_OVRCR_EXTI3) |\
                                     PIN_ASCR_DISABLED(GPIOE_M24SR64_Y_GPO) |\
                                     PIN_ASCR_DISABLED(GPIOE_SPSGRF_915_GPIO3_EXTI5) |\
                                     PIN_ASCR_DISABLED(GPIOE_SPBTLE_RF_IRQ_EXTI6) |\
                                     PIN_ASCR_DISABLED(GPIOE_DFSDM1_DATIN2) |\
                                     PIN_ASCR_DISABLED(GPIOE_ISM43362_RST) |\
                                     PIN_ASCR_DISABLED(GPIOE_DFSDM1_CKOUT) |\
                                     PIN_ASCR_DISABLED(GPIOE_QUADSPI_CLK) | \
                                     PIN_ASCR_DISABLED(GPIOE_QUADSPI_NCS) | \
                                     PIN_ASCR_DISABLED(GPIOE_QUADSPI_BK1_IO0) |\
                                     PIN_ASCR_DISABLED(GPIOE_QUADSPI_BK1_IO1) |\
                                     PIN_ASCR_DISABLED(GPIOE_QUADSPI_BK1_IO2) |\
                                     PIN_ASCR_DISABLED(GPIOE_QUADSPI_BK1_IO3))
#define VAL_GPIOE_LOCKR             (PIN_LOCKR_DISABLED(GPIOE_ISM43362_SPI3_CSN) |\
                                     PIN_LOCKR_DISABLED(GPIOE_ISM43362_DRDY_EXTI1) |\
                                     PIN_LOCKR_DISABLED(GPIOE_M24SR64_Y_RF_DISABLE) |\
                                     PIN_LOCKR_DISABLED(GPIOE_USB_OTG_OVRCR_EXTI3) |\
                                     PIN_LOCKR_DISABLED(GPIOE_M24SR64_Y_GPO) |\
                                     PIN_LOCKR_DISABLED(GPIOE_SPSGRF_915_GPIO3_EXTI5) |\
                                     PIN_LOCKR_DISABLED(GPIOE_SPBTLE_RF_IRQ_EXTI6) |\
                                     PIN_LOCKR_DISABLED(GPIOE_DFSDM1_DATIN2) |\
                                     PIN_LOCKR_DISABLED(GPIOE_ISM43362_RST) |\
                                     PIN_LOCKR_DISABLED(GPIOE_DFSDM1_CKOUT) |\
                                     PIN_LOCKR_DISABLED(GPIOE_QUADSPI_CLK) |\
                                     PIN_LOCKR_DISABLED(GPIOE_QUADSPI_NCS) |\
                                     PIN_LOCKR_DISABLED(GPIOE_QUADSPI_BK1_IO0) |\
                                     PIN_LOCKR_DISABLED(GPIOE_QUADSPI_BK1_IO1) |\
                                     PIN_LOCKR_DISABLED(GPIOE_QUADSPI_BK1_IO2) |\
                                     PIN_LOCKR_DISABLED(GPIOE_QUADSPI_BK1_IO3))

/*
 * GPIOF setup:
 *
 * PF0  - PIN0                      (analog).
 * PF1  - PIN1                      (analog).
 * PF2  - PIN2                      (analog).
 * PF3  - PIN3                      (analog).
 * PF4  - PIN4                      (analog).
 * PF5  - PIN5                      (analog).
 * PF6  - PIN6                      (analog).
 * PF7  - PIN7                      (analog).
 * PF8  - PIN8                      (analog).
 * PF9  - PIN9                      (analog).
 * PF10 - PIN10                     (analog).
 * PF11 - PIN11                     (analog).
 * PF12 - PIN12                     (analog).
 * PF13 - PIN13                     (analog).
 * PF14 - PIN14                     (analog).
 * PF15 - PIN15                     (analog).
 */
#define VAL_GPIOF_MODER             (PIN_MODE_ANALOG(GPIOF_PIN0) |          \
                                     PIN_MODE_ANALOG(GPIOF_PIN1) |          \
                                     PIN_MODE_ANALOG(GPIOF_PIN2) |          \
                                     PIN_MODE_ANALOG(GPIOF_PIN3) |          \
                                     PIN_MODE_ANALOG(GPIOF_PIN4) |          \
                                     PIN_MODE_ANALOG(GPIOF_PIN5) |          \
                                     PIN_MODE_ANALOG(GPIOF_PIN6) |          \
                                     PIN_MODE_ANALOG(GPIOF_PIN7) |          \
                                     PIN_MODE_ANALOG(GPIOF_PIN8) |          \
                                     PIN_MODE_ANALOG(GPIOF_PIN9) |          \
                                     PIN_MODE_ANALOG(GPIOF_PIN10) |         \
                                     PIN_MODE_ANALOG(GPIOF_PIN11) |         \
                                     PIN_MODE_ANALOG(GPIOF_PIN12) |         \
                                     PIN_MODE_ANALOG(GPIOF_PIN13) |         \
                                     PIN_MODE_ANALOG(GPIOF_PIN14) |         \
                                     PIN_MODE_ANALOG(GPIOF_PIN15))
#define VAL_GPIOF_OTYPER            (PIN_OTYPE_PUSHPULL(GPIOF_PIN0) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOF_PIN1) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOF_PIN2) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOF_PIN3) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOF_PIN4) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOF_PIN5) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOF_PIN6) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOF_PIN7) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOF_PIN8) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOF_PIN9) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOF_PIN10) |      \
                                     PIN_OTYPE_PUSHPULL(GPIOF_PIN11) |      \
                                     PIN_OTYPE_PUSHPULL(GPIOF_PIN12) |      \
                                     PIN_OTYPE_PUSHPULL(GPIOF_PIN13) |      \
                                     PIN_OTYPE_PUSHPULL(GPIOF_PIN14) |      \
                                     PIN_OTYPE_PUSHPULL(GPIOF_PIN15))
#define VAL_GPIOF_OSPEEDR           (PIN_OSPEED_HIGH(GPIOF_PIN0) |          \
                                     PIN_OSPEED_HIGH(GPIOF_PIN1) |          \
                                     PIN_OSPEED_HIGH(GPIOF_PIN2) |          \
                                     PIN_OSPEED_HIGH(GPIOF_PIN3) |          \
                                     PIN_OSPEED_HIGH(GPIOF_PIN4) |          \
                                     PIN_OSPEED_HIGH(GPIOF_PIN5) |          \
                                     PIN_OSPEED_HIGH(GPIOF_PIN6) |          \
                                     PIN_OSPEED_HIGH(GPIOF_PIN7) |          \
                                     PIN_OSPEED_HIGH(GPIOF_PIN8) |          \
                                     PIN_OSPEED_HIGH(GPIOF_PIN9) |          \
                                     PIN_OSPEED_HIGH(GPIOF_PIN10) |         \
                                     PIN_OSPEED_HIGH(GPIOF_PIN11) |         \
                                     PIN_OSPEED_HIGH(GPIOF_PIN12) |         \
                                     PIN_OSPEED_HIGH(GPIOF_PIN13) |         \
                                     PIN_OSPEED_HIGH(GPIOF_PIN14) |         \
                                     PIN_OSPEED_HIGH(GPIOF_PIN15))
#define VAL_GPIOF_PUPDR             (PIN_PUPDR_FLOATING(GPIOF_PIN0) |       \
                                     PIN_PUPDR_FLOATING(GPIOF_PIN1) |       \
                                     PIN_PUPDR_FLOATING(GPIOF_PIN2) |       \
                                     PIN_PUPDR_FLOATING(GPIOF_PIN3) |       \
                                     PIN_PUPDR_FLOATING(GPIOF_PIN4) |       \
                                     PIN_PUPDR_FLOATING(GPIOF_PIN5) |       \
                                     PIN_PUPDR_FLOATING(GPIOF_PIN6) |       \
                                     PIN_PUPDR_FLOATING(GPIOF_PIN7) |       \
                                     PIN_PUPDR_FLOATING(GPIOF_PIN8) |       \
                                     PIN_PUPDR_FLOATING(GPIOF_PIN9) |       \
                                     PIN_PUPDR_FLOATING(GPIOF_PIN10) |      \
                                     PIN_PUPDR_FLOATING(GPIOF_PIN11) |      \
                                     PIN_PUPDR_FLOATING(GPIOF_PIN12) |      \
                                     PIN_PUPDR_FLOATING(GPIOF_PIN13) |      \
                                     PIN_PUPDR_FLOATING(GPIOF_PIN14) |      \
                                     PIN_PUPDR_FLOATING(GPIOF_PIN15))
#define VAL_GPIOF_ODR               (PIN_ODR_HIGH(GPIOF_PIN0) |             \
                                     PIN_ODR_HIGH(GPIOF_PIN1) |             \
                                     PIN_ODR_HIGH(GPIOF_PIN2) |             \
                                     PIN_ODR_HIGH(GPIOF_PIN3) |             \
                                     PIN_ODR_HIGH(GPIOF_PIN4) |             \
                                     PIN_ODR_HIGH(GPIOF_PIN5) |             \
                                     PIN_ODR_HIGH(GPIOF_PIN6) |             \
                                     PIN_ODR_HIGH(GPIOF_PIN7) |             \
                                     PIN_ODR_HIGH(GPIOF_PIN8) |             \
                                     PIN_ODR_HIGH(GPIOF_PIN9) |             \
                                     PIN_ODR_HIGH(GPIOF_PIN10) |            \
                                     PIN_ODR_HIGH(GPIOF_PIN11) |            \
                                     PIN_ODR_HIGH(GPIOF_PIN12) |            \
                                     PIN_ODR_HIGH(GPIOF_PIN13) |            \
                                     PIN_ODR_HIGH(GPIOF_PIN14) |            \
                                     PIN_ODR_HIGH(GPIOF_PIN15))
#define VAL_GPIOF_AFRL              (PIN_AFIO_AF(GPIOF_PIN0, 0U) |          \
                                     PIN_AFIO_AF(GPIOF_PIN1, 0U) |          \
                                     PIN_AFIO_AF(GPIOF_PIN2, 0U) |          \
                                     PIN_AFIO_AF(GPIOF_PIN3, 0U) |          \
                                     PIN_AFIO_AF(GPIOF_PIN4, 0U) |          \
                                     PIN_AFIO_AF(GPIOF_PIN5, 0U) |          \
                                     PIN_AFIO_AF(GPIOF_PIN6, 0U) |          \
                                     PIN_AFIO_AF(GPIOF_PIN7, 0U))
#define VAL_GPIOF_AFRH              (PIN_AFIO_AF(GPIOF_PIN8, 0U) |          \
                                     PIN_AFIO_AF(GPIOF_PIN9, 0U) |          \
                                     PIN_AFIO_AF(GPIOF_PIN10, 0U) |         \
                                     PIN_AFIO_AF(GPIOF_PIN11, 0U) |         \
                                     PIN_AFIO_AF(GPIOF_PIN12, 0U) |         \
                                     PIN_AFIO_AF(GPIOF_PIN13, 0U) |         \
                                     PIN_AFIO_AF(GPIOF_PIN14, 0U) |         \
                                     PIN_AFIO_AF(GPIOF_PIN15, 0U))
#define VAL_GPIOF_ASCR              (PIN_ASCR_DISABLED(GPIOF_PIN0) |        \
                                     PIN_ASCR_DISABLED(GPIOF_PIN1) |        \
                                     PIN_ASCR_DISABLED(GPIOF_PIN2) |        \
                                     PIN_ASCR_DISABLED(GPIOF_PIN3) |        \
                                     PIN_ASCR_DISABLED(GPIOF_PIN4) |        \
                                     PIN_ASCR_DISABLED(GPIOF_PIN5) |        \
                                     PIN_ASCR_DISABLED(GPIOF_PIN6) |        \
                                     PIN_ASCR_DISABLED(GPIOF_PIN7) |        \
                                     PIN_ASCR_DISABLED(GPIOF_PIN8) |        \
                                     PIN_ASCR_DISABLED(GPIOF_PIN9) |        \
                                     PIN_ASCR_DISABLED(GPIOF_PIN10) |       \
                                     PIN_ASCR_DISABLED(GPIOF_PIN11) |       \
                                     PIN_ASCR_DISABLED(GPIOF_PIN12) |       \
                                     PIN_ASCR_DISABLED(GPIOF_PIN13) |       \
                                     PIN_ASCR_DISABLED(GPIOF_PIN14) |       \
                                     PIN_ASCR_DISABLED(GPIOF_PIN15))
#define VAL_GPIOF_LOCKR             (PIN_LOCKR_DISABLED(GPIOF_PIN0) |       \
                                     PIN_LOCKR_DISABLED(GPIOF_PIN1) |       \
                                     PIN_LOCKR_DISABLED(GPIOF_PIN2) |       \
                                     PIN_LOCKR_DISABLED(GPIOF_PIN3) |       \
                                     PIN_LOCKR_DISABLED(GPIOF_PIN4) |       \
                                     PIN_LOCKR_DISABLED(GPIOF_PIN5) |       \
                                     PIN_LOCKR_DISABLED(GPIOF_PIN6) |       \
                                     PIN_LOCKR_DISABLED(GPIOF_PIN7) |       \
                                     PIN_LOCKR_DISABLED(GPIOF_PIN8) |       \
                                     PIN_LOCKR_DISABLED(GPIOF_PIN9) |       \
                                     PIN_LOCKR_DISABLED(GPIOF_PIN10) |      \
                                     PIN_LOCKR_DISABLED(GPIOF_PIN11) |      \
                                     PIN_LOCKR_DISABLED(GPIOF_PIN12) |      \
                                     PIN_LOCKR_DISABLED(GPIOF_PIN13) |      \
                                     PIN_LOCKR_DISABLED(GPIOF_PIN14) |      \
                                     PIN_LOCKR_DISABLED(GPIOF_PIN15))

/*
 * GPIOG setup:
 *
 * PG0  - PIN0                      (analog).
 * PG1  - PIN1                      (analog).
 * PG2  - PIN2                      (analog).
 * PG3  - PIN3                      (analog).
 * PG4  - PIN4                      (analog).
 * PG5  - PIN5                      (analog).
 * PG6  - PIN6                      (analog).
 * PG7  - PIN7                      (analog).
 * PG8  - PIN8                      (analog).
 * PG9  - PIN9                      (analog).
 * PG10 - PIN10                     (analog).
 * PG11 - PIN11                     (analog).
 * PG12 - PIN12                     (analog).
 * PG13 - PIN13                     (analog).
 * PG14 - PIN14                     (analog).
 * PG15 - PIN15                     (analog).
 */
#define VAL_GPIOG_MODER             (PIN_MODE_ANALOG(GPIOG_PIN0) |          \
                                     PIN_MODE_ANALOG(GPIOG_PIN1) |          \
                                     PIN_MODE_ANALOG(GPIOG_PIN2) |          \
                                     PIN_MODE_ANALOG(GPIOG_PIN3) |          \
                                     PIN_MODE_ANALOG(GPIOG_PIN4) |          \
                                     PIN_MODE_ANALOG(GPIOG_PIN5) |          \
                                     PIN_MODE_ANALOG(GPIOG_PIN6) |          \
                                     PIN_MODE_ANALOG(GPIOG_PIN7) |          \
                                     PIN_MODE_ANALOG(GPIOG_PIN8) |          \
                                     PIN_MODE_ANALOG(GPIOG_PIN9) |          \
                                     PIN_MODE_ANALOG(GPIOG_PIN10) |         \
                                     PIN_MODE_ANALOG(GPIOG_PIN11) |         \
                                     PIN_MODE_ANALOG(GPIOG_PIN12) |         \
                                     PIN_MODE_ANALOG(GPIOG_PIN13) |         \
                                     PIN_MODE_ANALOG(GPIOG_PIN14) |         \
                                     PIN_MODE_ANALOG(GPIOG_PIN15))
#define VAL_GPIOG_OTYPER            (PIN_OTYPE_PUSHPULL(GPIOG_PIN0) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOG_PIN1) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOG_PIN2) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOG_PIN3) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOG_PIN4) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOG_PIN5) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOG_PIN6) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOG_PIN7) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOG_PIN8) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOG_PIN9) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOG_PIN10) |      \
                                     PIN_OTYPE_PUSHPULL(GPIOG_PIN11) |      \
                                     PIN_OTYPE_PUSHPULL(GPIOG_PIN12) |      \
                                     PIN_OTYPE_PUSHPULL(GPIOG_PIN13) |      \
                                     PIN_OTYPE_PUSHPULL(GPIOG_PIN14) |      \
                                     PIN_OTYPE_PUSHPULL(GPIOG_PIN15))
#define VAL_GPIOG_OSPEEDR           (PIN_OSPEED_VERYLOW(GPIOG_PIN0) |       \
                                     PIN_OSPEED_VERYLOW(GPIOG_PIN1) |       \
                                     PIN_OSPEED_VERYLOW(GPIOG_PIN2) |       \
                                     PIN_OSPEED_VERYLOW(GPIOG_PIN3) |       \
                                     PIN_OSPEED_VERYLOW(GPIOG_PIN4) |       \
                                     PIN_OSPEED_VERYLOW(GPIOG_PIN5) |       \
                                     PIN_OSPEED_VERYLOW(GPIOG_PIN6) |       \
                                     PIN_OSPEED_VERYLOW(GPIOG_PIN7) |       \
                                     PIN_OSPEED_VERYLOW(GPIOG_PIN8) |       \
                                     PIN_OSPEED_VERYLOW(GPIOG_PIN9) |       \
                                     PIN_OSPEED_VERYLOW(GPIOG_PIN10) |      \
                                     PIN_OSPEED_VERYLOW(GPIOG_PIN11) |      \
                                     PIN_OSPEED_VERYLOW(GPIOG_PIN12) |      \
                                     PIN_OSPEED_VERYLOW(GPIOG_PIN13) |      \
                                     PIN_OSPEED_VERYLOW(GPIOG_PIN14) |      \
                                     PIN_OSPEED_VERYLOW(GPIOG_PIN15))
#define VAL_GPIOG_PUPDR             (PIN_PUPDR_FLOATING(GPIOG_PIN0) |       \
                                     PIN_PUPDR_FLOATING(GPIOG_PIN1) |       \
                                     PIN_PUPDR_FLOATING(GPIOG_PIN2) |       \
                                     PIN_PUPDR_FLOATING(GPIOG_PIN3) |       \
                                     PIN_PUPDR_FLOATING(GPIOG_PIN4) |       \
                                     PIN_PUPDR_FLOATING(GPIOG_PIN5) |       \
                                     PIN_PUPDR_FLOATING(GPIOG_PIN6) |       \
                                     PIN_PUPDR_FLOATING(GPIOG_PIN7) |       \
                                     PIN_PUPDR_FLOATING(GPIOG_PIN8) |       \
                                     PIN_PUPDR_FLOATING(GPIOG_PIN9) |       \
                                     PIN_PUPDR_FLOATING(GPIOG_PIN10) |      \
                                     PIN_PUPDR_FLOATING(GPIOG_PIN11) |      \
                                     PIN_PUPDR_FLOATING(GPIOG_PIN12) |      \
                                     PIN_PUPDR_FLOATING(GPIOG_PIN13) |      \
                                     PIN_PUPDR_FLOATING(GPIOG_PIN14) |      \
                                     PIN_PUPDR_FLOATING(GPIOG_PIN15))
#define VAL_GPIOG_ODR               (PIN_ODR_HIGH(GPIOG_PIN0) |             \
                                     PIN_ODR_HIGH(GPIOG_PIN1) |             \
                                     PIN_ODR_HIGH(GPIOG_PIN2) |             \
                                     PIN_ODR_HIGH(GPIOG_PIN3) |             \
                                     PIN_ODR_HIGH(GPIOG_PIN4) |             \
                                     PIN_ODR_HIGH(GPIOG_PIN5) |             \
                                     PIN_ODR_HIGH(GPIOG_PIN6) |             \
                                     PIN_ODR_HIGH(GPIOG_PIN7) |             \
                                     PIN_ODR_HIGH(GPIOG_PIN8) |             \
                                     PIN_ODR_HIGH(GPIOG_PIN9) |             \
                                     PIN_ODR_HIGH(GPIOG_PIN10) |            \
                                     PIN_ODR_HIGH(GPIOG_PIN11) |            \
                                     PIN_ODR_HIGH(GPIOG_PIN12) |            \
                                     PIN_ODR_HIGH(GPIOG_PIN13) |            \
                                     PIN_ODR_HIGH(GPIOG_PIN14) |            \
                                     PIN_ODR_HIGH(GPIOG_PIN15))
#define VAL_GPIOG_AFRL              (PIN_AFIO_AF(GPIOG_PIN0, 0U) |          \
                                     PIN_AFIO_AF(GPIOG_PIN1, 0U) |          \
                                     PIN_AFIO_AF(GPIOG_PIN2, 0U) |          \
                                     PIN_AFIO_AF(GPIOG_PIN3, 0U) |          \
                                     PIN_AFIO_AF(GPIOG_PIN4, 0U) |          \
                                     PIN_AFIO_AF(GPIOG_PIN5, 0U) |          \
                                     PIN_AFIO_AF(GPIOG_PIN6, 0U) |          \
                                     PIN_AFIO_AF(GPIOG_PIN7, 0U))
#define VAL_GPIOG_AFRH              (PIN_AFIO_AF(GPIOG_PIN8, 0U) |          \
                                     PIN_AFIO_AF(GPIOG_PIN9, 0U) |          \
                                     PIN_AFIO_AF(GPIOG_PIN10, 0U) |         \
                                     PIN_AFIO_AF(GPIOG_PIN11, 0U) |         \
                                     PIN_AFIO_AF(GPIOG_PIN12, 0U) |         \
                                     PIN_AFIO_AF(GPIOG_PIN13, 0U) |         \
                                     PIN_AFIO_AF(GPIOG_PIN14, 0U) |         \
                                     PIN_AFIO_AF(GPIOG_PIN15, 0U))
#define VAL_GPIOG_ASCR              (PIN_ASCR_DISABLED(GPIOG_PIN0) |        \
                                     PIN_ASCR_DISABLED(GPIOG_PIN1) |        \
                                     PIN_ASCR_DISABLED(GPIOG_PIN2) |        \
                                     PIN_ASCR_DISABLED(GPIOG_PIN3) |        \
                                     PIN_ASCR_DISABLED(GPIOG_PIN4) |        \
                                     PIN_ASCR_DISABLED(GPIOG_PIN5) |        \
                                     PIN_ASCR_DISABLED(GPIOG_PIN6) |        \
                                     PIN_ASCR_DISABLED(GPIOG_PIN7) |        \
                                     PIN_ASCR_DISABLED(GPIOG_PIN8) |        \
                                     PIN_ASCR_DISABLED(GPIOG_PIN9) |        \
                                     PIN_ASCR_DISABLED(GPIOG_PIN10) |       \
                                     PIN_ASCR_DISABLED(GPIOG_PIN11) |       \
                                     PIN_ASCR_DISABLED(GPIOG_PIN12) |       \
                                     PIN_ASCR_DISABLED(GPIOG_PIN13) |       \
                                     PIN_ASCR_DISABLED(GPIOG_PIN14) |       \
                                     PIN_ASCR_DISABLED(GPIOG_PIN15))
#define VAL_GPIOG_LOCKR             (PIN_LOCKR_DISABLED(GPIOG_PIN0) |       \
                                     PIN_LOCKR_DISABLED(GPIOG_PIN1) |       \
                                     PIN_LOCKR_DISABLED(GPIOG_PIN2) |       \
                                     PIN_LOCKR_DISABLED(GPIOG_PIN3) |       \
                                     PIN_LOCKR_DISABLED(GPIOG_PIN4) |       \
                                     PIN_LOCKR_DISABLED(GPIOG_PIN5) |       \
                                     PIN_LOCKR_DISABLED(GPIOG_PIN6) |       \
                                     PIN_LOCKR_DISABLED(GPIOG_PIN7) |       \
                                     PIN_LOCKR_DISABLED(GPIOG_PIN8) |       \
                                     PIN_LOCKR_DISABLED(GPIOG_PIN9) |       \
                                     PIN_LOCKR_DISABLED(GPIOG_PIN10) |      \
                                     PIN_LOCKR_DISABLED(GPIOG_PIN11) |      \
                                     PIN_LOCKR_DISABLED(GPIOG_PIN12) |      \
                                     PIN_LOCKR_DISABLED(GPIOG_PIN13) |      \
                                     PIN_LOCKR_DISABLED(GPIOG_PIN14) |      \
                                     PIN_LOCKR_DISABLED(GPIOG_PIN15))

/*
 * GPIOH setup:
 *
 * PH0  - OSC_IN                    (input floating).
 * PH1  - OSC_OUT                   (input floating).
 * PH2  - PIN2                      (analog).
 * PH3  - PIN3                      (analog).
 * PH4  - PIN4                      (analog).
 * PH5  - PIN5                      (analog).
 * PH6  - PIN6                      (analog).
 * PH7  - PIN7                      (analog).
 * PH8  - PIN8                      (analog).
 * PH9  - PIN9                      (analog).
 * PH10 - PIN10                     (analog).
 * PH11 - PIN11                     (analog).
 * PH12 - PIN12                     (analog).
 * PH13 - PIN13                     (analog).
 * PH14 - PIN14                     (analog).
 * PH15 - PIN15                     (analog).
 */
#define VAL_GPIOH_MODER             (PIN_MODE_INPUT(GPIOH_OSC_IN) |         \
                                     PIN_MODE_INPUT(GPIOH_OSC_OUT) |        \
                                     PIN_MODE_ANALOG(GPIOH_PIN2) |          \
                                     PIN_MODE_ANALOG(GPIOH_PIN3) |          \
                                     PIN_MODE_ANALOG(GPIOH_PIN4) |          \
                                     PIN_MODE_ANALOG(GPIOH_PIN5) |          \
                                     PIN_MODE_ANALOG(GPIOH_PIN6) |          \
                                     PIN_MODE_ANALOG(GPIOH_PIN7) |          \
                                     PIN_MODE_ANALOG(GPIOH_PIN8) |          \
                                     PIN_MODE_ANALOG(GPIOH_PIN9) |          \
                                     PIN_MODE_ANALOG(GPIOH_PIN10) |         \
                                     PIN_MODE_ANALOG(GPIOH_PIN11) |         \
                                     PIN_MODE_ANALOG(GPIOH_PIN12) |         \
                                     PIN_MODE_ANALOG(GPIOH_PIN13) |         \
                                     PIN_MODE_ANALOG(GPIOH_PIN14) |         \
                                     PIN_MODE_ANALOG(GPIOH_PIN15))
#define VAL_GPIOH_OTYPER            (PIN_OTYPE_PUSHPULL(GPIOH_OSC_IN) |     \
                                     PIN_OTYPE_PUSHPULL(GPIOH_OSC_OUT) |    \
                                     PIN_OTYPE_PUSHPULL(GPIOH_PIN2) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOH_PIN3) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOH_PIN4) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOH_PIN5) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOH_PIN6) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOH_PIN7) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOH_PIN8) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOH_PIN9) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOH_PIN10) |      \
                                     PIN_OTYPE_PUSHPULL(GPIOH_PIN11) |      \
                                     PIN_OTYPE_PUSHPULL(GPIOH_PIN12) |      \
                                     PIN_OTYPE_PUSHPULL(GPIOH_PIN13) |      \
                                     PIN_OTYPE_PUSHPULL(GPIOH_PIN14) |      \
                                     PIN_OTYPE_PUSHPULL(GPIOH_PIN15))
#define VAL_GPIOH_OSPEEDR           (PIN_OSPEED_HIGH(GPIOH_OSC_IN) |        \
                                     PIN_OSPEED_HIGH(GPIOH_OSC_OUT) |       \
                                     PIN_OSPEED_VERYLOW(GPIOH_PIN2) |       \
                                     PIN_OSPEED_VERYLOW(GPIOH_PIN3) |       \
                                     PIN_OSPEED_VERYLOW(GPIOH_PIN4) |       \
                                     PIN_OSPEED_VERYLOW(GPIOH_PIN5) |       \
                                     PIN_OSPEED_VERYLOW(GPIOH_PIN6) |       \
                                     PIN_OSPEED_VERYLOW(GPIOH_PIN7) |       \
                                     PIN_OSPEED_VERYLOW(GPIOH_PIN8) |       \
                                     PIN_OSPEED_VERYLOW(GPIOH_PIN9) |       \
                                     PIN_OSPEED_VERYLOW(GPIOH_PIN10) |      \
                                     PIN_OSPEED_VERYLOW(GPIOH_PIN11) |      \
                                     PIN_OSPEED_VERYLOW(GPIOH_PIN12) |      \
                                     PIN_OSPEED_VERYLOW(GPIOH_PIN13) |      \
                                     PIN_OSPEED_VERYLOW(GPIOH_PIN14) |      \
                                     PIN_OSPEED_VERYLOW(GPIOH_PIN15))
#define VAL_GPIOH_PUPDR             (PIN_PUPDR_FLOATING(GPIOH_OSC_IN) |     \
                                     PIN_PUPDR_FLOATING(GPIOH_OSC_OUT) |    \
                                     PIN_PUPDR_FLOATING(GPIOH_PIN2) |       \
                                     PIN_PUPDR_FLOATING(GPIOH_PIN3) |       \
                                     PIN_PUPDR_FLOATING(GPIOH_PIN4) |       \
                                     PIN_PUPDR_FLOATING(GPIOH_PIN5) |       \
                                     PIN_PUPDR_FLOATING(GPIOH_PIN6) |       \
                                     PIN_PUPDR_FLOATING(GPIOH_PIN7) |       \
                                     PIN_PUPDR_FLOATING(GPIOH_PIN8) |       \
                                     PIN_PUPDR_FLOATING(GPIOH_PIN9) |       \
                                     PIN_PUPDR_FLOATING(GPIOH_PIN10) |      \
                                     PIN_PUPDR_FLOATING(GPIOH_PIN11) |      \
                                     PIN_PUPDR_FLOATING(GPIOH_PIN12) |      \
                                     PIN_PUPDR_FLOATING(GPIOH_PIN13) |      \
                                     PIN_PUPDR_FLOATING(GPIOH_PIN14) |      \
                                     PIN_PUPDR_FLOATING(GPIOH_PIN15))
#define VAL_GPIOH_ODR               (PIN_ODR_HIGH(GPIOH_OSC_IN) |           \
                                     PIN_ODR_HIGH(GPIOH_OSC_OUT) |          \
                                     PIN_ODR_HIGH(GPIOH_PIN2) |             \
                                     PIN_ODR_HIGH(GPIOH_PIN3) |             \
                                     PIN_ODR_HIGH(GPIOH_PIN4) |             \
                                     PIN_ODR_HIGH(GPIOH_PIN5) |             \
                                     PIN_ODR_HIGH(GPIOH_PIN6) |             \
                                     PIN_ODR_HIGH(GPIOH_PIN7) |             \
                                     PIN_ODR_HIGH(GPIOH_PIN8) |             \
                                     PIN_ODR_HIGH(GPIOH_PIN9) |             \
                                     PIN_ODR_HIGH(GPIOH_PIN10) |            \
                                     PIN_ODR_HIGH(GPIOH_PIN11) |            \
                                     PIN_ODR_HIGH(GPIOH_PIN12) |            \
                                     PIN_ODR_HIGH(GPIOH_PIN13) |            \
                                     PIN_ODR_HIGH(GPIOH_PIN14) |            \
                                     PIN_ODR_HIGH(GPIOH_PIN15))
#define VAL_GPIOH_AFRL              (PIN_AFIO_AF(GPIOH_OSC_IN, 0U) |        \
                                     PIN_AFIO_AF(GPIOH_OSC_OUT, 0U) |       \
                                     PIN_AFIO_AF(GPIOH_PIN2, 0U) |          \
                                     PIN_AFIO_AF(GPIOH_PIN3, 0U) |          \
                                     PIN_AFIO_AF(GPIOH_PIN4, 0U) |          \
                                     PIN_AFIO_AF(GPIOH_PIN5, 0U) |          \
                                     PIN_AFIO_AF(GPIOH_PIN6, 0U) |          \
                                     PIN_AFIO_AF(GPIOH_PIN7, 0U))
#define VAL_GPIOH_AFRH              (PIN_AFIO_AF(GPIOH_PIN8, 0U) |          \
                                     PIN_AFIO_AF(GPIOH_PIN9, 0U) |          \
                                     PIN_AFIO_AF(GPIOH_PIN10, 0U) |         \
                                     PIN_AFIO_AF(GPIOH_PIN11, 0U) |         \
                                     PIN_AFIO_AF(GPIOH_PIN12, 0U) |         \
                                     PIN_AFIO_AF(GPIOH_PIN13, 0U) |         \
                                     PIN_AFIO_AF(GPIOH_PIN14, 0U) |         \
                                     PIN_AFIO_AF(GPIOH_PIN15, 0U))
#define VAL_GPIOH_ASCR              (PIN_ASCR_DISABLED(GPIOH_OSC_IN) |      \
                                     PIN_ASCR_DISABLED(GPIOH_OSC_OUT) |     \
                                     PIN_ASCR_DISABLED(GPIOH_PIN2) |        \
                                     PIN_ASCR_DISABLED(GPIOH_PIN3) |        \
                                     PIN_ASCR_DISABLED(GPIOH_PIN4) |        \
                                     PIN_ASCR_DISABLED(GPIOH_PIN5) |        \
                                     PIN_ASCR_DISABLED(GPIOH_PIN6) |        \
                                     PIN_ASCR_DISABLED(GPIOH_PIN7) |        \
                                     PIN_ASCR_DISABLED(GPIOH_PIN8) |        \
                                     PIN_ASCR_DISABLED(GPIOH_PIN9) |        \
                                     PIN_ASCR_DISABLED(GPIOH_PIN10) |       \
                                     PIN_ASCR_DISABLED(GPIOH_PIN11) |       \
                                     PIN_ASCR_DISABLED(GPIOH_PIN12) |       \
                                     PIN_ASCR_DISABLED(GPIOH_PIN13) |       \
                                     PIN_ASCR_DISABLED(GPIOH_PIN14) |       \
                                     PIN_ASCR_DISABLED(GPIOH_PIN15))
#define VAL_GPIOH_LOCKR             (PIN_LOCKR_DISABLED(GPIOH_OSC_IN) |     \
                                     PIN_LOCKR_DISABLED(GPIOH_OSC_OUT) |    \
                                     PIN_LOCKR_DISABLED(GPIOH_PIN2) |       \
                                     PIN_LOCKR_DISABLED(GPIOH_PIN3) |       \
                                     PIN_LOCKR_DISABLED(GPIOH_PIN4) |       \
                                     PIN_LOCKR_DISABLED(GPIOH_PIN5) |       \
                                     PIN_LOCKR_DISABLED(GPIOH_PIN6) |       \
                                     PIN_LOCKR_DISABLED(GPIOH_PIN7) |       \
                                     PIN_LOCKR_DISABLED(GPIOH_PIN8) |       \
                                     PIN_LOCKR_DISABLED(GPIOH_PIN9) |       \
                                     PIN_LOCKR_DISABLED(GPIOH_PIN10) |      \
                                     PIN_LOCKR_DISABLED(GPIOH_PIN11) |      \
                                     PIN_LOCKR_DISABLED(GPIOH_PIN12) |      \
                                     PIN_LOCKR_DISABLED(GPIOH_PIN13) |      \
                                     PIN_LOCKR_DISABLED(GPIOH_PIN14) |      \
                                     PIN_LOCKR_DISABLED(GPIOH_PIN15))

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

#if !defined(_FROM_ASM_)
#ifdef __cplusplus
extern "C" {
#endif
  void boardInit(void);
#ifdef __cplusplus
}
#endif
#endif /* _FROM_ASM_ */

#endif /* BOARD_H */
