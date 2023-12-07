#include "hid/chan.hpp"

template <> uint8_t * chan::SensorImpl<AccelerometerData>::Desc::report(uint16_t *size) {
  static uint8_t desc[] = {
    0x05, 0x01,                    // USAGE_PAGE (Generic Desktop)
    0x09, 0x04,                    // USAGE (Joystick)
    0xa1, 0x01,                    // COLLECTION (Application)
    0xa1, 0x00,                    //   COLLECTION (Physical)
    0x05, 0x01,                    //     USAGE_PAGE (Generic Desktop)
    0x09, 0x30,                    //     USAGE (X)
    0x09, 0x31,                    //     USAGE (Y)
    0x09, 0x32,                    //     USAGE (Z)
    0x16, 0x01, 0x80,              //     LOGICAL_MINIMUM (-32767)
    0x26, 0xFF, 0x7F,              //     LOGICAL_MAXIMUM (32767)
    0x75, 0x10,                    //     REPORT_SIZE (16)
    0x95, 0x03,                    //     REPORT_COUNT (3)
    0x81, 0x02,                    //     INPUT (Data,Var,Abs)
    0xc0,                          //   END_COLLECTION
    0xc0                           // END_COLLECTION
  };
  *size = sizeof(desc);
  return desc;
}

template <> uint8_t * chan::SensorImpl<BarometerData>::Desc::report(uint16_t *size) {
  static uint8_t desc[] = {
    USAGE_PAGE(2), LSB(0xFFAB), MSB(0xFFAB),
    USAGE(2), LSB(0x0200), MSB(0x0200),
    COLLECTION(1), 0x01, // Collection (Application)

    REPORT_SIZE(1), 0x08, // 8 bits
    LOGICAL_MINIMUM(1), 0x00,
    LOGICAL_MAXIMUM(1), 0xFF,

    REPORT_COUNT(1), 1,
    USAGE(1), 0x01,
    INPUT(1), 0x02, // Data, Var, Abs

    REPORT_COUNT(1), 1,
    USAGE(1), 0x02,
    OUTPUT(1), 0x02, // Data, Var, Abs

    END_COLLECTION(0),
  };
  *size = sizeof(desc);
  return desc;
}

template <> uint8_t * chan::SensorImpl<ColorData>::Desc::report(uint16_t *size) {
  static uint8_t desc[] = {
    USAGE_PAGE(2), LSB(0xFFAB), MSB(0xFFAB),
    USAGE(2), LSB(0x0200), MSB(0x0200),
    COLLECTION(1), 0x01, // Collection (Application)

    REPORT_SIZE(1), 0x08, // 8 bits
    LOGICAL_MINIMUM(1), 0x00,
    LOGICAL_MAXIMUM(1), 0xFF,

    REPORT_COUNT(1), 1,
    USAGE(1), 0x01,
    INPUT(1), 0x02, // Data, Var, Abs

    REPORT_COUNT(1), 1,
    USAGE(1), 0x02,
    OUTPUT(1), 0x02, // Data, Var, Abs

    END_COLLECTION(0),
  };
  *size = sizeof(desc);
  return desc;
}

template <> uint8_t * chan::SensorImpl<GestureData>::Desc::report(uint16_t *size) {
  static uint8_t desc[] = {
    USAGE_PAGE(2), LSB(0xFFAB), MSB(0xFFAB),
    USAGE(2), LSB(0x0200), MSB(0x0200),
    COLLECTION(1), 0x01, // Collection (Application)

    REPORT_SIZE(1), 0x08, // 8 bits
    LOGICAL_MINIMUM(1), 0x00,
    LOGICAL_MAXIMUM(1), 0xFF,

    REPORT_COUNT(1), 1,
    USAGE(1), 0x01,
    INPUT(1), 0x02, // Data, Var, Abs

    REPORT_COUNT(1), 1,
    USAGE(1), 0x02,
    OUTPUT(1), 0x02, // Data, Var, Abs

    END_COLLECTION(0),
  };
  *size = sizeof(desc);
  return desc;
}

template <> uint8_t * chan::SensorImpl<GyroscopeData>::Desc::report(uint16_t *size) {
  static uint8_t desc[] = {
    USAGE_PAGE(2), LSB(0xFFAB), MSB(0xFFAB),
    USAGE(2), LSB(0x0200), MSB(0x0200),
    COLLECTION(1), 0x01, // Collection (Application)

    REPORT_SIZE(1), 0x08, // 8 bits
    LOGICAL_MINIMUM(1), 0x00,
    LOGICAL_MAXIMUM(1), 0xFF,

    REPORT_COUNT(1), 1,
    USAGE(1), 0x01,
    INPUT(1), 0x02, // Data, Var, Abs

    REPORT_COUNT(1), 1,
    USAGE(1), 0x02,
    OUTPUT(1), 0x02, // Data, Var, Abs

    END_COLLECTION(0),
  };
  *size = sizeof(desc);
  return desc;
}

template <> uint8_t * chan::SensorImpl<HumidityData>::Desc::report(uint16_t *size) {
  static uint8_t desc[] = {
    USAGE_PAGE(2), LSB(0xFFAB), MSB(0xFFAB),
    USAGE(2), LSB(0x0200), MSB(0x0200),
    COLLECTION(1), 0x01, // Collection (Application)

    REPORT_SIZE(1), 0x08, // 8 bits
    LOGICAL_MINIMUM(1), 0x00,
    LOGICAL_MAXIMUM(1), 0xFF,

    REPORT_COUNT(1), 1,
    USAGE(1), 0x01,
    INPUT(1), 0x02, // Data, Var, Abs

    REPORT_COUNT(1), 1,
    USAGE(1), 0x02,
    OUTPUT(1), 0x02, // Data, Var, Abs

    END_COLLECTION(0),
  };
  *size = sizeof(desc);
  return desc;
}

template <> uint8_t * chan::SensorImpl<KeepAliveData>::Desc::report(uint16_t *size) {
  static uint8_t desc[] = {
    USAGE_PAGE(2), LSB(0xFFAB), MSB(0xFFAB),
    USAGE(2), LSB(0x0200), MSB(0x0200),
    COLLECTION(1), 0x01, // Collection (Application)

    REPORT_SIZE(1), 0x08, // 8 bits
    LOGICAL_MINIMUM(1), 0x00,
    LOGICAL_MAXIMUM(1), 0xFF,

    REPORT_COUNT(1), 1,
    USAGE(1), 0x01,
    INPUT(1), 0x02, // Data, Var, Abs

    REPORT_COUNT(1), 1,
    USAGE(1), 0x02,
    OUTPUT(1), 0x02, // Data, Var, Abs

    END_COLLECTION(0),
  };
  *size = sizeof(desc);
  return desc;
}

template <> uint8_t * chan::SensorImpl<MagnetometerData>::Desc::report(uint16_t *size) {
  static uint8_t desc[] = {
    USAGE_PAGE(2), LSB(0xFFAB), MSB(0xFFAB),
    USAGE(2), LSB(0x0200), MSB(0x0200),
    COLLECTION(1), 0x01, // Collection (Application)

    REPORT_SIZE(1), 0x08, // 8 bits
    LOGICAL_MINIMUM(1), 0x00,
    LOGICAL_MAXIMUM(1), 0xFF,

    REPORT_COUNT(1), 1,
    USAGE(1), 0x01,
    INPUT(1), 0x02, // Data, Var, Abs

    REPORT_COUNT(1), 1,
    USAGE(1), 0x02,
    OUTPUT(1), 0x02, // Data, Var, Abs

    END_COLLECTION(0),
  };
  *size = sizeof(desc);
  return desc;
}

template <> uint8_t * chan::SensorImpl<MicrophoneData>::Desc::report(uint16_t *size) {
  static uint8_t desc[] = {
    USAGE_PAGE(2), LSB(0xFFAB), MSB(0xFFAB),
    USAGE(2), LSB(0x0200), MSB(0x0200),
    COLLECTION(1), 0x01, // Collection (Application)

    REPORT_SIZE(1), 0x08, // 8 bits
    LOGICAL_MINIMUM(1), 0x00,
    LOGICAL_MAXIMUM(1), 0xFF,

    REPORT_COUNT(1), 1,
    USAGE(1), 0x01,
    INPUT(1), 0x02, // Data, Var, Abs

    REPORT_COUNT(1), 1,
    USAGE(1), 0x02,
    OUTPUT(1), 0x02, // Data, Var, Abs

    END_COLLECTION(0),
  };
  *size = sizeof(desc);
  return desc;
}

template <> uint8_t * chan::SensorImpl<ProximityData>::Desc::report(uint16_t *size) {
  static uint8_t desc[] = {
    USAGE_PAGE(2), LSB(0xFFAB), MSB(0xFFAB),
    USAGE(2), LSB(0x0200), MSB(0x0200),
    COLLECTION(1), 0x01, // Collection (Application)

    REPORT_SIZE(1), 0x08, // 8 bits
    LOGICAL_MINIMUM(1), 0x00,
    LOGICAL_MAXIMUM(1), 0xFF,

    REPORT_COUNT(1), 1,
    USAGE(1), 0x01,
    INPUT(1), 0x02, // Data, Var, Abs

    REPORT_COUNT(1), 1,
    USAGE(1), 0x02,
    OUTPUT(1), 0x02, // Data, Var, Abs

    END_COLLECTION(0),
  };
  *size = sizeof(desc);
  return desc;
}

template <> uint8_t * chan::SensorImpl<ScaleData>::Desc::report(uint16_t *size) {
  static uint8_t desc[] = {
    USAGE_PAGE(2), LSB(0xFFAB), MSB(0xFFAB),
    USAGE(2), LSB(0x0200), MSB(0x0200),
    COLLECTION(1), 0x01, // Collection (Application)

    REPORT_SIZE(1), 0x08, // 8 bits
    LOGICAL_MINIMUM(1), 0x00,
    LOGICAL_MAXIMUM(1), 0xFF,

    REPORT_COUNT(1), 1,
    USAGE(1), 0x01,
    INPUT(1), 0x02, // Data, Var, Abs

    REPORT_COUNT(1), 1,
    USAGE(1), 0x02,
    OUTPUT(1), 0x02, // Data, Var, Abs

    END_COLLECTION(0),
  };
  *size = sizeof(desc);
  return desc;
}

template <> uint8_t * chan::SensorImpl<TemperatureData>::Desc::report(uint16_t *size) {
  static uint8_t desc[] = {
    USAGE_PAGE(2), LSB(0xFFAB), MSB(0xFFAB),
    USAGE(2), LSB(0x0200), MSB(0x0200),
    COLLECTION(1), 0x01, // Collection (Application)

    REPORT_SIZE(1), 0x08, // 8 bits
    LOGICAL_MINIMUM(1), 0x00,
    LOGICAL_MAXIMUM(1), 0xFF,

    REPORT_COUNT(1), 1,
    USAGE(1), 0x01,
    INPUT(1), 0x02, // Data, Var, Abs

    REPORT_COUNT(1), 1,
    USAGE(1), 0x02,
    OUTPUT(1), 0x02, // Data, Var, Abs

    END_COLLECTION(0),
  };
  *size = sizeof(desc);
  return desc;
}

template <> uint8_t * chan::SensorImpl<ThermometerData>::Desc::report(uint16_t *size) {
  static uint8_t desc[] = {
    USAGE_PAGE(2), LSB(0xFFAB), MSB(0xFFAB),
    USAGE(2), LSB(0x0200), MSB(0x0200),
    COLLECTION(1), 0x01, // Collection (Application)

    REPORT_SIZE(1), 0x08, // 8 bits
    LOGICAL_MINIMUM(1), 0x00,
    LOGICAL_MAXIMUM(1), 0xFF,

    REPORT_COUNT(1), 1,
    USAGE(1), 0x01,
    INPUT(1), 0x02, // Data, Var, Abs

    REPORT_COUNT(1), 1,
    USAGE(1), 0x02,
    OUTPUT(1), 0x02, // Data, Var, Abs

    END_COLLECTION(0),
  };
  *size = sizeof(desc);
  return desc;
}

template <> uint8_t * chan::SensorImpl<AccelerometerData>::Desc::config(uint8_t index) {
  static uint8_t desc[] = {
    CONFIGURATION_DESCRIPTOR_LENGTH,    // bLength
    CONFIGURATION_DESCRIPTOR,           // bDescriptorType
    LSB(_defaultConfigLength),          // wTotalLength (LSB)
    MSB(_defaultConfigLength),          // wTotalLength (MSB)
    0x01,                               // bNumInterfaces
    _defaultConfigIndex,                // bConfigurationValue
    0x00,                               // iConfiguration
    C_RESERVED | C_SELF_POWERED,        // bmAttributes
    C_POWER(0),                         // bMaxPower

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
    (uint8_t)(LSB(SensorImpl<AccelerometerData>::get().report_desc_length())), // wDescriptorLength (LSB)
    (uint8_t)(MSB(SensorImpl<AccelerometerData>::get().report_desc_length())), // wDescriptorLength (MSB)

    ENDPOINT_DESCRIPTOR_LENGTH,         // bLength
    ENDPOINT_DESCRIPTOR,                // bDescriptorType
    endpoint_dir_device_to_host(next_endpoint()),  // bEndpointAddress
    E_INTERRUPT,                        // bmAttributes
    LSB(MAX_HID_REPORT_SIZE),           // wMaxPacketSize (LSB)
    MSB(MAX_HID_REPORT_SIZE),           // wMaxPacketSize (MSB)
    1,                                  // bInterval (milliseconds)
  };
  return desc;
}

template <> uint8_t * chan::SensorImpl<BarometerData>::Desc::config(uint8_t index) {
  static uint8_t desc[] = {
    CONFIGURATION_DESCRIPTOR_LENGTH,    // bLength
    CONFIGURATION_DESCRIPTOR,           // bDescriptorType
    LSB(_defaultConfigLength),          // wTotalLength (LSB)
    MSB(_defaultConfigLength),          // wTotalLength (MSB)
    0x01,                               // bNumInterfaces
    _defaultConfigIndex,                // bConfigurationValue
    0x00,                               // iConfiguration
    C_RESERVED | C_SELF_POWERED,        // bmAttributes
    C_POWER(0),                         // bMaxPower

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
    (uint8_t)(LSB(SensorImpl<BarometerData>::get().report_desc_length())), // wDescriptorLength (LSB)
    (uint8_t)(MSB(SensorImpl<BarometerData>::get().report_desc_length())), // wDescriptorLength (MSB)

    ENDPOINT_DESCRIPTOR_LENGTH,         // bLength
    ENDPOINT_DESCRIPTOR,                // bDescriptorType
    endpoint_dir_device_to_host(next_endpoint()),  // bEndpointAddress
    E_INTERRUPT,                        // bmAttributes
    LSB(MAX_HID_REPORT_SIZE),           // wMaxPacketSize (LSB)
    MSB(MAX_HID_REPORT_SIZE),           // wMaxPacketSize (MSB)
    1,                                  // bInterval (milliseconds)
  };
  return desc;
}

template <> uint8_t * chan::SensorImpl<ColorData>::Desc::config(uint8_t index) {
  static uint8_t desc[] = {
    CONFIGURATION_DESCRIPTOR_LENGTH,    // bLength
    CONFIGURATION_DESCRIPTOR,           // bDescriptorType
    LSB(_defaultConfigLength),          // wTotalLength (LSB)
    MSB(_defaultConfigLength),          // wTotalLength (MSB)
    0x01,                               // bNumInterfaces
    _defaultConfigIndex,                // bConfigurationValue
    0x00,                               // iConfiguration
    C_RESERVED | C_SELF_POWERED,        // bmAttributes
    C_POWER(0),                         // bMaxPower

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
    (uint8_t)(LSB(SensorImpl<ColorData>::get().report_desc_length())), // wDescriptorLength (LSB)
    (uint8_t)(MSB(SensorImpl<ColorData>::get().report_desc_length())), // wDescriptorLength (MSB)

    ENDPOINT_DESCRIPTOR_LENGTH,         // bLength
    ENDPOINT_DESCRIPTOR,                // bDescriptorType
    endpoint_dir_device_to_host(next_endpoint()),  // bEndpointAddress
    E_INTERRUPT,                        // bmAttributes
    LSB(MAX_HID_REPORT_SIZE),           // wMaxPacketSize (LSB)
    MSB(MAX_HID_REPORT_SIZE),           // wMaxPacketSize (MSB)
    1,                                  // bInterval (milliseconds)
  };
  return desc;
}

template <> uint8_t * chan::SensorImpl<GestureData>::Desc::config(uint8_t index) {
  static uint8_t desc[] = {
    CONFIGURATION_DESCRIPTOR_LENGTH,    // bLength
    CONFIGURATION_DESCRIPTOR,           // bDescriptorType
    LSB(_defaultConfigLength),          // wTotalLength (LSB)
    MSB(_defaultConfigLength),          // wTotalLength (MSB)
    0x01,                               // bNumInterfaces
    _defaultConfigIndex,                // bConfigurationValue
    0x00,                               // iConfiguration
    C_RESERVED | C_SELF_POWERED,        // bmAttributes
    C_POWER(0),                         // bMaxPower

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
    (uint8_t)(LSB(SensorImpl<GestureData>::get().report_desc_length())), // wDescriptorLength (LSB)
    (uint8_t)(MSB(SensorImpl<GestureData>::get().report_desc_length())), // wDescriptorLength (MSB)

    ENDPOINT_DESCRIPTOR_LENGTH,         // bLength
    ENDPOINT_DESCRIPTOR,                // bDescriptorType
    endpoint_dir_device_to_host(next_endpoint()),  // bEndpointAddress
    E_INTERRUPT,                        // bmAttributes
    LSB(MAX_HID_REPORT_SIZE),           // wMaxPacketSize (LSB)
    MSB(MAX_HID_REPORT_SIZE),           // wMaxPacketSize (MSB)
    1,                                  // bInterval (milliseconds)
  };
  return desc;
}

template <> uint8_t * chan::SensorImpl<GyroscopeData>::Desc::config(uint8_t index) {
  static uint8_t desc[] = {
    CONFIGURATION_DESCRIPTOR_LENGTH,    // bLength
    CONFIGURATION_DESCRIPTOR,           // bDescriptorType
    LSB(_defaultConfigLength),          // wTotalLength (LSB)
    MSB(_defaultConfigLength),          // wTotalLength (MSB)
    0x01,                               // bNumInterfaces
    _defaultConfigIndex,                // bConfigurationValue
    0x00,                               // iConfiguration
    C_RESERVED | C_SELF_POWERED,        // bmAttributes
    C_POWER(0),                         // bMaxPower

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
    (uint8_t)(LSB(SensorImpl<GyroscopeData>::get().report_desc_length())), // wDescriptorLength (LSB)
    (uint8_t)(MSB(SensorImpl<GyroscopeData>::get().report_desc_length())), // wDescriptorLength (MSB)

    ENDPOINT_DESCRIPTOR_LENGTH,         // bLength
    ENDPOINT_DESCRIPTOR,                // bDescriptorType
    endpoint_dir_device_to_host(next_endpoint()),  // bEndpointAddress
    E_INTERRUPT,                        // bmAttributes
    LSB(MAX_HID_REPORT_SIZE),           // wMaxPacketSize (LSB)
    MSB(MAX_HID_REPORT_SIZE),           // wMaxPacketSize (MSB)
    1,                                  // bInterval (milliseconds)
  };
  return desc;
}

template <> uint8_t * chan::SensorImpl<HumidityData>::Desc::config(uint8_t index) {
  static uint8_t desc[] = {
    CONFIGURATION_DESCRIPTOR_LENGTH,    // bLength
    CONFIGURATION_DESCRIPTOR,           // bDescriptorType
    LSB(_defaultConfigLength),          // wTotalLength (LSB)
    MSB(_defaultConfigLength),          // wTotalLength (MSB)
    0x01,                               // bNumInterfaces
    _defaultConfigIndex,                // bConfigurationValue
    0x00,                               // iConfiguration
    C_RESERVED | C_SELF_POWERED,        // bmAttributes
    C_POWER(0),                         // bMaxPower

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
    (uint8_t)(LSB(SensorImpl<HumidityData>::get().report_desc_length())), // wDescriptorLength (LSB)
    (uint8_t)(MSB(SensorImpl<HumidityData>::get().report_desc_length())), // wDescriptorLength (MSB)

    ENDPOINT_DESCRIPTOR_LENGTH,         // bLength
    ENDPOINT_DESCRIPTOR,                // bDescriptorType
    endpoint_dir_device_to_host(next_endpoint()),  // bEndpointAddress
    E_INTERRUPT,                        // bmAttributes
    LSB(MAX_HID_REPORT_SIZE),           // wMaxPacketSize (LSB)
    MSB(MAX_HID_REPORT_SIZE),           // wMaxPacketSize (MSB)
    1,                                  // bInterval (milliseconds)
  };
  return desc;
}

template <> uint8_t * chan::SensorImpl<KeepAliveData>::Desc::config(uint8_t index) {
  static uint8_t desc[] = {
    CONFIGURATION_DESCRIPTOR_LENGTH,    // bLength
    CONFIGURATION_DESCRIPTOR,           // bDescriptorType
    LSB(_defaultConfigLength),          // wTotalLength (LSB)
    MSB(_defaultConfigLength),          // wTotalLength (MSB)
    0x01,                               // bNumInterfaces
    _defaultConfigIndex,                // bConfigurationValue
    0x00,                               // iConfiguration
    C_RESERVED | C_SELF_POWERED,        // bmAttributes
    C_POWER(0),                         // bMaxPower

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
    (uint8_t)(LSB(SensorImpl<KeepAliveData>::get().report_desc_length())), // wDescriptorLength (LSB)
    (uint8_t)(MSB(SensorImpl<KeepAliveData>::get().report_desc_length())), // wDescriptorLength (MSB)

    ENDPOINT_DESCRIPTOR_LENGTH,         // bLength
    ENDPOINT_DESCRIPTOR,                // bDescriptorType
    endpoint_dir_device_to_host(next_endpoint()),  // bEndpointAddress
    E_INTERRUPT,                        // bmAttributes
    LSB(MAX_HID_REPORT_SIZE),           // wMaxPacketSize (LSB)
    MSB(MAX_HID_REPORT_SIZE),           // wMaxPacketSize (MSB)
    1,                                  // bInterval (milliseconds)
  };
  return desc;
}

template <> uint8_t * chan::SensorImpl<MagnetometerData>::Desc::config(uint8_t index) {
  static uint8_t desc[] = {
    CONFIGURATION_DESCRIPTOR_LENGTH,    // bLength
    CONFIGURATION_DESCRIPTOR,           // bDescriptorType
    LSB(_defaultConfigLength),          // wTotalLength (LSB)
    MSB(_defaultConfigLength),          // wTotalLength (MSB)
    0x01,                               // bNumInterfaces
    _defaultConfigIndex,                // bConfigurationValue
    0x00,                               // iConfiguration
    C_RESERVED | C_SELF_POWERED,        // bmAttributes
    C_POWER(0),                         // bMaxPower

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
    (uint8_t)(LSB(SensorImpl<MagnetometerData>::get().report_desc_length())), // wDescriptorLength (LSB)
    (uint8_t)(MSB(SensorImpl<MagnetometerData>::get().report_desc_length())), // wDescriptorLength (MSB)

    ENDPOINT_DESCRIPTOR_LENGTH,         // bLength
    ENDPOINT_DESCRIPTOR,                // bDescriptorType
    endpoint_dir_device_to_host(next_endpoint()),  // bEndpointAddress
    E_INTERRUPT,                        // bmAttributes
    LSB(MAX_HID_REPORT_SIZE),           // wMaxPacketSize (LSB)
    MSB(MAX_HID_REPORT_SIZE),           // wMaxPacketSize (MSB)
    1,                                  // bInterval (milliseconds)
  };
  return desc;
}

template <> uint8_t * chan::SensorImpl<MicrophoneData>::Desc::config(uint8_t index) {
  static uint8_t desc[] = {
    CONFIGURATION_DESCRIPTOR_LENGTH,    // bLength
    CONFIGURATION_DESCRIPTOR,           // bDescriptorType
    LSB(_defaultConfigLength),          // wTotalLength (LSB)
    MSB(_defaultConfigLength),          // wTotalLength (MSB)
    0x01,                               // bNumInterfaces
    _defaultConfigIndex,                // bConfigurationValue
    0x00,                               // iConfiguration
    C_RESERVED | C_SELF_POWERED,        // bmAttributes
    C_POWER(0),                         // bMaxPower

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
    (uint8_t)(LSB(SensorImpl<MicrophoneData>::get().report_desc_length())), // wDescriptorLength (LSB)
    (uint8_t)(MSB(SensorImpl<MicrophoneData>::get().report_desc_length())), // wDescriptorLength (MSB)

    ENDPOINT_DESCRIPTOR_LENGTH,         // bLength
    ENDPOINT_DESCRIPTOR,                // bDescriptorType
    endpoint_dir_device_to_host(next_endpoint()),  // bEndpointAddress
    E_INTERRUPT,                        // bmAttributes
    LSB(MAX_HID_REPORT_SIZE),           // wMaxPacketSize (LSB)
    MSB(MAX_HID_REPORT_SIZE),           // wMaxPacketSize (MSB)
    1,                                  // bInterval (milliseconds)
  };
  return desc;
}

template <> uint8_t * chan::SensorImpl<ProximityData>::Desc::config(uint8_t index) {
  static uint8_t desc[] = {
    CONFIGURATION_DESCRIPTOR_LENGTH,    // bLength
    CONFIGURATION_DESCRIPTOR,           // bDescriptorType
    LSB(_defaultConfigLength),          // wTotalLength (LSB)
    MSB(_defaultConfigLength),          // wTotalLength (MSB)
    0x01,                               // bNumInterfaces
    _defaultConfigIndex,                // bConfigurationValue
    0x00,                               // iConfiguration
    C_RESERVED | C_SELF_POWERED,        // bmAttributes
    C_POWER(0),                         // bMaxPower

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
    (uint8_t)(LSB(SensorImpl<ProximityData>::get().report_desc_length())), // wDescriptorLength (LSB)
    (uint8_t)(MSB(SensorImpl<ProximityData>::get().report_desc_length())), // wDescriptorLength (MSB)

    ENDPOINT_DESCRIPTOR_LENGTH,         // bLength
    ENDPOINT_DESCRIPTOR,                // bDescriptorType
    endpoint_dir_device_to_host(next_endpoint()),  // bEndpointAddress
    E_INTERRUPT,                        // bmAttributes
    LSB(MAX_HID_REPORT_SIZE),           // wMaxPacketSize (LSB)
    MSB(MAX_HID_REPORT_SIZE),           // wMaxPacketSize (MSB)
    1,                                  // bInterval (milliseconds)
  };
  return desc;
}

template <> uint8_t * chan::SensorImpl<ScaleData>::Desc::config(uint8_t index) {
  static uint8_t desc[] = {
    CONFIGURATION_DESCRIPTOR_LENGTH,    // bLength
    CONFIGURATION_DESCRIPTOR,           // bDescriptorType
    LSB(_defaultConfigLength),          // wTotalLength (LSB)
    MSB(_defaultConfigLength),          // wTotalLength (MSB)
    0x01,                               // bNumInterfaces
    _defaultConfigIndex,                // bConfigurationValue
    0x00,                               // iConfiguration
    C_RESERVED | C_SELF_POWERED,        // bmAttributes
    C_POWER(0),                         // bMaxPower

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
    (uint8_t)(LSB(SensorImpl<ScaleData>::get().report_desc_length())), // wDescriptorLength (LSB)
    (uint8_t)(MSB(SensorImpl<ScaleData>::get().report_desc_length())), // wDescriptorLength (MSB)

    ENDPOINT_DESCRIPTOR_LENGTH,         // bLength
    ENDPOINT_DESCRIPTOR,                // bDescriptorType
    endpoint_dir_device_to_host(next_endpoint()),  // bEndpointAddress
    E_INTERRUPT,                        // bmAttributes
    LSB(MAX_HID_REPORT_SIZE),           // wMaxPacketSize (LSB)
    MSB(MAX_HID_REPORT_SIZE),           // wMaxPacketSize (MSB)
    1,                                  // bInterval (milliseconds)
  };
  return desc;
}

template <> uint8_t * chan::SensorImpl<TemperatureData>::Desc::config(uint8_t index) {
  static uint8_t desc[] = {
    CONFIGURATION_DESCRIPTOR_LENGTH,    // bLength
    CONFIGURATION_DESCRIPTOR,           // bDescriptorType
    LSB(_defaultConfigLength),          // wTotalLength (LSB)
    MSB(_defaultConfigLength),          // wTotalLength (MSB)
    0x01,                               // bNumInterfaces
    _defaultConfigIndex,                // bConfigurationValue
    0x00,                               // iConfiguration
    C_RESERVED | C_SELF_POWERED,        // bmAttributes
    C_POWER(0),                         // bMaxPower

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
    (uint8_t)(LSB(SensorImpl<TemperatureData>::get().report_desc_length())), // wDescriptorLength (LSB)
    (uint8_t)(MSB(SensorImpl<TemperatureData>::get().report_desc_length())), // wDescriptorLength (MSB)

    ENDPOINT_DESCRIPTOR_LENGTH,         // bLength
    ENDPOINT_DESCRIPTOR,                // bDescriptorType
    endpoint_dir_device_to_host(next_endpoint()),  // bEndpointAddress
    E_INTERRUPT,                        // bmAttributes
    LSB(MAX_HID_REPORT_SIZE),           // wMaxPacketSize (LSB)
    MSB(MAX_HID_REPORT_SIZE),           // wMaxPacketSize (MSB)
    1,                                  // bInterval (milliseconds)
  };
  return desc;
}

template <> uint8_t * chan::SensorImpl<ThermometerData>::Desc::config(uint8_t index) {
  static uint8_t desc[] = {
    CONFIGURATION_DESCRIPTOR_LENGTH,    // bLength
    CONFIGURATION_DESCRIPTOR,           // bDescriptorType
    LSB(_defaultConfigLength),          // wTotalLength (LSB)
    MSB(_defaultConfigLength),          // wTotalLength (MSB)
    0x01,                               // bNumInterfaces
    _defaultConfigIndex,                // bConfigurationValue
    0x00,                               // iConfiguration
    C_RESERVED | C_SELF_POWERED,        // bmAttributes
    C_POWER(0),                         // bMaxPower

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
    (uint8_t)(LSB(SensorImpl<ThermometerData>::get().report_desc_length())), // wDescriptorLength (LSB)
    (uint8_t)(MSB(SensorImpl<ThermometerData>::get().report_desc_length())), // wDescriptorLength (MSB)

    ENDPOINT_DESCRIPTOR_LENGTH,         // bLength
    ENDPOINT_DESCRIPTOR,                // bDescriptorType
    endpoint_dir_device_to_host(next_endpoint()),  // bEndpointAddress
    E_INTERRUPT,                        // bmAttributes
    LSB(MAX_HID_REPORT_SIZE),           // wMaxPacketSize (LSB)
    MSB(MAX_HID_REPORT_SIZE),           // wMaxPacketSize (MSB)
    1,                                  // bInterval (milliseconds)
  };
  return desc;
}
