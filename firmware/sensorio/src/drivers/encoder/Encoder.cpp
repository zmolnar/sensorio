/**
 * @file Encoder.cpp
 * @brief
 */

/*****************************************************************************/
/* INCLUDES                                                                  */
/*****************************************************************************/
#include "Encoder.h"

#include <Power.h>
#include <lvgl.h>

#include <core/LvglThread.h>

#include <driver/gpio.h>
#include <esp_log.h>
#include <freertos/FreeRTOS.h>
#include <freertos/timers.h>

/*****************************************************************************/
/* DEFINED CONSTANTS                                                         */
/*****************************************************************************/
#define ENC_LEFT      GPIO_NUM_23
#define ENC_LEFT_SEL  GPIO_SEL_23
#define ENC_RIGHT     GPIO_NUM_12
#define ENC_RIGHT_SEL GPIO_SEL_12
#define ENC_OK        GPIO_NUM_36
#define ENC_OK_SEL    GPIO_SEL_36

#define ENC_SHUTDOWN_DELAY     pdMS_TO_TICKS(5000)
#define ENC_TIMER_START_PERIOD pdMS_TO_TICKS(300)
#define ENC_TIMER_MIN_PERIOD   pdMS_TO_TICKS(20)

#define DEBOUNCE_PERIOD pdMS_TO_TICKS(50)

/*****************************************************************************/
/* TYPE DEFINITIONS                                                          */
/*****************************************************************************/
typedef enum {
  STOP,
  UP,
  DOWN,
} Direction_t;

typedef struct Encoder_s {
  int16_t counter;
  lv_indev_state_t state;
  lv_group_t *group;
  Direction_t direction;
  TimerHandle_t timer;
  struct {
    TimerHandle_t left;
    TimerHandle_t ok;
    TimerHandle_t right;
  } debounceTimers;
  struct {
    uint8_t left;
    uint8_t ok;
    uint8_t right;
  } level;
} Encoder_t;

typedef enum {
  LOW = 0,
  HIGH = 1,
} LogicLevel_t;

/*****************************************************************************/
/* MACRO DEFINITIONS                                                         */
/*****************************************************************************/

/*****************************************************************************/
/* DEFINITION OF GLOBAL CONSTANTS AND VARIABLES                              */
/*****************************************************************************/
static Encoder_t encoder;
static const char *tag = "ENC";

/*****************************************************************************/
/* DECLARATION OF LOCAL FUNCTIONS                                            */
/*****************************************************************************/
static void debounceEndLeftCb(TimerHandle_t xTimer);
static void debounceEndRightCb(TimerHandle_t xTimer);
static void debounceEndOkCb(TimerHandle_t xTimer);
static void counterTick(TimerHandle_t xTimer);
static void shutdownCallback(TimerHandle_t xTimer);
static void encoderLeftISR(void *p);
static void encoderRightISR(void *p);
static void encoderOkISR(void *p);

/*****************************************************************************/
/* DEFINITION OF LOCAL FUNCTIONS                                             */
/*****************************************************************************/
static void debounceEndLeftCb(TimerHandle_t xTimer)
{
  gpio_intr_enable(ENC_LEFT);
}

static void debounceEndRightCb(TimerHandle_t xTimer)
{
  gpio_intr_enable(ENC_RIGHT);
}

static void debounceEndOkCb(TimerHandle_t xTimer)
{
  gpio_intr_enable(ENC_OK);
}

static void counterTick(TimerHandle_t xTimer)
{
  (void)xTimer;

  switch (encoder.direction) {
  case UP:
    ++encoder.counter;
    break;
  case DOWN:
    --encoder.counter;
    break;
  default:
    encoder.counter = 0;
    break;
  }

  // Update timer period
  TickType_t period;
  period = xTimerGetPeriod(encoder.timer);
  period = (TickType_t)((float)period * 0.9);
  if (period < ENC_TIMER_MIN_PERIOD) {
    period = ENC_TIMER_MIN_PERIOD;
  }
  xTimerChangePeriodFromISR(encoder.timer, period, 0);

  // Restart timer
  BaseType_t higherPrioTaskWoken;
  xTimerStartFromISR(encoder.timer, &higherPrioTaskWoken);
  if (pdTRUE == higherPrioTaskWoken) {
    portYIELD_FROM_ISR();
  }
}

static void shutdownCallback(TimerHandle_t xTimer)
{
  LvglShutdownRequested();
}

static void encoderLeftISR(void *p)
{
  (void)p;

  int level = gpio_get_level(ENC_LEFT);

  if ((HIGH == level) && (LOW == encoder.level.left)) {
    gpio_intr_disable(ENC_RIGHT);
    gpio_intr_disable(ENC_OK);
    --encoder.counter;
    encoder.direction = DOWN;
    encoder.timer = xTimerCreate(
        "encoder timer", ENC_TIMER_START_PERIOD, pdFALSE, 0, counterTick);
    xTimerStartFromISR(encoder.timer, 0);
    encoder.level.left = HIGH;
  } else if ((LOW == level) && (HIGH == encoder.level.left)) {
    encoder.level.left = LOW;
    xTimerStop(encoder.timer, 0);
    xTimerDelete(encoder.timer, 0);
    encoder.direction = STOP;

    gpio_intr_disable(ENC_LEFT);
    xTimerStartFromISR(encoder.debounceTimers.left, 0);

    if (pdFALSE == xTimerIsTimerActive(encoder.debounceTimers.ok)) {
      gpio_intr_enable(ENC_OK);
    }

    if (pdFALSE == xTimerIsTimerActive(encoder.debounceTimers.right)) {
      gpio_intr_enable(ENC_RIGHT);
    }
  }
}

static void encoderRightISR(void *p)
{
  (void)p;

  int level = gpio_get_level(ENC_RIGHT);

  if ((HIGH == level) && (LOW == encoder.level.right)) {
    gpio_intr_disable(ENC_LEFT);
    gpio_intr_disable(ENC_OK);
    ++encoder.counter;
    encoder.direction = UP;
    encoder.timer = xTimerCreate(
        "encoder timer", ENC_TIMER_START_PERIOD, pdFALSE, 0, counterTick);
    xTimerStartFromISR(encoder.timer, 0);
    encoder.level.right = HIGH;
  } else if ((LOW == level) && (HIGH == encoder.level.right)) {
    encoder.level.right = LOW;
    xTimerStop(encoder.timer, 0);
    xTimerDelete(encoder.timer, 0);
    encoder.direction = STOP;

    gpio_intr_disable(ENC_RIGHT);
    xTimerStartFromISR(encoder.debounceTimers.right, 0);

    if (pdFALSE == xTimerIsTimerActive(encoder.debounceTimers.ok)) {
      gpio_intr_enable(ENC_OK);
    }

    if (pdFALSE == xTimerIsTimerActive(encoder.debounceTimers.left)) {
      gpio_intr_enable(ENC_LEFT);
    }
  }
}

void encoderOkISR(void *p)
{
  (void)p;

  int level = gpio_get_level(ENC_OK);

  if ((LOW == level) && (HIGH == encoder.level.ok)) {
    gpio_intr_disable(ENC_LEFT);
    gpio_intr_disable(ENC_RIGHT);
    encoder.state = LV_INDEV_STATE_PR;
    encoder.timer = xTimerCreate(
        "encoder timer", ENC_SHUTDOWN_DELAY, pdFALSE, 0, shutdownCallback);
    xTimerStartFromISR(encoder.timer, 0);
    encoder.level.ok = LOW;
  } else if ((HIGH == level) && (LOW == encoder.level.ok)) {
    encoder.level.ok = HIGH;
    xTimerStop(encoder.timer, 0);
    xTimerDelete(encoder.timer, 0);
    encoder.state = LV_INDEV_STATE_REL;

    gpio_intr_disable(ENC_OK);
    xTimerStartFromISR(encoder.debounceTimers.ok, 0);

    if (pdFALSE == xTimerIsTimerActive(encoder.debounceTimers.right)) {
      gpio_intr_enable(ENC_RIGHT);
    }

    if (pdFALSE == xTimerIsTimerActive(encoder.debounceTimers.left)) {
      gpio_intr_enable(ENC_LEFT);
    }
  } else {
    ;
  }
}

static bool EncoderRead(lv_indev_drv_t *indev_drv, lv_indev_data_t *data)
{
  (void)indev_drv;

  // TODO avoid race conditions!
  if (0 != encoder.counter) {
    data->state = LV_INDEV_STATE_REL;
    data->enc_diff = encoder.counter;
    encoder.counter = 0;
  } else {
    data->state = encoder.state;
    data->enc_diff = 0;
  }

  return false;
}

static void configureLeftButtonAndDebounceTimer(void)
{
  gpio_config_t conf;
  conf.intr_type = GPIO_INTR_DISABLE;
  conf.mode = GPIO_MODE_INPUT;
  conf.pin_bit_mask = ENC_LEFT_SEL;
  conf.pull_down_en = GPIO_PULLDOWN_DISABLE;
  conf.pull_up_en = GPIO_PULLUP_DISABLE;

  gpio_config(&conf);
  gpio_set_intr_type(ENC_LEFT, GPIO_INTR_ANYEDGE);
  gpio_isr_handler_add(ENC_LEFT, encoderLeftISR, NULL);

  encoder.debounceTimers.left = xTimerCreate(
      "debounce timer left", DEBOUNCE_PERIOD, pdFALSE, 0, debounceEndLeftCb);
  configASSERT(encoder.debounceTimers.left);
}

static void configureOkButtonAndDebounceTimer(void)
{
  gpio_config_t conf;
  conf.intr_type = GPIO_INTR_DISABLE;
  conf.mode = GPIO_MODE_INPUT;
  conf.pin_bit_mask = ENC_OK_SEL;
  conf.pull_down_en = GPIO_PULLDOWN_DISABLE;
  conf.pull_up_en = GPIO_PULLUP_DISABLE;
  gpio_config(&conf);

  gpio_set_intr_type(ENC_OK, GPIO_INTR_ANYEDGE);
  gpio_isr_handler_add(ENC_OK, encoderOkISR, NULL);

  encoder.debounceTimers.ok = xTimerCreate(
      "debounce timer ok", DEBOUNCE_PERIOD, pdFALSE, 0, debounceEndOkCb);
  configASSERT(encoder.debounceTimers.ok);
}

static void configureRightButtonAndDebounceTimer(void)
{
  gpio_config_t conf;
  conf.intr_type = GPIO_INTR_DISABLE;
  conf.mode = GPIO_MODE_INPUT;
  conf.pin_bit_mask = ENC_RIGHT_SEL;
  conf.pull_down_en = GPIO_PULLDOWN_DISABLE;
  conf.pull_up_en = GPIO_PULLUP_DISABLE;
  gpio_config(&conf);

  gpio_set_intr_type(ENC_RIGHT, GPIO_INTR_ANYEDGE);
  gpio_isr_handler_add(ENC_RIGHT, encoderRightISR, NULL);

  encoder.debounceTimers.right = xTimerCreate(
      "debounce timer right", DEBOUNCE_PERIOD, pdFALSE, 0, debounceEndRightCb);
  configASSERT(encoder.debounceTimers.right);
}

/*****************************************************************************/
/* DEFINITION OF GLOBAL FUNCTIONS                                            */
/*****************************************************************************/
void EncoderInit(void)
{
  encoder.counter = 0;
  encoder.direction = STOP;
  encoder.state = LV_INDEV_STATE_REL;
  encoder.level.left = LOW;
  encoder.level.ok = HIGH;
  encoder.level.right = LOW;

  configureLeftButtonAndDebounceTimer();
  configureOkButtonAndDebounceTimer();
  configureRightButtonAndDebounceTimer();

  gpio_intr_enable(ENC_LEFT);
  gpio_intr_enable(ENC_OK);
  gpio_intr_enable(ENC_RIGHT);
}

void EncoderRegisterDriver(lv_group_t *group)
{
  lv_indev_drv_t enc_drv;
  lv_indev_drv_init(&enc_drv);
  enc_drv.type = LV_INDEV_TYPE_ENCODER;
  enc_drv.read_cb = EncoderRead;
  lv_indev_t *enc_indev = lv_indev_drv_register(&enc_drv);

  encoder.group = group;
  lv_indev_set_group(enc_indev, encoder.group);
}

/****************************** END OF FILE **********************************/
