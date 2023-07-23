#ifndef ble_hpp
#define ble_hpp

#ifdef ARDUINO
#ifdef ESP_PLATFORM
#include <BLEDevice.h>
#else // ! ESP_PLATFORM
#include <ArduinoBLE.h>
#endif // ! ESP_PLATFORM
#endif // ARDUINO

#include "uuid.hpp"

#define BLE_DEVICE_NAME                "FreePOS"
#define BLE_LOCAL_NAME                 "FreePOS"
#define BLE_BUFFER_SIZES               20
#define BLE_SENSORS_CHAR_FLAG          BLERead | BLENotify | BLEBroadcast

template <typename T>
union TaggedData {
  static_assert(sizeof(T) <= sizeof(uint64_t) - sizeof(uint32_t),
    "TaggedData<T>: sizeof(T) exceeds available space (32 bits maximum)");

  uint64_t u64;
  struct _ss {
    uint32_t tag;
    T value;
    uint8_t _pad[sizeof(u64) - sizeof(tag) - sizeof(value)];
  } s;

  TaggedData() = default;
  TaggedData(T value, uint32_t tag):
    s{tag, value} {}
  TaggedData(T value, msec_t tag = msecs()):
    s{tag.count(), value} {}
  TaggedData(uint64_t value):
    u64(value) {}
  TaggedData(uint8_t *pData, size_t length):
    u64(0ULL) /* zeroize entire structure */ {
      if (length > sizeof(u64))
        { length = sizeof(u64); }
      memcpy(&u64, pData, length);
    }

  operator uint64_t() { return u64; }
};

#endif // ble_hpp
