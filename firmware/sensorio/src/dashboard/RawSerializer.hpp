
#ifndef RAWSERIALIZER_H
#define RAWSERIALIZER_H

#include <dashboard/Config.hpp>

namespace Config {

  class RawSerializer : public ASerializer {
    static const char *tag;
  public:
    bool serialize(etl::vector_ext<uint8_t> &blob) override final;
    bool deserialize(const etl::vector_ext<uint8_t> &blob) override final;
  };
};

#endif
