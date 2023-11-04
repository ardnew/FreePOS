#ifndef src_max31865_hpp
#define src_max31865_hpp

#include <SPI.h>

namespace MAX31865 {

// Standard platinum resistance thermometer (SPRT) temperature calculations
// with the following class template parameters:
//   NRef: nominal resistance at 0°C      PT100=100Ω, PT1000=1000Ω
//   RRef: reference resistance at 0°C    PT100=430Ω, PT1000=4300Ω (typical)
template <uint16_t NRef, uint16_t RRef>
struct SPRT {
  static constexpr float from_adc(
      const uint16_t adc,
      const float A = 3.9083e-3F,
      const float B = -5.775e-7F) {
    float r = adc;
    r /= 32768.0F;
    r *= (float)RRef;

    float z1 = -A;
    float z2 = A * A - (4 * B);
    float z3 = (4 * B) / (float)NRef;
    float z4 = 2 * B;

    float t = z2 + (z3 * r);
    t = (sqrt(t) + z1) / z4;

    if (t >= 0)
      return t;

    // ugh.
    r /= NRef;
    r *= 100; // normalize to 100 ohm

    float rpoly = r;

    t = -242.02;
    t += 2.2228 * rpoly;
    rpoly *= r; // square
    t += 2.5859e-3 * rpoly;
    rpoly *= r; // ^3
    t -= 4.8260e-6 * rpoly;
    rpoly *= r; // ^4
    t -= 2.8183e-8 * rpoly;
    rpoly *= r; // ^5
    t += 1.5243e-10 * rpoly;

    return t;
  }
};

struct PT100:  public SPRT<100, 430> {};
struct PT1000: public SPRT<1000, 4300> {};

class Reg {
protected:
  using A8 = uint8_t;   // register address
  using A16 = uint8_t;  //  " (pair-wise)
  using V8 = uint8_t;   // register value
  using V16 = uint16_t; //  " (pair-wise)

  static const A8 _reg_config     = 0x00;
  static const A8 _reg_rtd_msb    = 0x01;
  static const A8 _reg_rtd_lsb    = 0x02;
  static const A8 _reg_hfault_msb = 0x03;
  static const A8 _reg_hfault_lsb = 0x04;
  static const A8 _reg_lfault_msb = 0x05;
  static const A8 _reg_lfault_lsb = 0x06;
  static const A8 _reg_fault_stat = 0x07;

  static const A16 _reg_rtd       = 0x01;
  static const A16 _reg_hfault    = 0x03;
  static const A16 _reg_lfault    = 0x05;

  static const V8 _config_bias      = 0x80;
  static const V8 _config_biasoff   = 0x00;
  static const V8 _config_modeauto  = 0x40;
  static const V8 _config_modeoff   = 0x00;
  static const V8 _config_1shot     = 0x20;
  static const V8 _config_1shotoff  = 0x00;
  static const V8 _config_numwires  = 0x10;
  static const V8 _config_3wire     = 0x10;
  static const V8 _config_24wire    = 0x00;
  static const V8 _config_cycleman2 = 0x0C;
  static const V8 _config_cycleman1 = 0x08;
  static const V8 _config_cycleauto = 0x04;
  static const V8 _config_cyclenone = 0x00;
  static const V8 _config_faultstat = 0x02;
  static const V8 _config_faultclr  = 0x02;
  static const V8 _config_filter    = 0x01;
  static const V8 _config_filt50hz  = 0x01;
  static const V8 _config_filt60hz  = 0x00;

  static const V8 _fault_highthresh = 0x80;
  static const V8 _fault_lowthresh  = 0x40;
  static const V8 _fault_refinlow   = 0x20;
  static const V8 _fault_refinhigh  = 0x10;
  static const V8 _fault_rtdinlow   = 0x08;
  static const V8 _fault_voverunder = 0x04;
};

template <typename Milliseconds>
using IdleFunc = std::function<void(Milliseconds)>;

template <class _SPI = SPIClass, class _SPIConfig = SPISettings,
  typename Milliseconds = unsigned long>
class IC: private Reg {
public:
  // Default constructor requires no arguments because anything we can't assume
  // reasonable defaults for is provided via method begin(...).
  IC(
    _SPI &bus = SPI,
    _SPIConfig &&busConfig =
        std::forward<_SPIConfig>(_SPIConfig(1000000UL, MSBFIRST, SPI_MODE1)),
    IdleFunc<Milliseconds> &&idle =
        std::forward<IdleFunc<Milliseconds>>(
#if defined(ARDUINO)
  // Use a platform-specific non-blocking delay function if available.
  #if defined(ARDUINO_ARCH_MBED)
          IdleFunc<Milliseconds>([](Milliseconds ms) {
            rtos::ThisThread::sleep_for(std::chrono::milliseconds(ms));
          })
  #elif defined(ESP_PLATFORM)
          IdleFunc<Milliseconds>([](Milliseconds ms) {
            vTaskDelay(pdMS_TO_TICKS(ms));
          })
  #else
          // Arduino uses a busy-wait loop. Not ideal, but it works.
          #warning "MAX31856: No idle function provided! Using Arduino delay()."
          IdleFunc<Milliseconds>([](Milliseconds ms) { delay(ms); })
  #endif
#else // ARDUINO
          // TODO: implement a busy-wait loop for generic platforms
          #warning "MAX31856: No idle function provided!"
          IdleFunc<Milliseconds>([](Milliseconds ms) {})
#endif // !ARDUINO
        )):
      _bus(bus), _busConfig(busConfig), _idle(idle),
      _rdyPin(-1), _spiPin(-1), _numWires(-1), _mode(0) {}
  void begin(const int rdyPin, const int spiPin, const int numWires) {
    _rdyPin = rdyPin;
    _spiPin = spiPin;
    _numWires = numWires;
    _mode = (
      // Initial configuration
      _config_biasoff |   //   bit 7: Vbias:                   0 (off)
      _config_modeoff |   //   bit 6: conversion mode:         0 (normally-off)
      _config_1shotoff |  //   bit 5: 1-shot:                  0 (off)
      (numWires == 3 ?    //   bit 4: 3-wire select:           <per argument numWires>
        _config_3wire :   //     "
        _config_24wire) | //     "
      _config_cyclenone | //   bit 3-2: fault detection cycle: 0 (none)
      _config_faultclr |  //   bit 1: fault status clear:      1 (clear any fault)
      _config_filt60hz    //   bit 0: 50/60 Hz filter select:  0 (60 Hz)
    );
    // Disable IC, enable SPI
    pinMode(_spiPin, OUTPUT);
    digitalWrite(_spiPin, HIGH);
    _bus.begin();
    // Initialize lo, hi threshold registers
    limit(0x0000, 0xFFFF);
    // Initialize configuration register
    write8(_reg_config, _mode);
  }

  void idle(Milliseconds const ms) { _idle(ms); }

  // Clear fault status
  void clear() { write8(_reg_config, _config_faultclr); }
  uint8_t fault() { return read8(_reg_fault_stat); }

  uint8_t check(Reg::V8 const cycle) {
    if (_config_cyclenone != cycle) {
      uint8_t c = read8(_reg_config);
      c &= _config_numwires | _config_filter;
      write8(_reg_config, c | cycle);
      if (_config_cycleauto != cycle) {
        return 0;
      }
      idle(1);
    }
    return fault();
  }

  void limit(Reg::V16 const lo, Reg::V16 const hi) {
    write16(_reg_lfault, lo);
    write16(_reg_hfault, hi);
  }

  float celsius() {
    return PT100::from_adc(resistance());
  }

  float fahrenheit() {
    return celsius() * 9.0F / 5.0F + 32.0F;
  }

  float resistance() {
    clear();
    uint8_t c = read8(_reg_config) & ~(_config_bias | _config_1shot);
    write8(_reg_config, c | _config_bias);
    idle(10);
    write8(_reg_config, c | _config_bias | _config_1shot);
    idle(65);
    uint16_t rtd = read16(_reg_rtd);
    write8(_reg_config, c); // disable bias (1-shot auto-clears)
    return rtd >> 1; // clear fault status
  }

private:
  _SPI &_bus;
  _SPIConfig &_busConfig;
  IdleFunc<Milliseconds> &_idle;

  int _rdyPin, _spiPin;
  int _numWires;
  uint8_t _mode;

  // read8 returns 1 byte from register address r.
  uint8_t read8(Reg::A8 const r) {
    uint8_t v;
    _bus.beginTransaction(_busConfig);
    digitalWrite(_spiPin, LOW);
    _bus.transfer(r & 0x7F);
    v = _bus.transfer(0x00);
    digitalWrite(_spiPin, HIGH);
    _bus.endTransaction();
    return v;
  }

  // read16 returns 2 bytes from register address r. MSB is read first.
  uint16_t read16(Reg::A16 const r) {
    uint8_t msb, lsb;
    _bus.beginTransaction(_busConfig);
    digitalWrite(_spiPin, LOW);
    _bus.transfer(r & 0x7F);
    msb = _bus.transfer(0x00);
    lsb = _bus.transfer(0x00);
    digitalWrite(_spiPin, HIGH);
    _bus.endTransaction();
    return (msb << 8) | lsb;
  }

  // write8 copies 1 byte from v to register address r.
  void write8(Reg::A8 const r, Reg::V8 const v) {
    _bus.beginTransaction(_busConfig);
    digitalWrite(_spiPin, LOW);
    _bus.transfer(r | 0x80);
    _bus.transfer(v);
    digitalWrite(_spiPin, HIGH);
    _bus.endTransaction();
  }

  // write16 copies 2 bytes from v to register address r. MSB is copied first.
  void write16(Reg::A16 const r, Reg::V16 const v) {
   _bus.beginTransaction(_busConfig);
    digitalWrite(_spiPin, LOW);
    _bus.transfer(r | 0x80);
    _bus.transfer(v >> 8);
    _bus.transfer(v & 0xFF);
    digitalWrite(_spiPin, HIGH);
    _bus.endTransaction();
  }
};

} // namespace MAX31865

#endif // src_max31865_hpp
