/**
 * @file epd_comm.c
 * @brief
 */

/*****************************************************************************/
/* INCLUDES                                                                  */
/*****************************************************************************/
#include "ch.h"
#include "hal.h"

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

/*****************************************************************************/
/* DECLARATION OF LOCAL FUNCTIONS                                            */
/*****************************************************************************/

/*****************************************************************************/
/* DEFINITION OF LOCAL FUNCTIONS                                             */
/*****************************************************************************/

/*****************************************************************************/
/* DEFINITION OF GLOBAL FUNCTIONS                                            */
/*****************************************************************************/
void EPD_SpiSelect(void)
{
  spiSelect(&SPID2);
}

void EPD_SpiUnselect(void)
{
  spiUnselect(&SPID2);
}

void EPD_SpiSend(const uint8_t obuf[], size_t olen)
{
  spiSend(&SPID2, olen, obuf);
}

void EPD_SpiReceive(uint8_t ibuf[], size_t ilen)
{
  spiReceive(&SPID2, ilen, ibuf);
}

void EPD_MosiLow(void)
{
  palSetPadMode(GPIOB, GPIOB_SPI2_MOSI, PAL_MODE_OUTPUT_PUSHPULL);
  palClearPad(GPIOB, GPIOB_SPI2_MOSI);
}

void EPD_SclkLow(void)
{
  palSetPadMode(GPIOB, GPIOB_SPI2_SCK, PAL_MODE_OUTPUT_PUSHPULL);
  palClearPad(GPIOB, GPIOB_SPI2_SCK);
}

void EPD_DischargeLow(void)
{
  palClearPad(GPIOB, GPIOB_EPD_DISCHARGE);
}

void EPD_DischargeHigh(void)
{
  palSetPad(GPIOB, GPIOB_EPD_DISCHARGE);
}

void EPD_VccOff(void)
{
  palSetPad(GPIOB, GPIOB_EPD_VCC_EN);
}

void EPD_VccOn(void)
{
  palClearPad(GPIOB, GPIOB_EPD_VCC_EN);
}

void EPD_ResetOff(void)
{
  palSetPad(GPIOA, GPIOA_EPD_RESET);
}

void EPD_ResetOn(void)
{
  palClearPad(GPIOA, GPIOA_EPD_RESET);
}

void EPD_SleepMilliseconds(uint32_t msec)
{
  chThdSleepMilliseconds(msec);
}

bool EPD_IsBusy(void)
{
  return PAL_HIGH == palReadPad(GPIOA, GPIOA_EPD_BUSY);
}

void EPD_Abort(const char* msg)
{
  chSysHalt(msg);
}

/****************************** END OF FILE **********************************/
