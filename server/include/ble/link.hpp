#ifndef include_ble_link_hpp
#define include_ble_link_hpp

#include "ble/chan.hpp"

namespace link {

class Hub {
private:
  static BLEDevice _central;
  static BLEService _sensors;

public:
  Hub() {}
  virtual ~Hub() {}

  bool init();
  bool connect();
  bool connected();

  template <typename T>
  int tx(packet::Data<typename T::Type>&& p) {
    return chan::SensorImpl<T>::get().write(std::move(p));
  }
};

}

#endif // include_ble_link_hpp
