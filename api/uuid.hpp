#ifndef uuid_hpp
#define uuid_hpp

#include "const.hpp"

#define UUID_STRLEN 36 // chars, excluding null terminator

using UUID = conststring<UUID_STRLEN + 1>;

static constexpr const UUID BLE_SENSORS_SERVICE_UUID      = "32F10000-F2EE-4050-8000-00805F9B34FB";
static constexpr const UUID BLE_SENSORS_ACCELX_CHAR_UUID  = "32F1A001-F2EE-4050-8000-00805F9B34FB";
static constexpr const UUID BLE_SENSORS_ACCELY_CHAR_UUID  = "32F1A002-F2EE-4050-8000-00805F9B34FB";
static constexpr const UUID BLE_SENSORS_ACCELZ_CHAR_UUID  = "32F1A003-F2EE-4050-8000-00805F9B34FB";
static constexpr const UUID BLE_SENSORS_BARPSI_CHAR_UUID  = "32F1B001-F2EE-4050-8000-00805F9B34FB";
static constexpr const UUID BLE_SENSORS_PRECIP_CHAR_UUID  = "32F1C001-F2EE-4050-8000-00805F9B34FB";
static constexpr const UUID BLE_SENSORS_PROXIM_CHAR_UUID  = "32D1D001-F2EE-4050-8000-00805F9B34FB";
static constexpr const UUID BLE_SENSORS_AIRTMP_CHAR_UUID  = "32F1E001-F2EE-4050-8000-00805F9B34FB";
static constexpr const UUID BLE_SENSORS_H2OTMP_CHAR_UUID  = "32F1E002-F2EE-4050-8000-00805F9B34FB";
static constexpr const UUID BLE_SENSORS_WEIGHT_CHAR_UUID  = "32F1F001-F2EE-4050-8000-00805F9B34FB";

#if 0
template <typename T>
constexpr T hextoul(const char *hex) {
  T result = 0;
  for (size_t i = 0; i < sizeof(T); ++i) {
    char c = hex[i];
    result <<= 4;
    if (c >= '0' && c <= '9') {
      result |= (c - '0');
    } else if (c >= 'a' && c <= 'f') {
      result |= (c - 'a' + 10);
    } else if (c >= 'A' && c <= 'F') {
      result |= (c - 'A' + 10);
    } else {
      break;
    }
  }
  return result;
}

template <typename T>
constexpr T hextoul(const std::string &hex) { return hextoul<T>(hex.c_str()); }

// "Return" a data type through template specialization based on the data type
// encoded in the first section of version 4 (random) UUID.
// The parameters that identify a data type are defined in DataTypeParams.
// Decoding these params from an actual UUID is done by dataType().
template<size_t, char, bool> struct Data {};
template<> struct Data<16, 'D', true>  { int16_t  value; };
template<> struct Data<16, 'D', false> { uint16_t value; };
template<> struct Data<32, 'D', true>  { int32_t  value; };
template<> struct Data<32, 'D', false> { uint32_t value; };
template<> struct Data<32, 'F', true>  { float    value; };

template <size_t N = UUID_STRLEN+1>
struct UUIDx: conststring<N> {
  using conststring<N>::conststring;
  constexpr UUIDx(const conststring<N> &u):
    conststring<N>(u), _group{ 0, 0, 0, 0, 0, 0 } {
    if (!(u[8] == '-' && u[13] == '-' && u[18] == '-' && u[23] == '-'))
      { throw std::invalid_argument("UUID: invalid string format"); }
    if (u[N] != 0)
      { throw std::invalid_argument("UUID: string is not null-terminated"); }
    // Encode the hexadecimal string representation of a UUID as 6 distinct
    // integer groups (A—F):
    //
    //    Group:        AAAAAAAA-BBBB-CCCC-DDDD-EEEEFFFFFFFF
    //  Size (bits):       32     16   16   16   16   32
    //
    // The left-most symbol of each group is the most significant byte in its
    // integer encoding.
    //
    // All UUIDs are technically UUID version 4 (random).
    // However, the following convention was used to encode meaningful
    // information into each:
    //
    //   - The data type of the BLE GATT characteristic identified by a UUID is
    //     encoded in the first 4 symbols of group A:
    //
    //       AAAAAAAA
    //       ↑↑↑↑
    //       ¦¦¦ \_ 0x1 (signed) or 0x0 (unsigned)
    //       ¦¦ \__ 0xD (integer) or 0xF (float)
    //       ¦ \___ Lower 4 bits of data type size (in bits)
    //        \____ Upper 4 bits of data type size (in bits)
    //
    //     For example, '32F1××××' would be a 32-bit IEEE 754 float, and
    //                  '16D0××××' would be a 16-bit unsigned integer.
    //
    //     Decoding this information is done by dataType().
    //
    _group.a = hextoul<uint32_t>(std::move(std::string{ u.c_str()[0], 9 }));
    _group.b = hextoul<uint16_t>(std::move(std::string{ u.c_str()[9, 4 }));
    _group.c = hextoul<uint16_t>(std::move(std::string{ u.c_str()[14, 4 }));
    _group.d = hextoul<uint16_t>(std::move(std::string{ u.c_str()[19, 4 }));
    _group.e = hextoul<uint16_t>(std::move(std::string{ u.c_str()[24, 4 }));
    _group.f = hextoul<uint32_t>(std::move(std::string{ u.c_str()[28, 8 }));
  }
  constexpr auto dataType() {
    constexpr size_t size     = _group.a >> 24;
    constexpr char   type     = (_group.a >> 20) & 0x0F;
    constexpr bool   isSigned = (_group.a >> 16) & 0x01 == 1;
    return Data<size, type, isSigned>{};
  }
private:
  struct __packed { uint32_t a; uint16_t b, c, d, e; uint32_t f; } _group;
};

using UUID = UUIDx<UUID_STRLEN + 1>;
#endif

#endif // uuid_hpp
