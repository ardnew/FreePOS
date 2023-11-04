#ifndef include_scale_hpp
#define include_scale_hpp

#include <HX711.h>

#include "wiring.hpp"
#include "spec.hpp"

class ScaleData: public SensorData {
  using SensorData::SensorData;
public:
  float read;
  using Type = decltype(read);
};

class Scale: private HX711, public Sensor<Scale, ScaleData, 25U> {
protected:
  void init() override {
    begin(SCALE_GPIO_DAT_PIN, SCALE_GPIO_CLK_PIN);
    set_scale(SCALE_FACTOR);
    set_offset(SCALE_OFFSET);
    tare();
  }
  void poll(msecu32_t const s) override {
    ScaleData d(s);
    d.read = get_units(SCALE_SAMPLE);
    push(d);
  }
  std::string name() override { return "Scale"; }
};

#endif // include_scale_hpp
