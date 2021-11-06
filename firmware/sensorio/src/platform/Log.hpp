
#ifndef LOG_H
#define LOG_H

#include <etl/string_stream.h>
#include <platform/Assert.hpp>

namespace Platform {
  namespace Log {

    enum class Level { ERROR, INFO, DEBUG };

    class Base {
      static Level level;

    protected:
      bool enabled{false};
      const char *tag;
      etl::string<128> msg {};
      etl::string_stream ss{msg};

      Base(const char *tag, Level level) :
          enabled(level <= Base::level), tag(tag)
      {
        Platform::Assert::Assert(nullptr != tag);
      }

      virtual ~Base(){};

    public:
      static void setLevel(Level level)
      {
        Base::level = level;
      }

      template <typename T>
      Base &operator<<(const T &v)
      {
        if (enabled) {
          ss << v;
        }
        return (*this);
      }
    };

    class Error : public Base {
    public:
      Error(const char *tag) : Base(tag, Level::ERROR)
      {
      }
      ~Error();
    };

    class Info : public Base {
    public:
      Info(const char *tag) : Base(tag, Level::INFO)
      {
      }
      ~Info();
    };

    class Debug : public Base {
    public:
      Debug(const char *tag) : Base(tag, Level::DEBUG)
      {
      }
      ~Debug();
    };
  }
}

#endif
