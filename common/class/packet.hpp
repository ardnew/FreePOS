#ifndef api_class_packet_hpp
#define api_class_packet_hpp

#include "native/core.hpp"
#include "native/bits.hpp"
#include "native/debug.hpp"

template <typename T, typename S>
using PacketBytes = std::array<uint8_t, sizeof(T) + sizeof(S)>;

// Packet represents a single value read from a sensor paired with a timestamp
// indicating when it was read.
//
// Packet objects are intended to be transmitted wirelessly over BLE as GATT
// characteristics. Thus, the timestamp is a relative number used only to ensure
// packet ordering by the receiver. It is NOT POSSIBLE for a receiving device to
// determine the absolute, real-world time encoded in a Packet's timestamp using
// this API alone. The user would need to implement their own mechanism for
// synchronizing the clocks of the transmitting and receiving devices.
template <typename T, typename S = msecu32_t::rep>
struct Packet: public PacketBytes<T, S> {
  using offset_t = typename Packet::const_pointer;
  Packet(const T v, const S s): PacketBytes<T, S>{} {
    std::copy(reinterpret_cast<offset_t>(&s),
              reinterpret_cast<offset_t>(&s) + sizeof(S),
              this->data());
    std::copy(reinterpret_cast<offset_t>(&v),
              reinterpret_cast<offset_t>(&v) + sizeof(T),
              this->data() + sizeof(S));
  }

  Packet(): Packet(T{}, 0) {}
  Packet(const T v): Packet(v, msecu32().count()) {}
  Packet(const Packet& a) {
    std::copy(a.data(), a.data() + a.packetSize(), this->data());
  }

  Packet(const uint8_t *ptr, const size_t size) {
    const size_t max = packetSize();
    std::fill(this->data(), this->data() + max, 0);
    const size_t num = size > max ? max : size;
    std::copy(ptr, ptr + num, this->data());
  }

  template <ByteOrder Order = ByteOrder::LSBFirst>
  S stamp() const { return readBytes<S, Order>(this->data()); }
  template <ByteOrder Order = ByteOrder::LSBFirst>
  T value() const { return readBytes<T, Order>(this->data() + sizeof(S)); }

  size_t packetSize() const {
    return this->size(); // number of bytes in an encoded packet
  }

  void describe() const {
    sputf("Packet<%lu> = { ", packetSize());
    for (const uint8_t b : *this) {
      sputf("%02x ", b);
    }
    std::stringstream s;
    s << "(stamp=" << stamp() << ", value=" << value() << ")";
    swritef("} = %s", s.str().c_str());
  }

private:
  template <typename DestType, ByteOrder Order>
  static constexpr DestType
  readBytes(offset_t p, const size_t n = sizeof(DestType)) noexcept {
    const size_t size = sizeof(DestType) > n ? n : sizeof(DestType);
    DestType out{};
    Bytes<Order>{}.copy(p, p + size, reinterpret_cast<uint8_t *>(&out));
    return out;
  }
};

#endif // api_class_packet_hpp
