#ifndef common_class_packet_hpp
#define common_class_packet_hpp

#include "native/core.hpp"
#include "native/bits.hpp"
#include "native/debug.hpp"

namespace packet {

template <typename T, typename S>
using Bytes = std::array<uint8_t, sizeof(T) + sizeof(S)>;

// Data represents a single value read from a sensor paired with a timestamp
// indicating when it was read.
//
// Data objects are intended to be transmitted over the wire (or radio)
// between devices. Thus, the timestamp is a relative number used only to ensure
// packet ordering by the receiver. It is NOT POSSIBLE for a receiving device to
// determine the absolute, real-world time encoded in a Data's timestamp using
// this API alone. The user would need to implement their own mechanism for
// synchronizing the clocks of the transmitting and receiving devices.
template <typename T, typename S = msecu32_t::rep>
struct Data: public Bytes<T, S> {
  using offset_t = typename Data::const_pointer;
  Data(const T v, const S s): Bytes<T, S>{} {
    std::copy(reinterpret_cast<offset_t>(&s),
              reinterpret_cast<offset_t>(&s) + sizeof(S),
              this->data());
    std::copy(reinterpret_cast<offset_t>(&v),
              reinterpret_cast<offset_t>(&v) + sizeof(T),
              this->data() + sizeof(S));
  }

  Data(): Data(T{}, 0) {}
  Data(const T v): Data(v, msecu32().count()) {}
  Data(const Data& a) {
    std::copy(a.data(), a.data() + a.size(), this->data());
  }

  Data(const uint8_t *ptr, const size_t sz) {
    const size_t max = size();
    std::fill(this->data(), this->data() + max, 0);
    const size_t num = sz > max ? max : sz;
    std::copy(ptr, ptr + num, this->data());
  }

  template <ByteOrder Order = ByteOrder::LSBFirst>
  S stamp() const { return get<S, Order>(this->data()); }
  template <ByteOrder Order = ByteOrder::LSBFirst>
  T value() const { return get<T, Order>(this->data() + sizeof(S)); }

  // (Inherited from std::array<uint8_t, sizeof(T) + sizeof(S)>)
  size_t size() const {
   return Bytes<T, S>::size(); // number of bytes in an encoded packet
  }

  void describe() const {
    sputf("Data<%lu> = { ", size());
    for (const uint8_t b : *this) {
      sputf("%02x ", b);
    }
    std::stringstream s;
    s << "(stamp=" << stamp() << ", value=" << value() << ")";
    swritef("} = %s", s.str().c_str());
  }

private:
  template <typename Rep, ByteOrder Order>
  static constexpr Rep get(offset_t p, const size_t n = sizeof(Rep)) noexcept {
    const size_t size = sizeof(Rep) > n ? n : sizeof(Rep);
    Rep out{};
    ::Bytes<Order>{}.copy(p, p + size, reinterpret_cast<uint8_t *>(&out));
    return out;
  }
};

} // namespace packet

#endif // common_class_packet_hpp
