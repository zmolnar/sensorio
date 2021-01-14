/**
 * @file Dashboard.cpp
 * @brief
 */

/*****************************************************************************/
/* INCLUDES                                                                  */
/*****************************************************************************/
#include "Dashboard.h"

#include <Arduino.h>
#include <EEPROM.h>

/*****************************************************************************/
/* DEFINED CONSTANTS                                                         */
/*****************************************************************************/
#define MAGIC          0xacbb7876
#define CONFIG_ADDRESS 0

/*****************************************************************************/
/* TYPE DEFINITIONS                                                          */
/*****************************************************************************/
typedef struct Data_s {
  GpsData_t gps;
  BpsData_t bps;
  ImuData_t imu;
} Data_t;

typedef struct Config_s {
  uint32_t magic;
  uint8_t  crc;
} Config_t;

typedef struct Locks_s {
  SemaphoreHandle_t config;
  SemaphoreHandle_t gps;
  SemaphoreHandle_t bps;
  SemaphoreHandle_t imu;
} Locks_t;

typedef struct Dashboard_s {
  Data_t   data;
  Config_t config;
  Locks_t  locks;
} Dashboard_t;

/*****************************************************************************/
/* MACRO DEFINITIONS                                                         */
/*****************************************************************************/
#define EEPROM_SIZE sizeof(Config_t)

/*****************************************************************************/
/* DEFINITION OF GLOBAL CONSTANTS AND VARIABLES                              */
/*****************************************************************************/
static Dashboard_t db;

static const Config_t defaultConfig = {
    .magic = MAGIC,
    .crc   = 0x00,
};

/*****************************************************************************/
/* DECLARATION OF LOCAL FUNCTIONS                                            */
/*****************************************************************************/

/*****************************************************************************/
/* DEFINITION OF LOCAL FUNCTIONS                                             */
/*****************************************************************************/
static uint8_t crc8(const uint8_t data[], size_t length)
{
  uint8_t crc = 0;
  for (size_t i = 0; i < length; ++i) {
    crc ^= data[i];
  }

  return crc;
}

static bool DbIsConfigValid(Config_t *cfg)
{
  bool magicOk = (MAGIC == cfg->magic);

  uint8_t crc   = crc8((uint8_t *)cfg, sizeof(*cfg) - 1);
  bool    crcOk = (cfg->crc == crc);

  return magicOk && crcOk;
}

static void DbLock(SemaphoreHandle_t &mutex)
{
  BaseType_t res;
  do {
    res = xSemaphoreTake(mutex, portMAX_DELAY);
  } while (pdTRUE != res);
}

static void DbUnlock(SemaphoreHandle_t &mutex)
{
  BaseType_t res;
  do {
    res = xSemaphoreGive(mutex);
  } while (pdTRUE != res);
}

/*****************************************************************************/
/* DEFINITION OF GLOBAL FUNCTIONS                                            */
/*****************************************************************************/
void DbInit(void)
{
  memset(&db, 0, sizeof(db));

  EEPROM.begin(EEPROM_SIZE);
  EEPROM.readBytes(CONFIG_ADDRESS, &db.config, sizeof(db.config));

  if (DbIsConfigValid(&db.config)) {
    Serial.println("Config restored successfully");
  } else {
    Serial.println("Config corrupted, default config loaded");
    db.config = defaultConfig;
  }

  db.locks.config = xSemaphoreCreateMutex();
  db.locks.gps    = xSemaphoreCreateMutex();
  db.locks.bps    = xSemaphoreCreateMutex();
  db.locks.imu    = xSemaphoreCreateMutex();

  if ((NULL == db.locks.config) || (NULL == db.locks.gps) ||
      (NULL == db.locks.bps) || (NULL == db.locks.imu)) {
    Serial.println("CreateMutex failed");

    // TODO write log
  }
}

void DbSaveConfig(void)
{
  DbLock(db.locks.config);
  EEPROM.writeBytes(CONFIG_ADDRESS, &db.config, sizeof(db.config));
  EEPROM.commit();
  DbUnlock(db.locks.config);
}

void DbDataGpsGet(GpsData_t *p)
{
  DbLock(db.locks.gps);
  memcpy(p, &db.data.gps, sizeof(GpsData_t));
  DbUnlock(db.locks.gps);
}

void DbDataGpsSet(GpsData_t *p)
{
  DbLock(db.locks.gps);
  memcpy(&db.data.gps, p, sizeof(GpsData_t));
  DbUnlock(db.locks.gps);
}

void DbDataBpsGet(BpsData_t *p)
{
  DbLock(db.locks.bps);
  memcpy(p, &db.data.bps, sizeof(BpsData_t));
  DbUnlock(db.locks.bps);
}

void DbDataBpsSet(BpsData_t *p)
{
  DbLock(db.locks.bps);
  memcpy(&db.data.bps, p, sizeof(BpsData_t));
  DbUnlock(db.locks.bps);
}

void DbDataImuGet(ImuData_t *p)
{
  DbLock(db.locks.imu);
  memcpy(p, &db.data.imu, sizeof(ImuData_t));
  DbUnlock(db.locks.imu);    
}

void DbDataImuSet(ImuData_t *p)
{
  DbLock(db.locks.imu);
  memcpy(&db.data.imu, p, sizeof(ImuData_t));
  DbUnlock(db.locks.imu);  
}

/****************************** END OF FILE **********************************/
