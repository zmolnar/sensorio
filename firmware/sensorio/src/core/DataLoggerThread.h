
#ifndef DATA_LOGGER_THREAD_H
#define DATA_LOGGER_THREAD_H

#include <core/LogFile.hpp>

void DataLoggerThread(void *p);

void DataLoggerThreadInit(void);

void DataLoggerSave(const LogFile::Block &block);

void LogWaitToFinish(void);

#endif /* DATA_LOGGER_THREAD_H */
