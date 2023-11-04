#ifndef radio_hpp
#define radio_hpp

#include "ipc.hpp"
#include "keepalive.hpp"
#include "spec.hpp"
#include "state.hpp"
#include "view.hpp"

class Radio : public BLEDevice,
              public BLEAdvertisedDeviceCallbacks,
              public BLEClientCallbacks {
  using BLEDevice::BLEDevice;
  using Device = ESP323248S035C<MainView>;

private:
  const Device &_dev;
  const MainView &_view;
  IPC &_chan;

  bool _doScan;
  bool _doConn;
  bool _isConn;

protected:
  BLEAdvertisedDevice *_server;
  BLEClient *_client;
  BLERemoteCharacteristic *_charPulses;
  BLERemoteCharacteristic *_charAccXYZ;
  BLERemoteCharacteristic *_charWeight;

  // BLERemoteCharacteristic *_charBarPsi;
  // BLERemoteCharacteristic *_charPrecip;
  // BLERemoteCharacteristic *_charProxim;
  // BLERemoteCharacteristic *_charAirTmp;
  // BLERemoteCharacteristic *_charH2OTmp;

protected:
  static constexpr msecu32_t _refresh = msecu32_t{500};

public:
  static inline constexpr msecu32_t refresh() { return _refresh; }

protected:
  KeepAlive<uint32_t> _alive;
  State _state;

public:
  Radio(Device &dev, IPC &chan, MainView &root)
      : _dev(dev), _view(root), _chan(chan), _doScan(true), _doConn(false),
        _isConn(false), _alive(std::chrono::seconds{5}),
        _state(dev.hw<RGB_PWM>()) {
    BLEDevice::init(DeviceName);
    BLEScan *s = getScan();
    s->setAdvertisedDeviceCallbacks(this);
    s->setInterval(1349);
    s->setWindow(449);
    s->setActiveScan(true);
    setState(State::ScanStart);
  }

  bool setState(State::Enum const state) {
    stracef("setState: %d", state);
    // Don't process unless state is changing
    if (_state == state) {
      return true;
    }

    switch (state) {
    case State::Disable:
      stracef("%s", "DISABLE!");
      _state = State::Disable; // always permitted
      _alive.enable(false);
      return true;

    case State::Reset:
      stracef("%s", "RESET!");
      _state = State::Reset; // always permitted
      _alive.enable(false);
      if (_client != nullptr) {
        if (_client->isConnected()) {
          stracef("%s", "force disconnect!");
          _client->disconnect();
        } else {
          stracef("%s", "cleanup client");
          delete _client;
        }
      }
      return true;

    case State::ScanStart:
      _state = State::ScanStart;
      getScan()->start(0);
      return true;

    case State::ConnectStart:
      _state = State::ConnectStart;
      // getScan()->stop();
      return true;

    case State::RegisterStart:
      _state = State::RegisterStart;
      return true;

    case State::PairedAndReady:
      _state = State::PairedAndReady;
      _alive.enable(true);
      return true;

    default:
      _alive.enable(false);
      return true;
    }
  }

  void update() {
    // stracef("update (%d)", _state);
    switch (_state) {
    case State::Disable:
      break;

    case State::Reset:
      // Wait until resources cleaned up before rescanning.
      // Resources are cleaned up in onDisconnect if we were connected.
      // Otherwise, they are cleaned up in setState for State::Reset.
      if (_client == nullptr) {
        setState(State::ScanStart);
      }
      break;

    case State::ScanStart:
      break;

    case State::ConnectStart:
      if (!connect()) {
        setState(State::Reset);
      }
      break;

    case State::RegisterStart:
      if (!registerCallbacks()) {
        setState(State::Reset);
      }
      break;

    case State::PairedAndReady: {
      packet::Data<uint32_t> PulsesPkt;
      if (_chan.Pulses.pop(PulsesPkt)) {
        _alive.update(PulsesPkt.value());
      }
    } break;

    default:
      setState(State::Reset);
      break;
    }
  }

  void onResult(BLEAdvertisedDevice dev) {
    stracef("[%s] %s: %s", DeviceName, "onResult", dev.getName().c_str());
    if (dev.getName() == DeviceName) {
      if (dev.haveServiceUUID() &&
          dev.isAdvertisingService(BLEUUID(UUID_SENSORS_SERVICE))) {
        stracef("%s", "create server");
        getScan()->stop();
        _server = new BLEAdvertisedDevice(dev);
        stracef("%s", "connect to device");
        setState(State::ConnectStart);
      }
    }
  }

  void onConnect(BLEClient *client) {
    stracef("%s", "<onConnect>");
    setState(State::RegisterStart);
  }

  void onDisconnect(BLEClient *client) {
    delete _client;
    delete _server;
  }

  bool connect() {
    stracef("%s", "createClient");
    _client = createClient();
    stracef("%s", "setClientCallbacks");
    _client->setClientCallbacks(this);
    stracef("%s", "connect");
    return _client->connect(_server);
  }

  bool registerCallbacks() {
    stracef("%s", "setMTU");
    if (!_client->setMTU(517)) {
      return false;
    }

    stracef("%s", "getService");
    BLERemoteService *svc;
    if ((svc = _client->getService(BLEUUID(UUID_SENSORS_SERVICE))) ==
        nullptr) {
      return false;
    }

    stracef("%s", "uuidCharBleWdt");
    if ((_charPulses = svc->getCharacteristic(
             BLEUUID(UUID_SENSORS_PULSES_CHAR))) == nullptr) {
      return false;
    }
    if (_charPulses->canNotify()) {
      _charPulses->registerForNotify(
          [&](BLERemoteCharacteristic *, uint8_t *data, size_t len, bool) {
            _chan.Pulses.push(data, len);
          });
    }

    stracef("%s", "uuidCharAccXYZ");
    if ((_charAccXYZ = svc->getCharacteristic(
             BLEUUID(UUID_SENSORS_ACCXYZ_CHAR))) == nullptr) {
      return false;
    }
    if (_charAccXYZ->canNotify()) {
      _charAccXYZ->registerForNotify(
          [&](BLERemoteCharacteristic *, uint8_t *data, size_t len, bool) {
            _chan.AccXyz.push(data, len);
          });
    }

    stracef("%s", "uuidCharWeight");
    if ((_charWeight = svc->getCharacteristic(
             BLEUUID(UUID_SENSORS_WEIGHT_CHAR))) == nullptr) {
      return false;
    }
    if (_charWeight->canNotify()) {
      _charWeight->registerForNotify(
          [&](BLERemoteCharacteristic *, uint8_t *data, size_t len, bool) {
            _chan.Weight.push(data, len);
          });
    }

    // stracef("%s", "uuidCharBarPsi");
    // if ((_charBarPsi =
    // svc->getCharacteristic(BLEUUID(UUID_SENSORS_BARPSI_CHAR))) ==
    // nullptr) {
    //   return false;
    // }
    // if (_charBarPsi->canNotify()) {
    //   _charBarPsi->registerForNotify(
    //     [&](BLERemoteCharacteristic *, uint8_t *data, size_t len, bool) {
    //       _chan.BarPsi.push(data, len);
    //     });
    // }

    // stracef("%s", "uuidCharPrecip");
    // if ((_charPrecip =
    // svc->getCharacteristic(BLEUUID(UUID_SENSORS_PRECIP_CHAR))) ==
    // nullptr) {
    //   return false;
    // }
    // if (_charPrecip->canNotify()) {
    //   _charPrecip->registerForNotify(
    //     [&](BLERemoteCharacteristic *, uint8_t *data, size_t len, bool) {
    //       _chan.Precip.push(data, len);
    //     });
    // }

    // stracef("%s", "uuidCharProxim");
    // if ((_charProxim =
    // svc->getCharacteristic(BLEUUID(UUID_SENSORS_PROXIM_CHAR))) ==
    // nullptr) {
    //   return false;
    // }
    // if (_charProxim->canNotify()) {
    //   _charProxim->registerForNotify(
    //     [&](BLERemoteCharacteristic *, uint8_t *data, size_t len, bool) {
    //       _chan.Proxim.push(data, len);
    //     });
    // }

    // stracef("%s", "uuidCharAirTmp");
    // if ((_charAirTmp =
    // svc->getCharacteristic(BLEUUID(UUID_SENSORS_AIRTMP_CHAR))) ==
    // nullptr) {
    //   return false;
    // }
    // if (_charAirTmp->canNotify()) {

    //   _charAirTmp->registerForNotify(
    //     [&](BLERemoteCharacteristic *, uint8_t *data, size_t len, bool) {
    //       _chan.AirTmp.push(data, len);
    //     });
    // }

    // stracef("%s", "uuidCharH2OTmp");
    // if ((_charH2OTmp =
    // svc->getCharacteristic(BLEUUID(UUID_SENSORS_H2OTMP_CHAR))) ==
    // nullptr) {
    //   return false;
    // }
    // if (_charH2OTmp->canNotify()) {
    //   _charH2OTmp->registerForNotify(   _chan.H2OTmp.onNotify());
    // }

    stracef("%s", "ok!");
    setState(State::PairedAndReady);
    return true;
  }
};

#endif // radio_hpp
