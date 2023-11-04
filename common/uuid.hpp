#ifndef common_uuid_hpp
#define common_uuid_hpp

#include "macro.hpp"

#define UUID_STRLEN 36 // chars, excluding null terminator

using UUID = const char (&)[UUID_STRLEN+1];

static constexpr UUID UUID_SENSORS_SERVICE      = "32F10000-F2EE-4050-8000-00805F9B34FB";
static constexpr UUID UUID_SENSORS_ACCXYZ_CHAR  = "32F1A001-F2EE-4050-8000-00805F9B34FB"; // Accelerometer  (LSM9DS1)
static constexpr UUID UUID_SENSORS_BARPSI_CHAR  = "32F1B001-F2EE-4050-8000-00805F9B34FB"; // Barometer      (LPS22HB)
static constexpr UUID UUID_SENSORS_COLORS_CHAR  = "32F1C001-F2EE-4050-8000-00805F9B34FB"; // Color          (APDS9960)
static constexpr UUID UUID_SENSORS_MOTION_CHAR  = "32F1C002-F2EE-4050-8000-00805F9B34FB"; // Gesture        (APDS9960)
static constexpr UUID UUID_SENSORS_ROTXYZ_CHAR  = "32F1B001-F2EE-4050-8000-00805F9B34FB"; // Gyroscope      (LSM9DS1)
static constexpr UUID UUID_SENSORS_PRECIP_CHAR  = "32F1D001-F2EE-4050-8000-00805F9B34FB"; // Humidity       (HTS221)
static constexpr UUID UUID_SENSORS_PULSES_CHAR  = "32F10001-F2EE-4050-8000-00805F9B34FB"; // KeepAlive      (internal)
static constexpr UUID UUID_SENSORS_MAGXYZ_CHAR  = "32F1A002-F2EE-4050-8000-00805F9B34FB"; // Magnetometer   (LSM9DS1)
static constexpr UUID UUID_SENSORS_PDMMIC_CHAR  = "32F1E001-F2EE-4050-8000-00805F9B34FB"; // Microphone     (MP34DT05)
static constexpr UUID UUID_SENSORS_PROXIM_CHAR  = "32F1C003-F2EE-4050-8000-00805F9B34FB"; // Proximity      (APDS9960)
static constexpr UUID UUID_SENSORS_WEIGHT_CHAR  = "32F1F001-F2EE-4050-8000-00805F9B34FB"; // Scale          (HX711)
static constexpr UUID UUID_SENSORS_AIRTMP_CHAR  = "32F1D002-F2EE-4050-8000-00805F9B34FB"; // Temperature    (HTS221)
static constexpr UUID UUID_SENSORS_H2OTMP_CHAR  = "32F1F002-F2EE-4050-8000-00805F9B34FB"; // Thermometer    (MAX31865)

#endif // common_uuid_hpp
