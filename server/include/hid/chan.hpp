#ifndef include_hid_chan_hpp
#define include_hid_chan_hpp

#include "spec.hpp"
#include "hid/ident.hpp"

namespace chan {

class Sensor: public arduino::USBHID {
public:
  using arduino::USBHID::USBHID;

  virtual ~Sensor() {}

protected:
  // We are piggybacking off of someone else's USB device class (Arduino, at the
  // moment), so we need to consider how other interfaces managed externally by
  // their BSP are assigning interface numbers, HID report IDs, endpoint
  // numbers, and etc. on this shared USB bus.
  //
  // Other interfaces may include more device class configurations (CDC-ACM/EEM,
  // MIDI, MSD, etc.), or they may include other interfaces defined on the same
  // HID device we are using (control/status, keyboard, mouse, joystick, etc.)
  // or serial emulation, control/status endpoints, and so on.
  //
  // Sensors are free to call next_endpoint() multiple times if they use more
  // than one endpoint. Be careful to call it only once per endpoint, though;
  // e.g., call it from a constructor or with static semantics via template
  // specialization.
  static constexpr uint8_t _endpoint_base = 5;
  static uint8_t next_endpoint() {
    static uint8_t endpoint = _endpoint_base;
    return endpoint++;
  }
  static constexpr uint8_t endpoint_dir_host_to_device(const uint8_t ep) {
    return ep & 0x7F; // OUT endpoint
  }
  static constexpr uint8_t endpoint_dir_device_to_host(const uint8_t ep) {
    return ep | 0x80; // IN endpoint
  }
};

// SensorImpl is the base class for all USB HID-based sensors.
//
// Functionality specific to individual sensors (configuration and report
// descriptors, etc.) is declared without implementation in the class template
// below. Then, using template specialization in chan.cpp, the sensor-specific
// functionality is implemented. There is no need to subclass or specialize
// the entire SensorImpl class for each sensor. If sensors are added/removed,
// you only need to add or remove the corresponding specialization in chan.cpp.
//
// Note that each sensor is implemented logically as its own HID device, even
// though they are all enumerated on a single physical HID device. This is done
// to easily add or remove sensors from the project with minimal overhead. We
// use Arduino's "pluggable USB" API to achieve this.
//
// The individual sensor HID devices are assigned VID/PID/Version numbers based
// on their UUIDs, so no need to manually assign them separately. Refer to the
// sscanf() format string in the Ident constructor for details.
//
// Conceptually, this single physical HID device is a "sensor hub" which neatly
// corresponds to the Bluetooth LE architecture; where the sensor hub is the
// GATT server and the individual sensors are GATT characteristics.
template <typename T>
class SensorImpl: public Sensor {
protected:
  struct Desc {
    static constexpr uint8_t _defaultConfigIndex = 1;
    static constexpr uint8_t _defaultConfigLength =
      1*(CONFIGURATION_DESCRIPTOR_LENGTH) +
      1*(INTERFACE_DESCRIPTOR_LENGTH) +
      1*(HID_DESCRIPTOR_LENGTH) +
      1*(ENDPOINT_DESCRIPTOR_LENGTH);
    static uint8_t *report(uint16_t *size);
    static uint8_t *config(uint8_t index);

    // Return the index of the <skip+1>'th descriptor of the specified type, or
    // return -1 if not found.
    //
    // For example, to get the position of the second endpoint in the
    // configuration descriptor, call:
    //   index(config(0), _defaultConfigLength, ENDPOINT_DESCRIPTOR_TYPE, 1);
    static int index(uint8_t *desc, size_t size, uint8_t type, size_t skip = 0) {
      uint8_t *p = desc;
      while (p < desc + size) {
        if (p[1] == type && skip-- == 0) {
          return p - desc;
        }
        p += p[0];
      }
      return -1;
    }
  };

public:
  SensorImpl(USBPhy *phy, const Ident ident):
    Sensor(phy, 0, 0, ident.vid, ident.pid, ident.ver) {}
  SensorImpl(): SensorImpl(get_usb_phy(), Ident(uuid<T>())) {}

  virtual ~SensorImpl() {}

  // The first call to get() will instantiate a static instance of SensorImpl<T>.
  // All subsequent calls will return a reference to the same instance.
  static __noinline SensorImpl<T>& get() {
    static SensorImpl<T> c;
    return c;
  }

  virtual const uint8_t *report_desc() override {
    return Desc::report(&reportLength);
  }
  virtual const uint8_t *configuration_desc(uint8_t index) override {
    return Desc::config(index);
  }

  int write(packet::Data<typename T::Type>&& value) {
    // lock();
    HID_REPORT rp;
    size_t n = value.size();
    if (n > sizeof(rp.data) - 1) {
      n = sizeof(rp.data) - 1;
    }
    std::copy_n(value.begin(), n, rp.data + 1);
    //rp.data[0] = T::Type;
    rp.length = n + 1;

    if (!USBHID::send(&rp)) {
      rp.length = -rp.length; // return (-1 × length) on failure
    }
    // unlock();
    return rp.length;
  }
};

// Return all SensorImpl<T> instances as a single array of Sensor.
template <typename ...T>
std::array<Sensor, sizeof...(T)> sensors() {
  return { SensorImpl<T>::get()... };
}

// AccelerometerData
// BarometerData
// ColorData
// GestureData
// GyroscopeData
// HumidityData
// KeepAliveData
// MagnetometerData
// MicrophoneData
// ProximityData
// ScaleData
// TemperatureData
// ThermometerData

} // namespace chan

#endif // include_hid_chan_hpp
