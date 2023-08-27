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

using BLETaggedShortCharacteristic         = BLETaggedCharacteristic<short>;
using BLETaggedUnsignedShortCharacteristic = BLETaggedCharacteristic<unsigned short>;
using BLETaggedIntCharacteristic           = BLETaggedCharacteristic<int>;
using BLETaggedUnsignedIntCharacteristic   = BLETaggedCharacteristic<unsigned int>;
using BLETaggedFloatCharacteristic         = BLETaggedCharacteristic<float>;

constexpr auto BLE_SENSORS_CHAR_MODE = BLERead | BLENotify | BLEBroadcast;

#endif // char_hpp
