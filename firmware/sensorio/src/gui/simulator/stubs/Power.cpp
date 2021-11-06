
#include "Power.h"
#include <platform/Log.hpp>

static const char tag[]{"power"};

using namespace Platform;

void PowerStart(void)
{
  Log::Info(tag) << "start";
}

void PowerStartupFinished(void)
{
  Log::Info(tag) << "startup finished";
}

void PowerStop(void)
{
  Log::Info(tag) << "stop";
}
