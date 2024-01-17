#ifndef include_hid_desc_hpp
#define include_hid_desc_hpp

#include "spec.hpp"
#include "hid/spec/sensor.h"

namespace desc {

template <typename T, T... Vals> struct Bytes {
public:
  using type = Bytes;
  using index_type = size_t;
  using value_type = T;
  using array_type = value_type[sizeof...(Vals)];

  static constexpr array_type const &data = {Vals...};
  static constexpr index_type const &size = sizeof...(Vals);
  constexpr value_type operator[](index_type i) noexcept { return data[i]; }

private:
  template <value_type Val> using value = std::integral_constant<value_type, Val>;

  // Define an Index=>Value mapping (i.e., data[I] = V)
  template <index_type Idx, value_type Sub> struct map {};

  // Implemented as a recursive constexpr function, map_func() returns the value
  // of the first map<Idx, Sub> that matches the given index I.
  // template <index_type Idx, value_type Sub, typename... Maps>
  // static constexpr auto map_func(index_type i, map<Idx, Sub> m, Maps...maps) {
  //   return i == Idx ? Sub : map_func(i, maps...);
  // }
  // static constexpr auto map_func(index_type i) { return data[i]; }

  // Implemented recursively through base class inheritance, map_apply achieves
  // the same result as map_func() above.
  template <index_type I, typename...>
  struct map_apply: value<data[I]> {};
  template <index_type I, index_type Idx, value_type Sub, typename... Maps>
  struct map_apply<I, map<Idx, Sub>, Maps...>:
    std::conditional<Idx == I, value<Sub>, map_apply<I, Maps...>>::type {};

public:
  template <value_type... Subs> class replace {
  private:
    template <typename, index_type... Idxs> struct zip;
    template <index_type... Is, index_type... Idxs>
    struct zip<std::index_sequence<Is...>, Idxs...>
        : Bytes<value_type, map_apply<Is, map<Idxs, Subs>...>::value...> {};

  public:
    template <index_type... Idxs>
    using at = zip<std::make_index_sequence<sizeof...(Vals)>, Idxs...>;
  };

  // Replace a pair of indices with a 16-bit value (index 0=LSB, 1=MSB).
  // It will be a compile-time error if number of indices given != 2.
  template <uint16_t U> struct with: replace<
    static_cast<value_type>(U & 0xFF),
    static_cast<value_type>((U >> 8) & 0xFF)
  > {};

  // Encode the size of the array as a 16-bit integer (little-endian).
  struct count: replace<
    static_cast<value_type>(size & 0xFF),
    static_cast<value_type>((size >> 8) & 0xFF)
  > {};
};

template <typename T> struct Report;

template <>
struct Report<AccelerometerData>: Bytes<uint8_t,
  HID_REPORT_ID(1),
  HID_USAGE_PAGE_SENSOR,
  HID_USAGE_SENSOR_TYPE_MOTION_ACCELEROMETER_3D,
  HID_COLLECTION(Physical),

    HID_USAGE_SENSOR_DATA_MOTION_ACCELERATION_X_AXIS,
    HID_LOGICAL_MIN_32(0x00, 0x00, 0x00, 0x00), // 32-bit little-endian float,
    HID_LOGICAL_MAX_32(0xFF, 0xFF, 0xFF, 0xFF), //  permit any bit pattern.
    HID_REPORT_SIZE(32),
    HID_REPORT_COUNT(1),
    HID_INPUT(Const_Var_Abs),

    HID_USAGE_SENSOR_DATA_MOTION_ACCELERATION_Y_AXIS,
    HID_LOGICAL_MIN_32(0x00, 0x00, 0x00, 0x00), // 32-bit little-endian float,
    HID_LOGICAL_MAX_32(0xFF, 0xFF, 0xFF, 0xFF), //  permit any bit pattern.
    HID_REPORT_SIZE(32),
    HID_REPORT_COUNT(1),
    HID_INPUT(Const_Var_Abs),

    HID_USAGE_SENSOR_DATA_MOTION_ACCELERATION_Z_AXIS,
    HID_LOGICAL_MIN_32(0x00, 0x00, 0x00, 0x00), // 32-bit little-endian float,
    HID_LOGICAL_MAX_32(0xFF, 0xFF, 0xFF, 0xFF), //  permit any bit pattern.
    HID_REPORT_SIZE(32),
    HID_REPORT_COUNT(1),
    HID_INPUT(Const_Var_Abs),

  HID_END_COLLECTION // end of accelerometer nested sub-collection
> {};

} // namespace desc

#endif // include_hid_desc_hpp
