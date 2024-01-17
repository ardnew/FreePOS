#ifndef include_hid_chan_hpp
#define include_hid_chan_hpp

#include "hid/desc.hpp"

namespace chan {

class Sensor {
public:
  Sensor(): _id(id()), _report({0UL}) {}
  virtual ~Sensor() {}
  using Report = HID_REPORT;

protected:
  const uint8_t _id;
  Report _report;

private:
  static uint8_t id() {
    static uint8_t id = 1; // HID report ID of first Sensor allocated.
    return id++;           // Subsequent objects are assigned sequential IDs.
  }
};

// SensorImpl is the most-derived class for all USB HID-based sensors.
//
// A single, physical HID device — implemented by link::Hub — is a "sensor hub"
// which corresponds nicely to the Bluetooth LE architecture; where the sensor
// hub is the GATT server and the individual sensors are GATT characteristics.
template <typename T>
class SensorImpl: public Sensor {
public:
  SensorImpl() = default;
  virtual ~SensorImpl() {}

  // All calls to get() will return a reference to the same static instance.
  static __noinline SensorImpl<T>& get() {
    static SensorImpl<T> c;
    return c;
  }

  const Report &write(packet::Data<typename T::Type>&& value) {
    // clear the report buffer
    std::fill_n(_report.data, sizeof(_report.data), uint8_t{0});
    size_t n = value.size();
    if (n > sizeof(_report.data) - 1) {
      n = sizeof(_report.data) - 1;
    }
    _report.data[0] = _id;
    std::copy_n(value.begin(), n, _report.data + 1);
    _report.length = n + 1;
    return _report;
  }

  // id returns the HID report ID for an individual sensor type T.
  //
  // All HID reports for a given sensor type T are identified by the same ID,
  // and no two sensor types may share the same ID.
  //
  // HID report IDs are assigned automatically during construction of the base
  // class (i.e., chan::Sensor()), and the user should not attempt to modify
  // them; consider them a managed, read-only attribute.
  //
  // ID 0 is reserved per HID spec. The first type of sensor allocated is
  // assigned ID 1, the second ID 2, and so on.
  uint8_t id() const { return _id; }

  // desc returns the HID report descriptor for an individual sensor type T.
  //
  // Since the HID device composition is implemented using a single HID device
  // interface (i.e., a single pair of IN/OUT endpoints), all sensor data is
  // described to the host using a single HID report descriptor.
  // This sole descriptor is a Top-Level Collection (TLC) consisting of nested
  // sub-collections for each sensor type.
  // Each of these sensor sub-collections describe individual HID reports and
  // are identified by unique HID report IDs.
  //
  // This technique is used and described in the HID Sensor Usage Tables
  // standard (published by the USB-IF) in section 4.2.5 "Sensor Collections".
  //
  // The alternative to this technique is to use multiple HID interfaces, each
  // with its own HID report descriptor and pair of IN/OUT endpoints.
  // However, the number of HID interfaces (read: sensors) would then be limited
  // by the number of available endpoints provided by the USB PHY, which are a
  // target hardware-limited resource (typically 2–7 IN/OUT pairs, or, sensors).
  //
  // Thus, the chosen technique is simpler to implement, more scalable, and
  // supported on a wider range of hardware.
  // The primary downside is going to be the latency of real-time sensor data
  // with contention for the single IN/OUT endpoint pair.
  // Given the current use-case of this project (kitchen scale), this seems to
  // be an acceptable tradeoff. :p
  //
  // It is not necessary to subclass SensorImpl for each sensor type T added or
  // removed; you only need to implement a class template specialization of
  // desc::Report<T> for the new type.
  // See the other specializations in "hid/desc.hpp" for examples.
  static constexpr auto report_desc() { return desc::Report<T>::array; }
};

} // namespace chan

#endif // include_hid_chan_hpp
