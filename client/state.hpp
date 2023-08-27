#ifndef state_hpp
#define state_hpp

#include "spec.hpp"
#include <esp_task_wdt.h>
class State {
public:
  enum Enum: uint8_t {
    Disable = 0,
    Disconnected,
    Reset,
    ScanStart,
    ScanInProgress,
    ScanSuccess,
    ConnectStart,
    ConnectSuccess,
    ConnectFailure,
    RegisterStart,
    RegisterCallbacks,
    RegisterSuccess,
    RegisterFailure,
    PairedAndReady,
    NumStates,
  };
  State() = delete;
  constexpr State(StatusLED &led): _enum(Disable), _led(led) {
  }
  constexpr operator Enum() const { return _enum; }
  void operator=(const Enum &e) {
    if (e == _enum) { return; }
    _enum = e;
    // swritef("State = %d\n", _enum);
    _show[_enum](_led);
  }

private:
  Enum _enum;
  StatusLED &_led;
  static const std::function<void(StatusLED &led)> _show[NumStates];
};

const std::function<void(StatusLED &led)> State::_show[State::Enum::NumStates] = {
  [](StatusLED &led) { // Disable
    led.fixed(COLOR_RED, 0x20);
  },
  [](StatusLED &led) { // Disconnected
    led.blink(COLOR_ORANGE, 0x20, 100, 50);
  },
  [](StatusLED &led) { // Reset
    led.pulse(COLOR_WHITE, 0x20, 0x10, 1);
  },
  [](StatusLED &led) { // ScanStart
    led.fixed(COLOR_NAVY, 0x20);
  },
  [](StatusLED &led) { // ScanInProgress
    led.pulse(COLOR_NAVY, 0x20, 0x10, 1);
  },
  [](StatusLED &led) { // ScanSuccess
    led.fixed(COLOR_BLUE, 0xFF);
  },
  [](StatusLED &led) { // ConnectStart
    led.pulse(COLOR_BLUE, 0x20, 0x10, 2);
  },
  [](StatusLED &led) { // ConnectSuccess
    led.fixed(COLOR_CYAN, 0xFF);
  },
  [](StatusLED &led) { // ConnectFailure
    led.blink(COLOR_PINK, 0x20, 100, 50);
  },
  [](StatusLED &led) { // RegisterStart
    led.fixed(COLOR_CYAN, 0x20);
  },
  [](StatusLED &led) { // RegisterCallbacks
    led.pulse(COLOR_CYAN, 0x20, 0x5, 2);
  },
  [](StatusLED &led) { // RegisterSuccess
    led.fixed(COLOR_GREEN, 0xFF);
  },
  [](StatusLED &led) { // RegisterFailure
    led.pulse(COLOR_PURPLE, 0x20, 0x10, 2);
  },
  [](StatusLED &led) { // PairedAndReady
    led.pulse(COLOR_GREEN, 0x40, 0x3, 1);
  },
};

#endif // state_hpp
