#ifndef include_keepalive_hpp
#define include_keepalive_hpp

#include "spec.hpp"

class KeepAliveData: public SensorData {
  using SensorData::SensorData;
public:
  uint32_t pulse;
  using Type = decltype(pulse);
};

class KeepAlive: public Sensor<KeepAlive, KeepAliveData, 500U> {
protected:
  void init() override {}
  void poll(msecu32_t const s) override {
    static int pulse = 0;
    KeepAliveData d(s);
    d.pulse = pulse++;
    push(d);
  }
  std::string name() override { return "KeepAlive"; }
};

#endif // include_keepalive_hpp
