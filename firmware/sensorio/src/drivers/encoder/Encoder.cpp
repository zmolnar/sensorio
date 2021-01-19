/**
 * @file Encoder.cpp
 * @brief
 */

/*****************************************************************************/
/* INCLUDES                                                                  */
/*****************************************************************************/
#include "Encoder.h"

#include "lvgl.h"
#include <Arduino.h>

#include "Power.h"
#include "gui/Sensorio.h"

/*****************************************************************************/
/* DEFINED CONSTANTS                                                         */
/*****************************************************************************/
#define ENC_LEFT  23
#define ENC_RIGHT 12
#define ENC_OK    36

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
  int16_t          counter;
  lv_indev_state_t state;
  lv_group_t *     group;
  Direction_t      direction;
  TimerHandle_t    timer;
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

/*****************************************************************************/
/* MACRO DEFINITIONS                                                         */
/*****************************************************************************/

/*****************************************************************************/
/* DEFINITION OF GLOBAL CONSTANTS AND VARIABLES                              */
/*****************************************************************************/
static Encoder_t encoder;

/*****************************************************************************/
/* DECLARATION OF LOCAL FUNCTIONS                                            */
/*****************************************************************************/
static void debounceEndLeftCb(TimerHandle_t xTimer);
static void debounceEndRightCb(TimerHandle_t xTimer);
static void debounceEndOkCb(TimerHandle_t xTimer);
static void counterTick(TimerHandle_t xTimer);
static void shutdownCallback(TimerHandle_t xTimer);
static void encoderLeftISR(void);
static void encoderRightISR(void);
static void encoderOkISR(void);

/*****************************************************************************/
/* DEFINITION OF LOCAL FUNCTIONS                                             */
/*****************************************************************************/
static void debounceEndLeftCb(TimerHandle_t xTimer)
{
  attachInterrupt(ENC_LEFT, encoderLeftISR, CHANGE);
}

static void debounceEndRightCb(TimerHandle_t xTimer)
{
  attachInterrupt(ENC_RIGHT, encoderRightISR, CHANGE);
}

static void debounceEndOkCb(TimerHandle_t xTimer)
{
  attachInterrupt(ENC_OK, encoderOkISR, CHANGE);
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

  Serial.println(encoder.counter);

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
  Serial.println("Shutdown!!!");
  SensorioConfirmExit();
}

static void encoderLeftISR(void)
{
  uint8_t state = digitalRead(ENC_LEFT);

  if ((HIGH == state) && (LOW == encoder.level.left)) {
    detachInterrupt(ENC_RIGHT);
    detachInterrupt(ENC_OK);
    --encoder.counter;
    encoder.direction = DOWN;
    encoder.timer     = xTimerCreate(
        "encoder timer", ENC_TIMER_START_PERIOD, pdFALSE, 0, counterTick);
    xTimerStartFromISR(encoder.timer, 0);
    encoder.level.left = HIGH;
  } else if ((LOW == state) && (HIGH == encoder.level.left)) {
    encoder.level.left = LOW;
    xTimerStop(encoder.timer, 0);
    xTimerDelete(encoder.timer, 0);
    encoder.direction = STOP;

    detachInterrupt(ENC_LEFT);
    xTimerStartFromISR(encoder.debounceTimers.left, 0);

    if (pdFALSE == xTimerIsTimerActive(encoder.debounceTimers.ok)) {
      attachInterrupt(ENC_OK, encoderOkISR, CHANGE);
    }

    if (pdFALSE == xTimerIsTimerActive(encoder.debounceTimers.right)) {
      attachInterrupt(ENC_RIGHT, encoderRightISR, CHANGE);
    }
  }
}

static void encoderRightISR(void)
{
  uint8_t state = digitalRead(ENC_RIGHT);
  
  if ((HIGH == state) && (LOW == encoder.level.right)) {
    detachInterrupt(ENC_LEFT);
    detachInterrupt(ENC_OK);
    ++encoder.counter;
    encoder.direction = UP;
    encoder.timer     = xTimerCreate(
        "encoder timer", ENC_TIMER_START_PERIOD, pdFALSE, 0, counterTick);
    xTimerStartFromISR(encoder.timer, 0);
    encoder.level.right = HIGH;
  } else if ((LOW == state) && (HIGH == encoder.level.right)) {
    encoder.level.right = LOW;
    xTimerStop(encoder.timer, 0);
    xTimerDelete(encoder.timer, 0);
    encoder.direction = STOP;

    detachInterrupt(ENC_RIGHT);
    xTimerStartFromISR(encoder.debounceTimers.right, 0);

    if (pdFALSE == xTimerIsTimerActive(encoder.debounceTimers.ok)) {
      attachInterrupt(ENC_OK, encoderOkISR, CHANGE);
    }

    if (pdFALSE == xTimerIsTimerActive(encoder.debounceTimers.left)) {
      attachInterrupt(ENC_LEFT, encoderLeftISR, CHANGE);
    }
  }
}

void encoderOkISR(void)
{
  uint8_t state = digitalRead(ENC_OK);

  if ((LOW == state) && (HIGH == encoder.level.ok)) {
    detachInterrupt(ENC_LEFT);
    detachInterrupt(ENC_RIGHT);
    encoder.state = LV_INDEV_STATE_PR;
    encoder.timer = xTimerCreate(
        "encoder timer", ENC_SHUTDOWN_DELAY, pdFALSE, 0, shutdownCallback);
    xTimerStartFromISR(encoder.timer, 0);
    encoder.level.ok = LOW;
  } else if ((HIGH == state) && (LOW == encoder.level.ok)) {
    encoder.level.ok = HIGH;
    xTimerStop(encoder.timer, 0);
    xTimerDelete(encoder.timer, 0);
    encoder.state = LV_INDEV_STATE_REL;

    detachInterrupt(ENC_OK);
    xTimerStartFromISR(encoder.debounceTimers.ok, 0);

    if (pdFALSE == xTimerIsTimerActive(encoder.debounceTimers.right)) {
      attachInterrupt(ENC_RIGHT, encoderRightISR, CHANGE);
    }

    if (pdFALSE == xTimerIsTimerActive(encoder.debounceTimers.left)) {
      attachInterrupt(ENC_LEFT, encoderLeftISR, CHANGE);
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
    data->state     = LV_INDEV_STATE_REL;
    data->enc_diff  = encoder.counter;
    encoder.counter = 0;
  } else {
    data->state    = encoder.state;
    data->enc_diff = 0;
  }

  return false;
}

/*****************************************************************************/
/* DEFINITION OF GLOBAL FUNCTIONS                                            */
/*****************************************************************************/
void EncoderInit(void)
{
  encoder.counter   = 0;
  encoder.direction = STOP;
  encoder.state     = LV_INDEV_STATE_REL;

  pinMode(ENC_LEFT, INPUT);
  pinMode(ENC_RIGHT, INPUT);
  pinMode(ENC_OK, INPUT);

  encoder.level.left  = LOW;
  encoder.level.ok    = HIGH;
  encoder.level.right = LOW;

  encoder.debounceTimers.left = xTimerCreate(
      "debounce timer left", DEBOUNCE_PERIOD, pdFALSE, 0, debounceEndLeftCb);

  encoder.debounceTimers.ok = xTimerCreate(
      "debounce timer ok", DEBOUNCE_PERIOD, pdFALSE, 0, debounceEndOkCb);

  encoder.debounceTimers.right = xTimerCreate(
      "debounce timer right", DEBOUNCE_PERIOD, pdFALSE, 0, debounceEndRightCb);

  if ((NULL == encoder.debounceTimers.left) ||
      (NULL == encoder.debounceTimers.ok) ||
      (NULL == encoder.debounceTimers.right)) {
    Serial.println("Failed to create timers");
  }

  attachInterrupt(ENC_LEFT, encoderLeftISR, CHANGE);
  attachInterrupt(ENC_RIGHT, encoderRightISR, CHANGE);
  attachInterrupt(ENC_OK, encoderOkISR, CHANGE);
}

void EncoderRegisterDriver(lv_group_t *group)
{
  lv_indev_drv_t enc_drv;
  lv_indev_drv_init(&enc_drv);
  enc_drv.type          = LV_INDEV_TYPE_ENCODER;
  enc_drv.read_cb       = EncoderRead;
  lv_indev_t *enc_indev = lv_indev_drv_register(&enc_drv);

  encoder.group = group;
  lv_indev_set_group(enc_indev, encoder.group);
}

/****************************** END OF FILE **********************************/
