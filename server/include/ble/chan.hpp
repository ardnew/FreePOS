#ifndef include_ble_chan_hpp
#define include_ble_chan_hpp

#include "spec.hpp"

namespace chan {

template <typename T>
class Sensor: public BLETypedCharacteristic<packet::Data<typename T::Type>> {
public:
  using BLETypedCharacteristic<packet::Data<typename T::Type>>::BLETypedCharacteristic;

  // The first call to get() will instantiate a static instance of Sensor<T>.
  // All subsequent calls will return a reference to the same instance.
  static Sensor<T>& get() {
    static Sensor<T> c(
      uuid<T>(), BLERead | BLENotify | BLEBroadcast);
    return c;
  }

  int write(packet::Data<typename T::Type>&& value) {
    return BLECharacteristic::writeValue(value.data(), value.size());
  }
};

// template <typename T, size_t N>
// using BLETaggedArrayCharacteristic = Sensor<std::array<T, N>>;

} // namespace chan

#endif // include_ble_chan_hpp
