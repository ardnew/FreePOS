#ifndef spec_hpp
#define spec_hpp

// FreePOS API
#include <native/core.hpp>
#include <native/debug.hpp>

#include <class/packet.hpp>

#include <uuid.hpp>
#include <version.hpp>

// nanosensors
#include <accelerometer.h>
#include <barometer.h>
// #include <color.h>
// #include <gesture.h>
// #include <gyroscope.h>
#include <humidity.h>
// #include <magnetometer.h>
#include <microphone.h>
#include <proximity.h>
#include <temperature.h>

#include <sensor.h>

uint8_t const SCALE_DATA_PIN = 2;
uint8_t const SCALE_CLOCK_PIN = 3;

float const SCALE_FACTOR = 450.53825F;
float const SCALE_OFFSET = 71708.73446F;
uint8_t const SCALE_SAMPLE = 1U;

#endif // spec_hpp
