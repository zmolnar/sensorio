
#include "Power.hpp"
#include <platform/Log.hpp>

static const char tag[]{"power"};

using namespace Platform;

void Power::start(void)
{
  Log::Info(tag) << "start";
}

void Power::startupFinished(void)
{
  Log::Info(tag) << "startup finished";
}

void Power::stop(void)
{
  Log::Info(tag) << "stop";
}
