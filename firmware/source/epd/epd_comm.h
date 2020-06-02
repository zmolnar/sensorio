/**
 * @file epd_comm.h
 * @brief
 */

#ifndef EPD_COMM_H
#define EPD_COMM_H

/*****************************************************************************/
/* INCLUDES                                                                  */
/*****************************************************************************/
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/*****************************************************************************/
/* DEFINED CONSTANTS                                                         */
/*****************************************************************************/

/*****************************************************************************/
/* MACRO DEFINITIONS                                                         */
/*****************************************************************************/

/*****************************************************************************/
/* TYPE DEFINITIONS                                                          */
/*****************************************************************************/

/*****************************************************************************/
/* DECLARATION OF GLOBAL VARIABLES                                           */
/*****************************************************************************/

/*****************************************************************************/
/* DECLARATION OF GLOBAL FUNCTIONS                                           */
/*****************************************************************************/
void EPD_SpiSelect(void);
void EPD_SpiUnselect(void);
void EPD_SpiSend(const uint8_t obuf[], size_t olen);
void EPD_SpiReceive(uint8_t ibuf[], size_t ilen);
void EPD_MosiLow(void);
void EPD_SclkLow(void);
void EPD_DischargeLow(void);
void EPD_DischargeHigh(void);
void EPD_VccOff(void);
void EPD_VccOn(void);
void EPD_ResetOff(void);
void EPD_ResetOn(void);
void EPD_SleepMilliseconds(uint32_t msec);
bool EPD_IsBusy(void);
void EPD_Abort(const char* msg);

#endif /* EPD_COMM_H */

/****************************** END OF FILE **********************************/
