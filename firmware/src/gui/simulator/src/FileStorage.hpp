
#ifndef DUMMYSTORAGE_H
#define DUMMYSTORAGE_H

#include <dashboard/Config.hpp>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <platform/Log.hpp>

using namespace Platform;

namespace Config {

  class FileStorage : public AStorage {
    const char *tag{"file-storage"};
    const char *fileName{"/tmp/sensorio-simulator-config.bin"};

  public:
    bool init()
    {
      Log::Info(tag) << fileName << " is used for storage";
      return false;
    }
    bool load(etl::vector_ext<uint8_t> &blob) override final
    {
      std::ifstream file{fileName, std::ios::binary};
      if (!file.is_open()) {
        Log::Info(tag) << fileName << " does not exist";
        return true;
      }

      size_t fileSize = getFileSize(file);
      if (blob.max_size() < fileSize) {
        Log::Error(tag) << "config file is too large";
        return true;
      }

      blob.resize(fileSize);
      file.read(reinterpret_cast<char *>(blob.data()), blob.size());
      bool error = file.fail();
      file.close();

      Log::Info(tag) << blob.size() << " bytes read from " << fileName;
      return error;
    }

    bool save(const etl::vector_ext<uint8_t> &blob) override final
    {
      std::ofstream file{fileName, std::ios::binary | std::ios::trunc};
      if (!file.is_open()) {
        Log::Error(tag) << "failed to open " << fileName;
        return true;
      }

      std::streampos before = file.tellp();
      file.write(reinterpret_cast<const char *>(blob.data()), blob.size());
      std::streampos after = file.tellp();
      bool error = file.fail();
      file.close();

      Log::Info(tag) << (after - before) << " bytes written into " << fileName;
      return error;
    }

  private:
    size_t getFileSize(std::ifstream &file)
    {
      file.seekg(0, std::ios::beg);
      auto start{file.tellg()};
      file.seekg(0, std::ios::end);
      auto end{file.tellg()};
      file.seekg(0, std::ios::beg);

      return static_cast<size_t>(end - start);
    }
  };
}

#endif
