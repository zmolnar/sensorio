//
//  This file is part of Sensorio.
//
//  Sensorio is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  Sensorio is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with Sensorio.  If not, see <https://www.gnu.org/licenses/>.
//

#include <drivers/lcd/SharpLcd.hpp>

#include <driver/gpio.h>
#include <driver/spi_master.h>

#include "lvgl.h"

static constexpr auto HORIZONTAL_RES{400};
static constexpr auto VERTICAL_RES{240};
static constexpr auto PIXEL_COUNT{(VERTICAL_RES * HORIZONTAL_RES)};

static constexpr auto LCD_MOSI{GPIO_NUM_21};
static constexpr auto LCD_MISO{-1};
static constexpr auto LCD_SCLK{GPIO_NUM_22};
static constexpr auto LCD_CS{GPIO_NUM_19};
static constexpr auto LCD_CS_SEL{GPIO_SEL_19};
static constexpr auto LCD_SPI_FREQ{2000000};
static constexpr auto LCD_SPI_MODE{0};

static constexpr auto BIT_WRITECMD{0x01}; // 0x80 in LSB format
static constexpr auto BIT_VCOM{0x02};     // 0x40 in LSB format
static constexpr auto BIT_CLEAR{0x04};    // 0x20 in LSB format

typedef enum {
  LOW = 0,
  HIGH = 1,
} Level;

static spi_device_handle_t lcd_spi;
static lv_disp_buf_t disp_buf;
static lv_color_t frame_buffer[PIXEL_COUNT / 8];

static uint8_t vcom = 0;
static bool vcomUpdateNeeded = false;

static void spiSendByte(spi_device_handle_t spi, uint8_t byte) {
  spi_transaction_t trans;
  memset(&trans, 0, sizeof(trans));
  trans.length = 8;
  trans.tx_buffer = &byte;
  esp_err_t err = spi_device_transmit(spi, &trans);
  configASSERT(ESP_OK == err);
}

static void
spiSendBuffer(spi_device_handle_t spi, uint8_t *buf, size_t length) {
  spi_transaction_t trans;
  memset(&trans, 0, sizeof(trans));
  trans.length = length * 8;
  trans.tx_buffer = buf;
  esp_err_t err = spi_device_transmit(spi, &trans);
  configASSERT(ESP_OK == err);
}

static void SharpLcdFlushCb(lv_disp_drv_t *disp_drv,
                            const lv_area_t *area,
                            lv_color_t *color_p) {
  // Transform area from LVGL's coordinate system to the drivers' coordinate
  // system.
  lv_area_t t_area;
  t_area.x1 = area->y1;
  t_area.y1 = VERTICAL_RES - 1 - area->x2;
  t_area.x2 = area->y2;
  t_area.y2 = VERTICAL_RES - 1 - area->x1;

  // Start transaction
  gpio_set_level(LCD_CS, HIGH);

  // Send write command and VCOM bit
  spiSendByte(lcd_spi, vcom | BIT_WRITECMD);
  vcomUpdateNeeded = false;

  for (size_t line = t_area.y1; line < t_area.y2; ++line) {
    size_t length = (t_area.x2 - t_area.x1 + 1) / 8;
    uint8_t *data = (uint8_t *)color_p + ((line - t_area.y1) * length);

    configASSERT(length == (HORIZONTAL_RES / 8));

    spiSendByte(lcd_spi, (uint8_t)line);  // Line address
    spiSendBuffer(lcd_spi, data, length); // Line data
    spiSendByte(lcd_spi, 0x00);           // Padding byte
  }

  // Send last padding byte
  spiSendByte(lcd_spi, 0x00);

  // End transaction
  gpio_set_level(LCD_CS, LOW);

  /* IMPORTANT!!!
   * Inform the graphics library that you are ready with the flushing*/
  lv_disp_flush_ready(disp_drv);
}

static void SharpLcdSetPxCb(lv_disp_drv_t *disp_drv,
                            uint8_t *buf,
                            lv_coord_t buf_w,
                            lv_coord_t x,
                            lv_coord_t y,
                            lv_color_t color,
                            lv_opa_t opa) {
  lv_coord_t xf = y;
  lv_coord_t yf = buf_w - 1 - x;

  size_t i = (yf * (HORIZONTAL_RES / 8)) + (xf >> 3);
  uint8_t mask = 0b00000001 << (xf % 8);

  if (lv_color_brightness(color) > 128) {
    buf[i] |= mask;
  } else {
    buf[i] &= ~mask;
  }
}

static void SharpLcdRounderCb(struct _disp_drv_t *disp_drv, lv_area_t *a) {
  a->y1 = 0;
  a->y2 = HORIZONTAL_RES - 1;
}

static void clearScreen(void) {
  uint8_t cmd[2] = {
      BIT_CLEAR,
      0x00,
  };

  gpio_set_level(LCD_CS, HIGH);
  spiSendBuffer(lcd_spi, cmd, sizeof(cmd));
  gpio_set_level(LCD_CS, LOW);
}

static void sendVcom(void) {
  uint8_t cmd[2] = {
      BIT_VCOM,
      0x00,
  };

  gpio_set_level(LCD_CS, HIGH);
  spiSendBuffer(lcd_spi, cmd, sizeof(cmd));
  gpio_set_level(LCD_CS, LOW);
}

void SharpLcdInit(void) {
  // Configure SPI interface
  spi_bus_config_t buscfg = {
      .mosi_io_num = LCD_MOSI,
      .miso_io_num = LCD_MISO,
      .sclk_io_num = LCD_SCLK,
      .quadwp_io_num = -1,
      .quadhd_io_num = -1,
      .max_transfer_sz = 0,
      .flags = 0,
      .intr_flags = 0,
  };

  spi_device_interface_config_t devcfg = {
      .command_bits = 0,
      .address_bits = 0,
      .dummy_bits = 0,
      .mode = LCD_SPI_MODE,
      .duty_cycle_pos = 0,
      .cs_ena_pretrans = 0,
      .cs_ena_posttrans = 0,
      .clock_speed_hz = LCD_SPI_FREQ,
      .input_delay_ns = 0,
      .spics_io_num = -1,
      .flags = SPI_DEVICE_BIT_LSBFIRST,
      .queue_size = 1,
      .pre_cb = 0,
      .post_cb = 0,
  };

  esp_err_t err = spi_bus_initialize(VSPI_HOST, &buscfg, 0);
  assert(err == ESP_OK);

  err = spi_bus_add_device(VSPI_HOST, &devcfg, &lcd_spi);
  assert(err == ESP_OK);

  // Configure chip-select pin.
  gpio_config_t conf;
  conf.intr_type = GPIO_INTR_DISABLE;
  conf.mode = GPIO_MODE_OUTPUT;
  conf.pin_bit_mask = LCD_CS_SEL;
  conf.pull_down_en = GPIO_PULLDOWN_DISABLE;
  conf.pull_up_en = GPIO_PULLUP_DISABLE;
  gpio_config(&conf);
  gpio_set_level(LCD_CS, LOW);

  clearScreen();
}

void SharpLcdRegisterDriver(void) {
  memset(frame_buffer, 0, sizeof(frame_buffer));
  lv_disp_buf_init(&disp_buf, frame_buffer, NULL, PIXEL_COUNT);

  lv_disp_drv_t disp_drv;
  lv_disp_drv_init(&disp_drv);
  disp_drv.buffer = &disp_buf;
  disp_drv.flush_cb = SharpLcdFlushCb;
  disp_drv.set_px_cb = SharpLcdSetPxCb;
  disp_drv.rounder_cb = SharpLcdRounderCb;

  lv_disp_drv_register(&disp_drv);
}

void SharpLcdToggleVcom(void) {
  vcom ^= BIT_VCOM;
  vcomUpdateNeeded = true;
}

void SharpLcdSendVcomIfNeeded(void) {
  if (vcomUpdateNeeded) {
    sendVcom();
    vcomUpdateNeeded = false;
  }
}
