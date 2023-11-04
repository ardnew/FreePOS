#ifndef common_native_core_hpp
#define common_native_core_hpp

/// Default includes

#ifdef ARDUINO
  #include <Arduino.h>
  #ifdef ESP_PLATFORM
    #include <BLEDevice.h>
    #include <esp_err.h>
    #include <esp_task_wdt.h>
    #include <freertos/FreeRTOS.h>
    #include <freertos/task.h>
    #include <sdkconfig.h>
    #include <stdio.h>
    #include <stdlib.h>
  #else // ! ESP_PLATFORM
    #include <mbed.h>
    #if defined(FREEPOS_LINK_BLE)
      #include <ArduinoBLE.h>
    #elif defined(FREEPOS_LINK_HID)
      #include <PluggableUSBHID.h>
      #include <usb/usb_phy_api.h>
      #include <USBDescriptor.h>
      #include <PlatformMutex.h>
    #endif
  #endif // (!) ESP_PLATFORM
#endif // ARDUINO

#include <algorithm>
#include <chrono>
#include <cmath>
#include <cstdint>
#include <iomanip>
#include <mutex>
#include <sstream>
#include <stdexcept>
#include <stdio.h> // snprintf, sscanf
#include <stdint.h>
#include <string>
#include <tuple>
#include <vector>

#include <cronos.h>

/// Default namespaces

using namespace std::literals::chrono_literals;

/// Default macros, constants, and types

using iopin = uint8_t; // GPIO pin (on-board/breakout numbering)

constexpr iopin NOPIN = ~((iopin)0U); // undefined pin

#endif // common_native_core_hpp
