#ifndef radio_hpp
#define radio_hpp

#include <BLEDevice.h>
#include <ESP323248S035.h>

#include "spec.hpp"

template <typename T>
class Radio:
  public BLEDevice,
  public BLEAdvertisedDeviceCallbacks,
  public BLEClientCallbacks {
  using BLEDevice::BLEDevice;

private:
  const T &_hmi;
  BLEAdvertisedDevice *_server;
  BLEClient *_client;
  BLERemoteCharacteristic *_charAccelX;
  BLERemoteCharacteristic *_charAccelY;
  BLERemoteCharacteristic *_charAccelZ;
  BLERemoteCharacteristic *_charBarPsi;
  BLERemoteCharacteristic *_charPrecip;
  BLERemoteCharacteristic *_charProxim;
  BLERemoteCharacteristic *_charAirTmp;
  BLERemoteCharacteristic *_charH20Tmp;
  BLERemoteCharacteristic *_charWeight;
  bool _doScan;
  bool _doConn;
  bool _isConn;

protected:
  static constexpr msec_t _refresh = msec_t{100};
public:
  static inline constexpr msec_t refresh() { return _refresh; }

  class State {
  public:
    enum Enum: uint8_t {
      Disable = 0,
      Reset,
      ScanStart,
      ConnectStart,
      RegisterStart,
      PairedWithAdvertisedDevice,
    };
    State() = delete;
    constexpr State(const Enum &e): _enum(e) {}
    constexpr operator Enum() const { return _enum; }

  private:
    Enum _enum;
  };

protected:
  State _state;

public:
  Radio(const T &hmi):
    _hmi(hmi), _server(nullptr), _doScan(true), _doConn(false), _isConn(false),
      _state(State::Reset) {
    BLEDevice::init(BLE_DEVICE_NAME);
    BLEScan *s = getScan();
    s->setAdvertisedDeviceCallbacks(this);
    s->setInterval(1349);
    s->setWindow(449);
    s->setActiveScan(true);
    setState(State::ScanStart);
  }

  bool setState(State const state) {
    stracef("setState: %d", state);
    // Don't process unless state is changing
    if (_state == state) { return true; }

    switch (state) {
    case State::Disable:
      stracef("%s", "DISABLE!");
      _state = State::Disable; // always permitted
      return true;

    case State::Reset:
      stracef("%s", "RESET!");
      _state = State::Reset; // always permitted
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

    case State::PairedWithAdvertisedDevice:
      _state = State::PairedWithAdvertisedDevice;
      return true;
    }
  }

  void update() {
    stracef("update (%d)", _state);
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

    case State::PairedWithAdvertisedDevice:
      break;

    default:
      break;
    }
  }

  void onResult(BLEAdvertisedDevice dev) {
    stracef("%s", "onResult");
    if (dev.getName() == BLE_DEVICE_NAME) {
      if (dev.haveServiceUUID() && dev.isAdvertisingService(uuidService)) {
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
    if ((svc = _client->getService(uuidService)) == nullptr) {
      return false;
    }

    stracef("%s", "uuidCharAccelX");
    if ((_charAccelX = svc->getCharacteristic(uuidCharAccelX)) == nullptr) {
      return false;
    }
    if (_charAccelX->canNotify()) {
      _charAccelX->registerForNotify(
        [&](BLERemoteCharacteristic* pBLERemoteCharacteristic,
          uint8_t* pData, size_t length, bool isNotify) {

        });
    }

    stracef("%s", "uuidCharAccelY");
    if ((_charAccelY = svc->getCharacteristic(uuidCharAccelY)) == nullptr) {
      return false;
    }
    if (_charAccelY->canNotify()) {
      _charAccelY->registerForNotify(
        [&](BLERemoteCharacteristic* pBLERemoteCharacteristic,
          uint8_t* pData, size_t length, bool isNotify) {

        });
    }

    stracef("%s", "uuidCharAccelZ");
    if ((_charAccelZ = svc->getCharacteristic(uuidCharAccelZ)) == nullptr) {
      return false;
    }
    if (_charAccelZ->canNotify()) {
      _charAccelZ->registerForNotify(
        [&](BLERemoteCharacteristic* pBLERemoteCharacteristic,
          uint8_t* pData, size_t length, bool isNotify) {

        });
    }

    stracef("%s", "uuidCharBarPsi");
    if ((_charBarPsi = svc->getCharacteristic(uuidCharBarPsi)) == nullptr) {
      return false;
    }
    if (_charBarPsi->canNotify()) {
      _charBarPsi->registerForNotify(
        [&](BLERemoteCharacteristic* pBLERemoteCharacteristic,
          uint8_t* pData, size_t length, bool isNotify) {

        });
    }

    stracef("%s", "uuidCharPrecip");
    if ((_charPrecip = svc->getCharacteristic(uuidCharPrecip)) == nullptr) {
      return false;
    }
    if (_charPrecip->canNotify()) {
      _charPrecip->registerForNotify(
        [&](BLERemoteCharacteristic* pBLERemoteCharacteristic,
          uint8_t* pData, size_t length, bool isNotify) {

        });
    }

    stracef("%s", "uuidCharProxim");
    if ((_charProxim = svc->getCharacteristic(uuidCharProxim)) == nullptr) {
      return false;
    }
    if (_charProxim->canNotify()) {
      _charProxim->registerForNotify(
        [&](BLERemoteCharacteristic* pBLERemoteCharacteristic,
          uint8_t* pData, size_t length, bool isNotify) {

        });
    }

    stracef("%s", "uuidCharAirTmp");
    if ((_charAirTmp = svc->getCharacteristic(uuidCharAirTmp)) == nullptr) {
      return false;
    }
    if (_charAirTmp->canNotify()) {
      _charAirTmp->registerForNotify(
        [&](BLERemoteCharacteristic* pBLERemoteCharacteristic,
          uint8_t* pData, size_t length, bool isNotify) {

        });
    }

    stracef("%s", "uuidCharH20Tmp");
    if ((_charH20Tmp = svc->getCharacteristic(uuidCharH20Tmp)) == nullptr) {
      return false;
    }
    if (_charH20Tmp->canNotify()) {
      _charH20Tmp->registerForNotify(
        [&](BLERemoteCharacteristic* pBLERemoteCharacteristic,
          uint8_t* pData, size_t length, bool isNotify) {

        });
    }

    stracef("%s", "uuidCharWeight");
    if ((_charWeight = svc->getCharacteristic(uuidCharWeight)) == nullptr) {
      return false;
    }
    if (_charWeight->canNotify()) {
      _charWeight->registerForNotify(
        [&](BLERemoteCharacteristic* pBLERemoteCharacteristic,
          uint8_t* pData, size_t length, bool isNotify) {

        });
    }

    stracef("%s", "ok!");
    setState(State::PairedWithAdvertisedDevice);
    return true;
  }
};

#endif // radio_hpp
