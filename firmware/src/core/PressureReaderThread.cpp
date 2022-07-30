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

#include <core/PressureReaderThread.hpp>
#include <dashboard/Dashboard.hpp>
#include <drivers/ms5611/ms5611.hpp>
#include <platform/Log.hpp>

#include <driver/gpio.h>
#include <driver/i2c.h>
#include <esp_log.h>
#include <freertos/FreeRTOS.h>
#include <freertos/semphr.h>
#include <freertos/task.h>
#include <freertos/timers.h>

using namespace Platform;

static const char *tag = "bps-thread";

static constexpr auto I2C_SDA {GPIO_NUM_13};
static constexpr auto I2C_SCL {GPIO_NUM_14};
static constexpr uint32_t I2C_FREQ {400000};
static constexpr auto I2C_PORT {I2C_NUM_0};

static inline bool installMasterDriver(i2c_port_t port)
{
  return ESP_OK != i2c_driver_install(port, I2C_MODE_MASTER, 0, 0, 0);
}

static inline bool paramConfig(i2c_port_t port, const i2c_config_t *conf)
{
  return ESP_OK != i2c_param_config(port, conf);
}

static inline uint8_t readAddress(uint8_t addr)
{
  return (addr) << 1 | I2C_MASTER_READ;
}

static inline uint8_t writeAddress(uint8_t addr)
{
  return (addr) << 1 | I2C_MASTER_WRITE;
}

static bool i2c_init(void)
{
  i2c_config_t conf = {.mode = I2C_MODE_MASTER,
                       .sda_io_num = I2C_SDA,
                       .scl_io_num = I2C_SCL,
                       .sda_pullup_en = GPIO_PULLUP_ENABLE,
                       .scl_pullup_en = GPIO_PULLUP_ENABLE,
                       .master = {
                           .clk_speed = I2C_FREQ,
                       }};

  bool error = paramConfig(I2C_PORT, &conf);
  error = error || installMasterDriver(I2C_PORT);

  return error;
}

static size_t i2c_write(uint8_t addr, uint8_t buf[], size_t length)
{
  i2c_cmd_handle_t cmd = i2c_cmd_link_create();
  i2c_master_start(cmd);
  i2c_master_write_byte(cmd, writeAddress(addr), true);
  i2c_master_write(cmd, buf, length, true);
  i2c_master_stop(cmd);

  esp_err_t error = i2c_master_cmd_begin(I2C_PORT, cmd, portMAX_DELAY);
  i2c_cmd_link_delete(cmd);

  return ESP_OK == error ? length : 0;
}

static size_t i2c_read(uint8_t addr, uint8_t buf[], size_t length)
{
  i2c_cmd_handle_t cmd = i2c_cmd_link_create();
  i2c_master_start(cmd);
  i2c_master_write_byte(cmd, readAddress(addr), true);
  i2c_master_read(cmd, buf, length, I2C_MASTER_LAST_NACK);
  i2c_master_stop(cmd);

  esp_err_t error = i2c_master_cmd_begin(I2C_PORT, cmd, portMAX_DELAY);
  i2c_cmd_link_delete(cmd);

  return ESP_OK == error ? length : 0;
}

static void delay(uint32_t ms)
{
  vTaskDelay(pdMS_TO_TICKS(ms));
}

static SemaphoreHandle_t readBps;

void PressureReaderThread(void *p)
{
  MS5611 bps = MS5611(MS5611::CSB_LOW, i2c_init, i2c_write, i2c_read, delay);

  while (bps.start()) {
    Log::Error(tag) << "failed to start BPS";
    vTaskDelay(pdMS_TO_TICKS(1000));
  }

  Log::Info(tag) << "BPS started";

  while (1) {
    xSemaphoreTake(readBps, portMAX_DELAY);

    bool error = bps.update(MS5611::Osr::OSR_4096);

    if (!error) {
      Dashboard::Bps data{};
      data.raw.temp = bps.getRawTemp();
      data.raw.pressure = bps.getRawPressure();
      data.cooked.temp = bps.getCompensatedTemp();
      data.cooked.pressure = bps.getCompensatedPressure();
      dashboard.bps.set(data);
    } else {
      Log::Error(tag) << "BPS conversion error";
    }
  }
}

void PressureReaderThreadInit(void)
{
  readBps = xSemaphoreCreateBinary();
}

void SampleBps(void)
{
  BaseType_t xHigherPriorityTaskWoken = pdFALSE;
  xSemaphoreGiveFromISR(readBps, &xHigherPriorityTaskWoken);
  if (pdTRUE == xHigherPriorityTaskWoken) {
    portYIELD_FROM_ISR();
  }
}
