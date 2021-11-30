
#ifndef SYSLOG_H
#define SYSLOG_H

#include <core/DataLoggerThread.h>
#include <core/LogFile.hpp>
#include <esp_log.h>

class SysLog {
  // Singleton class
  static SysLog obj;

  LogFile::File<256> file;
  vprintf_like_t oldFunc;

  SysLog(const char *filename) : file{filename} {
    oldFunc = esp_log_set_vprintf(logHandler);
  }

  SysLog(const SysLog &) = delete;
  SysLog& operator=(const SysLog &) = delete;
  SysLog(SysLog&&) = delete;
  SysLog& operator=(SysLog&&) = delete;
  ~SysLog() = default;

public:

  static int logHandler(const char *fmt, va_list args) {
    auto &file = obj.file;

    file.lock();

    file.buffer().lock();
    auto *data = file.buffer().data();
    auto size = file.buffer().size();
    auto available = file.buffer().available();
    file.buffer().alloc();
    auto n = vsnprintf(data, available, fmt, args);

    if (available <= n) {
      file.buffer().resize(size);
      file.buffer().unlock();
      DataLoggerSave(file.buffer().getBlock());

      file.swapBuffers();
      file.buffer().lock();
      data = file.buffer().data();
      size = file.buffer().size();
      available = file.buffer().available();
      file.buffer().alloc();
      n = vsnprintf(data, available, fmt, args);

      if (available <= n) {
        file.buffer().resize(size);
        file.buffer().unlock();
        DataLoggerSave(file.buffer().getBlock());

        // Log entry dropped
      } else {
        file.buffer().resize(size + n);
        file.buffer().unlock();
      }
    } else {
      file.buffer().resize(size + n);
      file.buffer().unlock();
    }

    file.unlock();

    return obj.oldFunc(fmt, args);
  }
};

#endif
