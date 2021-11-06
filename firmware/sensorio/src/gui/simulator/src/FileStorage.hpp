
#ifndef DUMMYSTORAGE_H
#define DUMMYSTORAGE_H

#include <dashboard/Config.hpp>
#include <fstream>
#include <iostream>
#include <platform/Log.hpp>

using namespace Platform;

namespace Config {

  class FileStorage : public AStorage {
    const char *tag{"file-storage"};
    const char *fileName{"/tmp/config.bin"};

  public:
    bool init()
    {
      Log::Info(tag) << fileName << " is used for storage";
      return false;
    }
    bool load(etl::vector_ext<uint8_t> &blob) override final
    {
      std::ifstream file{};
      file.open(fileName);
      blob.resize(getFileSize(file));
      file.read(reinterpret_cast<char *>(blob.data()), blob.size());
      file.close();

      Log::Info(tag) << blob.size() << " bytes read from " << fileName;
      return false;
    }

    bool save(const etl::vector_ext<uint8_t> &blob) override final
    {
      std::ofstream file{};
      file.open(fileName, std::ios::trunc);
      size_t before = file.tellp();
      file.write(reinterpret_cast<const char *>(blob.data()), blob.size());
      size_t after = file.tellp();
      file.close();

      Log::Info(tag) << (after - before) << " bytes written into " << fileName;
      return false;
    }

  private:
    size_t getFileSize(std::ifstream &file)
    {
      Platform::Assert::Assert(file.is_open());

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
