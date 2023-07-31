#ifndef char_hpp
#define char_hpp

#include <sensor.h>

#include "spec.hpp"

template <typename T>
class BLETaggedCharacteristic : public BLETypedCharacteristic<Packet<T>> {
public:
  using BLETypedCharacteristic<Packet<T>>::BLETypedCharacteristic;
  int writeValue(Packet<T>&& value) {
    return BLECharacteristic::writeValue(value.data(), value.packetSize());
  }
};

using BLETaggedShortCharacteristic         = BLETaggedCharacteristic<Packet<short>>;
using BLETaggedUnsignedShortCharacteristic = BLETaggedCharacteristic<Packet<unsigned short>>;
using BLETaggedIntCharacteristic           = BLETaggedCharacteristic<Packet<int>>;
using BLETaggedUnsignedIntCharacteristic   = BLETaggedCharacteristic<Packet<unsigned int>>;
using BLETaggedFloatCharacteristic         = BLETaggedCharacteristic<Packet<float>>;

#endif // char_hpp
