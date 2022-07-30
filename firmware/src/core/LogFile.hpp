
#ifndef LOG_FILE_H
#define LOG_FILE_H

#include <cstdint>
#include <functional>

#include <etl/mutex.h>
#include <etl/span.h>
#include <etl/string.h>
#include <etl/string_stream.h>

namespace LogFile {
  class Block {
    using GetStorage = std::function<etl::span<char>()>;
    using Procedure = std::function<void(void)>;

  public:
    const char *filename;
    GetStorage storage;
    Procedure lock;
    Procedure unlock;
    Procedure clear;

    Block() = default;
    Block(const char *filename) : filename{filename} {
    }
    void setStorage(GetStorage storage) {
      this->storage = storage;
    }
    void setLock(Procedure lock) {
      this->lock = lock;
    }
    void setUnlock(Procedure unlock) {
      this->unlock = unlock;
    }
    void setClear(Procedure clear) {
      this->clear = clear;
    }
  };

  template <uint32_t SIZE>
  class Buffer {
    etl::mutex mutex;
    const char *filename;
    etl::string<SIZE> storage;
    etl::string_stream ss{};

  public:
    Buffer(const char *filename) : filename{filename}, ss{storage} {
      storage.clear();
      ss << etl::setprecision(3);
    }

    Block getBlock(void) {
      Block block{filename};
      block.setStorage([this]() { return etl::span<char>{storage}; });
      block.setLock([this]() { this->lock(); });
      block.setUnlock([this]() { this->unlock(); });
      block.setClear([this]() { this->clear(); });

      return block;
    }

    void lock() {
      mutex.lock();
    }

    void unlock() {
      mutex.unlock();
    }

    char *data() {
      return storage.data() + storage.size();
    }

    size_t size() {
      return storage.size();
    }

    void alloc() {
      storage.resize(storage.capacity());
    }

    size_t available() {
      return storage.capacity() - storage.size();
    }

    void resize(size_t n) {
      storage.resize(n);
    }

    void clear() {
      storage.clear();
    }

    bool truncated() {
      return storage.truncated();
    }

    template <typename T>
    Buffer &operator<<(T &&value) {
      ss << value;
      return *this;
    }
  };

  template <uint32_t SIZE>
  class File {
  public:
    using BufferFullCb = std::function<void(Block &&)>;

  private:
    etl::mutex mutex;
    Buffer<SIZE> A;
    Buffer<SIZE> B;
    Buffer<SIZE> *bufInUse;
    BufferFullCb cb;

  public:
    File(const char *filename) : mutex{}, A{filename}, B{filename}, bufInUse{&A} {
    }

    void setCallback(BufferFullCb cb) {
      this->cb = cb;
    }

    void lock() {
      mutex.lock();
    }

    void unlock() {
      mutex.unlock();
    }

    Buffer<SIZE> &buffer() {
      return *bufInUse;
    }

    void swapBuffers() {
      bufInUse = (&A == bufInUse) ? &B : &A;
    }

    void sendBuffer(Buffer<SIZE> &buf) {
      if (nullptr != cb) {
        cb(buf.getBlock());
      }
    }

    template <typename T>
    File &operator<<(T &&value) {
      lock();

      auto &primary = (*bufInUse);

      primary.lock();
      auto size = primary.size();
      primary << value;

      if (primary.truncated()) {
        primary.resize(size);
        sendBuffer(primary);
        primary.unlock();

        swapBuffers();

        auto &secondary = (*bufInUse);
        secondary.lock();
        size = secondary.size();
        buffer() << value;
        if (secondary.truncated()) {
          secondary.resize(size);
          sendBuffer(secondary);
          secondary.unlock();

        } else {
          secondary.unlock();
        }
      } else {
        primary.unlock();
      }

      unlock();

      return *this;
    }
  };
}

#endif
