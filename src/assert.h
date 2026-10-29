#pragma once

#ifdef DEBAG
#include "kernel_panic.h"

#define assert(condition)                                                                                              \
  if (!(condition)) {                                                                                                            \
    kernel_panic("Assert fail: %s at (%s:%d)\n", #condition, __FILE__, __LINE__); \                                                                                                      \
  }
#else
#define assert(condition)
#endif