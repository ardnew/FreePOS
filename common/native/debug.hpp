#ifndef common_native_debug_hpp
#define common_native_debug_hpp

#include "native/core.hpp"

// static_assert but with message constructed automatically from expression.
// Example:
//   compiler_assert(sizeof(void*) == sizeof(char));
//     -> "static assertion failed: sizeof(void*) == sizeof(char)"
#define compiler_assert(...) static_assert(__VA_ARGS__, #__VA_ARGS__)

// Four different options for the DEBUG macro:
//   0: Disable,
//   1: Enable,
//   undef:
//     CORE_DEBUG_LEVEL (ESP-IDF/Arduino) if DEBUG_SYNC is also undef
//     Enable if DEBUG_SYNC is defined
// #define DEBUG 1
// #define DEBUG_SYNC 1

// If you undefined DEBUG & DEBUG_SYNC above, you can enable/disable debug
// statements via ESP-IDF or Arduino Board Configuration (IDE/CLI, VS Code
// extension, etc.).
#ifndef DEBUG
  #ifdef DEBUG_SYNC
    #define DEBUG 1
  #else
    #define DEBUG CORE_DEBUG_LEVEL
  #endif
#endif

#ifdef ARDUINO
  #ifdef ARDUINO_ARCH_MBED
    #define SerialDebug (SerialUSB) // debug I/O stream (USB CDC)
  #else                           // ! ARDUINO_ARCH_MBED
    #define SerialDebug (Serial)    // debug I/O stream (UART)
  #endif                          // ! ARDUINO_ARCH_MBED
#endif

// Both Arduino Serial Monitor and GNU Screen tend to respect CR+LF as newline
// in more cases than LF alone.
#define EOL "\r\n"

#define __stracef(pre, fmt, ...)                                               \
  native::writef(SerialDebug, "%s%s@%s:%d", pre, __PRETTY_FUNCTION__,          \
                 __FILE__, __LINE__);                                          \
  native::writef(SerialDebug, fmt, __VA_ARGS__);
#define sputf(fmt, ...) native::putf(SerialDebug, fmt, __VA_ARGS__)
#define swritef(fmt, ...) native::writef(SerialDebug, fmt, __VA_ARGS__)
#define stracef(fmt, ...) __stracef("", fmt, __VA_ARGS__)
#define spanicf(fmt, ...)                                                      \
  __stracef("(PANIC)=", fmt, __VA_ARGS__);                                     \
  exit(-1);

namespace native {

#if DEBUG
template <typename... Args>
size_t putf(Stream &out, const char *fmt, Args... args) {
  // Cannot rely on the target implementation of Stream to provide printf
  size_t size = snprintf(nullptr, 0, fmt, args...);
  std::string str;
  str.reserve(size + 1);
  str.resize(size);
  snprintf(&str[0], size + 1, fmt, args...);
  return out.write(str.c_str(), size);
}
#else
template <typename... Args> size_t putf(Stream &, const char *, Args...) {
  return 0;
}
#endif

template <typename... Args>
size_t writef(Stream &out, const char *fmt, Args... args) {
  static constexpr char eol[] = EOL;
  return putf(out, fmt, args...) + out.write(eol, sizeof(eol) - 1);
}

struct log {
  static void init() {
#if DEBUG
    SerialDebug.begin(115200UL);
  #if DEBUG_SYNC
    while (!SerialDebug) {
      delay(10);
    }
  #endif // DEBUG_SYNC
#endif // DEBUG
  }
};

} // namespace native

#endif // common_native_debug_hpp
