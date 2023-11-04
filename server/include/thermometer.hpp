#ifndef include_thermometer_hpp
#define include_thermometer_hpp

#include <MAX31865.hpp>

#include "wiring.hpp"
#include "spec.hpp"

class ThermometerData: public SensorData {
  using SensorData::SensorData;
public:
  float fahrenheit;
  using Type = decltype(fahrenheit);
};

class Thermometer: private MAX31865::IC<>, public Sensor<Thermometer, ThermometerData, 50U> {
protected:
  void init() {
    begin(THERMOMETER_IRQ_RDY_PIN, THERMOMETER_SPI_SEL_PIN, 3);
  }
  void poll(msecu32_t const s) override {
    ThermometerData d(s);
    d.fahrenheit = fahrenheit();
    push(d);
  }
  std::string name() override { return "Thermometer"; }
};

#endif // include_thermometer_hpp
