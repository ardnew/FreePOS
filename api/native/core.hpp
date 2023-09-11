#ifndef api_native_core_hpp
#define api_native_core_hpp

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
#include <ArduinoBLE.h>
#endif // (!) ESP_PLATFORM
#endif // ARDUINO

#include <cronos.h>

#include <algorithm>
#include <chrono>
#include <cmath>
#include <cstdint>
#include <iomanip>
#include <mutex>
#include <sstream>
#include <stdexcept>
#include <stdio.h> // snprintf
#include <string>
#include <tuple>
#include <vector>

// Add any default namespaces below.
using namespace std::literals::chrono_literals;

#endif // api_native_core_hpp
