#ifndef debug_hpp
#define debug_hpp

#include <string>
#include <chrono>
#include <stdio.h> // snprintf

// static_assert but with message constructed automatically from expression.
// Example:
//   compiler_assert(sizeof(void*) == sizeof(char));
//     -> "static assertion failed: sizeof(void*) == sizeof(char)"
#define compiler_assert(...) \
  static_assert(__VA_ARGS__, #__VA_ARGS__)

// Three different options for the DEBUG macro:
//   0=Disable, 1=Enable, undef=CORE_DEBUG_LEVEL (ESP-IDF/Arduino)
#define DEBUG 1

// If you undefined DEBUG above, you can enable/disable debug statements via
// ESP-IDF or Arduino Board Configuration (IDE/CLI, VS Code extension, etc.).
#ifndef DEBUG
#define DEBUG CORE_DEBUG_LEVEL
#endif

#ifdef ARDUINO
#define SIO (Serial) // debug I/O stream (USB CDC)
#endif

// Both Arduino Serial Monitor and GNU Screen tend to respect CR+LF as newline
// in more cases than LF alone.
#define EOL "\r\n"

#if DEBUG
#define __stracef(pre, fmt, ...) \
  writef(SIO, "%s%s@%s:%d", pre, __PRETTY_FUNCTION__, __FILE__, __LINE__); \
  writef(SIO, fmt, __VA_ARGS__);
#define swritef(fmt, ...) writef(SIO, fmt, __VA_ARGS__)
#define stracef(fmt, ...) __stracef("", fmt, __VA_ARGS__)
#define spanicf(fmt, ...) __stracef("(PANIC)=", fmt, __VA_ARGS__); exit(-1);
#else
#undef DEBUG_SYNC
#define __stracef(...) /**/
#define swritef(...) /**/
#define stracef(...) /**/
#define spanicf(...) exit(-1);
#endif

template <typename... Args>
size_t writef(Stream& out, const char* fmt, Args... args) {
  static constexpr char eol[] = EOL;
  // Cannot rely on the target implementation of Stream to provide printf
  size_t size = snprintf(nullptr, 0, fmt, args...);
  std::string str;
  str.reserve(size + 1);
  str.resize(size);
  snprintf(&str[0], size + 1, fmt, args...);
  return out.write(str.c_str(), size) + out.write(eol);
}

struct Debug {
  static void init() {
#if DEBUG
    SIO.begin(115200);
#if DEBUG_SYNC
    while (!SIO) { delay(10); }
#endif // DEBUG_SYNC
#endif // DEBUG
  }
};

#endif // debug_hpp
