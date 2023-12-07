#include "hid/link.hpp"

bool link::Hub::init() {
  return true;
}

bool link::Hub::connect() {
  wait_ready();
  return ready();
}

bool link::Hub::connected() {
  return configured();
}
