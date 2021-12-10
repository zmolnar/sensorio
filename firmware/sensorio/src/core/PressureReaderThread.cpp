/**
 * @file PressureReaderThread.cpp
 * @brief
 */

/*****************************************************************************/
/* INCLUDES                                                                  */
/*****************************************************************************/
#include "PressureReaderThread.h"
#include "DataFilterThread.hpp"
#include "ImuManagerThread.h"

#include "dashboard/Dashboard.hpp"
#include "drivers/ms5611/ms5611.h"

#include <driver/gpio.h>
#include <driver/i2c.h>
#include <esp_log.h>
#include <freertos/FreeRTOS.h>
#include <freertos/semphr.h>
#include <freertos/task.h>
#include <freertos/timers.h>

/*****************************************************************************/
/* DEFINED CONSTANTS                                                         */
/*****************************************************************************/
#define SAMPLE_PERIOD_IN_US 25000

/*****************************************************************************/
/* TYPE DEFINITIONS                                                          */
/*****************************************************************************/

/*****************************************************************************/
/* MACRO DEFINITIONS                                                         */
/*****************************************************************************/

/*****************************************************************************/
/* DEFINITION OF GLOBAL CONSTANTS AND VARIABLES                              */
/*****************************************************************************/
static SemaphoreHandle_t readBps;
static const char *tag = "bps-thread";

static gpio_num_t i2c_gpio_sda = GPIO_NUM_13;
static gpio_num_t i2c_gpio_scl = GPIO_NUM_14;
static uint32_t i2c_frequency = 400000;
static i2c_port_t i2c_port = I2C_NUM_0;

/*****************************************************************************/
/* DECLARATION OF LOCAL FUNCTIONS                                            */
/*****************************************************************************/

/*****************************************************************************/
/* DEFINITION OF LOCAL FUNCTIONS                                             */
/*****************************************************************************/
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
                       .sda_io_num = i2c_gpio_sda,
                       .scl_io_num = i2c_gpio_scl,
                       .sda_pullup_en = GPIO_PULLUP_ENABLE,
                       .scl_pullup_en = GPIO_PULLUP_ENABLE,
                       .master = {
                           .clk_speed = i2c_frequency,
                       }};

  bool error = paramConfig(i2c_port, &conf);
  error = error || installMasterDriver(i2c_port);

  return error;
}

static size_t i2c_write(uint8_t addr, uint8_t buf[], size_t length)
{
  i2c_cmd_handle_t cmd = i2c_cmd_link_create();
  i2c_master_start(cmd);
  i2c_master_write_byte(cmd, writeAddress(addr), true);
  i2c_master_write(cmd, buf, length, true);
  i2c_master_stop(cmd);

  esp_err_t error = i2c_master_cmd_begin(i2c_port, cmd, portMAX_DELAY);
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

  esp_err_t error = i2c_master_cmd_begin(i2c_port, cmd, portMAX_DELAY);
  i2c_cmd_link_delete(cmd);

  return ESP_OK == error ? length : 0;
}

static void delay(uint32_t ms)
{
  vTaskDelay(pdMS_TO_TICKS(ms));
}

/*****************************************************************************/
/* DEFINITION OF GLOBAL FUNCTIONS                                            */
/*****************************************************************************/
void PressureReaderThread(void *p)
{
  MS5611 bps = MS5611(MS5611::CSB_LOW, i2c_init, i2c_write, i2c_read, delay);

  bool error = bps.start();
  for (size_t i = 0; (i < 5) && error; ++i) {
    ESP_LOGI(tag, "failed start attempt #%d", (int)i);
    error = bps.start();
    vTaskDelay(pdMS_TO_TICKS(500));
  }

  configASSERT(!error);
  ESP_LOGI(tag, "started");

  while (1) {
    xSemaphoreTake(readBps, portMAX_DELAY);

    error = bps.update(MS5611::Osr::OSR_4096);

    if (!error) {
      Dashboard::Bps data{};
      data.raw.temp = bps.getRawTemp();
      data.raw.pressure = bps.getRawPressure();
      data.cooked.temp = bps.getCompensatedTemp();
      data.cooked.pressure = bps.getCompensatedPressure();
      dashboard.bps.set(data);
    } else {
      ESP_LOGE(tag, "MS5611 conversion error");
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

/****************************** END OF FILE **********************************/
