#include "hid/chan.hpp"
#include "hid/spec/sensor.h"

template <>
const uint8_t * chan::SensorImpl<AccelerometerData>::desc(uint16_t *size) const {
  static uint8_t desc[] = {
    HID_REPORT_ID(1),
    HID_USAGE_PAGE_SENSOR,
    HID_USAGE_SENSOR_TYPE_MOTION_ACCELEROMETER_3D,
    HID_COLLECTION(Physical),

      HID_USAGE_SENSOR_DATA_MOTION_ACCELERATION_X_AXIS,
      HID_LOGICAL_MIN_32(0x00, 0x00, 0x00, 0x00), // 32-bit little-endian float,
      HID_LOGICAL_MAX_32(0xFF, 0xFF, 0xFF, 0xFF), //  permit any bit pattern.
      HID_REPORT_SIZE(32),
      HID_REPORT_COUNT(1),
      HID_INPUT(Const_Var_Abs),

      HID_USAGE_SENSOR_DATA_MOTION_ACCELERATION_Y_AXIS,
      HID_LOGICAL_MIN_32(0x00, 0x00, 0x00, 0x00), // 32-bit little-endian float,
      HID_LOGICAL_MAX_32(0xFF, 0xFF, 0xFF, 0xFF), //  permit any bit pattern.
      HID_REPORT_SIZE(32),
      HID_REPORT_COUNT(1),
      HID_INPUT(Const_Var_Abs),

      HID_USAGE_SENSOR_DATA_MOTION_ACCELERATION_Z_AXIS,
      HID_LOGICAL_MIN_32(0x00, 0x00, 0x00, 0x00), // 32-bit little-endian float,
      HID_LOGICAL_MAX_32(0xFF, 0xFF, 0xFF, 0xFF), //  permit any bit pattern.
      HID_REPORT_SIZE(32),
      HID_REPORT_COUNT(1),
      HID_INPUT(Const_Var_Abs),

    HID_END_COLLECTION, // end of accelerometer nested sub-collection
  };
  *size = sizeof(desc);
  return desc;
}

template <>
const uint8_t * chan::SensorImpl<BarometerData>::desc(uint16_t *size) const {
  static uint8_t desc[] = {
  };
  *size = sizeof(desc);
  return desc;
}

template <>
const uint8_t * chan::SensorImpl<ColorData>::desc(uint16_t *size) const {
  static uint8_t desc[] = {
  };
  *size = sizeof(desc);
  return desc;
}

template <>
const uint8_t * chan::SensorImpl<GestureData>::desc(uint16_t *size) const {
  static uint8_t desc[] = {
  };
  *size = sizeof(desc);
  return desc;
}

template <>
const uint8_t * chan::SensorImpl<GyroscopeData>::desc(uint16_t *size) const {
  static uint8_t desc[] = {
  };
  *size = sizeof(desc);
  return desc;
}

template <>
const uint8_t * chan::SensorImpl<HumidityData>::desc(uint16_t *size) const {
  static uint8_t desc[] = {
  };
  *size = sizeof(desc);
  return desc;
}

template <>
const uint8_t * chan::SensorImpl<KeepAliveData>::desc(uint16_t *size) const {
  static uint8_t desc[] = {
  };
  *size = sizeof(desc);
  return desc;
}

template <>
const uint8_t * chan::SensorImpl<MagnetometerData>::desc(uint16_t *size) const {
  static uint8_t desc[] = {
  };
  *size = sizeof(desc);
  return desc;
}

template <>
const uint8_t * chan::SensorImpl<MicrophoneData>::desc(uint16_t *size) const {
  static uint8_t desc[] = {
  };
  *size = sizeof(desc);
  return desc;
}

template <>
const uint8_t * chan::SensorImpl<ProximityData>::desc(uint16_t *size) const {
  static uint8_t desc[] = {
  };
  *size = sizeof(desc);
  return desc;
}

template <>
const uint8_t * chan::SensorImpl<ScaleData>::desc(uint16_t *size) const {
  static uint8_t desc[] = {
  };
  *size = sizeof(desc);
  return desc;
}

template <>
const uint8_t * chan::SensorImpl<TemperatureData>::desc(uint16_t *size) const {
  static uint8_t desc[] = {
  };
  *size = sizeof(desc);
  return desc;
}

template <>
const uint8_t * chan::SensorImpl<ThermometerData>::desc(uint16_t *size) const {
  static uint8_t desc[] = {
  };
  *size = sizeof(desc);
  return desc;
}
