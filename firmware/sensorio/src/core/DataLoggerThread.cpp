/**
 * @file DataLoggerThread.cpp
 * @brief
 */

/*****************************************************************************/
/* INCLUDES                                                                  */
/*****************************************************************************/
#include "DataLoggerThread.h"
#include "dashboard/Dashboard.h"

#include <Arduino.h>
#include <SD.h>
#include <SPI.h>

/*****************************************************************************/
/* DEFINED CONSTANTS                                                         */
/*****************************************************************************/

/*****************************************************************************/
/* TYPE DEFINITIONS                                                          */
/*****************************************************************************/
static const size_t bufcapacity = 1024;

typedef struct Storage_s {
  const uint8_t *data;
  size_t         length;
} Storage_t;

class Buffer {
private:
  char              buf[bufcapacity];
  size_t            end;
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
    char * buffer = buf + end;
    size_t length = freeCapacity();

    size_t n = vsnprintf(buffer, length, fmt, args);

    bool error = true;

    if (n < length) {
      end += n;
      error = false;
    }

    return error;
  }

  void clear()
  {
    buf[0] = '\0';
    end    = 0;
  }

  Storage_t getStorage()
  {
    Storage_t storage;
    storage.data   = (uint8_t *)buf;
    storage.length = end;
    return storage;
  }
};

typedef Buffer *Message_t;

class Logger {
private:
  Buffer *         bufInUse;
  xSemaphoreHandle mutex;
  Buffer           buf_A;
  Buffer           buf_B;

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
        Serial.println("Failed to save the message in the buffer");
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
bool createLogFile(char *path, size_t length, GpsData_t *gps)
{
  size_t n       = snprintf(path, length, "/%04d", (int)gps->time.year);
  bool   success = SD.mkdir(path);

  n += snprintf(path + n, length - n, "/%02d", (int)gps->time.month);
  success = success && SD.mkdir(path);

  n += snprintf(path + n, length - n, "/%02d", (int)gps->time.day);
  success = success && SD.mkdir(path);

  n += snprintf(path + n,
                length - n,
                "/%02d-%02d-%02d.dat",
                (int)gps->time.hour,
                (int)gps->time.minute,
                (int)gps->time.second);

  // Append running index if needed
  bool found = SD.exists(path);
  for (int i = 0; found && (i < 100); ++i) {
    size_t datlen = strlen(".dat");    
    snprintf(path + n - datlen, length - n + datlen, "_%02d.dat", i);
    found = SD.exists(path);
  }

  if (success) {
    Serial.print(path);
    Serial.println(" created successfully");
  } else {
    Serial.print("Failed to create ");
    Serial.println(path);
  }

  return success;
}

bool createTmpFile(char *path, size_t length)
{
  size_t n       = snprintf(path, length, "/tmp");
  bool   success = SD.mkdir(path);

  bool found = true;
  for (int i = 0; found && (i < 100); ++i) {
    snprintf(path + n, length - n, "/tmp_%02d.dat", i);
    found = SD.exists(path);
    if (found) {
      Serial.print(path);
      Serial.println(" already exists, try next");
    }
  }

  Serial.print("Tmp file created: ");
  Serial.println(path);

  return success && !found;
}

static bool moveTmpFile(const char *tmp_path, const char *logfile_path)
{
  bool success = false;

  File readFrom = SD.open(tmp_path, FILE_READ);
  if (readFrom) {
    File writeTo = SD.open(logfile_path, FILE_WRITE);
    if (writeTo) {
      size_t  n;
      uint8_t buf[64];
      while ((n = readFrom.read(buf, sizeof(buf))) > 0) {
        writeTo.write(buf, n);
      }

      writeTo.close();

      Serial.print("Copied ");
      Serial.print(n);
      Serial.print(" byte(s) from ");
      Serial.print(tmp_path);
      Serial.print(" to ");
      Serial.println(logfile_path);

      success = true;

    } else {
      Serial.print("Failed to open ");
      Serial.println(logfile_path);
    }

    readFrom.close();

  } else {
    Serial.print("Failed to open ");
    Serial.println(tmp_path);
  }

  return success;
}

const char *getLogfileName(void)
{
  static enum {
    NOT_INITED,
    TMP_IN_USE,
    FILE_CREATED,
  } state = NOT_INITED;

  static char path[128];

  const char *logfile = path;

  GpsData_t gps;
  DbDataGpsGet(&gps);

  bool dateTimeIsValid = 2000U < gps.time.year;
  dateTimeIsValid &= gps.time.month <= 12U;
  dateTimeIsValid &= gps.time.day <= 31U;

  switch (state) {
  case NOT_INITED: {
    if (dateTimeIsValid) {
      if (createLogFile(path, sizeof(path), &gps)) {
        state = FILE_CREATED;
      } else {
        path[0] = '\0';
        logfile = NULL;
      }
    } else {
      bool found = createTmpFile(path, sizeof(path));
      if (found) {
        state = TMP_IN_USE;
      } else {
        path[0] = '\0';
        logfile = NULL;
      }
    }
    break;
  }
  case TMP_IN_USE: {
    if (dateTimeIsValid) {
      char tmp_path[sizeof(path)];
      memcpy(tmp_path, path, sizeof(tmp_path));
      if (createLogFile(path, sizeof(path), &gps)) {
        bool success = moveTmpFile(tmp_path, path);
        if (success) {
          SD.remove(tmp_path);
        }
        state = FILE_CREATED;
      } else {
        // Continue to use the temp file.
        memcpy(path, tmp_path, sizeof(path));
      }

      createLogFile(path, sizeof(path), &gps);
      state = FILE_CREATED;
    } else {
      ;
    }
    break;
  }
  case FILE_CREATED: {
    break;
  }
  default: {
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

/*****************************************************************************/
/* DEFINITION OF GLOBAL FUNCTIONS                                            */
/*****************************************************************************/
void DataLoggerThread(void *p)
{
  sdlock = xSemaphoreCreateMutex();

  SPIClass sdcSpi(HSPI);
  sdcSpi.begin(2, 5, 0, 15);

  while (!SD.begin(15, sdcSpi)) {
    Serial.println("Failed to initialize SD card");
    delay(1000);
  }

  while (1) {
    Message_t  msg;
    BaseType_t result = xQueueReceive(logger.queue, &msg, portMAX_DELAY);

    if (pdTRUE == result) {

      lockSdCard();

      Buffer *buf = msg;

      if (buf) {
        const char *logfile = getLogfileName();
        if (NULL != logfile) {
          File file = SD.open(logfile, FILE_APPEND);

          buf->lock();

          if (file) {
            Storage_t storage = buf->getStorage();
            file.write(storage.data, storage.length);
            file.close();
          } else {
            Serial.print("Failed to open ");
            Serial.println(logfile);
            Serial.println("Log entry is dropped");
          }

          buf->clear();
          buf->unlock();
        }
      }

      unlockSdCard();

    } else {
      Serial.println("Failed to read message queue");
    }
  }
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
