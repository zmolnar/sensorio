/**
 * @file SharpLcd.cpp
 * @brief
 */

/*****************************************************************************/
/* INCLUDES                                                                  */
/*****************************************************************************/
#include "SharpLcd.h"

#include <Arduino.h>
#include <SPI.h>

#include "lvgl.h"

/*****************************************************************************/
/* DEFINED CONSTANTS                                                         */
/*****************************************************************************/
#define SHARP_LCD_HOR_RES     400
#define SHARP_LCD_VER_RES     240
#define SHARP_LCD_PIXEL_COUNT (SHARP_LCD_VER_RES * SHARP_LCD_HOR_RES)

#define LCD_MOSI          21
#define LCD_MISO          34 // Use input only pin (LCD does not send any data)
#define LCD_SCLK          22
#define LCD_CS            19
#define LCD_SPI_FREQ      2000000
#define LCD_SPI_BYTEORDER LSBFIRST
#define LCD_SPI_MODE      SPI_MODE0

#define BIT_WRITECMD (0x01) // 0x80 in LSB format
#define BIT_VCOM     (0x02) // 0x40 in LSB format
#define BIT_CLEAR    (0x04) // 0x20 in LSB format

/*****************************************************************************/
/* TYPE DEFINITIONS                                                          */
/*****************************************************************************/

/*****************************************************************************/
/* MACRO DEFINITIONS                                                         */
/*****************************************************************************/

/*****************************************************************************/
/* DEFINITION OF GLOBAL CONSTANTS AND VARIABLES                              */
/*****************************************************************************/
static SPIClass      lcd_spi = SPIClass();
static lv_disp_buf_t disp_buf;
static lv_color_t    frame_buffer[SHARP_LCD_PIXEL_COUNT / 8];

static uint8_t vcom             = 0;
static bool    vcomUpdateNeeded = false;

/*****************************************************************************/
/* DECLARATION OF LOCAL FUNCTIONS                                            */
/*****************************************************************************/

/*****************************************************************************/
/* DEFINITION OF LOCAL FUNCTIONS                                             */
/*****************************************************************************/
static void SharpLcdFlushCb(lv_disp_drv_t *disp_drv, const lv_area_t *area, lv_color_t *color_p)
{
  // Transform area from LVGL's coordinate system to the drivers' coordinate system.
  lv_area_t t_area;
  t_area.x1 = area->y1;
  t_area.y1 = SHARP_LCD_VER_RES - 1 - area->x1;
  t_area.x2 = area->y2;
  t_area.y2 = SHARP_LCD_VER_RES - 1 - area->x2;

  SPISettings cfg = SPISettings(LCD_SPI_FREQ, LCD_SPI_BYTEORDER, LCD_SPI_MODE);
  lcd_spi.beginTransaction(cfg);
  digitalWrite(LCD_CS, HIGH);
  lcd_spi.write(vcom | BIT_WRITECMD);
  vcomUpdateNeeded = false;

  for (size_t line = t_area.y1; line < t_area.y2; ++line) {
    size_t   length = (t_area.x2 - t_area.x1) / 8;
    uint8_t *data   = (uint8_t *)color_p + ((line - t_area.y1) * length);

    configASSERT(length == (SHARP_LCD_HOR_RES / 8));

    lcd_spi.write((uint8_t)line);     // Line address
    lcd_spi.writeBytes(data, length); // Line data
    lcd_spi.write(0x00);              // Padding byte
  }

  // Send last padding byte
  lcd_spi.write(0x00);

  digitalWrite(LCD_CS, LOW);
  lcd_spi.endTransaction();

  /* IMPORTANT!!!
   * Inform the graphics library that you are ready with the flushing*/
  lv_disp_flush_ready(disp_drv);
}

static void SharpLcdSetPxCb(lv_disp_drv_t *disp_drv,
                            uint8_t *      buf,
                            lv_coord_t     buf_w,
                            lv_coord_t     x,
                            lv_coord_t     y,
                            lv_color_t     color,
                            lv_opa_t       opa)
{
  size_t  i    = (buf_w >> 3) * y + (x >> 3);
  uint8_t mask = 0b10000000 >> (x % 8);

  if (lv_color_brightness(color) > 128) {
    buf[i] |= mask;
  } else {
    buf[i] &= ~mask;
  }
}

static void SharpLcdRounderCb(struct _disp_drv_t *disp_drv, lv_area_t *a)
{
  a->y1 = 0;
  a->y2 = SHARP_LCD_HOR_RES - 1;
}

/*****************************************************************************/
/* DEFINITION OF GLOBAL FUNCTIONS                                            */
/*****************************************************************************/
void SharpLcdInit(void)
{
  // Configure chip-select pin.
  pinMode(LCD_CS, OUTPUT);
  digitalWrite(LCD_CS, LOW);

  // Configure SPI periphers.
  lcd_spi.begin(LCD_SCLK, LCD_MISO, LCD_MOSI, LCD_CS);
  lcd_spi.setHwCs(false);

  // Send clear-screen command.
  SPISettings cfg = SPISettings(LCD_SPI_FREQ, LCD_SPI_BYTEORDER, LCD_SPI_MODE);
  lcd_spi.beginTransaction(cfg);
  digitalWrite(LCD_CS, HIGH);
  lcd_spi.write(BIT_CLEAR);
  lcd_spi.write(0x00);
  digitalWrite(LCD_CS, LOW);
  lcd_spi.endTransaction();
}

void SharpLcdRegisterDriver(void)
{
  SharpLcdInit();

  memset(frame_buffer, 0, sizeof(frame_buffer));
  lv_disp_buf_init(&disp_buf, frame_buffer, NULL, SHARP_LCD_PIXEL_COUNT);

  lv_disp_drv_t disp_drv;
  lv_disp_drv_init(&disp_drv);
  disp_drv.buffer     = &disp_buf;
  disp_drv.flush_cb   = SharpLcdFlushCb;
  disp_drv.set_px_cb  = SharpLcdSetPxCb;
  disp_drv.rounder_cb = SharpLcdRounderCb;

  lv_disp_drv_register(&disp_drv);
}

void SharpLcdToggleVcom(void)
{
  vcom ^= BIT_VCOM;
  vcomUpdateNeeded = true;
}

void SharpLcdSendVcomIfNeeded(void)
{
  if (vcomUpdateNeeded) {
    SPISettings cfg = SPISettings(LCD_SPI_FREQ, LCD_SPI_BYTEORDER, LCD_SPI_MODE);
    lcd_spi.beginTransaction(cfg);
    digitalWrite(LCD_CS, HIGH);
    lcd_spi.write(BIT_VCOM);
    lcd_spi.write(0x00);
    digitalWrite(LCD_CS, LOW);
    lcd_spi.endTransaction();

    vcomUpdateNeeded = false;
  }
}

/****************************** END OF FILE **********************************/
