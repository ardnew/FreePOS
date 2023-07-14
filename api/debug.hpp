#ifndef debug_hpp
#define debug_hpp

#include <string>
#include <stdio.h> // snprintf

#define DEBUG 1

#ifndef DEBUG
#define DEBUG CORE_DEBUG_LEVEL
#endif

#ifdef ARDUINO
#define SIO (Serial) // debug I/O stream (USB CDC)
#endif
#define EOL "\r\n"

#define DEBUG_SYNC // wait until host connects to SIO

#if DEBUG
#define __stracef(pre, fmt, ...) \
  writef(SIO, "%s%s@%s:%d", pre, __PRETTY_FUNCTION__, __FILE__, __LINE__); \
  writef(SIO, fmt, __VA_ARGS__);
#define swritef(fmt, ...) writef(SIO, fmt, __VA_ARGS__)
#define stracef(fmt, ...) __stracef("", fmt, __VA_ARGS__)
#define spanicf(fmt, ...) __stracef("(PANIC)=", fmt, __VA_ARGS__); exit(-1);
#else
#undef DEBUG_SYNC
#define __stracef(pre, fmt, ...) /**/
#define swritef(fmt, ...) /**/
#define stracef(fmt, ...) /**/
#define spanicf(fmt, ...) exit(-1);
#endif

template<typename... Args>
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

#endif // debug_hpp
