#ifndef include_spec_hpp
#define include_spec_hpp

// "Link" type controls which transport interface is used for client-server
// communication. Specify which link TYPE to use by adding either one of the
// following macro definitions to your compiler flags (e.g., CFLAGS+=-D{TYPE}):
//
//   FREEPOS_LINK_BLE  »==>  Bluetooth Low Energy (BLE)
//   FREEPOS_LINK_HID  »==>  USB Human Interface Device (HID)
//
#if !defined(FREEPOS_LINK_BLE) && !defined(FREEPOS_LINK_HID)
  #warning "No link type specified! Using default: FREEPOS_LINK_BLE"
  #define FREEPOS_LINK_BLE // default link type
#endif

/// FreePOS API

#include <native/core.hpp>
#include <native/debug.hpp>

#include <class/packet.hpp>

#include <uuid.hpp>
#include <version.hpp>

/// nanosensors

#include <accelerometer.h>
#include <barometer.h>
#include <color.h>
#include <gesture.h>
#include <gyroscope.h>
#include <humidity.h>
#include <magnetometer.h>
#include <microphone.h>
#include <proximity.h>
#include <temperature.h>

#include <sensor.h>

/// FreePOS Server

#include "keepalive.hpp"
#include "scale.hpp"
#include "thermometer.hpp"

template <typename T> static constexpr UUID uuid();
template <> constexpr UUID uuid<SensorData>() { return UUID_SENSORS_SERVICE; }
template <> constexpr UUID uuid<AccelerometerData>() { return UUID_SENSORS_ACCXYZ_CHAR; }
template <> constexpr UUID uuid<BarometerData>() { return UUID_SENSORS_BARPSI_CHAR; }
template <> constexpr UUID uuid<ColorData>() { return UUID_SENSORS_COLORS_CHAR; }
template <> constexpr UUID uuid<GestureData>() { return UUID_SENSORS_MOTION_CHAR; }
template <> constexpr UUID uuid<GyroscopeData>() { return UUID_SENSORS_ROTXYZ_CHAR; }
template <> constexpr UUID uuid<HumidityData>() { return UUID_SENSORS_PRECIP_CHAR; }
template <> constexpr UUID uuid<KeepAliveData>() { return UUID_SENSORS_PULSES_CHAR; }
template <> constexpr UUID uuid<MagnetometerData>() { return UUID_SENSORS_MAGXYZ_CHAR; }
template <> constexpr UUID uuid<MicrophoneData>() { return UUID_SENSORS_PDMMIC_CHAR; }
template <> constexpr UUID uuid<ProximityData>() { return UUID_SENSORS_PROXIM_CHAR; }
template <> constexpr UUID uuid<ScaleData>() { return UUID_SENSORS_WEIGHT_CHAR; }
template <> constexpr UUID uuid<TemperatureData>() { return UUID_SENSORS_AIRTMP_CHAR; }
template <> constexpr UUID uuid<ThermometerData>() { return UUID_SENSORS_H2OTMP_CHAR; }

#endif // include_spec_hpp
