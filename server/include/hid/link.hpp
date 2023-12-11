#ifndef include_hid_link_hpp
#define include_hid_link_hpp

#include "hid/chan.hpp"
#include "hid/ident.hpp"
#include "hid/spec/sensor.h"

namespace link {

template <class... T>
class SensorPool {
public:
  // tuple holds each concrete SensorImpl<T> object from T.
  using tuple = std::tuple<chan::SensorImpl<T>...>;
  // array holds each base Sensor object from T, convenient for iterating.
  using array = std::array<chan::Sensor, sizeof...(T)>;

  SensorPool():
    // get() returns a reference to static SensorImpl<T> instances.
    _tuple(chan::SensorImpl<T>::get()...),
    _array({ chan::SensorImpl<T>::get()... }) {}
  virtual ~SensorPool() {}

protected:
  tuple _tuple;
  array _array;
};

// Identify all of the sensors we wish to present on the USB bus.
//
// Each sensor is encapsulated and identified by a separate HID report ID, but
// all belong to a single interface defined in a single devcie configuration.
//
// This is a cheap way to implement composite HID devices, but it minimizes the
// required number of device endpoints (which are hardware-limited resources).
//
// Using this scheme, all sensor HID reports are transmitted over a single
// IN-interrupt endpoint, and thus there is no limit on the number of sensors we
// can support.
//
// See also chan::SensorImpl<T>::desc() in include/hid/chan.hpp for more info.
using Sensors = SensorPool<
  AccelerometerData
  // BarometerData,
  // ColorData,
  // GestureData,
  // GyroscopeData,
  // HumidityData,
  // KeepAliveData,
  // MagnetometerData,
  // MicrophoneData,
  // ProximityData,
  // ScaleData,
  // TemperatureData,
  // ThermometerData,
>;

class Hub: public arduino::USBHID, public Sensors {
protected:
  const Ident _ident;

public:

  Hub(USBPhy *phy, const Ident ident):
    arduino::USBHID(phy, 0, 0, ident.vid, ident.pid, ident.ver),
    _ident(ident) {}
  Hub(): Hub(get_usb_phy(), Ident(uuid<SensorData>())) {}
  virtual ~Hub() {}

  bool init();
  bool connect();
  bool connected();

  template <typename T>
  int tx(packet::Data<typename T::Type>&& p) {
    const chan::Sensor::Report rp = chan::SensorImpl<T>::get().write(std::move(p));
    if (!USBHID::send(&rp)) {
      return -rp.length; // return -1 × length on error
    }
    return rp.length;
  }

  //virtual void callback_reset() {} override;
  //virtual void callback_state_change(USBDevice::DeviceState new_state) override;
  //virtual uint32_t callback_request(const USBDevice::setup_packet_t *setup, USBDevice::RequestResult *result, uint8_t** data) override;
  //virtual bool callback_request_xfer_done(const USBDevice::setup_packet_t *setup, bool aborted) override;
  //virtual bool callback_set_configuration(uint8_t configuration) override;
  //virtual void callback_set_interface(uint16_t interface, uint8_t alternate) override;
  //virtual void init(EndpointResolver& resolver) override;
  //virtual const uint8_t *string_iinterface_desc() override;
  //virtual uint8_t getProductVersion() override;

  // virtual const uint8_t *device_desc() override {
  //   static uint8_t desc[] = {
  //     DEVICE_DESCRIPTOR_LENGTH,         // bLength
  //     DEVICE_DESCRIPTOR,                // bDescriptorType
  //     LSB(USB_VERSION_2_0),             // bcdUSB.lsb
  //     MSB(USB_VERSION_2_0),             // bcdUSB.msb
  //     0x00,                             // bDeviceClass
  //     0x00,                             // bDeviceSubClass
  //     0x00,                             // bDeviceProtocol
  //     endpoint_max_packet_size(0x00),   // bMaxPacketSize0
  //     LSB(_ident.vid), MSB(_ident.vid), // idVendor
  //     LSB(_ident.pid), MSB(_ident.pid), // idProduct
  //     LSB(_ident.ver), MSB(_ident.ver), // bcdDevice
  //     STRING_OFFSET_IMANUFACTURER,      // iManufacturer
  //     STRING_OFFSET_IPRODUCT,           // iProduct
  //     STRING_OFFSET_ISERIAL,            // iSerialNumber
  //     0x01                              // bNumConfigurations
  //   };
  //   return desc;
  // }

  const uint8_t *configuration_desc(uint8_t index) {
    if (index != 0) {
      return nullptr;
    }

    (void)report_desc();

    uint8_t desc[] = {
      CONFIGURATION_DESCRIPTOR_LENGTH,    // bLength
      CONFIGURATION_DESCRIPTOR,           // bDescriptorType
      LSB(descriptor::config_size),       // wTotalLength (LSB)
      MSB(descriptor::config_size),       // wTotalLength (MSB)
      0x01,                               // bNumInterfaces
      0x01,                               // bConfigurationValue
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
      (uint8_t)(LSB(reportLength)),       // wDescriptorLength (LSB)
      (uint8_t)(MSB(reportLength)),       // wDescriptorLength (MSB)

      ENDPOINT_DESCRIPTOR_LENGTH,         // bLength
      ENDPOINT_DESCRIPTOR,                // bDescriptorType
      0x80 | 1,                                                      // bEndpointAddress
      E_INTERRUPT,                        // bmAttributes
      LSB(MAX_HID_REPORT_SIZE),           // wMaxPacketSize (LSB)
      MSB(MAX_HID_REPORT_SIZE),           // wMaxPacketSize (MSB)
      1,                                  // bInterval (milliseconds)
    };
    MBED_ASSERT(sizeof(desc) == descriptor::config_size);
    memcpy(_desc.config, desc, descriptor::config_size);
    return _desc.config;
  }

  const uint8_t *report_desc() {
    uint8_t desc[] = {
      HID_USAGE_PAGE_SENSOR,
      HID_USAGE_SENSOR_TYPE_COLLECTION,
      HID_COLLECTION(Application),
      HID_END_COLLECTION,
    };
    reportLength = descriptor::report_size;
    MBED_ASSERT(sizeof(desc) == descriptor::report_size);
    memcpy(_desc.report, desc, descriptor::report_size);
    return _desc.report;
  }

private:
  class descriptor {
  public:
    static constexpr size_t config_size =
      1 * CONFIGURATION_DESCRIPTOR_LENGTH +
      1 * INTERFACE_DESCRIPTOR_LENGTH +
      1 * HID_DESCRIPTOR_LENGTH +
      1 * ENDPOINT_DESCRIPTOR_LENGTH;
    static constexpr size_t report_size = 7UL;
    uint8_t config[config_size];
    uint8_t report[report_size];
  } _desc;
};

// Return the index of the <skip+1>'th descriptor of the specified type, or
// return -1 if not found.
//
// For example, to get the position of the second endpoint in the
// configuration descriptor, call:
//   index(config(0), _defaultConfigLength, ENDPOINT_DESCRIPTOR_TYPE, 1);
// static int index(uint8_t *desc, size_t size, uint8_t type, size_t skip = 0) {
//   uint8_t *p = desc;
//   while (p < desc + size) {
//     if (p[1] == type && skip-- == 0) {
//       return p - desc;
//     }
//     p += p[0];
//   }
//   return -1;
// }


}

#endif // include_hid_link_hpp
