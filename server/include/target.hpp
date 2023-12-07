#ifndef include_target_hpp
#define include_target_hpp

#include "spec.hpp"

#if defined(FREEPOS_LINK_BLE)
  #include <ble/link.hpp>
#elif defined(FREEPOS_LINK_HID)
  #include <hid/link.hpp>
#endif

class Target: private link::Hub {
protected:
  Accelerometer _accelerometer;
  //Barometer _barometer;
  //Color _color;
  //Gesture _gesture;
  //Gyroscope _gyroscope;
  //Humidity _humidity;
  //KeepAlive _keepAlive;
  //Magnetometer _magnetometer;
  //Microphone _microphone;
  //Proximity _proximity;
  //Scale _scale;
  //Temperature _temperature;
  //Thermometer _thermometer;

public:
  Target();
  virtual ~Target();
  bool init();
  void update();
};

extern Target target;

#endif // include_target_hpp
