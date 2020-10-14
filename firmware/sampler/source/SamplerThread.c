/**
 * @file SamplerThread.c
 * @brief
 */

/*****************************************************************************/
/* INCLUDES                                                                  */
/*****************************************************************************/
#include "SamplerThread.h"

#include "ch.h"
#include "hal.h"
#include "ex.h"
#include "lps22hb.h"
#include "lsm6dsl.h"

#include "chprintf.h"
#include "memstreams.h"

#include <string.h>

/*****************************************************************************/
/* DEFINED CONSTANTS                                                         */
/*****************************************************************************/

/*****************************************************************************/
/* TYPE DEFINITIONS                                                          */
/*****************************************************************************/
typedef enum {
  DATA_READY_BARO,
  DATA_READY_ACC_GYRO,
} DataReadySource_t;

/*****************************************************************************/
/* MACRO DEFINITIONS                                                         */
/*****************************************************************************/

/*****************************************************************************/
/* DEFINITION OF GLOBAL CONSTANTS AND VARIABLES                              */
/*****************************************************************************/
static msg_t     events[10];
static mailbox_t samplerMailbox;

static LPS22HBDriver LPS22HB;
static LSM6DSLDriver LSM6DSL;

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

static LSM6DSLConfig lsm6dslConfig = {
    .i2cp            = &I2CD2,
    .i2ccfg          = &i2cConfig,
    .slaveaddress    = 0x6A,
    .accsensitivity  = NULL,
    .accbias         = NULL,
    .accfullscale    = LSM6DSL_ACC_FS_4G,
    .accoutdatarate  = LSM6DSL_ACC_ODR_416Hz,
    .gyrosensitivity = NULL,
    .gyrobias        = NULL,
    .gyrofullscale   = LSM6DSL_GYRO_FS_500DPS,
    .gyrooutdatarate = LSM6DSL_GYRO_ODR_416Hz,
};

static SerialConfig serialCfg = 
{
  .speed = 460800,
  .cr1 = 0,
  .cr2 = 0,
  .cr3 = 0,
};

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

static void accGyroDataReady(void *p)
{
  (void)p;

  chSysLockFromISR();
  chMBPostI(&samplerMailbox, DATA_READY_ACC_GYRO);
  chSysUnlockFromISR();
}

void printSWO(const char *fmt, ...)
{
  MemoryStream ms;
  uint8_t      buf[128];
  memset(buf, 0, sizeof(buf));

  msObjectInit(&ms, buf, sizeof(buf), 0);

  va_list ap;
  va_start(ap, fmt);
  size_t n = chvprintf((BaseSequentialStream *)&ms, fmt, ap);
  va_end(ap);

  sdWrite(&SD4, buf, n);

#if 0
  size_t i = 0;
  for (i = 0; buf[i] != '\0'; ++i)
    ITM_SendChar(buf[i]);
#endif    
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
  sdStart(&SD4, &serialCfg);

  chThdSleepMilliseconds(100);

  float    pressure        = 0;
  uint32_t pressureCounter = 0;
  uint32_t accGyroCounter  = 0;

  struct accGyroData_s {
    float acc[3];
    float gyro[3];
  } accGyroData;

  memset(events, 0, sizeof(events));
  chMBObjectInit(&samplerMailbox, events, sizeof(events) / sizeof(events[0]));

  palSetLineCallback(LINE_LPS22HB_INT_DRDY_EXTI10, baroDataReady, NULL);
  palEnableLineEvent(LINE_LPS22HB_INT_DRDY_EXTI10, PAL_EVENT_MODE_RISING_EDGE);

  lps22hbObjectInit(&LPS22HB);
  lps22hbStart(&LPS22HB, &lps22hbConfig);
  lps22hbBarometerReadCooked(&LPS22HB, &pressure);

  palSetLineCallback(LINE_LSM6DSL_INT1_EXTI11, accGyroDataReady, NULL);
  palEnableLineEvent(LINE_LSM6DSL_INT1_EXTI11, PAL_EVENT_MODE_RISING_EDGE);

  lsm6dslObjectInit(&LSM6DSL);
  lsm6dslStart(&LSM6DSL, &lsm6dslConfig);
  lsm6dslGyroscopeReadCooked(&LSM6DSL, accGyroData.gyro);
  lsm6dslAccelerometerReadCooked(&LSM6DSL, accGyroData.acc);

  while (true) {
    msg_t evt;
    msg_t msg = chMBFetchTimeout(&samplerMailbox, &evt, TIME_INFINITE);

    if (MSG_OK == msg) {
      systime_t now = chVTGetSystemTime();

      switch ((DataReadySource_t)evt) {
      case DATA_READY_BARO: {
        lps22hbBarometerReadCooked(&LPS22HB, &pressure);

        pressureCounter++;
        printSWO("%9d %7d %4.4f\r\n", (int)now, (int)pressureCounter, pressure);
        
        break;
      }
      case DATA_READY_ACC_GYRO: {
        lsm6dslGyroscopeReadCooked(&LSM6DSL, accGyroData.gyro);
        lsm6dslAccelerometerReadCooked(&LSM6DSL, accGyroData.acc);

        accGyroCounter++;
        printSWO(
            "%9d %7d "
            "%4.4f %4.4f %4.4f "
            "%4.4f %4.4f %4.4f\r\n",
            (int)now,
            (int)accGyroCounter,
            accGyroData.acc[0],
            accGyroData.acc[1],
            accGyroData.acc[2],
            accGyroData.gyro[0],
            accGyroData.gyro[1],
            accGyroData.gyro[2]);

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
