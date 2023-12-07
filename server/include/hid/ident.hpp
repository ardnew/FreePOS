#ifndef include_hid_ident_hpp
#define include_hid_ident_hpp

// Ident encapsulates the hardware identification of a USB HID device including
// the vendor ID, product ID, and product version.
//
// Devices are required to report these attributes during enumeration and are
// assigned by the USB Implementers Forum (USB-IF) to ensure uniqueness.
//
// However, that costs money and we don't have any. So we are going to use the
// first 32 bits of our somewhat-randomly-assigned UUIDs (type 4) we created for
// the Bluetooth LE (GATT) services.
struct Ident {
  uint16_t vid, pid, ver;
  Ident() : Ident(0, 0, 0) {}
  Ident(const uint16_t vid, const uint16_t pid, const uint16_t ver)
      : vid(vid), pid(pid), ver(ver) {}
  Ident(UUID&& uuid) { sscanf(&uuid[0], "%04hx%01hx%03hx", &vid, &pid, &ver); }
  virtual ~Ident() {}
};

#endif // include_hid_ident_hpp
