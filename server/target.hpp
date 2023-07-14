#ifndef target_hpp
#define target_hpp

#include <accelerometer.h>
#include <barometer.h>
// #include <color.h>
// #include <gesture.h>
// #include <gyroscope.h>
#include <humidity.h>
// #include <magnetometer.h>
#include <microphone.h>
#include <proximity.h>
#include <temperature.h>

#include "scale.hpp"

class Target {
private:
  BLEService _sensors;
  BLEFloatCharacteristic _accelXChar;
  BLEFloatCharacteristic _accelYChar;
  BLEFloatCharacteristic _accelZChar;
  BLEFloatCharacteristic _barPsiChar;
  BLEFloatCharacteristic _precipChar;
  BLEIntCharacteristic   _proximChar;
  BLEFloatCharacteristic _airTmpChar;
  BLEFloatCharacteristic _h2oTmpChar;
  BLEFloatCharacteristic _weightChar;
protected:
  Accelerometer _accelerometer;
  Barometer _barometer;
  //Color _color;
  //Gesture _gesture;
  //Gyroscope _gyroscope;
  Humidity _humidity;
  //Magnetometer _magnetometer;
  Microphone _microphone;
  Proximity _proximity;
  Temperature _temperature;
  //Thermometer _thermometer;
  //Scale _scale;
public:
  Target();
  virtual ~Target();
  bool init();
  void update();
};

extern Target target;

#endif // target_hpp
