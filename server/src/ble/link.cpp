#include "ble/link.hpp"

// Define all static members of Link::Hub
BLEDevice link::Hub::_central;
BLEService link::Hub::_sensors(UUID_SENSORS_SERVICE);

bool link::Hub::init() {

  if (!BLE.begin()) {
    spanicf("link initialization failed: %s", "BLE.begin()");
  }
  swritef("%s", "radio initialized");

  BLE.setDeviceName(ServerName);
  BLE.setLocalName(DeviceName);
  BLE.setAdvertisedService(_sensors);

  _sensors.addCharacteristic(chan::SensorImpl<AccelerometerData>::get());
  _sensors.addCharacteristic(chan::SensorImpl<BarometerData>::get());
  _sensors.addCharacteristic(chan::SensorImpl<ColorData>::get());
  _sensors.addCharacteristic(chan::SensorImpl<GestureData>::get());
  _sensors.addCharacteristic(chan::SensorImpl<GyroscopeData>::get());
  _sensors.addCharacteristic(chan::SensorImpl<HumidityData>::get());
  _sensors.addCharacteristic(chan::SensorImpl<KeepAliveData>::get());
  _sensors.addCharacteristic(chan::SensorImpl<MagnetometerData>::get());
  _sensors.addCharacteristic(chan::SensorImpl<MicrophoneData>::get());
  _sensors.addCharacteristic(chan::SensorImpl<ProximityData>::get());
  _sensors.addCharacteristic(chan::SensorImpl<ScaleData>::get());
  _sensors.addCharacteristic(chan::SensorImpl<TemperatureData>::get());
  _sensors.addCharacteristic(chan::SensorImpl<ThermometerData>::get());

  BLE.addService(_sensors);
  BLE.advertise();

  swritef("%s", "wireless services initialized");

  return true;
}

bool link::Hub::connect() {
  // Discard any existing connection and start a new one.
  // Otherwise, check connected() to prevent dropping existing connections.
  if ((_central = BLE.central())) {
    swritef("connected: %s", _central.address());
  }
  return connected();
}

bool link::Hub::connected() {
  // operator BLEDevice::bool() returns true iff an address has been assigned.
  // It does not test the state of the radio or ATT driver.
  if (_central) {
    if (_central.connected()) {
      // Both the software stack and hardware agree there is an active link.
      return true;
    }
    // _central was assigned an address at some point, but either the radio or
    // ATT driver indicate an inactive link. This means either the connection
    // was lost, or we've introduced a race condition on the BLE stack.
    //
    // In either case, we need to reestablish connection with another device by
    // restarting the BLE stack via connect().
    swritef("disconnected: %s", _central.address());
  }
  return false;
}
