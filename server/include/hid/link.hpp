#ifndef include_hid_link_hpp
#define include_hid_link_hpp

#include "hid/chan.hpp"
#include "hid/ident.hpp"

namespace link {

class Hub: public arduino::USBHID {
public:
  static auto sensors() {
    return chan::sensors<
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
      // TemperatureData
      // ThermometerData
    >();
  }

  Hub(USBPhy *phy, const Ident ident):
    arduino::USBHID(phy, 0, 0, ident.vid, ident.pid, ident.ver) {
      sensors();
    }
  Hub(): Hub(get_usb_phy(), Ident(uuid<SensorData>())) {}
  virtual ~Hub() {}

  bool init();
  bool connect();
  bool connected();

  template <typename T>
  int tx(packet::Data<typename T::Type>&& p) {
    return chan::SensorImpl<T>::get().write(std::move(p));
  }

  //virtual const uint8_t *configuration_desc(uint8_t index) override;
  //virtual void callback_reset() {} override;
  //virtual void callback_state_change(USBDevice::DeviceState new_state) override;
  //virtual uint32_t callback_request(const USBDevice::setup_packet_t *setup, USBDevice::RequestResult *result, uint8_t** data) override;
  //virtual bool callback_request_xfer_done(const USBDevice::setup_packet_t *setup, bool aborted) override;
  //virtual bool callback_set_configuration(uint8_t configuration) override;
  //virtual void callback_set_interface(uint16_t interface, uint8_t alternate) override;
  //virtual void init(EndpointResolver& resolver) override;
  //virtual const uint8_t *string_iinterface_desc() override;
  //virtual uint8_t getProductVersion() override;

  virtual const uint8_t *report_desc() override {
    static uint8_t desc[] = {
    };
    reportLength = sizeof(desc);
    return desc;
  }
};

}

#endif // include_hid_link_hpp
