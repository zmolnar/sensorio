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
  struct GPS {
    bool     locked;
    uint32_t altitude;
    uint32_t course;
    uint32_t latitude;
    uint32_t longitude;
    uint32_t numOfSatellites;
    uint32_t speed;
    struct Time_s {
      uint32_t year;
      uint8_t  month;
      uint8_t  day;
      uint8_t  hour;
      uint8_t  minute;
      uint8_t  second;
    } time;
  } gps;
} Data_t;

typedef struct Config_s {
  uint32_t magic;
  uint8_t crc;
} Config_t;

typedef struct Locks_s {
  SemaphoreHandle_t config;
  SemaphoreHandle_t gpsData;
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

  uint8_t crc = crc8((uint8_t *)cfg, sizeof(*cfg) - 1);
  bool crcOk   = (cfg->crc == crc);

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

  db.locks.gpsData = xSemaphoreCreateMutex();
  if (NULL == db.locks.gpsData) {
    Serial.println("CreateMutex failed");
  }
  db.locks.config  = xSemaphoreCreateMutex();
  if (NULL == db.locks.config) {
    Serial.println("CreateMutex failed");
  }
}

void DbSaveConfig(void)
{
  DbLock(db.locks.config);
  EEPROM.writeBytes(CONFIG_ADDRESS, &db.config, sizeof(db.config));
  EEPROM.commit();
  DbUnlock(db.locks.config);
}

void DbDataGpsLock(void)
{
  DbLock(db.locks.gpsData);
}

void DbDataGpsUnlock(void)
{
  DbUnlock(db.locks.gpsData);
}

void DbDataGpsSetLocked(bool state)
{
  db.data.gps.locked = state;
}

void DbDataGpsSetAltitude(uint32_t altitude)
{
  db.data.gps.altitude = altitude;
}

void DbDataGpsSetCourse(uint32_t course)
{
  db.data.gps.course = course;
}

void DbDataGpsSetLatitude(uint32_t latitude)
{
  db.data.gps.latitude = latitude;
}

void DbDataGpsSetLongitude(uint32_t longitude)
{
  db.data.gps.longitude = longitude;
}

void DbDataGpsSetNumOfSatellites(uint32_t sats)
{
  db.data.gps.numOfSatellites = sats;
}

void DbDataGpsSetSpeed(uint32_t speed)
{
  db.data.gps.speed = speed;
}

void DbDataGpsSetYear(uint32_t year)
{
  db.data.gps.time.year = year;
}

void DbDataGpsSetMonth(uint8_t month)
{
  db.data.gps.time.month = month;
}

void DbDataGpsSetDay(uint8_t day)
{
  db.data.gps.time.day = day;
}

void DbDataGpsSetHour(uint8_t hour)
{
  db.data.gps.time.hour = hour;
}

void DbDataGpsSetMinute(uint8_t minute)
{
  db.data.gps.time.minute = minute;
}

void DbDataGpsSetSecond(uint8_t sec)
{
  db.data.gps.time.second = sec;
}

bool DbDataGpsGetLocked(void)
{
  return db.data.gps.locked;
}

uint32_t DbDataGpsGetAltitude(void)
{
  return db.data.gps.altitude;
}

uint32_t DbDataGpsGetCourse(void)
{
  return db.data.gps.course;
}

uint32_t DbDataGpsGetLatitude(void)
{
  return db.data.gps.latitude;
}

uint32_t DbDataGpsGetLongitude(void)
{
  return db.data.gps.longitude;
}

uint32_t DbDataGpsGetnumOfSatellites(void)
{
  return db.data.gps.numOfSatellites;
}

uint32_t DbDataGpsGetSpeed(void)
{
  return db.data.gps.speed;
}

uint32_t DbDataGpsGetYear(void)
{
  return db.data.gps.time.year;
}

uint8_t DbDataGpsGetMonth(void)
{
  return db.data.gps.time.month;
}

uint8_t DbDataGpsGetDay(void)
{
  return db.data.gps.time.day;
}

uint8_t DbDataGpsGetHour(void)
{
  return db.data.gps.time.hour;
}

uint8_t DbDataGpsGetMinute(void)
{
  return db.data.gps.time.minute;
}

uint8_t DbDataGpsGetSecond(void)
{
  return db.data.gps.time.second;
}

/****************************** END OF FILE **********************************/
