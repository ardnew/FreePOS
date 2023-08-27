#ifndef keepalive_hpp
#define keepalive_hpp

#include "spec.hpp"

class KeepAliveData: public SensorData {
  using SensorData::SensorData;
public:
  uint32_t pulse;
  using GattType = decltype(pulse);
};

class KeepAlive: public Sensor<KeepAlive, KeepAliveData, 1000U> {
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

#endif // keepalive_hpp
