
#include <dashboard/NvsStorage.hpp>
#include <nvs.h>
#include <nvs_flash.h>
#include <platform/Log.hpp>

using namespace Config;
using namespace Platform;

const char *NvsStorage::NvsStorage::tag{"nvs-storage"};

bool NvsStorage::init()
{
  esp_err_t err = nvs_flash_init();
  if (err == ESP_ERR_NVS_NO_FREE_PAGES ||
      err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
    // NVS partition was truncated and needs to be erased. Retry nvs_flash_init
    ESP_ERROR_CHECK(nvs_flash_erase());
    err = nvs_flash_init();
  }

  if (ESP_OK != err) {
    Log::Error(tag) << "failed to initialize NVS";
  } else {
    Log::Error(tag) << "NVS initialized";
  }

  return ESP_OK != err;
}

bool NvsStorage::loadLength(size_t &length)
{
  bool error{true};

  nvs_handle_t handle{};
  if (ESP_OK == nvs_open("storage", NVS_READONLY, &handle)) {
    if (ESP_OK == nvs_get_u32(handle, "length", &length)) {
      Log::Info(tag) << "length is " << length << " bytes";
      error = false;
    } else {
      Log::Error(tag) << "length not found";
    }
    nvs_close(handle);
  } else {
    Log::Error(tag) << "failed to open storage";
  }

  return error;
}

bool NvsStorage::loadBlob(etl::vector_ext<uint8_t> &blob, size_t length)
{
  if (length <= blob.max_size()) {
    nvs_handle_t handle{};
    if (ESP_OK == nvs_open("storage", NVS_READONLY, &handle)) {
      blob.resize(length);
      if (ESP_OK == nvs_get_blob(handle, "blob", blob.data(), &length)) {
        Log::Info(tag) << "blob loaded (" << length << " bytes)";
      } else {
        Log::Error(tag) << "blob not found";
        blob.clear();
      }
      nvs_close(handle);
    } else {
      Log::Error(tag) << "failed to open storage";
    }
  } else {
    Log::Error(tag) << "buffer too small (" << blob.max_size() << " < "
                    << length << ")";
  }

  return (0 == blob.size());
}

bool NvsStorage::saveLength(size_t length)
{
  bool error{true};

  nvs_handle_t handle{};
  if (ESP_OK == nvs_open("storage", NVS_READWRITE, &handle)) {
    if (ESP_OK == nvs_set_u32(handle, "length", length)) {
      Log::Info(tag) << "length is set to " << length << " bytes";
      error = false;
    } else {
      Log::Error(tag) << "length not found";
    }
    nvs_close(handle);
  } else {
    Log::Error(tag) << "failed to open storage";
  }

  return error;
}

bool NvsStorage::saveBlob(const etl::vector_ext<uint8_t> &blob)
{
  bool error{true};

  nvs_handle_t handle{};
  if (ESP_OK == nvs_open("storage", NVS_READWRITE, &handle)) {
    if (ESP_OK == nvs_set_blob(handle, "blob", blob.data(), blob.size())) {
      Log::Info(tag) << "blob saved (" << blob.size() << " bytes)";
      error = false;
    } else {
      Log::Error(tag) << "blob not found";
    }
    nvs_close(handle);
  } else {
    Log::Error(tag) << "failed to open storage";
  }

  return error;
}

bool NvsStorage::load(etl::vector_ext<uint8_t> &blob)
{
  size_t length{0};
  return loadLength(length) || loadBlob(blob, length);
}

bool NvsStorage::save(const etl::vector_ext<uint8_t> &blob)
{
  return saveLength(blob.size()) || saveBlob(blob);
}
