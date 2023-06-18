#ifndef spec_h
#define spec_h

// ----------------------------------------------------------------- INCLUDES --

#include <Arduino.h>

// ------------------------------------------------------------------ DEFINES --

#define IO_DEBUG       Serial
#define WR_EOL         "\r\n"

#if LV_USE_LOG
#define IO_DEBUG_LOG   1
#define IO_DEBUG_TRACE 1 // all non-Raw log messages include call site info
#endif

// If defined non-zero, do not call setAddrWindow (send CASET/PASET commands)
// unless the address coordinates are different from the previous call.
// This is an efficient optimization for ILI9xxx, but it seems to corrupt the
// signal on ST77xx controllers.
#ifdef ILI9341_DRIVER
//#define LCD_LAZY_ASET 1
#endif // ILI9341_DRIVER

// -------------------------------------------------------------------- TYPES --

typedef unsigned long msec_t; // millisecond duration ( Arduino.h: millis() )
typedef unsigned long usec_t; // microsecond duration ( Arduino.h: micros() )

class LogLevel {
public:
  enum Value: int {
    NONE        = -1,
    Raw,     // =  0,
    Info,    // =  1,
    Warn,    // =  2,
    Error,   // =  3,
    COUNT    // =  4,
  };
  LogLevel() = default;
  constexpr LogLevel(Value value): _value(value) {}
  constexpr operator Value() const { return _value; }
  explicit operator bool() const = delete;
private:
  Value _value;
};

// ---------------------------------------------------------------- CONSTANTS --

msec_t const MSECS_PER_TARGET_UPDATE =  5;
msec_t const MSECS_PER_LCD_UPDATE    = 10;
msec_t const MSECS_PER_SCALE_UPDATE  = 25;

uint16_t const LCD_WIDTH_PX        = 320U;
uint16_t const LCD_HEIGHT_PX       = 240U;

uint8_t  const LCD_ROTATION        = 3;
uint16_t const LCD_TOUCH_CAL[]     = { 409, 3480, 285, 3482, 1 };

uint8_t const SCALE_DATA_PIN       = 16;
uint8_t const SCALE_CLOCK_PIN      = 17;

float   const SCALE_FACTOR         =   450.53825F;
float   const SCALE_OFFSET         = 71708.73446F;
uint8_t const SCALE_SAMPLE         = 1U;

size_t const LOG_MAX_BYTES         = 1023LU; // +1 reserved for '\0'

// ------------------------------------------------------------------- MACROS --

#define unused(x)        ;((void)(x));
#define invoke(o,fn)     ((o).*(fn))
#define tvswap(type,a,b) { type c = (a); a = (b); b = (c); }

// Concatenate <per> to "prev_" for the static variable name so that multiple
// calls to throttle can be nested in a common scope.
// syntax of <per> must be a variable identifier, macro, or numeric literal.
#define throttle(fn,now,per) {         \
    static msec_t prev_ ## per = 0UL;  \
    if ((now) - prev_ ## per >= per) { \
      fn;                              \
      prev_ ## per = (now);            \
    }                                  \
  }

#if IO_DEBUG_TRACE
#define infof(fmt, ...)  log(LogLevel::Info, true, __FILE__, __LINE__, __PRETTY_FUNCTION__, fmt, __VA_ARGS__)
#define warnf(fmt, ...)  log(LogLevel::Warn, true, __FILE__, __LINE__, __PRETTY_FUNCTION__, fmt, __VA_ARGS__)
#define errf(fmt, ...)   log(LogLevel::Error, true, __FILE__, __LINE__, __PRETTY_FUNCTION__, fmt, __VA_ARGS__)
#else
#define infof(fmt, ...)  log(LogLevel::Info, false, nullptr, -1, nullptr, fmt, __VA_ARGS__)
#define warnf(fmt, ...)  log(LogLevel::Warn, false, nullptr, -1, nullptr, fmt, __VA_ARGS__)
#define errf(fmt, ...)   log(LogLevel::Error, false, nullptr, -1, nullptr, fmt, __VA_ARGS__)
#endif

#define logf(fmt, ...)   log(LogLevel::Raw, false, nullptr,  -1, nullptr, fmt, __VA_ARGS__)
#define tracef(fmt, ...) log(LogLevel::Raw, true, __FILE__, __LINE__, __PRETTY_FUNCTION__, fmt, __VA_ARGS__)

LV_FONT_DECLARE(beans_16);

#define BEANS_SCALE     "\xEF\x92\x96"
#define BEANS_THERMO    "\xEF\x9D\xA9"
#define BEANS_CLOCK     "\xEF\x80\x97"
#define BEANS_STOPWATCH "\xEF\x8B\xB2"
#define BEANS_FILTER    "\xEF\x82\xB0"
#define BEANS_MUG       "\xEF\x9E\xB6"

// ----------------------------------------------------------- FUNCTION DECLS --

extern void log(
    LogLevel level,
    bool trace,
    char const *filename,
    int lineno,
    char const *funcname,
    char const *fmt,
    ...
);

#endif // spec_h
