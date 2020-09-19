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

#include "chprintf.h"
#include "memstreams.h"
#include <string.h>

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
typedef enum {
  DATA_READY_BARO,
} DataReadySource_t;

static msg_t     events[10];
static mailbox_t samplerMailbox;

static LPS22HBDriver LPS22HB;

static I2CConfig i2cConfig = {
    .timingr = STM32_TIMINGR_PRESC(15U) | STM32_TIMINGR_SCLDEL(4U) | STM32_TIMINGR_SDADEL(2U) |
               STM32_TIMINGR_SCLH(15U) | STM32_TIMINGR_SCLL(21U),
    .cr1 = 0,
    .cr2 = 0,
};

static LPS22HBConfig lps22hbConfig = {
    .i2cp              = &I2CD2,
    .i2ccfg            = &i2cConfig,
    .slaveaddress      = 0x5D,
    .barosensitivity   = NULL,
    .barobias          = NULL,
    .thermosensitivity = NULL,
    .thermobias        = NULL,
    .outputdatarate    = LPS22HB_ODR_75HZ,
};

void printSWO(const char *fmt, ...)
{
  MemoryStream ms;
  uint8_t buf[128];
  memset(buf, 0, sizeof(buf));

  msObjectInit(&ms, buf, sizeof(buf), 0);

  va_list ap;
  va_start(ap, fmt);
  chvprintf((BaseSequentialStream *)&ms, fmt, ap);
  va_end(ap);

  size_t i = 0;
  for (i = 0; buf[i] != '\0'; ++i)
    ITM_SendChar(buf[i]);
}

/*****************************************************************************/
/* DECLARATION OF LOCAL FUNCTIONS                                            */
/*****************************************************************************/
static void baroDataReady(void *p)
{
  (void)p;

  chSysLockFromISR();
  chMBPostI(&samplerMailbox, DATA_READY_BARO);
  chSysUnlockFromISR();
}

/*****************************************************************************/
/* DEFINITION OF LOCAL FUNCTIONS                                             */
/*****************************************************************************/

/*****************************************************************************/
/* DEFINITION OF GLOBAL FUNCTIONS                                            */
/*****************************************************************************/
THD_FUNCTION(SamplerThread, arg)
{
  (void)arg;
  chRegSetThreadName("sampler-thread");

  float pressure = 0;
  uint32_t pressureCounter = 0;

  memset(events, 0, sizeof(events));
  chMBObjectInit(&samplerMailbox, events, sizeof(events) / sizeof(events[0]));

  palSetLineCallback(LINE_LPS22HB_INT_DRDY_EXTI10, baroDataReady, NULL);
  palEnableLineEvent(LINE_LPS22HB_INT_DRDY_EXTI10, PAL_EVENT_MODE_RISING_EDGE);

  lps22hbObjectInit(&LPS22HB);
  lps22hbStart(&LPS22HB, &lps22hbConfig);
  lps22hbBarometerReadCooked(&LPS22HB, &pressure);

  while (true) {
    msg_t evt;
    msg_t msg = chMBFetchTimeout(&samplerMailbox, &evt, TIME_INFINITE);

    if (MSG_OK == msg) {
      switch ((DataReadySource_t)evt) {
      case DATA_READY_BARO: {
        systime_t now = chVTGetSystemTime();
        lps22hbBarometerReadCooked(&LPS22HB, &pressure);
        printSWO("%9d %7d %4.4f\n", (int)now, (int)pressureCounter, pressure);

        pressureCounter++;
        break;
      }
      default: {
        break;
      }
      }
    }
  }
}

/****************************** END OF FILE **********************************/
