#include "hid/link.hpp"

bool link::Hub::init() {

  //if (!BLE.begin()) {
    spanicf("link initialization failed: %s", "BLE.begin()");
  //}
  swritef("%s", "radio initialized");

  swritef("%s", "wireless services initialized");

  return true;
}

bool link::Hub::connect() {
  // Discard any existing connection and start a new one.
  // Otherwise, check connected() to prevent dropping existing connections.
  return true;
}

bool link::Hub::connected() {
  // operator BLEDevice::bool() returns true iff an address has been assigned.
  // It does not test the state of the radio or ATT driver.
  return false;
}
