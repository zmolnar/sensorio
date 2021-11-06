/**
 * @file DataLoggerThread.cpp
 * @brief
 */

/*****************************************************************************/
/* INCLUDES                                                                  */
/*****************************************************************************/
#include "DataLoggerThread.h"

#include <dashboard/Dashboard.hpp>

#include <esp_err.h>
#include <esp_log.h>
#include <esp_vfs_fat.h>
#include <freertos/FreeRTOS.h>
#include <freertos/queue.h>
#include <freertos/semphr.h>
#include <freertos/task.h>

#include "sdmmc_cmd.h"
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/unistd.h>

/*****************************************************************************/
/* DEFINED CONSTANTS                                                         */
/*****************************************************************************/

/*****************************************************************************/
/* TYPE DEFINITIONS                                                          */
/*****************************************************************************/
static const char *tag = "data-logger-thread";

static const size_t bufcapacity = 1024;

typedef struct Storage_s {
  const uint8_t *data;
  size_t length;
} Storage_t;

class Buffer {
private:
  char buf[bufcapacity];
  size_t end;
  SemaphoreHandle_t mutex;

public:
  Buffer() : end(0), mutex(xSemaphoreCreateMutex())
  {
    memset(buf, 0, sizeof(buf));
  }

  ~Buffer()
  {
  }

  size_t freeCapacity()
  {
    return bufcapacity - end;
  }

  void lock()
  {
    BaseType_t res;
    do {
      res = xSemaphoreTake(mutex, portMAX_DELAY);
    } while (pdTRUE != res);
  }

  void unlock()
  {
    BaseType_t res;
    do {
      res = xSemaphoreGive(mutex);
    } while (pdTRUE != res);
  }

  bool push_back(const char *fmt, va_list args)
  {
    char *buffer = buf + end;
    size_t length = freeCapacity();

    size_t n = vsnprintf(buffer, length, fmt, args);

    bool error = !(n < length);
    end += error ? 0 : n;

    buf[end] = '\0';

    return error;
  }

  void clear()
  {
    buf[0] = '\0';
    end = 0;
  }

  Storage_t getStorage()
  {
    Storage_t storage;
    storage.data = (uint8_t *)buf;
    storage.length = end;
    return storage;
  }
};

typedef Buffer *Message_t;

class Logger {
private:
  Buffer *bufInUse;
  xSemaphoreHandle mutex;
  Buffer buf_A;
  Buffer buf_B;

public:
  QueueHandle_t queue;

  Logger() :
      bufInUse(&buf_A),
      mutex(xSemaphoreCreateMutex()),
      buf_A(),
      buf_B(),
      queue(xQueueCreate(5, sizeof(Message_t)))
  {
    configASSERT(queue);
  }
  ~Logger()
  {
  }

  void lock()
  {
    BaseType_t res;
    do {
      res = xSemaphoreTake(mutex, portMAX_DELAY);
    } while (pdTRUE != res);
  }

  void unlock()
  {
    BaseType_t res;
    do {
      res = xSemaphoreGive(mutex);
    } while (pdTRUE != res);
  }

  void switchBuffer()
  {
    bufInUse = (&buf_A == bufInUse) ? &buf_B : &buf_A;
  }

  void write(const char *fmt, va_list args)
  {
    lock();

    bufInUse->lock();

    // Try to write it into the buffer in use
    bool error = bufInUse->push_back(fmt, args);

    if (error) {
      // The buffer doesn't have enough space, switch is needed.
      // Send message to the worker thread to save the buffer on the
      // SD card.
      (void)xQueueSend(queue, &bufInUse, pdMS_TO_TICKS(5));

      // Change buffer in use
      bufInUse->unlock();
      switchBuffer();
      bufInUse->lock();

      // Write into the new buffer
      error = bufInUse->push_back(fmt, args);

      if (error) {
        // Couldn't write it into the new buffer neither, drop the entry.
        ESP_LOGE(tag, "Failed to save the message in the buffer");
      }
    }

    bufInUse->unlock();

    unlock();
  }
};

/*****************************************************************************/
/* MACRO DEFINITIONS                                                         */
/*****************************************************************************/

/*****************************************************************************/
/* DEFINITION OF GLOBAL CONSTANTS AND VARIABLES                              */
/*****************************************************************************/
static Logger logger;
static SemaphoreHandle_t sdlock;

/*****************************************************************************/
/* DECLARATION OF LOCAL FUNCTIONS                                            */
/*****************************************************************************/

/*****************************************************************************/
/* DEFINITION OF LOCAL FUNCTIONS                                             */
/*****************************************************************************/
static bool createDirectory(const char *path)
{
  int ret = mkdir(path, 777U);
  return (0 != ret) && (EEXIST != errno);
}

static bool fileExists(const char *path)
{
  struct stat st;
  return 0 == stat(path, &st);
}

static bool
createLogFile(char *path, size_t length, Dashboard::Gps::DateTime &dt)
{
  size_t n = snprintf(path, length, "/sdcard/%04d", (int)dt.year);
  bool error = createDirectory(path);

  n += snprintf(path + n, length - n, "/%02d", (int)dt.month);
  error = error || createDirectory(path);

  n += snprintf(path + n, length - n, "/%02d", (int)dt.day);
  error = error || createDirectory(path);

  n += snprintf(path + n,
                length - n,
                "/%02d-%02d-%02d.dat",
                (int)dt.hour,
                (int)dt.minute,
                (int)dt.second);

  // Append running index if needed
  bool found = fileExists(path);
  for (int i = 0; found && (i < 100); ++i) {
    size_t datlen = strlen(".dat");
    snprintf(path + n - datlen, length - n + datlen, "_%02d.dat", i);
    found = fileExists(path);
  }

  if (error) {
    ESP_LOGE(tag, "Failed to create %s", path);
  } else {
    ESP_LOGI(tag, "%s created successfully", path);
  }

  return error;
}

static bool isDateTimeValid(Dashboard::Gps::DateTime &lt)
{
  bool isvalid = 2000U < lt.year;
  isvalid &= lt.month <= 12U;
  isvalid &= lt.day <= 31U;

  return isvalid;
}

static const char *getLogfileName(void)
{
  static enum {
    NOT_INITED,
    FILE_CREATED,
  } state = NOT_INITED;

  static char path[128];
  const char *logfile = path;

  switch (state) {
  case NOT_INITED: {
    Dashboard::Gps gps{dashboard.gps.get()};
    Dashboard::Gps::DateTime lt{gps.gmt};

    if (isDateTimeValid(lt)) {
      bool error = createLogFile(path, sizeof(path), lt);
      if (error) {
        path[0] = '\0';
        logfile = NULL;
      } else {
        state = FILE_CREATED;
      }
    }
    break;
  }
  case FILE_CREATED:
  default: {
    logfile = path;
    break;
  }
  }

  return logfile;
}

static void lockSdCard(void)
{
  BaseType_t res;
  do {
    res = xSemaphoreTake(sdlock, portMAX_DELAY);
  } while (pdTRUE != res);
}

static void unlockSdCard(void)
{
  BaseType_t res;
  do {
    res = xSemaphoreGive(sdlock);
  } while (pdTRUE != res);
}

static bool initializeSdCard(void)
{
  ESP_LOGI(tag, "Initializing SD card");

  sdmmc_host_t host = SDSPI_HOST_DEFAULT();
  host.flags = SDMMC_HOST_FLAG_SPI;
  host.max_freq_khz = 20000;

  sdspi_slot_config_t slot_config = SDSPI_SLOT_CONFIG_DEFAULT();
  slot_config.gpio_miso = GPIO_NUM_5;
  slot_config.gpio_mosi = GPIO_NUM_0;
  slot_config.gpio_sck = GPIO_NUM_2;
  slot_config.gpio_cs = GPIO_NUM_15;

  esp_vfs_fat_sdmmc_mount_config_t mount_config = {
      .format_if_mount_failed = false,
      .max_files = 5,
      .allocation_unit_size = 16 * 1024,
  };

  sdmmc_card_t *card;
  esp_err_t ret = esp_vfs_fat_sdmmc_mount(
      "/sdcard", &host, &slot_config, &mount_config, &card);

  if (ret != ESP_OK) {
    if (ret == ESP_FAIL) {
      ESP_LOGE(tag,
               "Failed to mount filesystem. "
               "If you want the card to be formatted, set "
               "format_if_mount_failed = true.");
    } else {
      ESP_LOGE(tag,
               "Failed to initialize the card (%s). "
               "Make sure SD card lines have pull-up resistors in place.",
               esp_err_to_name(ret));
    }
  }

  return ESP_OK != ret;
}

/*****************************************************************************/
/* DEFINITION OF GLOBAL FUNCTIONS                                            */
/*****************************************************************************/
void DataLoggerThread(void *p)
{
  // Try to mount SD card until it succeeds.
  while (initializeSdCard()) {
    vTaskDelay(pdMS_TO_TICKS(2000));
  }

  while (1) {
    Message_t msg;
    BaseType_t result = xQueueReceive(logger.queue, &msg, portMAX_DELAY);

    if (pdTRUE == result) {
      lockSdCard();

      Buffer *buf = msg;

      if (buf) {
        const char *logfile = getLogfileName();
        if (NULL != logfile) {
          FILE *f = fopen(logfile, "a");

          buf->lock();

          if (f) {
            Storage_t storage = buf->getStorage();
            fprintf(f, "%s", storage.data);
            fclose(f);
          } else {
            ESP_LOGE(tag, "Failed to open %s, log entry is dropped", logfile);
          }

          buf->clear();
          buf->unlock();
        }
      }

      unlockSdCard();
    }
  }
}

void DataLoggerThreadInit(void)
{
  sdlock = xSemaphoreCreateMutex();
}

void LogAppend(const char *fmt, ...)
{
  va_list args;
  va_start(args, fmt);
  logger.write(fmt, args);
  va_end(args);
}

void LogWaitToFinish(void)
{
  // Lock the mutex and don't release it, because it's gonna power down soon.
  lockSdCard();
}

/****************************** END OF FILE **********************************/
