#ifndef include_hid_chan_hpp
#define include_hid_chan_hpp

#include "spec.hpp"

namespace chan {

struct Ident {
  uint16_t vid, pid, ver;
  Ident() : Ident(0, 0, 0) {}
  Ident(const uint16_t vid, const uint16_t pid, const uint16_t ver)
      : vid(vid), pid(pid), ver(ver) {}
  Ident(UUID&& uuid) { sscanf(&uuid[0], "%04hx%01hx%03hx", &vid, &pid, &ver); }
};

// Sensor is the base class for all USB HID-based sensors.
//
// Since the sensors are each defined via template specialization, this base
// class is used to implement common functionality shared by all sensors.
//
// Note that each sensor is implemented logically as its own HID device, even
// though they are all enumerated on a single physical HID device. This is done
// to easily add or remove sensors from the project with minimal overhead. We
// use Arduino's "pluggable USB" API to implement this.
//
// Conceptually, this single physical HID device is a "sensor hub" which neatly
// corresponds to the Bluetooth LE architecture; where the sensor hub is the
// GATT server and the individual sensors are GATT characteristics.
template <typename T>
class Sensor: public arduino::USBHID {
protected:
  static PlatformMutex _sync; // exclusive access to the USB bus
  PlatformMutex _lock;        // exclusive access to the sensor

  decl_func_alias_(lock,   _lock.lock);
  decl_func_alias_(unlock, _lock.unlock);
  decl_func_alias_(sync,   _sync.lock);
  decl_func_alias_(async,  _sync.unlock);

public:
  Sensor(USBPhy *phy, const Ident ident):
    arduino::USBHID(phy, 0, 0, ident.vid, ident.pid, ident.ver) {}
  Sensor(): Sensor(get_usb_phy(), Ident(uuid<T>())) {}

  // The first call to get() will instantiate a static instance of Sensor<T>.
  // All subsequent calls will return a reference to the same instance.
  static Sensor<T>& get() {
    static Sensor<T> c;
    return c;
  }

  int write(packet::Data<typename T::Type>&& value) { return 0; }

  virtual const uint8_t *report_desc() override;
  virtual const uint8_t *configuration_desc(uint8_t index) override;
};

// template <> class Sensor<AccelerometerData>: public HID {
// public:
//   using HID::HID;

//   static inline constexpr Ident ident() { return { 0x0000, 0x0000, 0x0000 }; }

//   const uint8_t *report_desc() {
//     return 0;
//   }
//   const uint8_t *configuration_desc(uint8_t index) {
//     if (index != 0) {
//         return NULL;
//     }
//     static uint8_t desc[] = {};
//     reportLength = sizeof(desc);
//     return desc;
//   }
//   int write(packet::Data<AccelerometerData::Type>&& value) { return 0; }
// };

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

#if 0
        CONFIGURATION_DESCRIPTOR_LENGTH,    // bLength
        CONFIGURATION_DESCRIPTOR,           // bDescriptorType
        LSB(0xFFFF),                        // wTotalLength (LSB)
        MSB(0xFFFF),                        // wTotalLength (MSB)
        0x01,                               // bNumInterfaces
        1,                                  // bConfigurationValue
        0x00,                               // iConfiguration
        C_RESERVED | C_SELF_POWERED,        // bmAttributes
        C_POWER(100),                       // bMaxPower (mA)

        INTERFACE_DESCRIPTOR_LENGTH,        // bLength
        INTERFACE_DESCRIPTOR,               // bDescriptorType
        0x00,                               // bInterfaceNumber
        0x00,                               // bAlternateSetting
        0x01,                               // bNumEndpoints
        HID_CLASS,                          // bInterfaceClass
        HID_SUBCLASS_NONE,                  // bInterfaceSubClass
        HID_PROTOCOL_NONE,                  // bInterfaceProtocol
        0x00,                               // iInterface

        HID_DESCRIPTOR_LENGTH,              // bLength
        HID_DESCRIPTOR,                     // bDescriptorType
        LSB(HID_VERSION_1_11),              // bcdHID (LSB)
        MSB(HID_VERSION_1_11),              // bcdHID (MSB)
        0x00,                               // bCountryCode
        0x01,                               // bNumDescriptors
        REPORT_DESCRIPTOR,                  // bDescriptorType
        (uint8_t)(LSB(report_desc_length())), // wDescriptorLength (LSB)
        (uint8_t)(MSB(report_desc_length())), // wDescriptorLength (MSB)

        ENDPOINT_DESCRIPTOR_LENGTH,         // bLength
        ENDPOINT_DESCRIPTOR,                // bDescriptorType
        _int_in,                            // bEndpointAddress
        E_INTERRUPT,                        // bmAttributes
        LSB(MAX_HID_REPORT_SIZE),           // wMaxPacketSize (LSB)
        MSB(MAX_HID_REPORT_SIZE),           // wMaxPacketSize (MSB)
        5,                                  // bInterval (milliseconds)
#endif
