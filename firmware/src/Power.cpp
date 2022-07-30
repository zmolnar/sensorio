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

#include <Power.hpp>
#include <core/LvglThread.hpp>

#include <driver/gpio.h>

static constexpr gpio_num_t nBUTTON{GPIO_NUM_36};
static constexpr uint64_t nBUTTON_SEL{GPIO_SEL_36};
static constexpr gpio_num_t LOCK{GPIO_NUM_4};
static constexpr uint64_t LOCK_SEL{GPIO_SEL_4};

void Power::configurePins(void) {
  gpio_config_t conf;
  conf.intr_type = GPIO_INTR_DISABLE;
  conf.mode = GPIO_MODE_INPUT;
  conf.pin_bit_mask = nBUTTON_SEL;
  conf.pull_down_en = GPIO_PULLDOWN_DISABLE;
  conf.pull_up_en = GPIO_PULLUP_DISABLE;
  gpio_config(&conf);

  gpio_set_intr_type(nBUTTON, GPIO_INTR_POSEDGE);
  gpio_install_isr_service(ESP_INTR_FLAG_EDGE);
  gpio_isr_handler_add(nBUTTON, buttonReleasedCb, NULL);
  gpio_intr_enable(nBUTTON);

  conf.intr_type = GPIO_INTR_DISABLE;
  conf.mode = GPIO_MODE_OUTPUT;
  conf.pin_bit_mask = LOCK_SEL;
  conf.pull_down_en = GPIO_PULLDOWN_DISABLE;
  conf.pull_up_en = GPIO_PULLUP_DISABLE;
  gpio_config(&conf);
}

bool Power::isButtonPressed(void) {
  return 0 == gpio_get_level(nBUTTON);
}

void Power::lockPower(void) {
  gpio_set_level(LOCK, 1);
}

void Power::unlockPower(void) {
  gpio_set_level(LOCK, 0);
}

void Power::buttonReleasedCb(void *p) {
  (void)p;

  gpio_reset_pin(nBUTTON);

  if (Power::get().ready) {
    LvglStartupFinished();
    Power::get().lockPower();
  } else {
    Power::get().unlockPower();
  }
}

void Power::start(void) {
  configurePins();

  if (isButtonPressed()) {
    lockPower();
    ready = false;
  } else {
    unlockPower();
  }
}

void Power::startupFinished(void) {
  ready = true;
}

void Power::stop(void) {
  unlockPower();
}
