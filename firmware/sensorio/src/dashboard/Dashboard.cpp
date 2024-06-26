/**
 * @file Dashboard.cpp
 * @brief
 */

/*****************************************************************************/
/* INCLUDES                                                                  */
/*****************************************************************************/
#include "Dashboard.h"
#include "LocalTime.h"

#include <esp_log.h>
#include <freertos/FreeRTOS.h>
#include <freertos/semphr.h>
#include <nvs.h>
#include <nvs_flash.h>

#include <string.h>

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
  FilterParameters_t filterParams;
  FilterOutput_t filter;
  ImuData_t imu;
  Battery_t battery;
  Board_t board;
} Data_t;

typedef struct Config_s {
  uint32_t magic;
  SysParams_t params;
  struct {
    ImuOffset_t offset;
    uint32_t crc;
  } calibration;
  BeepSettings_t beep;
  uint32_t crc;
} Config_t;

typedef struct Locks_s {
  SemaphoreHandle_t config;
  SemaphoreHandle_t gps;
  SemaphoreHandle_t bps;
  SemaphoreHandle_t filterParams;
  SemaphoreHandle_t filter;
  SemaphoreHandle_t imu;
  SemaphoreHandle_t battery;
  SemaphoreHandle_t board;
} Locks_t;

typedef struct Dashboard_s {
  Data_t data;
  Config_t config;
  Locks_t locks;
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
    .params = {.location = {.utcOffset = 1},
               .screens = {.vario = {.chart_refresh_period = 1000}}},
    .calibration = {.offset =
                        {
                            .acc = {.x = 0, .y = 0, .z = 0, .r = 0},
                            .gyro = {.x = 0, .y = 0, .z = 0},
                            .mag = {.x = 0, .y = 0, .z = 0, .r = 0},
                        },
                    .crc = 0xff},
    .beep = {.volume = VOL_LOW},
    .crc = 0x00,
};

static const char *tag = "dashboard";

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

static bool isConfigValid(Config_t *cfg)
{
  uint8_t *data = (uint8_t *)cfg;
  size_t length = sizeof(*cfg) - sizeof(cfg->crc);
  uint8_t crc = crc8(data, length);

  bool magicOk = (MAGIC == cfg->magic);
  bool crcOk = (cfg->crc == crc);

  return magicOk && crcOk;
}

static void lockMutex(SemaphoreHandle_t &mutex)
{
  BaseType_t res;
  do {
    res = xSemaphoreTake(mutex, portMAX_DELAY);
  } while (pdTRUE != res);
}

static void unlockMutex(SemaphoreHandle_t &mutex)
{
  BaseType_t res;
  do {
    res = xSemaphoreGive(mutex);
  } while (pdTRUE != res);
}

static void readConfigFromNvs(Config_t *cfg)
{
  nvs_handle_t handle;
  esp_err_t err = nvs_open("storage", NVS_READWRITE, &handle);
  if (err != ESP_OK) {
    ESP_LOGE(tag, "Error (%s) opening NVS handle!\n", esp_err_to_name(err));
  } else {
    size_t length = sizeof(*cfg);
    err = nvs_get_blob(handle, "config", cfg, &length);
    switch (err) {
    case ESP_OK: {
      ESP_LOGI(tag, "Config fetched from NVS");
      break;
    }
    case ESP_ERR_NVS_NOT_FOUND: {
      ESP_LOGI(tag, "Config not found in NVS, use default values");
      *cfg = defaultConfig;
      break;
    }
    default: {
      ESP_LOGE(tag, "Error (%s) getting config!\n", esp_err_to_name(err));
      break;
    }
    }

    nvs_close(handle);
  }
}

static void writeConfigToNvs(Config_t *cfg)
{
  nvs_handle_t handle;
  esp_err_t err = nvs_open("storage", NVS_READWRITE, &handle);
  if (err != ESP_OK) {
    ESP_LOGE(tag, "Error (%s) opening NVS handle!\n", esp_err_to_name(err));
  } else {
    err = nvs_set_blob(handle, "config", cfg, sizeof(*cfg));
    switch (err) {
    case ESP_OK: {
      ESP_LOGI(tag, "Config written to NVS successfully");
      break;
    }
    default: {
      ESP_LOGE(tag, "Error (%s) saving config!\n", esp_err_to_name(err));
      break;
    }
    }

    nvs_close(handle);
  }
}

/*****************************************************************************/
/* DEFINITION OF GLOBAL FUNCTIONS                                            */
/*****************************************************************************/
void DbInit(void)
{
  memset(&db, 0, sizeof(db));

  esp_err_t err = nvs_flash_init();
  if (err == ESP_ERR_NVS_NO_FREE_PAGES ||
      err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
    // NVS partition was truncated and needs to be erased. Retry nvs_flash_init
    ESP_ERROR_CHECK(nvs_flash_erase());
    err = nvs_flash_init();
  }
  ESP_ERROR_CHECK(err);

  readConfigFromNvs(&db.config);

  if (isConfigValid(&db.config)) {
    ESP_LOGI(tag, "Config restored successfully");
    ESP_LOGI(tag, "Location:");
    ESP_LOGI(tag, "  UTC offset: %d", db.config.params.location.utcOffset);
    ESP_LOGI(tag, "Screens");
    ESP_LOGI(tag, "  Variometer");
    ESP_LOGI(tag,
             "    Chart refresh period: %d",
             db.config.params.screens.vario.chart_refresh_period);
    ESP_LOGI(tag, "CRC: %d", db.config.crc);
  } else {
    ESP_LOGE(tag, "Config corrupted, default config loaded");
    db.config = defaultConfig;
  }

  db.locks.config = xSemaphoreCreateMutex();
  configASSERT(NULL != db.locks.config);

  db.locks.gps = xSemaphoreCreateMutex();
  configASSERT(NULL != db.locks.gps);

  db.locks.bps = xSemaphoreCreateMutex();
  configASSERT(NULL != db.locks.bps);

  db.locks.filterParams = xSemaphoreCreateMutex();
  configASSERT(NULL != db.locks.filterParams);

  db.locks.filter = xSemaphoreCreateMutex();
  configASSERT(NULL != db.locks.filter);

  db.locks.imu = xSemaphoreCreateMutex();
  configASSERT(NULL != db.locks.imu);

  db.locks.battery = xSemaphoreCreateMutex();
  configASSERT(NULL != db.locks.battery);

  db.locks.board = xSemaphoreCreateMutex();
  configASSERT(NULL != db.locks.board);
}

void DbCfgSysParamsGet(SysParams_t *p)
{
  lockMutex(db.locks.config);
  memcpy(p, &db.config.params, sizeof(SysParams_t));
  unlockMutex(db.locks.config);
}

void DbCfgSysParamsSet(SysParams_t *p)
{
  lockMutex(db.locks.config);
  memcpy(&db.config.params, p, sizeof(SysParams_t));
  unlockMutex(db.locks.config);
}

bool DbCfgImuCalibrationIsValid(void)
{
  lockMutex(db.locks.config);

  uint8_t *data = (uint8_t *)&db.config.calibration.offset;
  size_t length = sizeof(db.config.calibration.offset);
  uint8_t crc = crc8(data, length);

  bool crcOk = (db.config.calibration.crc == crc);

  unlockMutex(db.locks.config);

  return crcOk;
}

void DbCfgImuCalibrationGet(ImuOffset_t *offset)
{
  lockMutex(db.locks.config);

  uint8_t *dst = (uint8_t *)offset;
  uint8_t *src = (uint8_t *)&db.config.calibration.offset;
  size_t length = sizeof(ImuOffset_t);

  memcpy(dst, src, length);

  unlockMutex(db.locks.config);
}

void DbCfgImuCalibrationSet(ImuOffset_t *offset)
{
  lockMutex(db.locks.config);

  uint8_t *dst = (uint8_t *)&db.config.calibration.offset;
  uint8_t *src = (uint8_t *)offset;
  size_t length = sizeof(ImuOffset_t);

  memcpy(dst, src, length);

  db.config.calibration.crc = crc8(dst, length);

  unlockMutex(db.locks.config);
}

void DbCfgBeepSettingsGet(BeepSettings_t *beep)
{
  lockMutex(db.locks.config);
  memcpy(beep, &db.config.beep, sizeof(BeepSettings_t));
  unlockMutex(db.locks.config);
}

void DbCfgBeepSettingsSet(BeepSettings_t *beep)
{
  lockMutex(db.locks.config);
  memcpy(&db.config.beep, beep, sizeof(BeepSettings_t));
  unlockMutex(db.locks.config);
}

void DbCfgSaveToEeprom(void)
{
  lockMutex(db.locks.config);

  uint8_t *data = (uint8_t *)&db.config;
  size_t length = sizeof(db.config) - sizeof(db.config.crc);
  db.config.crc = crc8(data, length);

  writeConfigToNvs(&db.config);

  unlockMutex(db.locks.config);
}

void DbDataGpsGet(GpsData_t *p)
{
  lockMutex(db.locks.gps);
  memcpy(p, &db.data.gps, sizeof(GpsData_t));
  unlockMutex(db.locks.gps);
}

void DbDataGpsSet(GpsData_t *p)
{
  lockMutex(db.locks.gps);
  memcpy(&db.data.gps, p, sizeof(GpsData_t));
  unlockMutex(db.locks.gps);
}

void DbDataBpsGet(BpsData_t *p)
{
  lockMutex(db.locks.bps);
  memcpy(p, &db.data.bps, sizeof(BpsData_t));
  unlockMutex(db.locks.bps);
}

void DbDataBpsSet(BpsData_t *p)
{
  lockMutex(db.locks.bps);
  memcpy(&db.data.bps, p, sizeof(BpsData_t));
  unlockMutex(db.locks.bps);
}

void DbDataLocalTimeGet(LocalTime_t *p)
{
  lockMutex(db.locks.gps);
  p->year = db.data.gps.time.year;
  p->month = db.data.gps.time.month;
  p->day = db.data.gps.time.day;
  p->hour = db.data.gps.time.hour;
  p->minute = db.data.gps.time.minute;
  p->second = db.data.gps.time.second;
  unlockMutex(db.locks.gps);

  lockMutex(db.locks.config);
  int offset = db.config.params.location.utcOffset;
  unlockMutex(db.locks.config);

  int dow = LtDayOfWeek(p->year, p->month, p->day);
  offset += LtIsDaylightSavingTime(p->day, p->month, dow) ? 1 : 0;
  LtAddOffsetToDateTime(p, offset);
}

void DbDataFilterParametersGet(FilterParameters_t *p)
{
  lockMutex(db.locks.filterParams);
  memcpy(p, &db.data.filterParams, sizeof(FilterParameters_t));
  unlockMutex(db.locks.filterParams);
}

void DbDataFilterParametersSet(FilterParameters_t *p)
{
  lockMutex(db.locks.filterParams);
  memcpy(&db.data.filterParams, p, sizeof(FilterParameters_t));
  unlockMutex(db.locks.filterParams);
}

void DbDataFilterOutputGet(FilterOutput_t *p)
{
  lockMutex(db.locks.filter);
  memcpy(p, &db.data.filter, sizeof(FilterOutput_t));
  unlockMutex(db.locks.filter);
}

void DbDataFilterOutputSet(FilterOutput_t *p)
{
  lockMutex(db.locks.filter);
  memcpy(&db.data.filter, p, sizeof(FilterOutput_t));
  unlockMutex(db.locks.filter);
}

void DbDataImuGet(ImuData_t *p)
{
  lockMutex(db.locks.imu);
  memcpy(p, &db.data.imu, sizeof(ImuData_t));
  unlockMutex(db.locks.imu);
}

void DbDataImuSet(ImuData_t *p)
{
  lockMutex(db.locks.imu);
  memcpy(&db.data.imu, p, sizeof(ImuData_t));
  unlockMutex(db.locks.imu);
}

void DbDataBatteryGet(Battery_t *p)
{
  lockMutex(db.locks.battery);
  memcpy(p, &db.data.battery, sizeof(Battery_t));
  unlockMutex(db.locks.battery);
}

void DbDataBatterySet(Battery_t *p)
{
  lockMutex(db.locks.battery);
  memcpy(&db.data.battery, p, sizeof(Battery_t));
  unlockMutex(db.locks.battery);
}

void DbDataBoardGet(Board_t *p)
{
  lockMutex(db.locks.board);
  memcpy(p, &db.data.board, sizeof(Board_t));
  unlockMutex(db.locks.board);
}

void DbDataBoardSet(Board_t *p)
{
  lockMutex(db.locks.board);
  memcpy(&db.data.board, p, sizeof(Board_t));
  unlockMutex(db.locks.board);
}

/****************************** END OF FILE **********************************/
