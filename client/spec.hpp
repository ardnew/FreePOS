#ifndef spec_hpp
#define spec_hpp

#include <Arduino.h>

// FreePOS API
#include "const.hpp"
#include "ble.hpp"
#include "debug.hpp"

static BLEUUID uuidService(BLE_SENSORS_SERVICE_UUID);

static BLEUUID uuidCharAccelX(BLE_SENSORS_ACCELX_CHAR_UUID);
static BLEUUID uuidCharAccelY(BLE_SENSORS_ACCELY_CHAR_UUID);
static BLEUUID uuidCharAccelZ(BLE_SENSORS_ACCELZ_CHAR_UUID);
static BLEUUID uuidCharBarPsi(BLE_SENSORS_BARPSI_CHAR_UUID);
static BLEUUID uuidCharPrecip(BLE_SENSORS_PRECIP_CHAR_UUID);
static BLEUUID uuidCharProxim(BLE_SENSORS_PROXIM_CHAR_UUID);
static BLEUUID uuidCharAirTmp(BLE_SENSORS_AIRTMP_CHAR_UUID);
static BLEUUID uuidCharH20Tmp(BLE_SENSORS_H2OTMP_CHAR_UUID);
static BLEUUID uuidCharWeight(BLE_SENSORS_WEIGHT_CHAR_UUID);

#endif // spec_hpp
