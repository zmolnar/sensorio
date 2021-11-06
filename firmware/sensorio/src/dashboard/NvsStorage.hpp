
#ifndef NVSSTORAGE_H
#define NVSSTORAGE_H

#include <dashboard/Config.hpp>

namespace Config {

  class NvsStorage : public AStorage {
    static const char *tag;
  public:
    bool init();
    bool load(etl::vector_ext<uint8_t> &blob) override final;
    bool save(const etl::vector_ext<uint8_t> &blob) override final;

  private:
    bool loadLength(size_t &length);
    bool loadBlob(etl::vector_ext<uint8_t> &blob, size_t length);
    bool saveLength(size_t length);
    bool saveBlob(const etl::vector_ext<uint8_t> &blob);
  };
}

#endif
