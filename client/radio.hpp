#ifndef radio_hpp
#define radio_hpp

#include "spec.hpp"
#include "state.hpp"
#include "view.hpp"
#include "keepalive.hpp"
#include "ipc.hpp"

class Radio:
  public BLEDevice,
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
  BLERemoteCharacteristic *_charAccelX;
  BLERemoteCharacteristic *_charAccelY;
  BLERemoteCharacteristic *_charAccelZ;
  BLERemoteCharacteristic *_charBarPsi;
  BLERemoteCharacteristic *_charPrecip;
  BLERemoteCharacteristic *_charProxim;
  BLERemoteCharacteristic *_charAirTmp;
  BLERemoteCharacteristic *_charH2OTmp;
  BLERemoteCharacteristic *_charWeight;

protected:
  static constexpr msecu32_t _refresh = msecu32_t{100};
public:
  static inline constexpr msecu32_t refresh() { return _refresh; }

protected:
  KeepAlive<uint32_t> _alive;
  State _state;

public:
  Radio(Device &dev, IPC &chan, MainView &root):
    _dev(dev), _view(root), _chan(chan), _server(nullptr),
      _doScan(true), _doConn(false), _isConn(false),
      _alive(std::chrono::seconds{5}), _state(dev.hw<RGB_PWM>()) {
    BLEDevice::init(BLEDeviceName);
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
    if (_state == state) { return true; }

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
      getScan()->stop();
      return true;

    case State::RegisterStart:
      _state = State::RegisterStart;
      return true;

    case State::PairedAndReady:
      _state = State::PairedAndReady;
      _alive.enable(true);
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

    case State::PairedAndReady:
      break;

    default:
      break;
    }
  }

  void onResult(BLEAdvertisedDevice dev) {
    stracef("[%s] %s: %s", BLEDeviceName, "onResult", dev.getName().c_str());
    if (dev.getName() == BLEDeviceName) {
      if (dev.haveServiceUUID() &&
          dev.isAdvertisingService(BLEUUID(UUID_BLE_SENSORS_SERVICE))) {
        stracef("%s", "create server");
        // getScan()->stop();
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
    if ((svc = _client->getService(BLEUUID(UUID_BLE_SENSORS_SERVICE))) == nullptr) {
      return false;
    }

    stracef("%s", "uuidCharBleWdt");
    if ((_charPulses = svc->getCharacteristic(BLEUUID(UUID_BLE_SENSORS_PULSES_CHAR))) == nullptr) {
      return false;
    }
    if (_charPulses->canNotify()) {
      _charPulses->registerForNotify(
        [&](BLERemoteCharacteristic* pBLERemoteCharacteristic,
          uint8_t* pData, size_t length, bool isNotify)
            { _alive.update(pData, length); });
    }

    stracef("%s", "uuidCharAccelX");
    if ((_charAccelX = svc->getCharacteristic(BLEUUID(UUID_BLE_SENSORS_ACCELX_CHAR))) == nullptr) {
      return false;
    }
    if (_charAccelX->canNotify()) {
      _charAccelX->registerForNotify(
        [&](BLERemoteCharacteristic* pBLERemoteCharacteristic,
          uint8_t* pData, size_t length, bool isNotify) {
          _chan.AccelX.push(Packet<float>(pData, length));
        });
    }

    stracef("%s", "uuidCharAccelY");
    if ((_charAccelY = svc->getCharacteristic(BLEUUID(UUID_BLE_SENSORS_ACCELY_CHAR))) == nullptr) {
      return false;
    }
    if (_charAccelY->canNotify()) {
      _charAccelY->registerForNotify(
        [&](BLERemoteCharacteristic* pBLERemoteCharacteristic,
          uint8_t* pData, size_t length, bool isNotify) {
          _chan.AccelY.push(Packet<float>(pData, length));
        });
    }

    stracef("%s", "uuidCharAccelZ");
    if ((_charAccelZ = svc->getCharacteristic(BLEUUID(UUID_BLE_SENSORS_ACCELZ_CHAR))) == nullptr) {
      return false;
    }
    if (_charAccelZ->canNotify()) {
      _charAccelZ->registerForNotify(
        [&](BLERemoteCharacteristic* pBLERemoteCharacteristic,
          uint8_t* pData, size_t length, bool isNotify) {
          _chan.AccelZ.push(Packet<float>(pData, length));
        });
    }

    stracef("%s", "uuidCharBarPsi");
    if ((_charBarPsi = svc->getCharacteristic(BLEUUID(UUID_BLE_SENSORS_BARPSI_CHAR))) == nullptr) {
      return false;
    }
    if (_charBarPsi->canNotify()) {
      _charBarPsi->registerForNotify(
        [&](BLERemoteCharacteristic* pBLERemoteCharacteristic,
          uint8_t* pData, size_t length, bool isNotify) {
          _chan.BarPsi.push(Packet<float>(pData, length));
        });
    }

    stracef("%s", "uuidCharPrecip");
    if ((_charPrecip = svc->getCharacteristic(BLEUUID(UUID_BLE_SENSORS_PRECIP_CHAR))) == nullptr) {
      return false;
    }
    if (_charPrecip->canNotify()) {
      _charPrecip->registerForNotify(
        [&](BLERemoteCharacteristic* pBLERemoteCharacteristic,
          uint8_t* pData, size_t length, bool isNotify) {
          _chan.Precip.push(Packet<float>(pData, length));
        });
    }

    stracef("%s", "uuidCharProxim");
    if ((_charProxim = svc->getCharacteristic(BLEUUID(UUID_BLE_SENSORS_PROXIM_CHAR))) == nullptr) {
      return false;
    }
    if (_charProxim->canNotify()) {
      _charProxim->registerForNotify(
        [&](BLERemoteCharacteristic* pBLERemoteCharacteristic,
          uint8_t* pData, size_t length, bool isNotify) {
          _chan.Proxim.push(Packet<int32_t>(pData, length));
        });
    }

    stracef("%s", "uuidCharAirTmp");
    if ((_charAirTmp = svc->getCharacteristic(BLEUUID(UUID_BLE_SENSORS_AIRTMP_CHAR))) == nullptr) {
      return false;
    }
    if (_charAirTmp->canNotify()) {
      _charAirTmp->registerForNotify(
        [&](BLERemoteCharacteristic* pBLERemoteCharacteristic,
          uint8_t* pData, size_t length, bool isNotify) {
          _chan.AirTmp.push(Packet<float>(pData, length));
        });
    }

    // stracef("%s", "uuidCharH2OTmp");
    // if ((_charH2OTmp = svc->getCharacteristic(BLEUUID(UUID_BLE_SENSORS_H2OTMP_CHAR))) == nullptr) {
    //   return false;
    // }
    // if (_charH2OTmp->canNotify()) {
    //   _charH2OTmp->registerForNotify(
    //     [&](BLERemoteCharacteristic* pBLERemoteCharacteristic,
    //       uint8_t* pData, size_t length, bool isNotify) {
    //       _chan.H2OTmp.push(Packet<float>(pData, length));
    //     });
    // }

    stracef("%s", "uuidCharWeight");
    if ((_charWeight = svc->getCharacteristic(BLEUUID(UUID_BLE_SENSORS_WEIGHT_CHAR))) == nullptr) {
      return false;
    }
    if (_charWeight->canNotify()) {
      _charWeight->registerForNotify(
        [&](BLERemoteCharacteristic* pBLERemoteCharacteristic,
          uint8_t* pData, size_t length, bool isNotify) {
          _chan.Weight.push(Packet<float>(pData, length));
        });
    }

    stracef("%s", "ok!");
    setState(State::PairedAndReady);
    return true;
  }
};

#endif // radio_hpp
