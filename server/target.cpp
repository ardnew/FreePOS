#include "target.hpp"
#include "spec.hpp"

Target::Target():
  _sensors(BLE_SENSORS_SERVICE_UUID),
  _accelXChar(BLE_SENSORS_ACCELX_CHAR_UUID, BLE_SENSORS_CHAR_FLAG),
  _accelYChar(BLE_SENSORS_ACCELY_CHAR_UUID, BLE_SENSORS_CHAR_FLAG),
  _accelZChar(BLE_SENSORS_ACCELZ_CHAR_UUID, BLE_SENSORS_CHAR_FLAG),
  _barPsiChar(BLE_SENSORS_BARPSI_CHAR_UUID, BLE_SENSORS_CHAR_FLAG),
  _precipChar(BLE_SENSORS_PRECIP_CHAR_UUID, BLE_SENSORS_CHAR_FLAG),
  _proximChar(BLE_SENSORS_PROXIM_CHAR_UUID, BLE_SENSORS_CHAR_FLAG),
  _airTmpChar(BLE_SENSORS_AIRTMP_CHAR_UUID, BLE_SENSORS_CHAR_FLAG),
  _h2oTmpChar(BLE_SENSORS_H2OTMP_CHAR_UUID, BLE_SENSORS_CHAR_FLAG),
  _weightChar(BLE_SENSORS_WEIGHT_CHAR_UUID, BLE_SENSORS_CHAR_FLAG) {
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

  BLE.setDeviceName(BLE_DEVICE_NAME);
  BLE.setLocalName(BLE_LOCAL_NAME);
  BLE.setAdvertisedService(_sensors);

  _sensors.addCharacteristic(_accelXChar);
  _sensors.addCharacteristic(_accelYChar);
  _sensors.addCharacteristic(_accelZChar);
  _sensors.addCharacteristic(_barPsiChar);
  _sensors.addCharacteristic(_precipChar);
  _sensors.addCharacteristic(_proximChar);
  _sensors.addCharacteristic(_airTmpChar);
  _sensors.addCharacteristic(_h2oTmpChar);
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

  delay(100);
  digitalWrite(PIN_ENABLE_SENSORS_3V3, HIGH);
  digitalWrite(PIN_ENABLE_I2C_PULLUP, HIGH);

  _accelerometer.start();
  _barometer.start();
  // _color.start();
  // _gesture.start();
  // _gyroscope.start();
  _humidity.start();
  // _magnetometer.start();
  _proximity.start();
  _temperature.start();
  // _thermometer.start();
  // _scale.start();

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
        _accelXChar.writeValue(
          Packet<decltype(acc.x)>(acc.x, acc.time()));
        _accelYChar.writeValue(
          Packet<decltype(acc.y)>(acc.y, acc.time()));
        _accelZChar.writeValue(
          Packet<decltype(acc.z)>(acc.z, acc.time()));
      }
      BarometerData bar;
      if (_barometer.pop(bar)) {
        _barPsiChar.writeValue(
          Packet<decltype(bar.psi)>(bar.psi, bar.time()));
      }
      //ColorData col;
      //if (_color.pop(col)) {}
      //GestureData ges;
      //if (_gesture.pop(ges)) {}
      //GyroscopeData gyr;
      //if (_gyroscope.pop(gyr)) {}
      HumidityData hum;
      if (_humidity.pop(hum)) {
        _precipChar.writeValue(
          Packet<decltype(hum.humidity)>(hum.humidity, hum.time()));
      }
      //MagnetometerData mag;
      //if (_magnetometer.pop(mag)) {}
      ProximityData pro;
      if (_proximity.pop(pro)) {
        _proximChar.writeValue(
          Packet<decltype(pro.proximity)>(pro.proximity, pro.time()));
      }
      TemperatureData tem;
      if (_temperature.pop(tem)) {
        _airTmpChar.writeValue(
          Packet<decltype(tem.fahrenheit)>(tem.fahrenheit, tem.time()));
      }
      //ThermometerData the;
      //if (_thermometer.pop(the)) {
      //  _h2oTmpChar.writeValue(
        // Packet<decltype(the.fahrenheit)>(the.fahrenheit, the.time()));
      //}
      //ScaleData sca;
      //if (_scale.pop(sca)) {
      //  _weightChar.writeValue(
        // Packet<decltype(sca.read)>(sca.read - sca.zero, sca.time()));
      //}
    }
    digitalWrite(LED_BUILTIN, LOW);
    swritef("disconnected: %s", central.address());
  }
}
