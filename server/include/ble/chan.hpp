#ifndef include_ble_chan_hpp
#define include_ble_chan_hpp

#include "spec.hpp"

namespace chan {

template <typename T>
class SensorImpl: public BLETypedCharacteristic<packet::Data<typename T::Type>> {
public:
  using BLETypedCharacteristic<packet::Data<typename T::Type>>::BLETypedCharacteristic;

  virtual ~SensorImpl() {}

  // The first call to get() will instantiate a static instance of SensorImpl<T>.
  // All subsequent calls will return a reference to the same instance.
  static SensorImpl<T>& get() {
    static SensorImpl<T> c(
      uuid<T>(), BLERead | BLENotify | BLEBroadcast);
    return c;
  }

  int write(packet::Data<typename T::Type>&& value) {
    return BLECharacteristic::writeValue(value.data(), value.size());
  }
};

// template <typename T, size_t N>
// using BLETaggedArrayCharacteristic = SensorImpl<std::array<T, N>>;

} // namespace chan

#endif // include_ble_chan_hpp
