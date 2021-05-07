/**
 * @file BeepControlThread.c
 * @brief Beep controller thread.
 * @author Molnar Zoltan
 */

/*******************************************************************************/
/* INCLUDES */
/*******************************************************************************/
#include "BeepControlThread.h"
#include "dashboard/Dashboard.h"

#include <Arduino.h>

/*******************************************************************************/
/* DEFINED CONSTANTS */
/*******************************************************************************/
#define BEEP_TIMER            0
#define BEEP_PWM              0
#define BEEP_RESOLUTION_BITS  8
#define BEEP_RESOLUTION_RANGE ((0x01UL << BEEP_RESOLUTION_BITS) - 1)
#define BEEP_PIN              33

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
static double liftThreshold    = 0.4;
static double liftOffThreshold = 0.1;
static double sinkThreshold    = -1.2;
static double sinkOffThreshold = -1;

static double maximumLift  = 6;
static double liftFreqBase = 600;
static double liftFreqMax  = 1500;
static double maximumSink  = (-6);
static double sinkFreqBase = 400;
static double sinkFreqMin  = 150;

static double beepDurationMinLift    = 350;
static double beepDurationMaxLift    = 100;
static double silenceDurationMinLift = 230;
static double silenceDurationMaxLift = 60;

static BeepControlState_t beepControlState = BEEP_DISABLED;
static BeepState_t        beepState        = BEEP_OFF;
static BeepVolume_t       beepVolume       = VOLUME_LOW;
static TimerState_t       timerState       = TIMER_STOPPED;

static uint32_t beepFrequency;
static uint32_t beepDuration;
static uint32_t silenceDuration;
static double   actualVario;

static hw_timer_t *  beepTimer;
static QueueHandle_t beepQueue;

/*******************************************************************************/
/* DECLARATION OF LOCAL FUNCTIONS */
/*******************************************************************************/

/*******************************************************************************/
/* DEFINITION OF LOCAL FUNCTIONS */
/*******************************************************************************/
static void setPwmFreqAndDutyCycle(uint32_t freq, BeepVolume_t volume)
{
  if (0 < volume) {
    ledcWriteTone(BEEP_PWM, (double)freq);
  }
  ledcWrite(BEEP_PWM, volume);
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

  double range  = liftFreqMax - liftFreqBase;
  double unit   = range / maximumLift;
  double offset = vario * unit;
  beepFrequency = (uint32_t)(liftFreqBase + offset);
}

static void calculateSinkFrequency(void)
{
  double vario = actualVario;
  if (vario < maximumSink)
    vario = maximumSink;

  double range  = sinkFreqBase - sinkFreqMin;
  double unit   = range / maximumSink;
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

  double range  = beepDurationMaxLift - beepDurationMinLift;
  double unit   = range / maximumLift;
  double offset = vario * unit;
  beepDuration  = (uint32_t)(beepDurationMinLift + offset);
}

static void calculateSilenceDuration(void)
{
  if (0 < actualVario) {
    double vario = actualVario;
    if (maximumLift < vario)
      vario = maximumLift;

    double range    = silenceDurationMaxLift - silenceDurationMinLift;
    double unit     = range / maximumLift;
    double offset   = vario * unit;
    silenceDuration = (uint32_t)(silenceDurationMinLift + offset);
  } else {
    silenceDuration = 0;
  }
}

static void startTimer(uint32_t delay_ms)
{
  uint32_t delay_us = delay_ms * 1000;
  timerStop(beepTimer);
  timerAlarmWrite(beepTimer, delay_us, true);
  timerAlarmEnable(beepTimer);
  timerStart(beepTimer);
  timerState = TIMER_RUNNING;
}

static void stopTimer()
{
  timerStop(beepTimer);
  timerAlarmDisable(beepTimer);
  timerState = TIMER_STOPPED;
}

static void startBeep(void)
{
  calculateBeepDuration();
  startTimer(beepDuration);
  enableBeep();
}

static void timerCallback(void)
{
  if (BEEP_DISABLED == beepControlState) {
    disableBeep();
    timerState = TIMER_STOPPED;
    return;
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
  delay(50);

  setPwmFreqAndDutyCycle(1000, VOLUME_HIGH);
  delay(100);

  setPwmFreqAndDutyCycle(1500, VOLUME_HIGH);
  delay(100);

  setPwmFreqAndDutyCycle(2000, VOLUME_HIGH);
  delay(100);
  setPwmFreqAndDutyCycle(0, VOLUME_ZERO);

  delay(100);
}

static void playShutdownSignal(void)
{
  delayMicroseconds(500);

  setPwmFreqAndDutyCycle(2000, VOLUME_HIGH);
  delay(100);

  setPwmFreqAndDutyCycle(1500, VOLUME_HIGH);
  delay(100);

  setPwmFreqAndDutyCycle(1000, VOLUME_HIGH);
  delay(100);
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
    beepVolume = VOLUME_LOW;
  }
}

/*******************************************************************************/
/* DEFINITION OF GLOBAL FUNCTIONS */
/*******************************************************************************/
void BeepControlThread(void *p)
{
  (void)p;

  updateLocalsFromConfig();
  // playStartupSignal();

  while (1) {
    BeepCommand_t cmd;
    BaseType_t    result = xQueueReceive(beepQueue, &cmd, portMAX_DELAY);

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
  // Initialize hardware timer
  beepTimer = timerBegin(BEEP_TIMER, 80, true);
  timerAttachInterrupt(beepTimer, timerCallback, true);

  // Initialize PWM channel
  ledcSetup(BEEP_PWM, 0.0, BEEP_RESOLUTION_BITS);
  ledcAttachPin(BEEP_PIN, BEEP_PWM);

  // Initialize message queue
  beepQueue = xQueueCreate(10, sizeof(BeepCommand_t));
}

void BeepControlUpdate(void)
{
  BeepCommand_t cmd = UPDATE;
  (void)xQueueSend(beepQueue, &cmd, pdMS_TO_TICKS(5));
}

/****************************** END OF FILE  *********************************/
