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

static SerialConfig serialCfg = {
    .speed = 921600,
    .cr1   = 0,
    .cr2   = 0,
    .cr3   = 0,
};

virtual_timer_t baroTimer;
virtual_timer_t accGyroTimer;

/*****************************************************************************/
/* DECLARATION OF LOCAL FUNCTIONS                                            */
/*****************************************************************************/
void baroTimeoutCb(void *p)
{
  (void)p;

  chSysHalt("baro sensor timeout");
}

void accGyroTimeoutCb(void *p)
{
  (void)p;

  chSysHalt("acc/gyro sensor timeout");
}

static void baroDataReady(void *p)
{
  (void)p;

  chSysLockFromISR();
  chMBPostI(&samplerMailbox, DATA_READY_BARO);
  chVTSetI(&baroTimer, TIME_MS2I(15), baroTimeoutCb, NULL);
  chSysUnlockFromISR();
}

static void accGyroDataReady(void *p)
{
  (void)p;

  chSysLockFromISR();
  chMBPostI(&samplerMailbox, DATA_READY_ACC_GYRO);
  chVTSetI(&accGyroTimer, TIME_MS2I(5), accGyroTimeoutCb, NULL);
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
  sdStart(&SD4, &serialCfg);

  chThdSleepMilliseconds(100);

  memset(events, 0, sizeof(events));
  chMBObjectInit(&samplerMailbox, events, sizeof(events) / sizeof(events[0]));

  chVTObjectInit(&baroTimer);
  chVTObjectInit(&accGyroTimer);

  float pressure;
  float gyro[3];
  float acc[3];

  palSetLineCallback(LINE_LPS22HB_INT_DRDY_EXTI10, baroDataReady, NULL);
  palEnableLineEvent(LINE_LPS22HB_INT_DRDY_EXTI10, PAL_EVENT_MODE_RISING_EDGE);

  lps22hbObjectInit(&LPS22HB);
  lps22hbStart(&LPS22HB, &lps22hbConfig);
  lps22hbBarometerReadCooked(&LPS22HB, &pressure);

  palSetLineCallback(LINE_LSM6DSL_INT1_EXTI11, accGyroDataReady, NULL);
  palEnableLineEvent(LINE_LSM6DSL_INT1_EXTI11, PAL_EVENT_MODE_RISING_EDGE);

  lsm6dslObjectInit(&LSM6DSL);
  lsm6dslStart(&LSM6DSL, &lsm6dslConfig);
  lsm6dslGyroscopeReadCooked(&LSM6DSL, gyro);

  lsm6dslAccelerometerReadCooked(&LSM6DSL, acc);

  BaseSequentialStream *ostream = (BaseSequentialStream*)&SD4;

  while (true) {
    msg_t evt;
    msg_t msg = chMBFetchTimeout(&samplerMailbox, &evt, TIME_INFINITE);

    if (MSG_OK == msg) {

      switch ((DataReadySource_t)evt) {
      case DATA_READY_BARO: {
        lps22hbBarometerReadCooked(&LPS22HB, &pressure);
        chprintf(ostream, "1 %4.4f\n", pressure);

        break;
      }
      case DATA_READY_ACC_GYRO: {
        lsm6dslGyroscopeReadCooked(&LSM6DSL, gyro);
        lsm6dslAccelerometerReadCooked(&LSM6DSL, acc);
        chprintf(ostream, "2 %4.4f %4.4f %4.4f %4.4f %4.4f %4.4f\n",
                 acc[0], acc[1],acc[2], gyro[0], gyro[1], gyro[2]);

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
