#include "hid/chan.hpp"

namespace chan {

template <> const uint8_t * Sensor<AccelerometerData>::report_desc() { return nullptr; }
template <> const uint8_t * Sensor<BarometerData>::report_desc() { return nullptr; }
template <> const uint8_t * Sensor<ColorData>::report_desc() { return nullptr; }
template <> const uint8_t * Sensor<GestureData>::report_desc() { return nullptr; }
template <> const uint8_t * Sensor<GyroscopeData>::report_desc() { return nullptr; }
template <> const uint8_t * Sensor<HumidityData>::report_desc() { return nullptr; }
template <> const uint8_t * Sensor<KeepAliveData>::report_desc() { return nullptr; }
template <> const uint8_t * Sensor<MagnetometerData>::report_desc() { return nullptr; }
template <> const uint8_t * Sensor<MicrophoneData>::report_desc() { return nullptr; }
template <> const uint8_t * Sensor<ProximityData>::report_desc() { return nullptr; }
template <> const uint8_t * Sensor<ScaleData>::report_desc() { return nullptr; }
template <> const uint8_t * Sensor<TemperatureData>::report_desc() { return nullptr; }
template <> const uint8_t * Sensor<ThermometerData>::report_desc() { return nullptr; }

template <> const uint8_t * Sensor<AccelerometerData>::configuration_desc(uint8_t index) { return nullptr; }
template <> const uint8_t * Sensor<BarometerData>::configuration_desc(uint8_t index) { return nullptr; }
template <> const uint8_t * Sensor<ColorData>::configuration_desc(uint8_t index) { return nullptr; }
template <> const uint8_t * Sensor<GestureData>::configuration_desc(uint8_t index) { return nullptr; }
template <> const uint8_t * Sensor<GyroscopeData>::configuration_desc(uint8_t index) { return nullptr; }
template <> const uint8_t * Sensor<HumidityData>::configuration_desc(uint8_t index) { return nullptr; }
template <> const uint8_t * Sensor<KeepAliveData>::configuration_desc(uint8_t index) { return nullptr; }
template <> const uint8_t * Sensor<MagnetometerData>::configuration_desc(uint8_t index) { return nullptr; }
template <> const uint8_t * Sensor<MicrophoneData>::configuration_desc(uint8_t index) { return nullptr; }
template <> const uint8_t * Sensor<ProximityData>::configuration_desc(uint8_t index) { return nullptr; }
template <> const uint8_t * Sensor<ScaleData>::configuration_desc(uint8_t index) { return nullptr; }
template <> const uint8_t * Sensor<TemperatureData>::configuration_desc(uint8_t index) { return nullptr; }
template <> const uint8_t * Sensor<ThermometerData>::configuration_desc(uint8_t index) { return nullptr; }

} // namespace chan
