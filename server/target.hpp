#ifndef target_hpp
#define target_hpp

#include "keepalive.hpp"
#include "scale.hpp"
#include "char.hpp"

class Target {
private:
  BLEService _sensors;
  BLETaggedCharacteristic<KeepAliveData::GattType> _pulsesChar;
  BLETaggedCharacteristic<AccelerometerData::GattType> _accelXChar;
  BLETaggedCharacteristic<AccelerometerData::GattType> _accelYChar;
  BLETaggedCharacteristic<AccelerometerData::GattType> _accelZChar;
  BLETaggedCharacteristic<BarometerData::GattType> _barPsiChar;
  BLETaggedCharacteristic<HumidityData::GattType> _precipChar;
  BLETaggedCharacteristic<ProximityData::GattType> _proximChar;
  BLETaggedCharacteristic<TemperatureData::GattType> _airTmpChar;
  // BLETaggedCharacteristic<ThermometerData::GattType> _h2oTmpChar;
  BLETaggedCharacteristic<ScaleData::GattType> _weightChar;
protected:
  Accelerometer _accelerometer;
  Barometer _barometer;
  //Color _color;
  //Gesture _gesture;
  //Gyroscope _gyroscope;
  Humidity _humidity;
  KeepAlive _keepAlive;
  //Magnetometer _magnetometer;
  //Microphone _microphone;
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
