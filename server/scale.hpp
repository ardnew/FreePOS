#ifndef scale_hpp
#define scale_hpp

#include <HX711.h>
#include <sensor.h>

#include "spec.hpp"

class ScaleData: public SensorData {
  using SensorData::SensorData;
public:
  float read;
  float zero;
};

class Scale: private HX711, public Sensor<Scale, ScaleData, 25U> {
private:
  float _zero;
public:
  void zero(float val = 0.0F) { _zero = val; };
protected:
  void init() override {
    _zero = 0U;
    begin(SCALE_DATA_PIN, SCALE_CLOCK_PIN);
    set_scale(SCALE_FACTOR);
    set_offset(SCALE_OFFSET);
    tare();
  }
  void poll(msec_t const s) override {
    ScaleData d(s);
    d.read = get_units(SCALE_SAMPLE);
    d.zero = _zero;
    push(d);
  }
};

#endif // scale_hpp
