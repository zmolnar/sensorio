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

#include <core/DataLoggerThread.hpp>
#include <core/LogFile.hpp>
#include <core/SysLog.hpp>
#include <dashboard/Dashboard.hpp>
#include <platform/Log.hpp>

#include <dirent.h>
#include <esp_err.h>
#include <esp_vfs_fat.h>
#include <freertos/FreeRTOS.h>
#include <freertos/queue.h>
#include <freertos/semphr.h>
#include <freertos/task.h>

#include <etl/mutex.h>

#include "sdmmc_cmd.h"
#include <sys/stat.h>
#include <sys/unistd.h>

static const char *tag = "data-logger-thread";

static constexpr uint32_t QUEUE_LENGTH{10};
static QueueHandle_t queue{xQueueCreate(QUEUE_LENGTH, sizeof(LogFile::Block))};

static bool isDateTimeValid(Dashboard::Gps::DateTime &dt) {
  static uint32_t count{0};

  if (5 < count) {
    bool isvalid = 2000U < dt.year;
    isvalid &= dt.month <= 12U;
    isvalid &= dt.day <= 31U;
    return isvalid;
  } else {
    ++count;
    return false;
  }
}

DWORD get_fattime() {
  Dashboard::Gps gps {dashboard.gps.get()};
  DWORD timestamp {0};

  if(isDateTimeValid(gps.gmt)) {
    timestamp |= (DWORD)(gps.gmt.year - 1980) << 25;
    timestamp |= (DWORD)(gps.gmt.month) << 21;
    timestamp |= (DWORD)(gps.gmt.day) << 16;
    timestamp |= (DWORD)(gps.gmt.hour) << 11;
    timestamp |= (DWORD)(gps.gmt.minute) << 5;
    timestamp |= (DWORD)(gps.gmt.second) >> 1;
  } else {
    timestamp |= (DWORD)(2021U << 25);
    timestamp |= (DWORD)(1U << 21);
    timestamp |= (DWORD)(1U << 16);
    timestamp |= (DWORD)(0U << 11);
    timestamp |= (DWORD)(0U << 5);
    timestamp |= (DWORD)(0U >> 1);
  }

  return timestamp;
}

class Sdcard {
  etl::mutex mutex;
  sdmmc_host_t host;
  sdspi_slot_config_t slot_config;
  esp_vfs_fat_sdmmc_mount_config_t mount_config;

public:
  Sdcard() {
    host = (sdmmc_host_t)SDSPI_HOST_DEFAULT();
    host.flags = SDMMC_HOST_FLAG_SPI;
    host.max_freq_khz = 20000;

    slot_config = (sdspi_slot_config_t)SDSPI_SLOT_CONFIG_DEFAULT();
    slot_config.gpio_miso = GPIO_NUM_5;
    slot_config.gpio_mosi = GPIO_NUM_0;
    slot_config.gpio_sck = GPIO_NUM_2;
    slot_config.gpio_cs = GPIO_NUM_15;

    mount_config.format_if_mount_failed = false;
    mount_config.max_files = 5;
    mount_config.allocation_unit_size = 16 * 1024;
  }

  void lock() {
    mutex.lock();
  }

  void unlock() {
    mutex.unlock();
  }

  bool init(void) {
    using namespace Platform;

    Log::Info(tag) << "Initializing SD card";

    sdmmc_card_t *card;
    esp_err_t ret = esp_vfs_fat_sdmmc_mount("/sdcard", &host, &slot_config, &mount_config, &card);

    if (ret != ESP_OK) {
      if (ret == ESP_FAIL) {
        Log::Error(tag) << "Failed to mount filesystem";
      } else {
        Log::Error(tag) << "Failed to initialize the card";
      }
    } else {
      Log::Info(tag) << "SD card initialized successfully";
    }

    return ESP_OK != ret;
  }
};

class FileManager {
public:
  using Path = etl::string<128>;

private:
  enum class State {
    INIT,
    TMP,
    FINAL,
  };

  State state;
  Path root;

  bool fileExists(const Path &path) {
    struct stat st;
    return 0 == stat(path.c_str(), &st);
  }

  bool fileNotExists(const Path &path) {
    return !fileExists(path);
  }

  bool createDirectory(const Path &path) {
    int ret = mkdir(path.c_str(), 777U);
    return (0 != ret) && (EEXIST != errno);
  }

  bool deleteDirectory(const Path &path) {
    return 0 != unlink(path.c_str());
  }

  bool moveFromTo(const Path &from, const Path &to) {
    auto *dir = opendir(from.c_str());

    auto *item = readdir(dir);
    while (nullptr != item) {
      Path fromName{from}, toName{to};
      fromName += "/";
      fromName += item->d_name;
      toName += "/";
      toName += item->d_name;

      rename(fromName.c_str(), toName.c_str());

      item = readdir(dir);
    }

    closedir(dir);

    return false;
  }

  bool generateFinal(Dashboard::Gps::DateTime &dt, Path &path) {
    bool error{true};

    path.clear();
    etl::string_stream ss{path};
    ss << "/sdcard/" << dt.year;

    if (!createDirectory(path)) {
      ss << "/" << etl::setfill('0') << etl::setw(2) << dt.month;
      if (!createDirectory(path)) {
        ss << etl::setw(1) << "/";
        ss << etl::setfill('0') << etl::setw(2) << dt.day;
        if (!createDirectory(path)) {
          ss << etl::setw(1) << "/";
          ss << etl::setfill('0') << etl::setw(2) << dt.hour;
          ss << etl::setfill('0') << etl::setw(2) << dt.minute;
          ss << etl::setfill('0') << etl::setw(2) << dt.second;
          error = createDirectory(path);
        }
      }
    }

    return error;
  }

  bool generateTmp(Path &path) {
    path.clear();
    etl::string_stream ss{path};
    ss << "/sdcard/tmp";

    bool error{createDirectory(path)};

    if (!error) {
      const auto size = path.size();
      bool ready{false};
      for (uint32_t i = 0; (i < 200) && (!ready); ++i) {
        ss << "/" << i;
        if (fileNotExists(path)) {
          error = createDirectory(path);
          ready = true;
        } else {
          path.resize(size);
        }
      }
    }

    return error;
  }

public:
  FileManager() : state{State::INIT} {
  }

  bool generateRoot() {
    bool error{false};

    switch (state) {
    case State::INIT: {
      Dashboard::Gps gps{dashboard.gps.get()};
      Dashboard::Gps::DateTime &dt{gps.gmt};

      if (isDateTimeValid(dt)) {
        error = generateFinal(dt, root);
        if (!error) {
          state = State::FINAL;
        }
      } else {
        error = generateTmp(root);
        if (!error) {
          state = State::TMP;
        }
      }

      break;
    }
    case State::TMP: {
      Dashboard::Gps gps{dashboard.gps.get()};
      Dashboard::Gps::DateTime &dt{gps.gmt};

      Path tmp{root};

      if (isDateTimeValid(dt)) {
        error = generateFinal(dt, root);

        if (!error) {
          moveFromTo(tmp, root);
          deleteDirectory(tmp);
          state = State::FINAL;
        }
      }
      break;
    }
    case State::FINAL: {
      // No need to do anything, root has already been generated
      break;
    }
    default: {
      break;
    }
    }

    return error;
  }

  Path getPath(const char *filename) {
    Path path{root};
    etl::string_stream ss{path};
    ss << "/" << filename;

    return path;
  }
};

Sdcard sdcard{};
SysLog SysLog::obj{"sensorio.log"};

void DataLoggerThread(void *p) {
  (void)p;

  while (sdcard.init()) {
    vTaskDelay(pdMS_TO_TICKS(2000));
  }

  FileManager manager{};

  while (1) {
    LogFile::Block block{};
    BaseType_t result = xQueueReceive(queue, &block, portMAX_DELAY);

    if (pdTRUE == result) {
      sdcard.lock();

      block.lock();

      if (!manager.generateRoot()) {
        FileManager::Path path = manager.getPath(block.filename);

        FILE *file = fopen(path.c_str(), "a");
        fprintf(file, "%s", block.storage().data());
        fclose(file);
      }

      block.clear();
      block.unlock();

      sdcard.unlock();
    }
  }
}

void DataLoggerSave(const LogFile::Block &block) {
  (void)xQueueSend(queue, &block, pdMS_TO_TICKS(5));
}

void DataLoggerThreadInit(void) {
}

void LogWaitToFinish(void) {
  // Lock the mutex and don't release it, because it's gonna power down soon.
  sdcard.lock();
}
