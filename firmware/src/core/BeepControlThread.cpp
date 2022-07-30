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

#include <core/BeepControlThread.hpp>

#include <dashboard/Config.hpp>
#include <dashboard/Dashboard.hpp>
#include <platform/Assert.hpp>
#include <platform/Log.hpp>

#include <driver/ledc.h>
#include <driver/timer.h>
#include <esp_err.h>
#include <freertos/FreeRTOS.h>
#include <freertos/queue.h>
#include <freertos/task.h>

using namespace Platform;

using ::Config::System;
using Frequency = uint32_t;
using Duration = uint32_t;
using Callback = void (*)(void);

class Timer {
  static constexpr auto TIMER_GROUP{TIMER_GROUP_0};
  static constexpr auto TIMER{TIMER_0};
  static constexpr uint32_t DIVIDER{8U};

  static constexpr uint32_t millisecToTimerTick(uint32_t ms) {
    return ((ms) * ((TIMER_BASE_CLK / DIVIDER) / 1000));
  }

  Callback cb;

  static bool callback(void *arg) {
    auto *timer = reinterpret_cast<Timer *>(arg);
    timer->state = State::STOPPED;
    if (nullptr != timer->cb) {
      timer->cb();
    }
    return false;
  }

public:
  enum class State {
    STOPPED,
    RUNNING,
  };

  State state{State::STOPPED};

  void init() {
    timer_config_t config = {
        .alarm_en = TIMER_ALARM_DIS,
        .counter_en = TIMER_PAUSE,
        .intr_type = TIMER_INTR_LEVEL,
        .counter_dir = TIMER_COUNT_UP,
        .auto_reload = TIMER_AUTORELOAD_EN,
        .divider = DIVIDER,
    };
    timer_init(TIMER_GROUP, TIMER, &config);
    timer_set_counter_value(TIMER_GROUP, TIMER, 0);
  }

  void setCb(Callback cb) {
    this->cb = cb;
  }

  void start(Duration duration) {
    timer_pause(TIMER_GROUP, TIMER);
    timer_set_alarm_value(TIMER_GROUP, TIMER, millisecToTimerTick(duration));
    timer_set_alarm(TIMER_GROUP, TIMER, TIMER_ALARM_EN);
    timer_enable_intr(TIMER_GROUP, TIMER);
    timer_isr_callback_add(TIMER_GROUP, TIMER, Timer::callback, this, 0);
    timer_set_counter_value(TIMER_GROUP, TIMER, 0);
    timer_start(TIMER_GROUP, TIMER);

    state = State::RUNNING;
    this->cb = cb;
  }

  void stop() {
    timer_pause(TIMER_GROUP, TIMER);
    timer_set_alarm(TIMER_GROUP, TIMER, TIMER_ALARM_DIS);
    timer_disable_intr(TIMER_GROUP, TIMER);
    timer_isr_callback_remove(TIMER_GROUP, TIMER);

    state = State::STOPPED;
  }
};

class Pwm {
  static constexpr auto TIMER{LEDC_TIMER_1};
  static constexpr auto CHANNEL{LEDC_CHANNEL_0};
  static constexpr auto MODE{LEDC_HIGH_SPEED_MODE};
  static constexpr auto RESOLUTION_BITS{LEDC_TIMER_8_BIT};
  static constexpr auto OUT_PIN{GPIO_NUM_33};

  static constexpr uint32_t RESOLUTION_RANGE{
      (0x01UL << (uint32_t)RESOLUTION_BITS) - 1};

private:
  uint32_t calcDuty(System::Volume volume) {
    static constexpr uint32_t DUTY_ZERO{0U};
    static constexpr uint32_t DUTY_LOW{(uint32_t)(RESOLUTION_RANGE * 0.02)};
    static constexpr uint32_t DUTY_MED{(uint32_t)(RESOLUTION_RANGE * 0.05)};
    static constexpr uint32_t DUTY_HIGH{(uint32_t)(RESOLUTION_RANGE * 0.5)};

    uint32_t dutyMap[] = {
        [static_cast<int>(System::Volume::ZERO)] = DUTY_ZERO,
        [static_cast<int>(System::Volume::LOW)] = DUTY_LOW,
        [static_cast<int>(System::Volume::MED)] = DUTY_MED,
        [static_cast<int>(System::Volume::HIGH)] = DUTY_HIGH,
    };

    return dutyMap[static_cast<int>(volume)];
  }

  void setFreqAndDutyCycle(Frequency freq, System::Volume volume) {
    if ((System::Volume::ZERO != volume) && (0 < freq)) {
      auto duty = calcDuty(volume);
      ledc_timer_config_t timerConfig = {
          .speed_mode = MODE,
          .duty_resolution = RESOLUTION_BITS,
          .timer_num = TIMER,
          .freq_hz = freq,
          .clk_cfg = LEDC_AUTO_CLK,
      };
      ledc_timer_config(&timerConfig);

      ledc_channel_config_t pwmConfig = {
          .gpio_num = OUT_PIN,
          .speed_mode = MODE,
          .channel = CHANNEL,
          .intr_type = LEDC_INTR_DISABLE,
          .timer_sel = TIMER,
          .duty = duty,
          .hpoint = 0,
      };
      ledc_channel_config(&pwmConfig);
    } else {
      ledc_stop(MODE, CHANNEL, 0);
    }
  }

public:
  enum class State {
    ON,
    OFF,
  };

  State state{State::OFF};

  void enable(Frequency freq, System::Volume volume) {
    setFreqAndDutyCycle(freq, volume);
    state = State::ON;
  }

  void disable() {
    setFreqAndDutyCycle(0, System::Volume::ZERO);
    state = State::OFF;
  }
};

class Parameters {
public:
  double liftThreshold{0.4};
  double liftOffThreshold{0.1};
  double sinkThreshold{-1.2};
  double sinkOffThreshold{-1};
  double maximumLift{6};
  double liftFreqBase{600};
  double liftFreqMax{1500};
  double maximumSink{-6};
  double sinkFreqBase{400};
  double sinkFreqMin{150};
  double beepDurationMinLift{350};
  double beepDurationMaxLift{100};
  double silenceDurationMinLift{230};
  double silenceDurationMaxLift{60};

  Frequency calcFreq(double vario) {
    Frequency freq = 0.0;
    if (0.0 < vario) {
      freq = calcClimbFreq(vario);
    } else {
      freq = calcSinkFreq(vario);
    }

    return freq;
  }

  Frequency calcClimbFreq(double vario) {
    if (maximumLift < vario)
      vario = maximumLift;

    double range = liftFreqMax - liftFreqBase;
    double unit = range / maximumLift;
    double offset = vario * unit;

    return (Frequency)(liftFreqBase + offset);
  }

  Frequency calcSinkFreq(double vario) {
    if (vario < maximumSink)
      vario = maximumSink;

    double range = sinkFreqBase - sinkFreqMin;
    double unit = range / maximumSink;
    double offset = vario * unit;
    return (Frequency)(sinkFreqBase - offset);
  }

  Duration calcBeepDuration(double vario) {
    if (vario < 0)
      vario *= (-1);

    double range = beepDurationMaxLift - beepDurationMinLift;
    double unit = range / maximumLift;
    double offset = vario * unit;
    return (Duration)(beepDurationMinLift + offset);
  }

  Duration calcSilenceDuration(double vario) {
    double silenceDuration{0.0};

    if (0 < vario) {
      if (maximumLift < vario)
        vario = maximumLift;

      double range = silenceDurationMaxLift - silenceDurationMinLift;
      double unit = range / maximumLift;
      double offset = vario * unit;
      silenceDuration = (Duration)(silenceDurationMinLift + offset);
    } else {
      silenceDuration = 0;
    }

    return silenceDuration;
  }
};

class Beeper {
  enum class State {
    CLIMBING,
    SINKING,
    DISABLED,
  };

  class Config {
  public:
    System::Volume volume{System::Volume::HIGH};

    void update(::Config::System &system) {
      volume = system.beep.volume;
    }
  };

  State state{State::DISABLED};
  Parameters params{};
  Config config{};
  Pwm pwm{};
  Timer timer{};

  Frequency beepFreq{0U};
  Duration beepDuration{0U};
  Duration silenceDuration{0U};

  double vario{0.0};

public:
  Beeper() = default;

  void init() {
    timer.init();
  }

  void setTimerCb(Callback cb) {
    timer.setCb(cb);
  }

  void playStartupSignal() {
    vTaskDelay(pdMS_TO_TICKS(50));

    pwm.enable(1000, System::Volume::HIGH);
    vTaskDelay(pdMS_TO_TICKS(100));

    pwm.enable(1500, System::Volume::HIGH);
    vTaskDelay(pdMS_TO_TICKS(100));

    pwm.enable(2000, System::Volume::HIGH);
    vTaskDelay(pdMS_TO_TICKS(100));
    pwm.disable();

    vTaskDelay(pdMS_TO_TICKS(100));
  }

  bool shouldStart() {
    return (State::DISABLED != state) && (Timer::State::STOPPED == timer.state);
  }

  void updateState(double vario) {
    this->vario = vario;

    switch (state) {
    case State::CLIMBING: {
      if (vario < params.liftOffThreshold)
        state = State::DISABLED;
      break;
    }
    case State::SINKING: {
      if (params.sinkOffThreshold < vario)
        state = State::DISABLED;
      break;
    }
    case State::DISABLED: {
      if (params.liftThreshold < vario) {
        state = State::CLIMBING;
      } else if (vario < params.sinkThreshold) {
        state = State::SINKING;
      } else {
        ;
      }

      if (shouldStart()) {
        start();
      }

      break;
    }
    default: {
    }
    }
  }

  void updateConfig(::Config::System &system) {
    config.update(system);
  }

  void updateBeep() {
    if (State::DISABLED == state) {
      if (Pwm::State::ON == pwm.state) {
        pwm.disable();
      }
    } else {
      beepFreq = params.calcFreq(vario);
      beepDuration = params.calcBeepDuration(vario);
      silenceDuration = params.calcSilenceDuration(vario);

      switch (pwm.state) {
      case Pwm::State::ON: {
        if (0 == silenceDuration) {
          // When sinking, silenceDuration is 0, the beep is continuous
          timer.start(beepDuration);
        } else {
          // Start silence at the end of beep period
          pwm.disable();
          timer.start(silenceDuration);
        }
        break;
      }
      case Pwm::State::OFF: {
        // Start beep at the end of silence period
        pwm.enable(beepFreq, config.volume);
        timer.start(beepDuration);
      }
      default: {
      }
      }
    }
  }

  void start() {
    beepFreq = params.calcFreq(vario);
    pwm.enable(beepFreq, config.volume);

    beepDuration = params.calcBeepDuration(vario);
    timer.start(beepDuration);
  }

  void stop() {
    timer.stop();
    pwm.disable();
    state = State::DISABLED;
  }
};

enum class Command {
  SHUTDOWN,
  UPDATE_CONFIG,
  UPDATE_STATE,
  UPDATE_BEEP,
};

static QueueHandle_t beepQueue;

static void subscribeForFilter() {
  Mask mask = static_cast<Mask>(Dashboard::Dashboard::Data::FILTER);
  dashboard.subscribe(mask, []() {
    Command cmd = Command::UPDATE_STATE;
    (void)xQueueSend(beepQueue, &cmd, pdMS_TO_TICKS(5));
  });
}

static void subscribeForConfig() {
  Mask mask = static_cast<Mask>(Config::Component::SYSTEM);
  config.subscribe(mask, []() {
    Command cmd = Command::UPDATE_CONFIG;
    (void)xQueueSend(beepQueue, &cmd, pdMS_TO_TICKS(5));
  });
}

static void timerCb() {
  BaseType_t xHigherPriorityTaskWoken;
  Command cmd = Command::UPDATE_BEEP;
  (void)xQueueSendFromISR(beepQueue, &cmd, &xHigherPriorityTaskWoken);
  if(xHigherPriorityTaskWoken) {
    portYIELD_FROM_ISR();
  }
}

void BeepControlThread(void *p) {
  (void)p;

  Beeper beeper{};
  beeper.init();
  beeper.setTimerCb(timerCb);
  beeper.playStartupSignal();

  subscribeForFilter();
  subscribeForConfig();

  // Update from config after startup
  Command cmd = Command::UPDATE_CONFIG;
  (void)xQueueSend(beepQueue, &cmd, pdMS_TO_TICKS(5));

  while (1) {
    auto result = xQueueReceive(beepQueue, &cmd, portMAX_DELAY);

    if (pdTRUE == result) {
      switch (cmd) {
      case Command::SHUTDOWN: {
        beeper.stop();
        break;
      }
      case Command::UPDATE_CONFIG: {
        auto system{config.system.get()};
        beeper.updateConfig(system);
        subscribeForConfig();
        break;
      }
      case Command::UPDATE_STATE: {
        auto filter{dashboard.filter.get()};
        beeper.updateState(filter.vario.instant);
        subscribeForFilter();
        break;
      }
      case Command::UPDATE_BEEP: {
        beeper.updateBeep();
      }
      default: {
        break;
      }
      }
    }
  }
}

void BeepControlThreadInit(void) {
  beepQueue = xQueueCreate(50, sizeof(Command));
}
