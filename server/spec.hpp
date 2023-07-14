#ifndef spec_hpp
#define spec_hpp

#include <Arduino.h>

#include "ble.hpp"
#include "debug.hpp"

uint8_t const SCALE_DATA_PIN         = 2;
uint8_t const SCALE_CLOCK_PIN        = 3;

float   const SCALE_FACTOR           =   450.53825F;
float   const SCALE_OFFSET           = 71708.73446F;
uint8_t const SCALE_SAMPLE           = 1U;

#endif // spec_hpp
