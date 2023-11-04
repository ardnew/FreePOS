#ifndef common_native_bits_hpp
#define common_native_bits_hpp

#include "native/core.hpp"
#include "macro.hpp"

enum class ByteOrder: bool {
  MSBFirst     = false,
  LSBFirst     = true,
  BigEndian    = MSBFirst,
  LittleEndian = LSBFirst,
  Network      = BigEndian,
};

// Bytes wraps a copy function that can enforce a given byte ordering.
//
// This can be used, e.g., to ensure byte ordering when transferring structured
// data via GATT characteristics using Bluetooth LE.
template <ByteOrder> struct Bytes;

template <> struct Bytes<ByteOrder::MSBFirst> {
  decl_func_alias_(copy, std::reverse_copy);
};


template <> struct Bytes<ByteOrder::LSBFirst> {
  decl_func_alias_(copy, std::copy);
};

#endif // common_native_bits_hpp
