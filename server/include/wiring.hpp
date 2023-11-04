#ifndef include_wiring_hpp
#define include_wiring_hpp

#include <native/core.hpp>

/// Load cell (weight sensor)

iopin const SCALE_GPIO_DAT_PIN = 2U;
iopin const SCALE_GPIO_CLK_PIN = 3U;

float const SCALE_FACTOR = 450.53825F;
float const SCALE_OFFSET = 71708.73446F;
uint8_t const SCALE_SAMPLE = 1U;

/// Resistance temperature detector (RTD) (temperature sensor)

iopin const THERMOMETER_IRQ_RDY_PIN = 9U;
iopin const THERMOMETER_SPI_SEL_PIN = 10U;
iopin const THERMOMETER_SPI_SDI_PIN = 11U; // Hardware SPI
iopin const THERMOMETER_SPI_SDO_PIN = 12U; //
iopin const THERMOMETER_SPI_CLK_PIN = 13U; //

float const THERMOMETER_PT100_NREF = 100.0F; // PT100 nominal: 100Ω @ 0°C
float const THERMOMETER_PT100_RREF = 430.0F; // PT100 reference: 430Ω @ 0°C
float const THERMOMETER_PT1000_NREF = 1000.0F; // PT1000 nominal: 1000Ω @ 0°C
float const THERMOMETER_PT1000_RREF = 4300.0F; // PT1000 reference: 4300Ω @ 0°C

float const THERMOMETER_COEFF_A = 3.9083e-3F;
float const THERMOMETER_COEFF_B = -5.775e-7F;
float const THERMOMETER_NREF = THERMOMETER_PT100_NREF;
float const THERMOMETER_RREF = THERMOMETER_PT100_RREF;

#endif // include_wiring_hpp
