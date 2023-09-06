#ifndef char_hpp
#define char_hpp

#include "spec.hpp"

template <typename T>
class BLETaggedCharacteristic : public BLETypedCharacteristic<Packet<T>> {
public:
  using BLETypedCharacteristic<Packet<T>>::BLETypedCharacteristic;
  int writeValue(Packet<T>&& value) {
    return BLECharacteristic::writeValue(value.data(), value.packetSize());
  }
};

template <typename T, size_t N>
using BLETaggedArrayCharacteristic = BLETaggedCharacteristic<std::array<T, N>>;

constexpr auto BLE_SENSORS_CHAR_MODE = BLERead | BLENotify | BLEBroadcast;

#endif // char_hpp
