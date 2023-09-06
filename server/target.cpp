#include "target.hpp"
#include "spec.hpp"

Target::Target():
  _sensors(UUID_BLE_SENSORS_SERVICE),
  _pulsesChar(UUID_BLE_SENSORS_PULSES_CHAR, BLE_SENSORS_CHAR_MODE),
  _accXYZChar(UUID_BLE_SENSORS_ACCXYZ_CHAR, BLE_SENSORS_CHAR_MODE),
  _barPsiChar(UUID_BLE_SENSORS_BARPSI_CHAR, BLE_SENSORS_CHAR_MODE),
  _precipChar(UUID_BLE_SENSORS_PRECIP_CHAR, BLE_SENSORS_CHAR_MODE),
  _proximChar(UUID_BLE_SENSORS_PROXIM_CHAR, BLE_SENSORS_CHAR_MODE),
  _airTmpChar(UUID_BLE_SENSORS_AIRTMP_CHAR, BLE_SENSORS_CHAR_MODE),
  // _h2oTmpChar(UUID_BLE_SENSORS_H2OTMP_CHAR, BLE_SENSORS_CHAR_MODE),
  _weightChar(UUID_BLE_SENSORS_WEIGHT_CHAR, BLE_SENSORS_CHAR_MODE) {
  pinMode(PIN_ENABLE_SENSORS_3V3, OUTPUT);
  pinMode(PIN_ENABLE_I2C_PULLUP, OUTPUT);
  digitalWrite(PIN_ENABLE_SENSORS_3V3, LOW);
  digitalWrite(PIN_ENABLE_I2C_PULLUP, LOW);
}

Target::~Target() {}

bool Target::init() {

  if (!BLE.begin()) {
    spanicf("target initialization failed: %s", "BLE.begin()");
  }
  swritef("%s", "radio initialized");

  BLE.setDeviceName(BLEServerName);
  BLE.setLocalName(BLEDeviceName);
  BLE.setAdvertisedService(_sensors);

  _sensors.addCharacteristic(_pulsesChar);
  _sensors.addCharacteristic(_accXYZChar);
  _sensors.addCharacteristic(_barPsiChar);
  _sensors.addCharacteristic(_precipChar);
  _sensors.addCharacteristic(_proximChar);
  _sensors.addCharacteristic(_airTmpChar);
  // _sensors.addCharacteristic(_h2oTmpChar);
  _sensors.addCharacteristic(_weightChar);

  BLE.addService(_sensors);
  BLE.advertise();
//   0x15 0x12 0x0552 Multi-Sensor
//   0x024 0x00 0x0900 Generic Domestic Appliance
//         0x08 0x0908 Coffee maker
//   0x032 0x00 0x0C80 Generic Weight Scale
// Generic Attribute service 0x1801
// 0x181D Weight Scale service
// acceleration (metres per second squared) 0x2713
// Celsius temperature (degree Celsius) 0x272F
// pressure (bar) 0x2780
// pressure (millimetre of mercury) 0x2781
// pressure (pascal) 0x2724
// percentage 0x27AD
// magnetic field strength (ampere per metre) 0x2719
// length (metre) 0x2701
// luminous flux (lumen) 0x2730
  swritef("%s", "wireless services initialized");

  rtos::ThisThread::sleep_for(100ms);
  digitalWrite(PIN_ENABLE_SENSORS_3V3, HIGH);
  digitalWrite(PIN_ENABLE_I2C_PULLUP, HIGH);

  _accelerometer.start();
  _barometer.start();
  // _color.start();
  // _gesture.start();
  // _gyroscope.start();
  _humidity.start();
  _keepAlive.start();
  // _magnetometer.start();
  // _microphone.start();
  _proximity.start();
  _temperature.start();
  // _thermometer.start();
  _scale.start();

  swritef("%s", "sensors initialized");

  return true;
}

void Target::update() {
  static BLEDevice central;
  if ((central = BLE.central())) {
    swritef("connected: %s", central.address());
    digitalWrite(LED_BUILTIN, HIGH);
    while (central.connected()) {
      AccelerometerData acc;
      if (_accelerometer.pop(acc)) {
        _accXYZChar.writeValue(Packet<AccelerometerData::Type>({acc.x, acc.y, acc.z}, acc.time()));
      }
      BarometerData bar;
      if (_barometer.pop(bar)) {
        _barPsiChar.writeValue(Packet<BarometerData::Type>(bar.psi, bar.time()));
      }
      //ColorData col;
      //if (_color.pop(col)) {}
      //GestureData ges;
      //if (_gesture.pop(ges)) {}
      //GyroscopeData gyr;
      //if (_gyroscope.pop(gyr)) {}
      HumidityData hum;
      if (_humidity.pop(hum)) {
        _precipChar.writeValue(Packet<HumidityData::Type>(hum.humidity, hum.time()));
      }
      KeepAliveData kee;
      if (_keepAlive.pop(kee)) {
        _pulsesChar.writeValue(Packet<KeepAliveData::Type>(kee.pulse, kee.time()));
      }
      //MagnetometerData mag;
      //if (_magnetometer.pop(mag)) {}
      //MicrophoneData mic;
      //if (_microphone.pop(mic)) {}
      ProximityData pro;
      if (_proximity.pop(pro)) {
        _proximChar.writeValue(Packet<ProximityData::Type>(pro.proximity, pro.time()));
      }
      TemperatureData tem;
      if (_temperature.pop(tem)) {
        _airTmpChar.writeValue(Packet<TemperatureData::Type>(tem.fahrenheit, tem.time()));
      }
      //ThermometerData the;
      //if (_thermometer.pop(the)) {
      //  _h2oTmpChar.writeValue(Packet<ThermometerData::Type>(the.fahrenheit, the.time()));
      //}
      ScaleData sca;
      if (_scale.pop(sca)) {
        Packet<ScaleData::Type> pkt(sca.read, sca.time());
        swritef("%s", "≡===≡===≡===≡===≡===≡===≡===≡===≡===≡===≡===≡===≡===≡===≡");
        pkt.describe();
        _weightChar.writeValue(std::move(pkt));
      }
    }
    digitalWrite(LED_BUILTIN, LOW);
    swritef("disconnected: %s", central.address());
  }
}
