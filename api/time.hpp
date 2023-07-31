#ifndef time_hpp
#define time_hpp

#include "ticks.h"

// Millisecond ticks using native storage.
using msec_t = ticks<std::milli>;

// Millisecond ticks using fixed-width storage.
using msec32_t = ticks<uint32_t, std::milli>;

#endif // time_hpp
