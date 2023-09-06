#ifndef target_hpp
#define target_hpp

#include "keepalive.hpp"
#include "scale.hpp"
#include "char.hpp"

class Target {
private:
  BLEService _sensors;
  BLETaggedCharacteristic<KeepAliveData::Type> _pulsesChar;
  BLETaggedCharacteristic<AccelerometerData::Type> _accXYZChar;
  BLETaggedCharacteristic<BarometerData::Type> _barPsiChar;
  BLETaggedCharacteristic<HumidityData::Type> _precipChar;
  BLETaggedCharacteristic<ProximityData::Type> _proximChar;
  BLETaggedCharacteristic<TemperatureData::Type> _airTmpChar;
  // BLETaggedCharacteristic<ThermometerData::Type> _h2oTmpChar;
  BLETaggedCharacteristic<ScaleData::Type> _weightChar;
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
  Scale _scale;
public:
  Target();
  virtual ~Target();
  bool init();
  void update();
};

extern Target target;

#endif // target_hpp
