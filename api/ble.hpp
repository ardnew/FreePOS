#ifndef ble_hpp
#define ble_hpp

#ifdef ARDUINO
#ifdef ESP_PLATFORM
#include <BLEDevice.h>
#else // ! ESP_PLATFORM
#include <ArduinoBLE.h>
#endif // ! ESP_PLATFORM
#endif // ARDUINO

#include <chrono>
#include <cstdint>

// FreePOS API
#include "debug.hpp"
#include "uuid.hpp"
#include "time.hpp"

#define BLE_DEVICE_NAME                "FreePOS"
#define BLE_LOCAL_NAME                 "FreePOS"
#define BLE_BUFFER_SIZES               20
#define BLE_SENSORS_CHAR_FLAG          BLERead | BLENotify | BLEBroadcast

enum class Endian: bool { Lit = false, Big = true, Net = Big };

// Bytes is a helper struct to convert the byte order of values in a Packet.
template <typename T, Endian E> struct Bytes {};
template <typename T> struct Bytes<T, Endian::Lit>
  { static const std::function<T(T,T,T)> copy = std::copy; };
template <typename T> struct Bytes<T, Endian::Big>
  { static const std::function<T(T,T,T)> copy = std::reverse_copy; };

// Packet represents a single value read from a sensor paired with a timestamp
// indicating when it was read.
//
// Packet objects are intended to be transmitted wirelessly over BLE as GATT
// characteristics. Thus, the timestamp is a relative number used only to ensure
// packet ordering by the receiver.
template <typename T, typename S = msec32_t::rep>
struct Packet: public std::array<uint8_t, sizeof(T) + sizeof(S)> {
  Packet(const T v, const S s): std::array<uint8_t, sizeof(T) + sizeof(S)>{} {
    std::copy(reinterpret_cast<const uint8_t *>(&s),
              reinterpret_cast<const uint8_t *>(&s) + sizeof(S),
              this->data());
    std::copy(reinterpret_cast<const uint8_t *>(&v),
              reinterpret_cast<const uint8_t *>(&v) + sizeof(T),
              this->data() + sizeof(S));
  }

  Packet(const T v):
    Packet(v, msec32_t().count()) {}
  Packet(const Packet& a) {
    std::copy(a.data(), a.data() + a.packetSize(), this->data());
  }
  Packet(): Packet(T{}) {}

  Packet(const uint8_t *ptr, const size_t size) {
    const size_t max = packetSize();
    const size_t num = size > max ? max : size;
    const size_t off = max - num;
    std::copy(ptr, ptr + num, this->data());
    std::fill(this->data() + off, this->data() + max, 0);
  }

  template <Endian E = Endian::Lit>
  S stamp() const { return readBytes<S, E>(this->data()); }
  template <Endian E = Endian::Lit>
  T value() const { return readBytes<T, E>(this->data() + sizeof(S)); }

  size_t packetSize() const {
    return this->size(); // number of bytes in an encoded packet
  }

private:
  template <typename F, Endian E>
  static constexpr inline
  F readBytes(const uint8_t *ptr, const size_t size = sizeof(F)) {
    const size_t num = sizeof(F) > size ? size : sizeof(F);
    F out;
    Bytes<uint8_t *, E>::copy(ptr, ptr + num, reinterpret_cast<uint8_t *>(&out));
    // (ptr, ptr + num, reinterpret_cast<uint8_t *>(&out));
    return out;
  }
};


// using TaggedInt8     = Packet<int8_t>;
// using TaggedUInt8    = Packet<uint8_t>;

// using TaggedInt16    = Packet<int16_t>;
// using TaggedUInt16   = Packet<uint16_t>;

// using TaggedInt32    = Packet<int32_t>;
// using TaggedUInt32   = Packet<uint32_t>;

// using TaggedInt64    = Packet<int64_t>;
// using TaggedUInt64   = Packet<uint64_t>;

// using TaggedFloat32  = Packet<float>;
// using TaggedFloat64  = Packet<double>;

#endif // ble_hpp
