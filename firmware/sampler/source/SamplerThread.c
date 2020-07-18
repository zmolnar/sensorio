/**
 * @file SamplerThread.c
 * @brief
 */

/*****************************************************************************/
/* INCLUDES                                                                  */
/*****************************************************************************/
#include "SamplerThread.h"

#include "hal.h"
#include "ex.h"

#include "lps22hb.h"

/*****************************************************************************/
/* DEFINED CONSTANTS                                                         */
/*****************************************************************************/

/*****************************************************************************/
/* TYPE DEFINITIONS                                                          */
/*****************************************************************************/

/*****************************************************************************/
/* MACRO DEFINITIONS                                                         */
/*****************************************************************************/

/*****************************************************************************/
/* DEFINITION OF GLOBAL CONSTANTS AND VARIABLES                              */
/*****************************************************************************/
static LPS22HBDriver LPS22HB;

static I2CConfig i2cConfig = 
{
  .timingr = STM32_TIMINGR_PRESC(15U) |
  STM32_TIMINGR_SCLDEL(4U) | STM32_TIMINGR_SDADEL(2U) |
  STM32_TIMINGR_SCLH(15U)  | STM32_TIMINGR_SCLL(21U),
  .cr1 = 0,
  .cr2 = 0,
};

static LPS22HBConfig lps22hbConfig =
{
  .i2cp = &I2CD2,
  .i2ccfg = &i2cConfig,
  .slaveaddress = 0x5D,
  .barosensitivity = NULL,
  .barobias = NULL,
  .thermosensitivity = NULL,
  .thermobias = NULL,
  .outputdatarate = LPS22HB_ODR_75HZ,
};
  
/*****************************************************************************/
/* DECLARATION OF LOCAL FUNCTIONS                                            */
/*****************************************************************************/

/*****************************************************************************/
/* DEFINITION OF LOCAL FUNCTIONS                                             */
/*****************************************************************************/

/*****************************************************************************/
/* DEFINITION OF GLOBAL FUNCTIONS                                            */
/*****************************************************************************/
THD_FUNCTION(SamplerThread, arg) {
  (void)arg;
  chRegSetThreadName("sampler-thread");

  lps22hbObjectInit(&LPS22HB);
  lps22hbStart(&LPS22HB, &lps22hbConfig);

  float axes[3] = {0.0};

  while(true) {
    chThdSleepMilliseconds(1000);
    
    lps22hbBarometerReadCooked(&LPS22HB, axes);
  }
} 

/****************************** END OF FILE **********************************/
