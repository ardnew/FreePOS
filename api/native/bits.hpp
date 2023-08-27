#ifndef api_native_bits_hpp
#define api_native_bits_hpp

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
  FUNCTION_FORWARD(copy, std::reverse_copy);
};


template <> struct Bytes<ByteOrder::LSBFirst> {
  FUNCTION_FORWARD(copy, std::copy);
};

#endif // api_native_bits_hpp
