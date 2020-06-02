/**
 * @file epd_1_9.h
 * @brief
 */

#ifndef EPD_1_9_H
#define EPD_1_9_H

/*****************************************************************************/
/* INCLUDES                                                                  */
/*****************************************************************************/
#include "epd.h"

#include <stdint.h>
#include <stddef.h>

/*****************************************************************************/
/* DEFINED CONSTANTS                                                         */
/*****************************************************************************/

/*****************************************************************************/
/* MACRO DEFINITIONS                                                         */
/*****************************************************************************/

/*****************************************************************************/
/* TYPE DEFINITIONS                                                          */
/*****************************************************************************/
typedef enum {
  EPD_PIXEL_NOTHING = 0x00,
  EPD_PIXEL_WHITE   = 0x02,
  EPD_PIXEL_BLACK   = 0x03,
} EPD_PixelColor_t;

typedef struct EPD_Line_s {
  union {
    struct line_s {
      uint8_t scan_even[16];
      uint8_t data[36];
      uint8_t scan_odd[16];
      uint8_t border;
    } line;
    struct {
      uint8_t data[sizeof(struct line_s)];
    } bytes;
  };
} EPD_Line_t;

/*****************************************************************************/
/* DECLARATION OF GLOBAL VARIABLES                                           */
/*****************************************************************************/

/*****************************************************************************/
/* DECLARATION OF GLOBAL FUNCTIONS                                           */
/*****************************************************************************/
void EPD_setScanByte(EPD_Line_t *pline, size_t i);
void EPD_CreateDataByte(uint8_t byte, EPD_Stage_t stage, uint8_t *pd1, uint8_t *pd2);
void EPD_WriteFrame(const uint8_t image[128][144 / 8], EPD_Stage_t stage);
void EPD_WritePartialUpdateFrame(const uint8_t old[128][144 / 8], const uint8_t new[128][144 / 8]);
void EPD_WriteNothingFrame(void);
void EPD_WriteBorderDummyLine(void);

#endif /* EPD_1_9_H */

/****************************** END OF FILE **********************************/
