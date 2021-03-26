
#ifndef ENV_H
#define ENV_H

#if defined(PCSIM)
#include <cassert>
#define ASSERT assert
#else
#define ASSERT
#endif

#endif