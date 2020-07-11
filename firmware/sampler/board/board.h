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
 * Setup for STMicroelectronics STM32L475-Discovery for IoT board.
 */

/*
 * Board identifier.
 */
#define BOARD_ST_STM32L4_IOT_NODE
#define BOARD_NAME "STMicroelectronics STM32L4 IoT Node"

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
#define GPIOA_PIN0                  0U
#define GPIOA_PIN1                  1U
#define GPIOA_PIN2                  2U
#define GPIOA_PIN3                  3U
#define GPIOA_PIN4                  4U
#define GPIOA_PIN5                  5U
#define GPIOA_PIN6                  6U
#define GPIOA_PIN7                  7U
#define GPIOA_PIN8                  8U
#define GPIOA_PIN9                  9U
#define GPIOA_PIN10                 10U
#define GPIOA_OTG_FS_DM             11U
#define GPIOA_OTG_FS_DP             12U
#define GPIOA_SWDIO                 13U
#define GPIOA_SWCLK                 14U
#define GPIOA_PIN15                 15U

#define GPIOB_PIN0                  0U
#define GPIOB_PIN1                  1U
#define GPIOB_PIN2                  2U
#define GPIOB_SWO                   3U
#define GPIOB_PIN4                  3U
#define GPIOB_PIN5                  4U
#define GPIOB_PIN6                  5U
#define GPIOB_PIN6                  6U
#define GPIOB_PIN7                  7U
#define GPIOB_PIN8                  8U
#define GPIOB_PIN9                  9U
#define GPIOB_PIN10                 10U
#define GPIOB_PIN11                 11U
#define GPIOB_PIN12                 12U
#define GPIOB_PIN13                 13U
#define GPIOB_PIN14                 14U
#define GPIOB_PIN15                 15U

#define GPIOC_PIN0                  0U
#define GPIOC_PIN1                  1U
#define GPIOC_PIN2                  2U
#define GPIOC_PIN3                  3U
#define GPIOC_PIN4                  4U
#define GPIOC_PIN5                  5U
#define GPIOC_PIN6                  6U
#define GPIOC_PIN7                  7U
#define GPIOC_PIN8                  8U
#define GPIOC_PIN9                  9U
#define GPIOC_PIN10                 10U
#define GPIOC_PIN11                 11U
#define GPIOC_PIN12                 12U
#define GPIOC_PIN13                 13U
#define GPIOC_OSC32_IN              14U
#define GPIOC_OSC32_OUT             15U

#define GPIOD_PIN0                  0U 
#define GPIOD_PIN1                  1U 
#define GPIOD_PIN2                  2U 
#define GPIOD_PIN3                  3U 
#define GPIOD_PIN4                  4U 
#define GPIOD_PIN5                  5U 
#define GPIOD_PIN6                  6U 
#define GPIOD_PIN7                  7U 
#define GPIOD_PIN8                  8U 
#define GPIOD_PIN9                  9U 
#define GPIOD_PIN10                 10U
#define GPIOD_PIN11                 11U
#define GPIOD_PIN12                 12U
#define GPIOD_PIN13                 13U
#define GPIOD_PIN14                 14U
#define GPIOD_PIN15                 15U

#define GPIOE_PIN0                  0U
#define GPIOE_PIN1                  1U
#define GPIOE_PIN2                  2U
#define GPIOE_PIN3                  3U
#define GPIOE_PIN4                  4U
#define GPIOE_PIN5                  5U
#define GPIOE_PIN6                  6U
#define GPIOE_PIN7                  7U
#define GPIOE_PIN8                  8U
#define GPIOE_PIN9                  9U
#define GPIOE_PIN10                 10U
#define GPIOE_PIN11                 11U
#define GPIOE_PIN12                 12U
#define GPIOE_PIN13                 13U
#define GPIOE_PIN14                 14U
#define GPIOE_PIN15                 15U

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
#define LINE_OTG_FS_DM              PAL_LINE(GPIOA, GPIOA_OTG_FS_DM)
#define LINE_OTG_FS_DP              PAL_LINE(GPIOA, GPIOA_OTG_FS_DP)
#define LINE_SWDIO                  PAL_LINE(GPIOA, GPIOA_SWDIO)
#define LINE_SWCLK                  PAL_LINE(GPIOA, GPIOA_SWCLK)
#define LINE_SWO                    PAL_LINE(GPIOB, GPIOB_SWO)
#define LINE_OSC32_IN               PAL_LINE(GPIOC, GPIOC_OSC32_IN)
#define LINE_OSC32_OUT              PAL_LINE(GPIOC, GPIOC_OSC32_OUT)
#define LINE_OSC_IN                 PAL_LINE(GPIOH, GPIOH_OSC_IN)
#define LINE_OSC_OUT                PAL_LINE(GPIOH, GPIOH_OSC_OUT)

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
 * PA13 - SWDIO                     (alternate 0).
 * PA14 - SWCLK                     (alternate 0).
 */
#define VAL_GPIOA_MODER             (PIN_MODE_ANALOG(GPIOA_PIN0) |          \
                                     PIN_MODE_ANALOG(GPIOA_PIN1) |          \
                                     PIN_MODE_ANALOG(GPIOA_PIN2) |          \
                                     PIN_MODE_ANALOG(GPIOA_PIN3) |          \
                                     PIN_MODE_ANALOG(GPIOA_PIN4) |          \
                                     PIN_MODE_ANALOG(GPIOA_PIN5) |          \
                                     PIN_MODE_ANALOG(GPIOA_PIN6) |          \
                                     PIN_MODE_ANALOG(GPIOA_PIN7) |          \
                                     PIN_MODE_ANALOG(GPIOA_PIN8) |          \
                                     PIN_MODE_ANALOG(GPIOA_PIN9) |          \
                                     PIN_MODE_ANALOG(GPIOA_PIN10) |         \
                                     PIN_MODE_ANALOG(GPIOA_OTG_FS_DM) |     \
                                     PIN_MODE_ANALOG(GPIOA_OTG_FS_DP) |     \
                                     PIN_MODE_ALTERNATE(GPIOA_SWDIO) |     \
                                     PIN_MODE_ALTERNATE(GPIOA_SWCLK) |     \
                                     PIN_MODE_ANALOG(GPIOA_PIN15))
#define VAL_GPIOA_OTYPER            (PIN_OTYPE_FLOATING(GPIOA_PIN0) |      \
                                     PIN_OTYPE_FLOATING(GPIOA_PIN1) |      \                       
                                     PIN_OTYPE_FLOATING(GPIOA_PIN2) |      \                      
                                     PIN_OTYPE_FLOATING(GPIOA_PIN3) |      \                         
                                     PIN_OTYPE_FLOATING(GPIOA_PIN4) |      \                     
                                     PIN_OTYPE_FLOATING(GPIOA_PIN5) |      \                       
                                     PIN_OTYPE_FLOATING(GPIOA_PIN6) |      \                      
                                     PIN_OTYPE_FLOATING(GPIOA_PIN7) |      \                       
                                     PIN_OTYPE_FLOATING(GPIOA_PIN8) |      \                       
                                     PIN_OTYPE_FLOATING(GPIOA_PIN9) |      \                       
                                     PIN_OTYPE_FLOATING(GPIOA_PIN10) |     \                        
                                     PIN_OTYPE_FLOATING(GPIOA_OTG_FS_DM) | \                           
                                     PIN_OTYPE_FLOATING(GPIOA_OTG_FS_DP) | \                           
                                     PIN_OTYPE_PUSHPULL(GPIOA_SWDIO) |     \                           
                                     PIN_OTYPE_PUSHPULL(GPIOA_SWCLK) |     \                           
                                     PIN_OTYPE_FLOATING(GPIOA_PIN15)                          
#define VAL_GPIOA_OSPEEDR           (PIN_OSPEED_HIGH(GPIOA_PIN0) |         \              
                                     PIN_OSPEED_HIGH(GPIOA_PIN1) |         \                
                                     PIN_OSPEED_HIGH(GPIOA_PIN2) |         \               
                                     PIN_OSPEED_HIGH(GPIOA_PIN3) |         \                  
                                     PIN_OSPEED_HIGH(GPIOA_PIN4) |         \              
                                     PIN_OSPEED_HIGH(GPIOA_PIN5) |         \                
                                     PIN_OSPEED_HIGH(GPIOA_PIN6) |         \               
                                     PIN_OSPEED_HIGH(GPIOA_PIN7) |         \                
                                     PIN_OSPEED_HIGH(GPIOA_PIN8) |         \                
                                     PIN_OSPEED_HIGH(GPIOA_PIN9) |         \                
                                     PIN_OSPEED_HIGH(GPIOA_PIN10) |        \                 
                                     PIN_OSPEED_HIGH(GPIOA_OTG_FS_DM) |    \                    
                                     PIN_OSPEED_HIGH(GPIOA_OTG_FS_DP) |    \                    
                                     PIN_OSPEED_HIGH(GPIOA_SWDIO) |        \                    
                                     PIN_OSPEED_HIGH(GPIOA_SWCLK) |        \                    
                                     PIN_OSPEED_HIGH(GPIOA_PIN15))
#define VAL_GPIOA_PUPDR             (PIN_PUPDR_FLOATING(GPIOA_PIN0) |      \       
                                     PIN_PUPDR_FLOATING(GPIOA_PIN1) |      \         
                                     PIN_PUPDR_FLOATING(GPIOA_PIN2) |      \        
                                     PIN_PUPDR_FLOATING(GPIOA_PIN3) |      \           
                                     PIN_PUPDR_FLOATING(GPIOA_PIN4) |      \       
                                     PIN_PUPDR_FLOATING(GPIOA_PIN5) |      \         
                                     PIN_PUPDR_FLOATING(GPIOA_PIN6) |      \        
                                     PIN_PUPDR_FLOATING(GPIOA_PIN7) |      \         
                                     PIN_PUPDR_FLOATING(GPIOA_PIN8) |      \         
                                     PIN_PUPDR_FLOATING(GPIOA_PIN9) |      \         
                                     PIN_PUPDR_FLOATING(GPIOA_PIN10) |     \          
                                     PIN_PUPDR_FLOATING(GPIOA_OTG_FS_DM) | \             
                                     PIN_PUPDR_FLOATING(GPIOA_OTG_FS_DP) | \             
                                     PIN_PUPDR_PULLUP(GPIOA_SWDIO) |       \             
                                     PIN_PUPDR_PULLDOWN(GPIOA_SWCLK) |     \             
                                     PIN_PUPDR_FLOATING(GPIOA_PIN15))
#define VAL_GPIOA_ODR               (PIN_ODR_HIGH(GPIOA_PIN0) |       \
                                     PIN_ODR_HIGH(GPIOA_PIN1) |         \
                                     PIN_ODR_HIGH(GPIOA_PIN2) |        \
                                     PIN_ODR_HIGH(GPIOA_PIN3) |           \
                                     PIN_ODR_HIGH(GPIOA_PIN4) |       \
                                     PIN_ODR_HIGH(GPIOA_PIN5) |         \
                                     PIN_ODR_HIGH(GPIOA_PIN6) |        \
                                     PIN_ODR_HIGH(GPIOA_PIN7) |         \
                                     PIN_ODR_HIGH(GPIOA_PIN8) |         \
                                     PIN_ODR_HIGH(GPIOA_PIN9) |         \
                                     PIN_ODR_HIGH(GPIOA_PIN10) |         \
                                     PIN_ODR_HIGH(GPIOA_OTG_FS_DM) |        \
                                     PIN_ODR_HIGH(GPIOA_OTG_FS_DP) |        \
                                     PIN_ODR_HIGH(GPIOA_SWDIO) |            \
                                     PIN_ODR_HIGH(GPIOA_SWCLK) |            \
                                     PIN_ODR_HIGH(GPIOA_PIN15))
#define VAL_GPIOA_AFRL              (PIN_AFIO_AF(GPIOA_PIN0, 0U) |    \
                                     PIN_AFIO_AF(GPIOA_PIN1, 0U) |      \
                                     PIN_AFIO_AF(GPIOA_PIN2, 0U) |     \
                                     PIN_AFIO_AF(GPIOA_PIN3, 0U) |        \
                                     PIN_AFIO_AF(GPIOA_PIN4, 0U) |    \
                                     PIN_AFIO_AF(GPIOA_PIN5, 0U) |      \
                                     PIN_AFIO_AF(GPIOA_PIN6, 0U) |    \
                                     PIN_AFIO_AF(GPIOA_PIN7, 0U))
#define VAL_GPIOA_AFRH              (PIN_AFIO_AF(GPIOA_PIN8, 0U) |     \
                                     PIN_AFIO_AF(GPIOA_PIN9, 0U) |     \
                                     PIN_AFIO_AF(GPIOA_PIN10, 0U) |     \
                                     PIN_AFIO_AF(GPIOA_OTG_FS_DM, 0U) |    \
                                     PIN_AFIO_AF(GPIOA_OTG_FS_DP, 0U) |    \
                                     PIN_AFIO_AF(GPIOA_SWDIO, 0U) |         \
                                     PIN_AFIO_AF(GPIOA_SWCLK, 0U) |         \
                                     PIN_AFIO_AF(GPIOA_PIN15, 0U))
#define VAL_GPIOA_ASCR              (PIN_ASCR_DISABLED(GPIOA_PIN0) |  \
                                     PIN_ASCR_DISABLED(GPIOA_PIN1) |    \
                                     PIN_ASCR_DISABLED(GPIOA_PIN2) |   \
                                     PIN_ASCR_DISABLED(GPIOA_PIN3) |      \
                                     PIN_ASCR_DISABLED(GPIOA_PIN4) |  \
                                     PIN_ASCR_DISABLED(GPIOA_PIN5) |    \
                                     PIN_ASCR_DISABLED(GPIOA_PIN6) |   \
                                     PIN_ASCR_DISABLED(GPIOA_PIN7) |    \
                                     PIN_ASCR_DISABLED(GPIOA_PIN8) |    \
                                     PIN_ASCR_DISABLED(GPIOA_PIN9) |    \
                                     PIN_ASCR_DISABLED(GPIOA_PIN10) |    \
                                     PIN_ASCR_DISABLED(GPIOA_OTG_FS_DM) |   \
                                     PIN_ASCR_DISABLED(GPIOA_OTG_FS_DP) |   \
                                     PIN_ASCR_DISABLED(GPIOA_SWDIO) |       \
                                     PIN_ASCR_DISABLED(GPIOA_SWCLK) |       \
                                     PIN_ASCR_DISABLED(GPIOA_PIN15))
#define VAL_GPIOA_LOCKR             (PIN_LOCKR_DISABLED(GPIOA_PIN0) | \
                                     PIN_LOCKR_DISABLED(GPIOA_PIN1) |   \
                                     PIN_LOCKR_DISABLED(GPIOA_PIN2) |  \
                                     PIN_LOCKR_DISABLED(GPIOA_PIN3) |     \
                                     PIN_LOCKR_DISABLED(GPIOA_PIN4) | \
                                     PIN_LOCKR_DISABLED(GPIOA_PIN5) |   \
                                     PIN_LOCKR_DISABLED(GPIOA_PIN6) |  \
                                     PIN_LOCKR_DISABLED(GPIOA_PIN7) |   \
                                     PIN_LOCKR_DISABLED(GPIOA_PIN8) |   \
                                     PIN_LOCKR_DISABLED(GPIOA_PIN9) |   \
                                     PIN_LOCKR_DISABLED(GPIOA_PIN10) |   \
                                     PIN_LOCKR_DISABLED(GPIOA_OTG_FS_DM) |  \
                                     PIN_LOCKR_DISABLED(GPIOA_OTG_FS_DP) |  \
                                     PIN_LOCKR_DISABLED(GPIOA_SWDIO) |      \
                                     PIN_LOCKR_DISABLED(GPIOA_SWCLK) |      \
                                     PIN_LOCKR_DISABLED(GPIOA_PIN15))

/*
 * GPIOB setup:
 */
#define VAL_GPIOB_MODER             (PIN_MODE_ANALOG(GPIOB_PIN0) |  \
                                     PIN_MODE_ANALOG(GPIOB_PIN1) |   \
                                     PIN_MODE_ANALOG(GPIOB_PIN2) |       \
                                     PIN_MODE_ANALOG(GPIOB_SWO) |           \
                                     PIN_MODE_ANALOG(GPIOB_PIN5) |  \
                                     PIN_MODE_ANALOG(GPIOB_PIN6) |  \
                                     PIN_MODE_ANALOG(GPIOB_PIN6) |   \
                                     PIN_MODE_ANALOG(GPIOB_PIN7) |   \
                                     PIN_MODE_ANALOG(GPIOB_PIN8) |      \
                                     PIN_MODE_ANALOG(GPIOB_PIN9) |   \
                                     PIN_MODE_ANALOG(GPIOB_PIN10) |\
                                     PIN_MODE_ANALOG(GPIOB_PIN11) |\
                                     PIN_MODE_ANALOG(GPIOB_PIN12) |  \
                                     PIN_MODE_ANALOG(GPIOB_PIN13) |   \
                                     PIN_MODE_ANALOG(GPIOB_PIN14) |  \
                                     PIN_MODE_ANALOG(GPIOB_PIN15))
#define VAL_GPIOB_OTYPER            (PIN_OTYPE_FLOATING(GPIOB_PIN0) |  \
                                     PIN_OTYPE_FLOATING(GPIOB_PIN1) |   \
                                     PIN_OTYPE_FLOATING(GPIOB_PIN2) |    \
                                     PIN_OTYPE_FLOATING(GPIOB_SWO) |        \
                                     PIN_OTYPE_FLOATING(GPIOB_PIN5) |  \
                                     PIN_OTYPE_FLOATING(GPIOB_PIN6) |  \
                                     PIN_OTYPE_FLOATING(GPIOB_PIN6) |   \
                                     PIN_OTYPE_FLOATING(GPIOB_PIN7) |   \
                                     PIN_OTYPE_FLOATING(GPIOB_PIN8) |  \
                                     PIN_OTYPE_FLOATING(GPIOB_PIN9) |   \
                                     PIN_OTYPE_FLOATING(GPIOB_PIN10) |\
                                     PIN_OTYPE_FLOATING(GPIOB_PIN11) |\
                                     PIN_OTYPE_FLOATING(GPIOB_PIN12) |  \
                                     PIN_OTYPE_FLOATING(GPIOB_PIN13) |   \
                                     PIN_OTYPE_FLOATING(GPIOB_PIN14) |  \
                                     PIN_OTYPE_FLOATING(GPIOB_PIN15))
#define VAL_GPIOB_OSPEEDR           (PIN_OSPEED_HIGH(GPIOB_PIN0) |     \
                                     PIN_OSPEED_HIGH(GPIOB_PIN1) |      \
                                     PIN_OSPEED_HIGH(GPIOB_PIN2) |       \
                                     PIN_OSPEED_HIGH(GPIOB_SWO) |           \
                                     PIN_OSPEED_HIGH(GPIOB_PIN5) |     \
                                     PIN_OSPEED_HIGH(GPIOB_PIN6) |     \
                                     PIN_OSPEED_HIGH(GPIOB_PIN6) |      \
                                     PIN_OSPEED_HIGH(GPIOB_PIN7) |      \
                                     PIN_OSPEED_HIGH(GPIOB_PIN8) |     \
                                     PIN_OSPEED_HIGH(GPIOB_PIN9) |      \
                                     PIN_OSPEED_HIGH(GPIOB_PIN10) |   \
                                     PIN_OSPEED_HIGH(GPIOB_PIN11) |   \
                                     PIN_OSPEED_HIGH(GPIOB_PIN12) |     \
                                     PIN_OSPEED_HIGH(GPIOB_PIN13) |      \
                                     PIN_OSPEED_HIGH(GPIOB_PIN14) |     \
                                     PIN_OSPEED_HIGH(GPIOB_PIN15))
#define VAL_GPIOB_PUPDR             (PIN_PUPDR_FLOATING(GPIOB_PIN0) |  \
                                     PIN_PUPDR_FLOATING(GPIOB_PIN1) |   \
                                     PIN_PUPDR_FLOATING(GPIOB_PIN2) |    \
                                     PIN_PUPDR_FLOATING(GPIOB_SWO) |        \
                                     PIN_PUPDR_FLOATING(GPIOB_PIN5) |  \
                                     PIN_PUPDR_FLOATING(GPIOB_PIN6) |  \
                                     PIN_PUPDR_FLOATING(GPIOB_PIN6) |   \
                                     PIN_PUPDR_FLOATING(GPIOB_PIN7) |   \
                                     PIN_PUPDR_FLOATING(GPIOB_PIN8) |  \
                                     PIN_PUPDR_FLOATING(GPIOB_PIN9) |   \
                                     PIN_PUPDR_FLOATING(GPIOB_PIN10) |\
                                     PIN_PUPDR_FLOATING(GPIOB_PIN11) |\
                                     PIN_PUPDR_FLOATING(GPIOB_PIN12) |  \
                                     PIN_PUPDR_FLOATING(GPIOB_PIN13) |   \
                                     PIN_PUPDR_FLOATING(GPIOB_PIN14) |  \
                                     PIN_PUPDR_FLOATING(GPIOB_PIN15))
#define VAL_GPIOB_ODR               (PIN_ODR_HIGH(GPIOB_PIN0) |        \
                                     PIN_ODR_HIGH(GPIOB_PIN1) |         \
                                     PIN_ODR_HIGH(GPIOB_PIN2) |           \
                                     PIN_ODR_HIGH(GPIOB_SWO) |              \
                                     PIN_ODR_HIGH(GPIOB_PIN5) |        \
                                     PIN_ODR_HIGH(GPIOB_PIN6) |        \
                                     PIN_ODR_HIGH(GPIOB_PIN6) |         \
                                     PIN_ODR_HIGH(GPIOB_PIN7) |         \
                                     PIN_ODR_HIGH(GPIOB_PIN8) |        \
                                     PIN_ODR_HIGH(GPIOB_PIN9) |         \
                                     PIN_ODR_HIGH(GPIOB_PIN10) |      \
                                     PIN_ODR_HIGH(GPIOB_PIN11) |      \
                                     PIN_ODR_HIGH(GPIOB_PIN12) |        \
                                     PIN_ODR_HIGH(GPIOB_PIN13) |         \
                                     PIN_ODR_HIGH(GPIOB_PIN14) |        \
                                     PIN_ODR_HIGH(GPIOB_PIN15))
#define VAL_GPIOB_AFRL              (PIN_AFIO_AF(GPIOB_PIN0, 0U) |    \
                                     PIN_AFIO_AF(GPIOB_PIN1, 0U) |     \
                                     PIN_AFIO_AF(GPIOB_PIN2, 0U) |       \
                                     PIN_AFIO_AF(GPIOB_SWO, 0U) |           \
                                     PIN_AFIO_AF(GPIOB_PIN5, 0U) |    \
                                     PIN_AFIO_AF(GPIOB_PIN6, 0U) |    \
                                     PIN_AFIO_AF(GPIOB_PIN6, 0U) |      \
                                     PIN_AFIO_AF(GPIOB_PIN7, 0U))
#define VAL_GPIOB_AFRH              (PIN_AFIO_AF(GPIOB_PIN8, 0U) |     \
                                     PIN_AFIO_AF(GPIOB_PIN9, 0U) |     \
                                     PIN_AFIO_AF(GPIOB_PIN10, 0U) |   \
                                     PIN_AFIO_AF(GPIOB_PIN11, 0U) |   \
                                     PIN_AFIO_AF(GPIOB_PIN12, 0U) |    \
                                     PIN_AFIO_AF(GPIOB_PIN13, 0U) |     \
                                     PIN_AFIO_AF(GPIOB_PIN14, 0U) |    \
                                     PIN_AFIO_AF(GPIOB_PIN15, 0U))
#define VAL_GPIOB_ASCR              (PIN_ASCR_DISABLED(GPIOB_PIN0) |   \
                                     PIN_ASCR_DISABLED(GPIOB_PIN1) |    \
                                     PIN_ASCR_DISABLED(GPIOB_PIN2) |     \
                                     PIN_ASCR_ENABLED(GPIOB_SWO) |          \
                                     PIN_ASCR_DISABLED(GPIOB_PIN5) |   \
                                     PIN_ASCR_DISABLED(GPIOB_PIN6) |   \
                                     PIN_ASCR_DISABLED(GPIOB_PIN6) |    \
                                     PIN_ASCR_DISABLED(GPIOB_PIN7) |    \
                                     PIN_ASCR_DISABLED(GPIOB_PIN8) |   \
                                     PIN_ASCR_DISABLED(GPIOB_PIN9) |    \
                                     PIN_ASCR_DISABLED(GPIOB_PIN10) | \
                                     PIN_ASCR_DISABLED(GPIOB_PIN11) | \
                                     PIN_ASCR_DISABLED(GPIOB_PIN12) |   \
                                     PIN_ASCR_DISABLED(GPIOB_PIN13) |    \
                                     PIN_ASCR_DISABLED(GPIOB_PIN14) |   \
                                     PIN_ASCR_DISABLED(GPIOB_PIN15))
#define VAL_GPIOB_LOCKR             (PIN_LOCKR_DISABLED(GPIOB_PIN0) |  \
                                     PIN_LOCKR_DISABLED(GPIOB_PIN1) |   \
                                     PIN_LOCKR_DISABLED(GPIOB_PIN2) |    \
                                     PIN_LOCKR_DISABLED(GPIOB_SWO) |        \
                                     PIN_LOCKR_DISABLED(GPIOB_PIN5) |  \
                                     PIN_LOCKR_DISABLED(GPIOB_PIN6) |  \
                                     PIN_LOCKR_DISABLED(GPIOB_PIN6) |   \
                                     PIN_LOCKR_DISABLED(GPIOB_PIN7) |   \
                                     PIN_LOCKR_DISABLED(GPIOB_PIN8) |  \
                                     PIN_LOCKR_DISABLED(GPIOB_PIN9) |   \
                                     PIN_LOCKR_DISABLED(GPIOB_PIN10) |\
                                     PIN_LOCKR_DISABLED(GPIOB_PIN11) |\
                                     PIN_LOCKR_DISABLED(GPIOB_PIN12) |  \
                                     PIN_LOCKR_DISABLED(GPIOB_PIN13) |   \
                                     PIN_LOCKR_DISABLED(GPIOB_PIN14) |  \
                                     PIN_LOCKR_DISABLED(GPIOB_PIN15))

/*
 * GPIOC setup:
 *
 * PC0  - MAG_CS                    (output pushpull maximum).
 * PC1  - MAG_INT                   (input pulldown).
 * PC2  - MAG_DRDY                  (input pulldown).
 * PC3  - LCD_VLCD                  (alternate 11).
 * PC4  - LCD_SEG22                 (alternate 11).
 * PC5  - LCD_SEG1                  (alternate 11).
 * PC6  - LCD_SEG14                 (alternate 11).
 * PC7  - LCD_SEG9                  (alternate 11).
 * PC8  - LCD_SEG13                 (alternate 11).
 * PC9  - OTG_FS_PSON               (output pushpull maximum).
 * PC10 - OTG_FS_OVCURR             (input floating).
 * PC11 - OTG_FS_VBUS               (input floating).
 * PC12 - OTG_FS_ID                 (alternate 10).
 * PC13 - MFX_IRQ_OUT               (input pulldown).
 * PC14 - OSC32_IN                  (input floating).
 * PC15 - OSC32_OUT                 (input floating).
 */
#define VAL_GPIOC_MODER             (PIN_MODE_ANALOG(GPIOC_PIN0) |        \
                                     PIN_MODE_ANALOG(GPIOC_PIN1) |        \
                                     PIN_MODE_ANALOG(GPIOC_PIN2) |       \
                                     PIN_MODE_ANALOG(GPIOC_PIN3) |   \
                                     PIN_MODE_ANALOG(GPIOC_PIN4) |  \
                                     PIN_MODE_ANALOG(GPIOC_PIN5) |   \
                                     PIN_MODE_ANALOG(GPIOC_PIN6) |  \
                                     PIN_MODE_ANALOG(GPIOC_PIN7) |   \
                                     PIN_MODE_ANALOG(GPIOC_PIN8) |  \
                                     PIN_MODE_ANALOG(GPIOC_PIN9) |   \
                                     PIN_MODE_ANALOG(GPIOC_PIN10) |  \
                                     PIN_MODE_ANALOG(GPIOC_PIN11) |    \
                                     PIN_MODE_ANALOG(GPIOC_PIN12) |  \
                                     PIN_MODE_ANALOG(GPIOC_PIN13) |    \
                                     PIN_MODE_ANALOG(GPIOC_PIN14) |       \
                                     PIN_MODE_ANALOG(GPIOC_PIN15))
#define VAL_GPIOC_OTYPER            (PIN_OTYPE_FLOATING(GPIOC_PIN0) |     \
                                     PIN_OTYPE_FLOATING(GPIOC_PIN1) |    \
                                     PIN_OTYPE_FLOATING(GPIOC_PIN2) |   \
                                     PIN_OTYPE_FLOATING(GPIOC_PIN3) |   \
                                     PIN_OTYPE_FLOATING(GPIOC_PIN4) |  \
                                     PIN_OTYPE_FLOATING(GPIOC_PIN5) |   \
                                     PIN_OTYPE_FLOATING(GPIOC_PIN6) |  \
                                     PIN_OTYPE_FLOATING(GPIOC_PIN7) |   \
                                     PIN_OTYPE_FLOATING(GPIOC_PIN8) |  \
                                     PIN_OTYPE_FLOATING(GPIOC_PIN9) |\
                                     PIN_OTYPE_FLOATING(GPIOC_PIN10) |\
                                     PIN_OTYPE_FLOATING(GPIOC_PIN11) |\
                                     PIN_OTYPE_FLOATING(GPIOC_PIN12) |  \
                                     PIN_OTYPE_FLOATING(GPIOC_PIN13) |\
                                     PIN_OTYPE_FLOATING(GPIOC_PIN14) |   \
                                     PIN_OTYPE_FLOATING(GPIOC_PIN15))
#define VAL_GPIOC_OSPEEDR           (PIN_OSPEED_HIGH(GPIOC_PIN0) |        \
                                     PIN_OSPEED_HIGH(GPIOC_PIN1) |       \
                                     PIN_OSPEED_HIGH(GPIOC_PIN2) |      \
                                     PIN_OSPEED_HIGH(GPIOC_PIN3) |      \
                                     PIN_OSPEED_HIGH(GPIOC_PIN4) |     \
                                     PIN_OSPEED_HIGH(GPIOC_PIN5) |      \
                                     PIN_OSPEED_HIGH(GPIOC_PIN6) |     \
                                     PIN_OSPEED_HIGH(GPIOC_PIN7) |      \
                                     PIN_OSPEED_HIGH(GPIOC_PIN8) |     \
                                     PIN_OSPEED_HIGH(GPIOC_PIN9) |   \
                                     PIN_OSPEED_HIGH(GPIOC_PIN10) | \
                                     PIN_OSPEED_HIGH(GPIOC_PIN11) |   \
                                     PIN_OSPEED_HIGH(GPIOC_PIN12) |     \
                                     PIN_OSPEED_HIGH(GPIOC_PIN13) |   \
                                     PIN_OSPEED_VERYLOW(GPIOC_PIN14) |   \
                                     PIN_OSPEED_VERYLOW(GPIOC_PIN15))
#define VAL_GPIOC_PUPDR             (PIN_PUPDR_PULLUP(GPIOC_PIN0) |       \
                                     PIN_PUPDR_PULLDOWN(GPIOC_PIN1) |    \
                                     PIN_PUPDR_PULLDOWN(GPIOC_PIN2) |   \
                                     PIN_PUPDR_FLOATING(GPIOC_PIN3) |   \
                                     PIN_PUPDR_FLOATING(GPIOC_PIN4) |  \
                                     PIN_PUPDR_FLOATING(GPIOC_PIN5) |   \
                                     PIN_PUPDR_FLOATING(GPIOC_PIN6) |  \
                                     PIN_PUPDR_FLOATING(GPIOC_PIN7) |   \
                                     PIN_PUPDR_FLOATING(GPIOC_PIN8) |  \
                                     PIN_PUPDR_FLOATING(GPIOC_PIN9) |\
                                     PIN_PUPDR_FLOATING(GPIOC_PIN10) |\
                                     PIN_PUPDR_FLOATING(GPIOC_PIN11) |\
                                     PIN_PUPDR_FLOATING(GPIOC_PIN12) |  \
                                     PIN_PUPDR_PULLDOWN(GPIOC_PIN13) |\
                                     PIN_PUPDR_FLOATING(GPIOC_PIN14) |   \
                                     PIN_PUPDR_FLOATING(GPIOC_PIN15))
#define VAL_GPIOC_ODR               (PIN_ODR_HIGH(GPIOC_PIN0) |           \
                                     PIN_ODR_HIGH(GPIOC_PIN1) |          \
                                     PIN_ODR_HIGH(GPIOC_PIN2) |         \
                                     PIN_ODR_HIGH(GPIOC_PIN3) |         \
                                     PIN_ODR_HIGH(GPIOC_PIN4) |        \
                                     PIN_ODR_HIGH(GPIOC_PIN5) |         \
                                     PIN_ODR_HIGH(GPIOC_PIN6) |        \
                                     PIN_ODR_HIGH(GPIOC_PIN7) |         \
                                     PIN_ODR_HIGH(GPIOC_PIN8) |        \
                                     PIN_ODR_HIGH(GPIOC_PIN9) |      \
                                     PIN_ODR_HIGH(GPIOC_PIN10) |    \
                                     PIN_ODR_HIGH(GPIOC_PIN11) |      \
                                     PIN_ODR_HIGH(GPIOC_PIN12) |        \
                                     PIN_ODR_HIGH(GPIOC_PIN13) |      \
                                     PIN_ODR_HIGH(GPIOC_PIN14) |         \
                                     PIN_ODR_HIGH(GPIOC_PIN15))
#define VAL_GPIOC_AFRL              (PIN_AFIO_AF(GPIOC_PIN0, 0U) |        \
                                     PIN_AFIO_AF(GPIOC_PIN1, 0U) |       \
                                     PIN_AFIO_AF(GPIOC_PIN2, 0U) |      \
                                     PIN_AFIO_AF(GPIOC_PIN3, 11U) |     \
                                     PIN_AFIO_AF(GPIOC_PIN4, 11U) |    \
                                     PIN_AFIO_AF(GPIOC_PIN5, 11U) |     \
                                     PIN_AFIO_AF(GPIOC_PIN6, 11U) |    \
                                     PIN_AFIO_AF(GPIOC_PIN7, 11U))
#define VAL_GPIOC_AFRH              (PIN_AFIO_AF(GPIOC_PIN8, 11U) |    \
                                     PIN_AFIO_AF(GPIOC_PIN9, 0U) |   \
                                     PIN_AFIO_AF(GPIOC_PIN10, 0U) | \
                                     PIN_AFIO_AF(GPIOC_PIN11, 0U) |   \
                                     PIN_AFIO_AF(GPIOC_PIN12, 10U) |    \
                                     PIN_AFIO_AF(GPIOC_PIN13, 0U) |   \
                                     PIN_AFIO_AF(GPIOC_PIN14, 0U) |      \
                                     PIN_AFIO_AF(GPIOC_PIN15, 0U))
#define VAL_GPIOC_ASCR              (PIN_ASCR_DISABLED(GPIOC_PIN0) |      \
                                     PIN_ASCR_DISABLED(GPIOC_PIN1) |     \
                                     PIN_ASCR_DISABLED(GPIOC_PIN2) |    \
                                     PIN_ASCR_DISABLED(GPIOC_PIN3) |    \
                                     PIN_ASCR_DISABLED(GPIOC_PIN4) |   \
                                     PIN_ASCR_DISABLED(GPIOC_PIN5) |    \
                                     PIN_ASCR_DISABLED(GPIOC_PIN6) |   \
                                     PIN_ASCR_DISABLED(GPIOC_PIN7) |    \
                                     PIN_ASCR_DISABLED(GPIOC_PIN8) |   \
                                     PIN_ASCR_DISABLED(GPIOC_PIN9) | \
                                     PIN_ASCR_DISABLED(GPIOC_PIN10) |\
                                     PIN_ASCR_DISABLED(GPIOC_PIN11) | \
                                     PIN_ASCR_DISABLED(GPIOC_PIN12) |   \
                                     PIN_ASCR_DISABLED(GPIOC_PIN13) | \
                                     PIN_ASCR_DISABLED(GPIOC_PIN14) |    \
                                     PIN_ASCR_DISABLED(GPIOC_PIN15))
#define VAL_GPIOC_LOCKR             (PIN_LOCKR_DISABLED(GPIOC_PIN0) |     \
                                     PIN_LOCKR_DISABLED(GPIOC_PIN1) |    \
                                     PIN_LOCKR_DISABLED(GPIOC_PIN2) |   \
                                     PIN_LOCKR_DISABLED(GPIOC_PIN3) |   \
                                     PIN_LOCKR_DISABLED(GPIOC_PIN4) |  \
                                     PIN_LOCKR_DISABLED(GPIOC_PIN5) |   \
                                     PIN_LOCKR_DISABLED(GPIOC_PIN6) |  \
                                     PIN_LOCKR_DISABLED(GPIOC_PIN7) |   \
                                     PIN_LOCKR_DISABLED(GPIOC_PIN8) |  \
                                     PIN_LOCKR_DISABLED(GPIOC_PIN9) |\
                                     PIN_LOCKR_DISABLED(GPIOC_PIN10) |\
                                     PIN_LOCKR_DISABLED(GPIOC_PIN11) |\
                                     PIN_LOCKR_DISABLED(GPIOC_PIN12) |  \
                                     PIN_LOCKR_DISABLED(GPIOC_PIN13) |\
                                     PIN_LOCKR_DISABLED(GPIOC_PIN14) |   \
                                     PIN_LOCKR_DISABLED(GPIOC_PIN15))

/*
 * GPIOD setup:
 *
 * PD0  - EXT_RST                   (output pushpull maximum).
 * PD1  - MEMS_SCK                  (alternate 5).
 * PD2  - GYRO_INT1                 (input pulldown).
 * PD3  - MEMS_MISO                 (alternate 5).
 * PD4  - MEMS_MOSI                 (alternate 5).
 * PD5  - USART_TX                  (alternate 7).
 * PD6  - USART_RX                  (alternate 7).
 * PD7  - GYRO_CS                   (output pushpull maximum).
 * PD8  - LCD_SEG18                 (alternate 11).
 * PD9  - LCD_SEG5                  (alternate 11).
 * PD10 - LCD_SEG17                 (alternate 11).
 * PD11 - LCD_SEG6                  (alternate 11).
 * PD12 - LCD_SEG16                 (alternate 11).
 * PD13 - LCD_SEG7                  (alternate 11).
 * PD14 - LCD_SEG15                 (alternate 11).
 * PD15 - LCD_SEG8                  (alternate 11).
 */
#define VAL_GPIOD_MODER             (PIN_MODE_ANALOG(GPIOD_PIN0) |       \
                                     PIN_MODE_ANALOG(GPIOD_PIN1) |   \
                                     PIN_MODE_ANALOG(GPIOD_PIN2) |      \
                                     PIN_MODE_ANALOG(GPIOD_PIN3) |  \
                                     PIN_MODE_ANALOG(GPIOD_PIN4) |  \
                                     PIN_MODE_ANALOG(GPIOD_PIN5) |   \
                                     PIN_MODE_ANALOG(GPIOD_PIN6) |   \
                                     PIN_MODE_ANALOG(GPIOD_PIN7) |       \
                                     PIN_MODE_ANALOG(GPIOD_PIN8) |  \
                                     PIN_MODE_ANALOG(GPIOD_PIN9) |   \
                                     PIN_MODE_ANALOG(GPIOD_PIN10) |  \
                                     PIN_MODE_ANALOG(GPIOD_PIN11) |   \
                                     PIN_MODE_ANALOG(GPIOD_PIN12) |  \
                                     PIN_MODE_ANALOG(GPIOD_PIN13) |   \
                                     PIN_MODE_ANALOG(GPIOD_PIN14) |  \
                                     PIN_MODE_ANALOG(GPIOD_PIN15))
#define VAL_GPIOD_OTYPER            (PIN_OTYPE_FLOATING(GPIOD_PIN0) |    \
                                     PIN_OTYPE_FLOATING(GPIOD_PIN1) |   \
                                     PIN_OTYPE_FLOATING(GPIOD_PIN2) |  \
                                     PIN_OTYPE_FLOATING(GPIOD_PIN3) |  \
                                     PIN_OTYPE_FLOATING(GPIOD_PIN4) |  \
                                     PIN_OTYPE_FLOATING(GPIOD_PIN5) |   \
                                     PIN_OTYPE_FLOATING(GPIOD_PIN6) |   \
                                     PIN_OTYPE_FLOATING(GPIOD_PIN7) |    \
                                     PIN_OTYPE_FLOATING(GPIOD_PIN8) |  \
                                     PIN_OTYPE_FLOATING(GPIOD_PIN9) |   \
                                     PIN_OTYPE_FLOATING(GPIOD_PIN10) |  \
                                     PIN_OTYPE_FLOATING(GPIOD_PIN11) |   \
                                     PIN_OTYPE_FLOATING(GPIOD_PIN12) |  \
                                     PIN_OTYPE_FLOATING(GPIOD_PIN13) |   \
                                     PIN_OTYPE_FLOATING(GPIOD_PIN14) |  \
                                     PIN_OTYPE_FLOATING(GPIOD_PIN15))
#define VAL_GPIOD_OSPEEDR           (PIN_OSPEED_HIGH(GPIOD_PIN0) |       \
                                     PIN_OSPEED_HIGH(GPIOD_PIN1) |      \
                                     PIN_OSPEED_HIGH(GPIOD_PIN2) |     \
                                     PIN_OSPEED_HIGH(GPIOD_PIN3) |     \
                                     PIN_OSPEED_HIGH(GPIOD_PIN4) |     \
                                     PIN_OSPEED_HIGH(GPIOD_PIN5) |      \
                                     PIN_OSPEED_HIGH(GPIOD_PIN6) |      \
                                     PIN_OSPEED_HIGH(GPIOD_PIN7) |       \
                                     PIN_OSPEED_HIGH(GPIOD_PIN8) |     \
                                     PIN_OSPEED_HIGH(GPIOD_PIN9) |      \
                                     PIN_OSPEED_HIGH(GPIOD_PIN10) |     \
                                     PIN_OSPEED_HIGH(GPIOD_PIN11) |      \
                                     PIN_OSPEED_HIGH(GPIOD_PIN12) |     \
                                     PIN_OSPEED_HIGH(GPIOD_PIN13) |      \
                                     PIN_OSPEED_HIGH(GPIOD_PIN14) |     \
                                     PIN_OSPEED_HIGH(GPIOD_PIN15))
#define VAL_GPIOD_PUPDR             (PIN_PUPDR_PULLDOWN(GPIOD_PIN0) |    \
                                     PIN_PUPDR_PULLUP(GPIOD_PIN1) |     \
                                     PIN_PUPDR_PULLDOWN(GPIOD_PIN2) |  \
                                     PIN_PUPDR_PULLUP(GPIOD_PIN3) |    \
                                     PIN_PUPDR_PULLUP(GPIOD_PIN4) |    \
                                     PIN_PUPDR_FLOATING(GPIOD_PIN5) |   \
                                     PIN_PUPDR_FLOATING(GPIOD_PIN6) |   \
                                     PIN_PUPDR_PULLUP(GPIOD_PIN7) |      \
                                     PIN_PUPDR_FLOATING(GPIOD_PIN8) |  \
                                     PIN_PUPDR_FLOATING(GPIOD_PIN9) |   \
                                     PIN_PUPDR_FLOATING(GPIOD_PIN10) |  \
                                     PIN_PUPDR_FLOATING(GPIOD_PIN11) |   \
                                     PIN_PUPDR_FLOATING(GPIOD_PIN12) |  \
                                     PIN_PUPDR_FLOATING(GPIOD_PIN13) |   \
                                     PIN_PUPDR_FLOATING(GPIOD_PIN14) |  \
                                     PIN_PUPDR_FLOATING(GPIOD_PIN15))
#define VAL_GPIOD_ODR               (PIN_ODR_LOW(GPIOD_PIN0) |           \
                                     PIN_ODR_HIGH(GPIOD_PIN1) |         \
                                     PIN_ODR_HIGH(GPIOD_PIN2) |        \
                                     PIN_ODR_HIGH(GPIOD_PIN3) |        \
                                     PIN_ODR_HIGH(GPIOD_PIN4) |        \
                                     PIN_ODR_HIGH(GPIOD_PIN5) |         \
                                     PIN_ODR_HIGH(GPIOD_PIN6) |         \
                                     PIN_ODR_HIGH(GPIOD_PIN7) |          \
                                     PIN_ODR_HIGH(GPIOD_PIN8) |        \
                                     PIN_ODR_HIGH(GPIOD_PIN9) |         \
                                     PIN_ODR_HIGH(GPIOD_PIN10) |        \
                                     PIN_ODR_HIGH(GPIOD_PIN11) |         \
                                     PIN_ODR_HIGH(GPIOD_PIN12) |        \
                                     PIN_ODR_HIGH(GPIOD_PIN13) |         \
                                     PIN_ODR_HIGH(GPIOD_PIN14) |        \
                                     PIN_ODR_HIGH(GPIOD_PIN15))
#define VAL_GPIOD_AFRL              (PIN_AFIO_AF(GPIOD_PIN0, 0U) |       \
                                     PIN_AFIO_AF(GPIOD_PIN1, 5U) |      \
                                     PIN_AFIO_AF(GPIOD_PIN2, 0U) |     \
                                     PIN_AFIO_AF(GPIOD_PIN3, 5U) |     \
                                     PIN_AFIO_AF(GPIOD_PIN4, 5U) |     \
                                     PIN_AFIO_AF(GPIOD_PIN5, 7U) |      \
                                     PIN_AFIO_AF(GPIOD_PIN6, 7U) |      \
                                     PIN_AFIO_AF(GPIOD_PIN7, 0U))
#define VAL_GPIOD_AFRH              (PIN_AFIO_AF(GPIOD_PIN8, 11U) |    \
                                     PIN_AFIO_AF(GPIOD_PIN9, 11U) |     \
                                     PIN_AFIO_AF(GPIOD_PIN10, 11U) |    \
                                     PIN_AFIO_AF(GPIOD_PIN11, 11U) |     \
                                     PIN_AFIO_AF(GPIOD_PIN12, 11U) |    \
                                     PIN_AFIO_AF(GPIOD_PIN13, 11U) |     \
                                     PIN_AFIO_AF(GPIOD_PIN14, 11U) |    \
                                     PIN_AFIO_AF(GPIOD_PIN15, 11U))
#define VAL_GPIOD_ASCR              (PIN_ASCR_DISABLED(GPIOD_PIN0) |     \
                                     PIN_ASCR_DISABLED(GPIOD_PIN1) |    \
                                     PIN_ASCR_DISABLED(GPIOD_PIN2) |   \
                                     PIN_ASCR_DISABLED(GPIOD_PIN3) |   \
                                     PIN_ASCR_DISABLED(GPIOD_PIN4) |   \
                                     PIN_ASCR_DISABLED(GPIOD_PIN5) |    \
                                     PIN_ASCR_DISABLED(GPIOD_PIN6) |    \
                                     PIN_ASCR_DISABLED(GPIOD_PIN7) |     \
                                     PIN_ASCR_DISABLED(GPIOD_PIN8) |   \
                                     PIN_ASCR_DISABLED(GPIOD_PIN9) |    \
                                     PIN_ASCR_DISABLED(GPIOD_PIN10) |   \
                                     PIN_ASCR_DISABLED(GPIOD_PIN11) |    \
                                     PIN_ASCR_DISABLED(GPIOD_PIN12) |   \
                                     PIN_ASCR_DISABLED(GPIOD_PIN13) |    \
                                     PIN_ASCR_DISABLED(GPIOD_PIN14) |   \
                                     PIN_ASCR_DISABLED(GPIOD_PIN15))
#define VAL_GPIOD_LOCKR             (PIN_LOCKR_DISABLED(GPIOD_PIN0) |    \
                                     PIN_LOCKR_DISABLED(GPIOD_PIN1) |   \
                                     PIN_LOCKR_DISABLED(GPIOD_PIN2) |  \
                                     PIN_LOCKR_DISABLED(GPIOD_PIN3) |  \
                                     PIN_LOCKR_DISABLED(GPIOD_PIN4) |  \
                                     PIN_LOCKR_DISABLED(GPIOD_PIN5) |   \
                                     PIN_LOCKR_DISABLED(GPIOD_PIN6) |   \
                                     PIN_LOCKR_DISABLED(GPIOD_PIN7) |    \
                                     PIN_LOCKR_DISABLED(GPIOD_PIN8) |  \
                                     PIN_LOCKR_DISABLED(GPIOD_PIN9) |   \
                                     PIN_LOCKR_DISABLED(GPIOD_PIN10) |  \
                                     PIN_LOCKR_DISABLED(GPIOD_PIN11) |   \
                                     PIN_LOCKR_DISABLED(GPIOD_PIN12) |  \
                                     PIN_LOCKR_DISABLED(GPIOD_PIN13) |   \
                                     PIN_LOCKR_DISABLED(GPIOD_PIN14) |  \
                                     PIN_LOCKR_DISABLED(GPIOD_PIN15))

/*
 * GPIOE setup:
 *
 * PE0  - XL_CS                     (output pushpull maximum).
 * PE1  - XL_INT                    (input pulldown).
 * PE2  - SAI1_MCK                  (alternate 13).
 * PE3  - AUDIO_RST                 (output pushpull maximum).
 * PE4  - SAI1_FS                   (alternate 13).
 * PE5  - SAI1_SCK                  (alternate 13).
 * PE6  - SAI1_SD                   (alternate 13).
 * PE7  - AUDIO_DIN                 (alternate 6).
 * PE8  - LED_GREEN                 (output pushpull maximum).
 * PE9  - AUDIO_CLK                 (alternate 6).
 * PE10 - QSPI_CLK                  (alternate 10).
 * PE11 - QSPI_CS                   (alternate 10).
 * PE12 - QSPI_D0                   (alternate 10).
 * PE13 - QSPI_D1                   (alternate 10).
 * PE14 - QSPI_D2                   (alternate 10).
 * PE15 - QSPI_D3                   (alternate 10).
 */
#define VAL_GPIOE_MODER             (PIN_MODE_ANALOG(GPIOE_PIN0) |         \
                                     PIN_MODE_ANALOG(GPIOE_PIN1) |         \
                                     PIN_MODE_ANALOG(GPIOE_PIN2) |   \
                                     PIN_MODE_ANALOG(GPIOE_PIN3) |     \
                                     PIN_MODE_ANALOG(GPIOE_PIN4) |    \
                                     PIN_MODE_ANALOG(GPIOE_PIN5) |   \
                                     PIN_MODE_ANALOG(GPIOE_PIN6) |    \
                                     PIN_MODE_ANALOG(GPIOE_PIN7) |  \
                                     PIN_MODE_ANALOG(GPIOE_PIN8) |     \
                                     PIN_MODE_ANALOG(GPIOE_PIN9) |  \
                                     PIN_MODE_ANALOG(GPIOE_PIN10) |   \
                                     PIN_MODE_ANALOG(GPIOE_PIN11) |    \
                                     PIN_MODE_ANALOG(GPIOE_PIN12) |    \
                                     PIN_MODE_ANALOG(GPIOE_PIN13) |    \
                                     PIN_MODE_ANALOG(GPIOE_PIN14) |    \
                                     PIN_MODE_ANALOG(GPIOE_PIN15))
#define VAL_GPIOE_OTYPER            (PIN_OTYPE_FLOATING(GPIOE_PIN0) |      \
                                     PIN_OTYPE_FLOATING(GPIOE_PIN1) |     \
                                     PIN_OTYPE_FLOATING(GPIOE_PIN2) |   \
                                     PIN_OTYPE_FLOATING(GPIOE_PIN3) |  \
                                     PIN_OTYPE_FLOATING(GPIOE_PIN4) |    \
                                     PIN_OTYPE_FLOATING(GPIOE_PIN5) |   \
                                     PIN_OTYPE_FLOATING(GPIOE_PIN6) |    \
                                     PIN_OTYPE_FLOATING(GPIOE_PIN7) |  \
                                     PIN_OTYPE_FLOATING(GPIOE_PIN8) |  \
                                     PIN_OTYPE_FLOATING(GPIOE_PIN9) |  \
                                     PIN_OTYPE_FLOATING(GPIOE_PIN10) |   \
                                     PIN_OTYPE_FLOATING(GPIOE_PIN11) |    \
                                     PIN_OTYPE_FLOATING(GPIOE_PIN12) |    \
                                     PIN_OTYPE_FLOATING(GPIOE_PIN13) |    \
                                     PIN_OTYPE_FLOATING(GPIOE_PIN14) |    \
                                     PIN_OTYPE_FLOATING(GPIOE_PIN15))
#define VAL_GPIOE_OSPEEDR           (PIN_OSPEED_HIGH(GPIOE_PIN0) |         \
                                     PIN_OSPEED_HIGH(GPIOE_PIN1) |        \
                                     PIN_OSPEED_HIGH(GPIOE_PIN2) |      \
                                     PIN_OSPEED_HIGH(GPIOE_PIN3) |     \
                                     PIN_OSPEED_HIGH(GPIOE_PIN4) |       \
                                     PIN_OSPEED_HIGH(GPIOE_PIN5) |      \
                                     PIN_OSPEED_HIGH(GPIOE_PIN6) |       \
                                     PIN_OSPEED_HIGH(GPIOE_PIN7) |     \
                                     PIN_OSPEED_HIGH(GPIOE_PIN8) |     \
                                     PIN_OSPEED_HIGH(GPIOE_PIN9) |     \
                                     PIN_OSPEED_HIGH(GPIOE_PIN10) |      \
                                     PIN_OSPEED_HIGH(GPIOE_PIN11) |       \
                                     PIN_OSPEED_HIGH(GPIOE_PIN12) |       \
                                     PIN_OSPEED_HIGH(GPIOE_PIN13) |       \
                                     PIN_OSPEED_HIGH(GPIOE_PIN14) |       \
                                     PIN_OSPEED_HIGH(GPIOE_PIN15))
#define VAL_GPIOE_PUPDR             (PIN_PUPDR_PULLUP(GPIOE_PIN0) |        \
                                     PIN_PUPDR_PULLDOWN(GPIOE_PIN1) |     \
                                     PIN_PUPDR_FLOATING(GPIOE_PIN2) |   \
                                     PIN_PUPDR_FLOATING(GPIOE_PIN3) |  \
                                     PIN_PUPDR_FLOATING(GPIOE_PIN4) |    \
                                     PIN_PUPDR_FLOATING(GPIOE_PIN5) |   \
                                     PIN_PUPDR_FLOATING(GPIOE_PIN6) |    \
                                     PIN_PUPDR_FLOATING(GPIOE_PIN7) |  \
                                     PIN_PUPDR_FLOATING(GPIOE_PIN8) |  \
                                     PIN_PUPDR_FLOATING(GPIOE_PIN9) |  \
                                     PIN_PUPDR_PULLUP(GPIOE_PIN10) |     \
                                     PIN_PUPDR_PULLUP(GPIOE_PIN11) |      \
                                     PIN_PUPDR_PULLUP(GPIOE_PIN12) |      \
                                     PIN_PUPDR_PULLUP(GPIOE_PIN13) |      \
                                     PIN_PUPDR_PULLUP(GPIOE_PIN14) |      \
                                     PIN_PUPDR_PULLUP(GPIOE_PIN15))
#define VAL_GPIOE_ODR               (PIN_ODR_HIGH(GPIOE_PIN0) |            \
                                     PIN_ODR_HIGH(GPIOE_PIN1) |           \
                                     PIN_ODR_HIGH(GPIOE_PIN2) |         \
                                     PIN_ODR_HIGH(GPIOE_PIN3) |        \
                                     PIN_ODR_HIGH(GPIOE_PIN4) |          \
                                     PIN_ODR_HIGH(GPIOE_PIN5) |         \
                                     PIN_ODR_HIGH(GPIOE_PIN6) |          \
                                     PIN_ODR_HIGH(GPIOE_PIN7) |        \
                                     PIN_ODR_LOW(GPIOE_PIN8) |         \
                                     PIN_ODR_HIGH(GPIOE_PIN9) |        \
                                     PIN_ODR_HIGH(GPIOE_PIN10) |         \
                                     PIN_ODR_HIGH(GPIOE_PIN11) |          \
                                     PIN_ODR_HIGH(GPIOE_PIN12) |          \
                                     PIN_ODR_HIGH(GPIOE_PIN13) |          \
                                     PIN_ODR_HIGH(GPIOE_PIN14) |          \
                                     PIN_ODR_HIGH(GPIOE_PIN15))
#define VAL_GPIOE_AFRL              (PIN_AFIO_AF(GPIOE_PIN0, 0U) |         \
                                     PIN_AFIO_AF(GPIOE_PIN1, 0U) |        \
                                     PIN_AFIO_AF(GPIOE_PIN2, 13U) |     \
                                     PIN_AFIO_AF(GPIOE_PIN3, 0U) |     \
                                     PIN_AFIO_AF(GPIOE_PIN4, 13U) |      \
                                     PIN_AFIO_AF(GPIOE_PIN5, 13U) |     \
                                     PIN_AFIO_AF(GPIOE_PIN6, 13U) |      \
                                     PIN_AFIO_AF(GPIOE_PIN7, 6U))
#define VAL_GPIOE_AFRH              (PIN_AFIO_AF(GPIOE_PIN8, 0U) |     \
                                     PIN_AFIO_AF(GPIOE_PIN9, 6U) |     \
                                     PIN_AFIO_AF(GPIOE_PIN10, 10U) |     \
                                     PIN_AFIO_AF(GPIOE_PIN11, 10U) |      \
                                     PIN_AFIO_AF(GPIOE_PIN12, 10U) |      \
                                     PIN_AFIO_AF(GPIOE_PIN13, 10U) |      \
                                     PIN_AFIO_AF(GPIOE_PIN14, 10U) |      \
                                     PIN_AFIO_AF(GPIOE_PIN15, 10U))
#define VAL_GPIOE_ASCR              (PIN_ASCR_DISABLED(GPIOE_PIN0) |       \
                                     PIN_ASCR_DISABLED(GPIOE_PIN1) |      \
                                     PIN_ASCR_DISABLED(GPIOE_PIN2) |    \
                                     PIN_ASCR_DISABLED(GPIOE_PIN3) |   \
                                     PIN_ASCR_DISABLED(GPIOE_PIN4) |     \
                                     PIN_ASCR_DISABLED(GPIOE_PIN5) |    \
                                     PIN_ASCR_DISABLED(GPIOE_PIN6) |     \
                                     PIN_ASCR_DISABLED(GPIOE_PIN7) |   \
                                     PIN_ASCR_DISABLED(GPIOE_PIN8) |   \
                                     PIN_ASCR_DISABLED(GPIOE_PIN9) |   \
                                     PIN_ASCR_DISABLED(GPIOE_PIN10) |    \
                                     PIN_ASCR_DISABLED(GPIOE_PIN11) |     \
                                     PIN_ASCR_DISABLED(GPIOE_PIN12) |     \
                                     PIN_ASCR_DISABLED(GPIOE_PIN13) |     \
                                     PIN_ASCR_DISABLED(GPIOE_PIN14) |     \
                                     PIN_ASCR_DISABLED(GPIOE_PIN15))
#define VAL_GPIOE_LOCKR             (PIN_LOCKR_DISABLED(GPIOE_PIN0) |      \
                                     PIN_LOCKR_DISABLED(GPIOE_PIN1) |     \
                                     PIN_LOCKR_DISABLED(GPIOE_PIN2) |   \
                                     PIN_LOCKR_DISABLED(GPIOE_PIN3) |  \
                                     PIN_LOCKR_DISABLED(GPIOE_PIN4) |    \
                                     PIN_LOCKR_DISABLED(GPIOE_PIN5) |   \
                                     PIN_LOCKR_DISABLED(GPIOE_PIN6) |    \
                                     PIN_LOCKR_DISABLED(GPIOE_PIN7) |  \
                                     PIN_LOCKR_DISABLED(GPIOE_PIN8) |  \
                                     PIN_LOCKR_DISABLED(GPIOE_PIN9) |  \
                                     PIN_LOCKR_DISABLED(GPIOE_PIN10) |   \
                                     PIN_LOCKR_DISABLED(GPIOE_PIN11) |    \
                                     PIN_LOCKR_DISABLED(GPIOE_PIN12) |    \
                                     PIN_LOCKR_DISABLED(GPIOE_PIN13) |    \
                                     PIN_LOCKR_DISABLED(GPIOE_PIN14) |    \
                                     PIN_LOCKR_DISABLED(GPIOE_PIN15))

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
#define VAL_GPIOF_OTYPER            (PIN_OTYPE_FLOATING(GPIOF_PIN0) |       \
                                     PIN_OTYPE_FLOATING(GPIOF_PIN1) |       \
                                     PIN_OTYPE_FLOATING(GPIOF_PIN2) |       \
                                     PIN_OTYPE_FLOATING(GPIOF_PIN3) |       \
                                     PIN_OTYPE_FLOATING(GPIOF_PIN4) |       \
                                     PIN_OTYPE_FLOATING(GPIOF_PIN5) |       \
                                     PIN_OTYPE_FLOATING(GPIOF_PIN6) |       \
                                     PIN_OTYPE_FLOATING(GPIOF_PIN7) |       \
                                     PIN_OTYPE_FLOATING(GPIOF_PIN8) |       \
                                     PIN_OTYPE_FLOATING(GPIOF_PIN9) |       \
                                     PIN_OTYPE_FLOATING(GPIOF_PIN10) |      \
                                     PIN_OTYPE_FLOATING(GPIOF_PIN11) |      \
                                     PIN_OTYPE_FLOATING(GPIOF_PIN12) |      \
                                     PIN_OTYPE_FLOATING(GPIOF_PIN13) |      \
                                     PIN_OTYPE_FLOATING(GPIOF_PIN14) |      \
                                     PIN_OTYPE_FLOATING(GPIOF_PIN15))
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
#define VAL_GPIOG_OTYPER            (PIN_OTYPE_FLOATING(GPIOG_PIN0) |       \
                                     PIN_OTYPE_FLOATING(GPIOG_PIN1) |       \
                                     PIN_OTYPE_FLOATING(GPIOG_PIN2) |       \
                                     PIN_OTYPE_FLOATING(GPIOG_PIN3) |       \
                                     PIN_OTYPE_FLOATING(GPIOG_PIN4) |       \
                                     PIN_OTYPE_FLOATING(GPIOG_PIN5) |       \
                                     PIN_OTYPE_FLOATING(GPIOG_PIN6) |       \
                                     PIN_OTYPE_FLOATING(GPIOG_PIN7) |       \
                                     PIN_OTYPE_FLOATING(GPIOG_PIN8) |       \
                                     PIN_OTYPE_FLOATING(GPIOG_PIN9) |       \
                                     PIN_OTYPE_FLOATING(GPIOG_PIN10) |      \
                                     PIN_OTYPE_FLOATING(GPIOG_PIN11) |      \
                                     PIN_OTYPE_FLOATING(GPIOG_PIN12) |      \
                                     PIN_OTYPE_FLOATING(GPIOG_PIN13) |      \
                                     PIN_OTYPE_FLOATING(GPIOG_PIN14) |      \
                                     PIN_OTYPE_FLOATING(GPIOG_PIN15))
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
#define VAL_GPIOH_OTYPER            (PIN_OTYPE_FLOATING(GPIOH_OSC_IN) |     \
                                     PIN_OTYPE_FLOATING(GPIOH_OSC_OUT) |    \
                                     PIN_OTYPE_FLOATING(GPIOH_PIN2) |       \
                                     PIN_OTYPE_FLOATING(GPIOH_PIN3) |       \
                                     PIN_OTYPE_FLOATING(GPIOH_PIN4) |       \
                                     PIN_OTYPE_FLOATING(GPIOH_PIN5) |       \
                                     PIN_OTYPE_FLOATING(GPIOH_PIN6) |       \
                                     PIN_OTYPE_FLOATING(GPIOH_PIN7) |       \
                                     PIN_OTYPE_FLOATING(GPIOH_PIN8) |       \
                                     PIN_OTYPE_FLOATING(GPIOH_PIN9) |       \
                                     PIN_OTYPE_FLOATING(GPIOH_PIN10) |      \
                                     PIN_OTYPE_FLOATING(GPIOH_PIN11) |      \
                                     PIN_OTYPE_FLOATING(GPIOH_PIN12) |      \
                                     PIN_OTYPE_FLOATING(GPIOH_PIN13) |      \
                                     PIN_OTYPE_FLOATING(GPIOH_PIN14) |      \
                                     PIN_OTYPE_FLOATING(GPIOH_PIN15))
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
