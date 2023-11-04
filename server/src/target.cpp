#include "target.hpp"
#include "spec.hpp"

Target::Target(): link::Hub() {
  pinMode(PIN_ENABLE_SENSORS_3V3, OUTPUT);
  pinMode(PIN_ENABLE_I2C_PULLUP, OUTPUT);
  digitalWrite(PIN_ENABLE_SENSORS_3V3, LOW);
  digitalWrite(PIN_ENABLE_I2C_PULLUP, LOW);
}

Target::~Target() {}

bool Target::init() {

  if (!link::Hub::init()) {
    return false;
  }

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
  _scale.start();
  _temperature.start();
  // _thermometer.start();

  swritef("%s", "sensors initialized");

  return true;
}

void Target::update() {
  if (connect()) {
    digitalWrite(LED_BUILTIN, HIGH);
    while (connected()) {

      AccelerometerData acc;
      if (_accelerometer.pop(acc)) {
        tx<AccelerometerData>(packet::Data<AccelerometerData::Type>({acc.x, acc.y, acc.z}, acc.time()));
      }
      BarometerData bar;
      if (_barometer.pop(bar)) {
        tx<BarometerData>(packet::Data<BarometerData::Type>(bar.psi, bar.time()));
      }
      // ColorData col;
      // if (_color.pop(col)) {
      //   Link::tx<ColorData>(packet::Data<ColorData::Type>({col.red, col.green, col.blue}, col.time()));
      // }
      // GestureData ges;
      // if (_gesture.pop(ges)) {
      //   Link::tx<GestureData>(packet::Data<GestureData::Type>(ges.gesture, ges.time()));
      // }
      // GyroscopeData gyr;
      // if (_gyroscope.pop(gyr)) {
      //   Link::tx<GyroscopeData>(packet::Data<GyroscopeData::Type>({gyr.x, gyr.y, gyr.z}, gyr.time()));
      // }
      HumidityData hum;
      if (_humidity.pop(hum)) {
        tx<HumidityData>(packet::Data<HumidityData::Type>(hum.humidity, hum.time()));
      }
      KeepAliveData kee;
      if (_keepAlive.pop(kee)) {
        packet::Data<KeepAliveData::Type> pkt(kee.pulse, kee.time());
        swritef("%s", "≡===≡===≡===≡===≡===≡===≡===≡===≡===≡===≡===≡===≡===≡===≡");
        pkt.describe();
        tx<KeepAliveData>(std::move(pkt));
      }
      // MagnetometerData mag;
      // if (_magnetometer.pop(mag)) {
      //   Link::tx<MagnetometerData>(packet::Data<MagnetometerData::Type>({mag.x, mag.y, mag.z}, mag.time()));
      // }
      // MicrophoneData mic;
      // if (_microphone.pop(mic)) {
      //   Link::tx<MicrophoneData>(packet::Data<MicrophoneData::Type>(mic.read, mic.time()));
      // }
      ProximityData pro;
      if (_proximity.pop(pro)) {
        tx<ProximityData>(packet::Data<ProximityData::Type>(pro.proximity, pro.time()));
      }
      ScaleData sca;
      if (_scale.pop(sca)) {
        tx<ScaleData>(packet::Data<ScaleData::Type>(sca.read, sca.time()));
      }
      TemperatureData tem;
      if (_temperature.pop(tem)) {
        tx<TemperatureData>(packet::Data<TemperatureData::Type>(tem.fahrenheit, tem.time()));
      }
      // ThermometerData the;
      // if (_thermometer.pop(the)) {
      //   Link::tx<ThermometerData>(packet::Data<ThermometerData::Type>(the.fahrenheit, the.time()));
      // }

      // swritef("%s", "≡===≡===≡===≡===≡===≡===≡===≡===≡===≡===≡===≡===≡===≡===≡");
      // pkt.describe();
    }
    digitalWrite(LED_BUILTIN, LOW);
  }
}
