/**
 * @file Power.c
 * @brief
 */

/*****************************************************************************/
/* INCLUDES                                                                  */
/*****************************************************************************/
#include "Power.h"

#include <driver/gpio.h>
#include <esp_log.h>

#include "core/LvglThread.h"

/*****************************************************************************/
/* DEFINED CONSTANTS                                                         */
/*****************************************************************************/
#define nBT0       GPIO_NUM_36
#define nBT0_SEL   GPIO_SEL_36
#define POW_EN     GPIO_NUM_4
#define POW_EN_SEL GPIO_SEL_4

/*****************************************************************************/
/* TYPE DEFINITIONS                                                          */
/*****************************************************************************/

/*****************************************************************************/
/* MACRO DEFINITIONS                                                         */
/*****************************************************************************/

/*****************************************************************************/
/* DEFINITION OF GLOBAL CONSTANTS AND VARIABLES                              */
/*****************************************************************************/
static bool startupFinished = false;

/*****************************************************************************/
/* DECLARATION OF LOCAL FUNCTIONS                                            */
/*****************************************************************************/

/*****************************************************************************/
/* DEFINITION OF LOCAL FUNCTIONS                                             */
/*****************************************************************************/
static bool isBt0Pressed(void)
{
  return 0 == gpio_get_level(nBT0);
}

static void enablePower(void)
{
  gpio_set_level(POW_EN, 1);
}

static void disablePower(void)
{
  gpio_set_level(POW_EN, 0);
}

static void powerSwitchReleasedCb(void *p)
{
  (void)p;

  gpio_reset_pin(nBT0);

  if (startupFinished) {
    LvglStartupFinished();
    enablePower();
  } else {
    disablePower();
  }
}

static void configureBt0(void)
{
  gpio_config_t conf;
  conf.intr_type = GPIO_INTR_DISABLE;
  conf.mode = GPIO_MODE_INPUT;
  conf.pin_bit_mask = nBT0_SEL;
  conf.pull_down_en = GPIO_PULLDOWN_DISABLE;
  conf.pull_up_en = GPIO_PULLUP_DISABLE;
  gpio_config(&conf);

  gpio_set_intr_type(nBT0, GPIO_INTR_POSEDGE);
  gpio_install_isr_service(ESP_INTR_FLAG_EDGE);
  gpio_isr_handler_add(nBT0, powerSwitchReleasedCb, NULL);
  gpio_intr_enable(nBT0);
}

static void configurePowEn(void)
{
  gpio_config_t conf;
  conf.intr_type = GPIO_INTR_DISABLE;
  conf.mode = GPIO_MODE_OUTPUT;
  conf.pin_bit_mask = POW_EN_SEL;
  conf.pull_down_en = GPIO_PULLDOWN_DISABLE;
  conf.pull_up_en = GPIO_PULLUP_DISABLE;
  gpio_config(&conf);
}

/*****************************************************************************/
/* DEFINITION OF GLOBAL FUNCTIONS                                            */
/*****************************************************************************/
void PowerStart(void)
{
  configureBt0();
  configurePowEn();

  if (isBt0Pressed()) {
    enablePower();
    startupFinished = false;
  } else {
    disablePower();
  }
}

void PowerStartupFinished(void)
{
  startupFinished = true;
}

void PowerStop(void)
{
  disablePower();
}

/****************************** END OF FILE **********************************/
