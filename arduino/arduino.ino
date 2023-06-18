#include <Arduino.h>

#include "Target.h"

Target target;

void halt(const char *reason) {
	for (;; delay(1000)) {
	  errf("halt: %s", reason);
	}
}
	
void setup(void) {
#if IO_DEBUG_LOG
  (IO_DEBUG).begin(115200);
#endif
  if (!target.init()) { 
		halt("target.init() returned an error status");
	}
}

void loop(void) {
  target.update(millis());
}

void log(LogLevel level, bool trace, 
  const char *file, int line, const char *func, const char *fmt, ...) {
#if IO_DEBUG_LOG

  static const char *prefix[LogLevel::COUNT] = {
    "",      // Raw
    "[ ] ",  // Info
    "[*] ",  // Warn
    "[!] "   // Error
  };
  static char buff[LOG_MAX_BYTES + 1] = { 0 };
  static int step = 0; ++step;

  if (trace) {
    memset(buff, 0, LOG_MAX_BYTES + 1);
    snprintf(buff, LOG_MAX_BYTES, "%d~%s@%d:%s", step, file, line, func);
    (IO_DEBUG).print(prefix[level]);
    (IO_DEBUG).print(buff);
		(IO_DEBUG).print(WR_EOL);
  }

  if (nullptr != fmt) {
    va_list arg;
    va_start(arg, fmt);
    memset(buff, 0, LOG_MAX_BYTES + 1);
    vsnprintf(buff, LOG_MAX_BYTES, fmt, arg);
    va_end(arg);
    (IO_DEBUG).print(prefix[level]);
    (IO_DEBUG).print(buff);
		(IO_DEBUG).print(WR_EOL);
  }

#else
  unused(level);
  unused(trace);
  unused(file);
  unused(line);
  unused(func);
  unused(fmt);
#endif
}
