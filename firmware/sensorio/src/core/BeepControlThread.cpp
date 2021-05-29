/**
 * @file BeepControlThread.c
 * @brief Beep controller thread.
 * @author Molnar Zoltan
 */

/*******************************************************************************/
/* INCLUDES */
/*******************************************************************************/
#include "BeepControlThread.h"

#include <dashboard/Dashboard.h>

#include <driver/ledc.h>
#include <driver/timer.h>
#include <esp_err.h>
#include <esp_log.h>
#include <freertos/FreeRTOS.h>
#include <freertos/queue.h>
#include <freertos/task.h>

/*******************************************************************************/
/* DEFINED CONSTANTS */
/*******************************************************************************/
#define BEEP_TIMER_GROUP   TIMER_GROUP_0
#define BEEP_TIMER         TIMER_0
#define BEEP_TIMER_DIVIDER (8U)
#define MS_TO_TIMER_TICK(ms)                                                   \
  ((ms) * ((TIMER_BASE_CLK / BEEP_TIMER_DIVIDER) / 1000))

#define BEEP_PWM_TIMER        LEDC_TIMER_1
#define BEEP_PWM              LEDC_CHANNEL_0
#define BEEP_MODE             LEDC_HIGH_SPEED_MODE
#define BEEP_RESOLUTION_BITS  LEDC_TIMER_8_BIT
#define BEEP_RESOLUTION_RANGE ((0x01UL << BEEP_RESOLUTION_BITS) - 1)
#define BEEP_PIN              GPIO_NUM_33

#define VOLUME_ZERO 0
#define VOLUME_LOW  (uint32_t)(BEEP_RESOLUTION_RANGE * 0.02)
#define VOLUME_MED  (uint32_t)(BEEP_RESOLUTION_RANGE * 0.05)
#define VOLUME_HIGH (uint32_t)(BEEP_RESOLUTION_RANGE * 0.5)

/*******************************************************************************/
/* MACRO DEFINITIONS */
/*******************************************************************************/

/*******************************************************************************/
/* TYPE DEFINITIONS */
/*******************************************************************************/
typedef enum {
  BEEP_LIFTING,
  BEEP_SINKING,
  BEEP_DISABLED,
} BeepControlState_t;

typedef enum {
  BEEP_ON,
  BEEP_OFF,
} BeepState_t;

typedef enum {
  TIMER_STOPPED,
  TIMER_RUNNING,
} TimerState_t;

typedef enum {
  STEP_VOLUME,
  SYSTEM_SHUTDOWN,
  UPDATE,
} BeepCommand_t;

typedef uint32_t BeepVolume_t;

/*******************************************************************************/
/* DEFINITIONS OF GLOBAL CONSTANTS AND VARIABLES */
/*******************************************************************************/
static double liftThreshold = 0.4;
static double liftOffThreshold = 0.1;
static double sinkThreshold = -1.2;
static double sinkOffThreshold = -1;

static double maximumLift = 6;
static double liftFreqBase = 600;
static double liftFreqMax = 1500;
static double maximumSink = (-6);
static double sinkFreqBase = 400;
static double sinkFreqMin = 150;

static double beepDurationMinLift = 350;
static double beepDurationMaxLift = 100;
static double silenceDurationMinLift = 230;
static double silenceDurationMaxLift = 60;

static BeepControlState_t beepControlState = BEEP_DISABLED;
static BeepState_t beepState = BEEP_OFF;
static BeepVolume_t beepVolume = VOLUME_LOW;
static TimerState_t timerState = TIMER_STOPPED;

static uint32_t beepFrequency;
static uint32_t beepDuration;
static uint32_t silenceDuration;
static double actualVario;

static QueueHandle_t beepQueue;

/*******************************************************************************/
/* DECLARATION OF LOCAL FUNCTIONS */
/*******************************************************************************/

/*******************************************************************************/
/* DEFINITION OF LOCAL FUNCTIONS */
/*******************************************************************************/
static void setPwmFreqAndDutyCycle(uint32_t freq, BeepVolume_t volume)
{
  if ((0 < volume) && (0 < freq)) {
    ledc_timer_config_t timerConfig = {
        .speed_mode = BEEP_MODE,
        .duty_resolution = BEEP_RESOLUTION_BITS,
        .timer_num = BEEP_PWM_TIMER,
        .freq_hz = freq,
        .clk_cfg = LEDC_AUTO_CLK,
    };
    ledc_timer_config(&timerConfig);

    ledc_channel_config_t pwmConfig = {
        .gpio_num = BEEP_PIN,
        .speed_mode = BEEP_MODE,
        .channel = BEEP_PWM,
        .intr_type = LEDC_INTR_DISABLE,
        .timer_sel = BEEP_PWM_TIMER,
        .duty = volume,
        .hpoint = 0,
    };
    ledc_channel_config(&pwmConfig);
  } else {
    ledc_stop(BEEP_MODE, BEEP_PWM, 0);
  }
}

static void enableBeep(void)
{
  setPwmFreqAndDutyCycle(beepFrequency, beepVolume);
  beepState = BEEP_ON;
}

static void disableBeep(void)
{
  setPwmFreqAndDutyCycle(0, VOLUME_ZERO);
  beepState = BEEP_OFF;
}

static void readMeasurementData(void)
{
  FilterOutput_t data;
  DbDataFilterOutputGet(&data);
  actualVario = data.vario.instant;
}

static void calculateLiftFrequency(void)
{
  double vario = actualVario;
  if (maximumLift < vario)
    vario = maximumLift;

  double range = liftFreqMax - liftFreqBase;
  double unit = range / maximumLift;
  double offset = vario * unit;
  beepFrequency = (uint32_t)(liftFreqBase + offset);
}

static void calculateSinkFrequency(void)
{
  double vario = actualVario;
  if (vario < maximumSink)
    vario = maximumSink;

  double range = sinkFreqBase - sinkFreqMin;
  double unit = range / maximumSink;
  double offset = vario * unit;
  beepFrequency = (uint32_t)(sinkFreqBase - offset);
}

static void calculateBeepFrequency(void)
{
  if (0 < actualVario)
    calculateLiftFrequency();
  else
    calculateSinkFrequency();
}

static void calculateBeepDuration(void)
{
  double vario = actualVario;
  if (vario < 0)
    vario *= (-1);

  double range = beepDurationMaxLift - beepDurationMinLift;
  double unit = range / maximumLift;
  double offset = vario * unit;
  beepDuration = (uint32_t)(beepDurationMinLift + offset);
}

static void calculateSilenceDuration(void)
{
  if (0 < actualVario) {
    double vario = actualVario;
    if (maximumLift < vario)
      vario = maximumLift;

    double range = silenceDurationMaxLift - silenceDurationMinLift;
    double unit = range / maximumLift;
    double offset = vario * unit;
    silenceDuration = (uint32_t)(silenceDurationMinLift + offset);
  } else {
    silenceDuration = 0;
  }
}

static bool IRAM_ATTR timerCallback(void *args);

static void startTimer(uint32_t delay_ms)
{
  timer_pause(BEEP_TIMER_GROUP, BEEP_TIMER);
  timer_set_alarm_value(
      BEEP_TIMER_GROUP, BEEP_TIMER, MS_TO_TIMER_TICK(delay_ms));
  timer_set_alarm(BEEP_TIMER_GROUP, BEEP_TIMER, TIMER_ALARM_EN);
  timer_enable_intr(BEEP_TIMER_GROUP, BEEP_TIMER);
  timer_isr_callback_add(BEEP_TIMER_GROUP, BEEP_TIMER, timerCallback, NULL, 0);
  timer_set_counter_value(BEEP_TIMER_GROUP, BEEP_TIMER, 0);
  timer_start(BEEP_TIMER_GROUP, BEEP_TIMER);

  timerState = TIMER_RUNNING;
}

static void stopTimer()
{
  timer_pause(BEEP_TIMER_GROUP, BEEP_TIMER);
  timer_set_alarm(BEEP_TIMER_GROUP, BEEP_TIMER, TIMER_ALARM_DIS);
  timer_disable_intr(BEEP_TIMER_GROUP, BEEP_TIMER);
  timer_isr_callback_remove(BEEP_TIMER_GROUP, BEEP_TIMER);

  timerState = TIMER_STOPPED;
}

static void startBeep(void)
{
  calculateBeepDuration();
  startTimer(beepDuration);
  enableBeep();
}

static bool IRAM_ATTR timerCallback(void *args)
{
  (void)args;

  if (BEEP_DISABLED == beepControlState) {
    disableBeep();
    timerState = TIMER_STOPPED;
    return false;
  }

  calculateSilenceDuration();
  calculateBeepDuration();

  switch (beepState) {
  case BEEP_ON:
    if (0 == silenceDuration) {
      startTimer(beepDuration);
    } else {
      disableBeep();
      startTimer(silenceDuration);
    }
    break;
  case BEEP_OFF:
    enableBeep();
    startTimer(beepDuration);
    break;
  default:
    break;
  }

  return false;
}

static void updateBeeperStateMachine(void)
{
  switch (beepControlState) {
  case BEEP_LIFTING:
    if (actualVario < liftOffThreshold)
      beepControlState = BEEP_DISABLED;
    break;
  case BEEP_SINKING:
    if (sinkOffThreshold < actualVario)
      beepControlState = BEEP_DISABLED;
    break;
  case BEEP_DISABLED:
    if (liftThreshold < actualVario)
      beepControlState = BEEP_LIFTING;
    if (actualVario < sinkThreshold)
      beepControlState = BEEP_SINKING;

    if ((BEEP_DISABLED != beepControlState) && (TIMER_STOPPED == timerState))
      startBeep();

    break;
  default:
    break;
  }
}

static void playStartupSignal(void)
{
  vTaskDelay(pdMS_TO_TICKS(50));

  setPwmFreqAndDutyCycle(1000, VOLUME_HIGH);
  vTaskDelay(pdMS_TO_TICKS(100));

  setPwmFreqAndDutyCycle(1500, VOLUME_HIGH);
  vTaskDelay(pdMS_TO_TICKS(100));

  setPwmFreqAndDutyCycle(2000, VOLUME_HIGH);
  vTaskDelay(pdMS_TO_TICKS(100));
  setPwmFreqAndDutyCycle(0, VOLUME_ZERO);

  vTaskDelay(pdMS_TO_TICKS(100));
}

static void playShutdownSignal(void)
{
  setPwmFreqAndDutyCycle(2000, VOLUME_HIGH);
  vTaskDelay(pdMS_TO_TICKS(100));

  setPwmFreqAndDutyCycle(1500, VOLUME_HIGH);
  vTaskDelay(pdMS_TO_TICKS(100));

  setPwmFreqAndDutyCycle(1000, VOLUME_HIGH);
  vTaskDelay(pdMS_TO_TICKS(100));

  setPwmFreqAndDutyCycle(0, VOLUME_ZERO);
}

static void handleSystemShutdownEvent(void)
{
  if (TIMER_RUNNING == timerState)
    stopTimer();
  disableBeep();
  beepControlState = BEEP_DISABLED;
  playShutdownSignal();
}

static void handleSignalProcessorEvent(void)
{
  readMeasurementData();
  calculateBeepFrequency();
  updateBeeperStateMachine();
}

static void updateLocalsFromConfig(void)
{
  static const BeepVolume_t conversion[4] = {
      VOLUME_ZERO,
      VOLUME_LOW,
      VOLUME_MED,
      VOLUME_HIGH,
  };

  BeepSettings_t beep;
  DbCfgBeepSettingsGet(&beep);

  if ((0 <= beep.volume) && (beep.volume <= 3)) {
    beepVolume = conversion[beep.volume];
  } else {
    beepVolume = VOLUME_MED;
  }
}

static void initializeTimer(void)
{
  timer_config_t config = {
      .alarm_en = TIMER_ALARM_DIS,
      .counter_en = TIMER_PAUSE,
      .intr_type = TIMER_INTR_LEVEL,
      .counter_dir = TIMER_COUNT_UP,
      .auto_reload = TIMER_AUTORELOAD_EN,
      .divider = BEEP_TIMER_DIVIDER,
  };
  timer_init(BEEP_TIMER_GROUP, BEEP_TIMER, &config);
  timer_set_counter_value(BEEP_TIMER_GROUP, BEEP_TIMER, 0);
}

/*******************************************************************************/
/* DEFINITION OF GLOBAL FUNCTIONS */
/*******************************************************************************/
void BeepControlThread(void *p)
{
  (void)p;

  initializeTimer();

  updateLocalsFromConfig();
  playStartupSignal();

  while (1) {
    BeepCommand_t cmd;
    BaseType_t result = xQueueReceive(beepQueue, &cmd, portMAX_DELAY);

    if (pdTRUE == result) {
      updateLocalsFromConfig();
      switch (cmd) {
      case SYSTEM_SHUTDOWN: {
        handleSystemShutdownEvent();
        break;
      }
      case UPDATE: {
        handleSignalProcessorEvent();
        break;
      }
      default: {
        break;
      }
      }
    }
  }
}

void BeepControlThreadInit(void)
{
  beepQueue = xQueueCreate(10, sizeof(BeepCommand_t));
}

void BeepControlUpdate(void)
{
  BeepCommand_t cmd = UPDATE;
  (void)xQueueSend(beepQueue, &cmd, pdMS_TO_TICKS(5));
}

/****************************** END OF FILE  *********************************/
