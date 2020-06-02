/**
 * @file epd_frame.c
 * @brief
 */

/*****************************************************************************/
/* INCLUDES                                                                  */
/*****************************************************************************/
#include "epd_1_9.h"
#include "epd.h"

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

/*****************************************************************************/
/* DECLARATION OF LOCAL FUNCTIONS                                            */
/*****************************************************************************/

/*****************************************************************************/
/* DEFINITION OF LOCAL FUNCTIONS                                             */
/*****************************************************************************/

/*****************************************************************************/
/* DEFINITION OF GLOBAL FUNCTIONS                                            */
/*****************************************************************************/
void EPD_setScanByte(EPD_Line_t *pline, size_t i)
{
  size_t j = i + 1;

  if (j % 2) {
    size_t shift               = 127 - j;
    size_t byte                = shift / 8;
    pline->line.scan_odd[byte] = 0xC0 >> (shift % 8);
  } else {
    size_t shift                = j - 2;
    size_t byte                 = shift / 8;
    pline->line.scan_even[byte] = 0xC0 >> (shift % 8);
  }
}

void EPD_CreateDataBytes(uint8_t byte, EPD_Stage_t stage, uint8_t *pd1, uint8_t *pd2)
{
  size_t i = 0;
  for (i = 0; i < 8; ++i) {
    uint8_t mask = 0x01 << i;

    EPD_PixelColor_t pixel;

    switch (stage) {
    case EPD_STAGE_1: {
      // Write the inverse
      pixel = (byte & mask) ? EPD_PIXEL_WHITE : EPD_PIXEL_BLACK;
      break;
    }
    case EPD_STAGE_2: {
      // Write white
      pixel = (byte & mask) ? EPD_PIXEL_NOTHING : EPD_PIXEL_WHITE;
      break;
    }
    case EPD_STAGE_3: {
      // Write the inverse
      pixel = (byte & mask) ? EPD_PIXEL_NOTHING : EPD_PIXEL_BLACK;
      break;
    }
    case EPD_STAGE_4: {
      // Write image
      pixel = (byte & mask) ? EPD_PIXEL_BLACK : EPD_PIXEL_WHITE;
      break;
    }
    default: {
      break;
    }
    }

    if (i < 4)
      *pd1 |= ((uint8_t)pixel) << (((4 - i) * 2) - 2);
    else
      *pd2 |= ((uint8_t)pixel) << (((8 - i) * 2) - 2);
  }
}

#include "ch.h"

void EPD_WriteFrame(const uint8_t image[128][144 / 8], EPD_Stage_t stage)
{
  EPD_Line_t line;

  systime_t start = chVTGetSystemTime();

  uint8_t border = (EPD_STAGE_4 == stage) ? 0xAA : 0x00;

  size_t y = 0;
  for (y = 0; y < 128; ++y) {
    memset(&line, 0, sizeof(line));

    EPD_setScanByte(&line, y);
    line.line.border = border;

    size_t x = 0;
    for (x = 0; x < (144 / 8); ++x) {
      uint8_t b    = image[y][(144 / 8) - x - 1];
      uint8_t *pd1 = &line.line.data[2 * x];
      uint8_t *pd2 = &line.line.data[2 * x + 1];

      EPD_CreateDataBytes(b, stage, pd1, pd2);
    }

    EPD_Write(0x0a, line.bytes.data, sizeof(line.bytes.data));

    // Turn on OE
    uint8_t cmd  = 0x02;
    uint8_t data = 0x07;
    EPD_Write(cmd, &data, 1);
  }

  systime_t end = chVTGetSystemTime();
}

void EPD_WritePartialUpdateFrame(const uint8_t old[128][144 / 8], const uint8_t new[128][144 / 8])
{
  EPD_Line_t line;

  uint8_t border = 0xAA;

  size_t y = 0;
  for (y = 0; y < 128; ++y) {
    memset(&line, 0, sizeof(line));

    EPD_setScanByte(&line, y);
    line.line.border = border;

    size_t x = 0;
    for (x = 0; x < (144 / 8); ++x) {
      uint8_t bo = old[y][(144 / 8) - x - 1];
      uint8_t bn = new[y][(144 / 8) - x - 1];

      uint8_t *pd1 = &line.line.data[2 * x];
      uint8_t *pd2 = &line.line.data[2 * x + 1];

      size_t i = 0;
      for (i = 0; i < 8; ++i) {
        uint8_t mask = 0x01 << i;

        EPD_PixelColor_t pixel;
        if ((bo & mask) == (bn & mask))
          pixel = EPD_PIXEL_NOTHING;
        else if (bn & mask)
          pixel = EPD_PIXEL_BLACK;
        else
          pixel = EPD_PIXEL_WHITE;

        if (i < 4)
          *pd1 |= ((uint8_t)pixel) << (((4 - i) * 2) - 2);
        else
          *pd2 |= ((uint8_t)pixel) << (((8 - i) * 2) - 2);
      }
    }

    EPD_Write(0x0a, line.bytes.data, sizeof(line.bytes.data));

    // Turn on OE
    uint8_t cmd  = 0x02;
    uint8_t data = 0x07;
    EPD_Write(cmd, &data, 1);
  }
}

void EPD_WriteNothingFrame(void)
{
  EPD_Line_t line;

  uint8_t border = 0xAA;

  size_t y = 0;
  for (y = 0; y < 128; ++y) {
    memset(&line, 0, sizeof(line));

    EPD_setScanByte(&line, y);
    line.line.border = border;
    EPD_Write(0x0a, line.bytes.data, sizeof(line.bytes.data));

    // Turn on OE
    uint8_t cmd  = 0x02;
    uint8_t data = 0x07;
    EPD_Write(cmd, &data, 1);
  }
}

void EPD_WriteBorderDummyLine(void)
{
  EPD_Line_t line;

  memset(&line, 0, sizeof(line));
  line.line.border = 0xAA;

  EPD_Write(0x0a, line.bytes.data, sizeof(line.bytes.data));

  // Turn on OE
  uint8_t cmd  = 0x02;
  uint8_t data = 0x07;
  EPD_Write(cmd, &data, 1);
}

/****************************** END OF FILE **********************************/
