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

#include <core/LvglThread.hpp>
#include <drivers/encoder/Encoder.hpp>

#include <lvgl.h>

#include <driver/gpio.h>
#include <freertos/FreeRTOS.h>
#include <freertos/timers.h>

static constexpr auto ENC_LEFT{GPIO_NUM_23};
static constexpr auto ENC_LEFT_SEL{GPIO_SEL_23};
static constexpr auto ENC_RIGHT{GPIO_NUM_12};
static constexpr auto ENC_RIGHT_SEL{GPIO_SEL_12};
static constexpr auto ENC_OK{GPIO_NUM_36};
static constexpr auto ENC_OK_SEL{GPIO_SEL_36};

static constexpr auto ENC_SHUTDOWN_DELAY{pdMS_TO_TICKS(5000)};
static constexpr auto ENC_TIMER_START_PERIOD{pdMS_TO_TICKS(300)};
static constexpr auto ENC_TIMER_MIN_PERIOD{pdMS_TO_TICKS(20)};

static constexpr auto DEBOUNCE_PERIOD{pdMS_TO_TICKS(50)};

enum class Direction {
  STOP,
  UP,
  DOWN,
};

enum class Level {
  LOW = 0,
  HIGH = 1,
};

bool isLevelLow(int level) {
  return static_cast<int>(Level::LOW) == level;
}

bool isLevelLow(Level level) {
  return Level::LOW == level;
}

bool isLevelHigh(int level) {
  return static_cast<int>(Level::HIGH) == level;
}

bool isLevelHigh(Level level) {
  return Level::HIGH == level;
}

class Encoder {
public:
  int16_t counter;
  lv_indev_state_t state;
  lv_group_t *group;
  Direction direction;
  TimerHandle_t timer;
  struct {
    TimerHandle_t left;
    TimerHandle_t ok;
    TimerHandle_t right;
  } debounceTimers;
  struct {
    Level left;
    Level ok;
    Level right;
  } level;

  void init() {
    counter = 0;
    direction = Direction::STOP;
    state = LV_INDEV_STATE_REL;
    level.left = Level::LOW;
    level.ok = Level::HIGH;
    level.right = Level::LOW;
  }
};

static Encoder encoder;

static void debounceEndLeftCb(TimerHandle_t xTimer) {
  gpio_intr_enable(ENC_LEFT);
}

static void debounceEndRightCb(TimerHandle_t xTimer) {
  gpio_intr_enable(ENC_RIGHT);
}

static void debounceEndOkCb(TimerHandle_t xTimer) {
  gpio_intr_enable(ENC_OK);
}

static void counterTick(TimerHandle_t xTimer) {
  (void)xTimer;

  switch (encoder.direction) {
  case Direction::UP:
    ++encoder.counter;
    break;
  case Direction::DOWN:
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

static void shutdownCallback(TimerHandle_t xTimer) {
  LvglShutdownRequested();
}

static void encoderLeftISR(void *p) {
  (void)p;

  int level = gpio_get_level(ENC_LEFT);

  if (isLevelHigh(level) && isLevelLow(encoder.level.left)) {
    gpio_intr_disable(ENC_RIGHT);
    gpio_intr_disable(ENC_OK);
    --encoder.counter;
    encoder.direction = Direction::DOWN;
    encoder.timer = xTimerCreate(
        "encoder timer", ENC_TIMER_START_PERIOD, pdFALSE, 0, counterTick);
    xTimerStartFromISR(encoder.timer, 0);
    encoder.level.left = Level::HIGH;
  } else if (isLevelLow(level) && isLevelHigh(encoder.level.left)) {
    encoder.level.left = Level::LOW;
    xTimerStop(encoder.timer, 0);
    xTimerDelete(encoder.timer, 0);
    encoder.direction = Direction::STOP;

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

static void encoderRightISR(void *p) {
  (void)p;

  int level = gpio_get_level(ENC_RIGHT);

  if (isLevelHigh(level) && isLevelLow(encoder.level.right)) {
    gpio_intr_disable(ENC_LEFT);
    gpio_intr_disable(ENC_OK);
    ++encoder.counter;
    encoder.direction = Direction::UP;
    encoder.timer = xTimerCreate(
        "encoder timer", ENC_TIMER_START_PERIOD, pdFALSE, 0, counterTick);
    xTimerStartFromISR(encoder.timer, 0);
    encoder.level.right = Level::HIGH;
  } else if (isLevelLow(level) && isLevelHigh(encoder.level.right)) {
    encoder.level.right = Level::LOW;
    xTimerStop(encoder.timer, 0);
    xTimerDelete(encoder.timer, 0);
    encoder.direction = Direction::STOP;

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

void encoderOkISR(void *p) {
  (void)p;

  int level = gpio_get_level(ENC_OK);

  if (isLevelLow(level) && isLevelHigh(encoder.level.ok)) {
    gpio_intr_disable(ENC_LEFT);
    gpio_intr_disable(ENC_RIGHT);
    encoder.state = LV_INDEV_STATE_PR;
    encoder.timer = xTimerCreate(
        "encoder timer", ENC_SHUTDOWN_DELAY, pdFALSE, 0, shutdownCallback);
    xTimerStartFromISR(encoder.timer, 0);
    encoder.level.ok = Level::LOW;
  } else if (isLevelHigh(level) && isLevelLow(encoder.level.ok)) {
    encoder.level.ok = Level::HIGH;
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

static bool EncoderRead(lv_indev_drv_t *indev_drv, lv_indev_data_t *data) {
  (void)indev_drv;

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

static void configureLeftButtonAndDebounceTimer(void) {
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

static void configureOkButtonAndDebounceTimer(void) {
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

static void configureRightButtonAndDebounceTimer(void) {
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

void EncoderInit(void) {
  encoder.init();

  configureLeftButtonAndDebounceTimer();
  configureOkButtonAndDebounceTimer();
  configureRightButtonAndDebounceTimer();

  gpio_intr_enable(ENC_LEFT);
  gpio_intr_enable(ENC_OK);
  gpio_intr_enable(ENC_RIGHT);
}

void EncoderRegisterDriver(lv_group_t *group) {
  lv_indev_drv_t enc_drv;
  lv_indev_drv_init(&enc_drv);
  enc_drv.type = LV_INDEV_TYPE_ENCODER;
  enc_drv.read_cb = EncoderRead;
  lv_indev_t *enc_indev = lv_indev_drv_register(&enc_drv);

  encoder.group = group;
  lv_indev_set_group(enc_indev, encoder.group);
}
