#ifndef radio_hpp
#define radio_hpp

#include <tuple>

#include <BLEDevice.h>
#include <ESP323248S035.h>
#include <StatusLED.h>

#include "spec.hpp"
#include "state.hpp"
#include "view.hpp"

static BLEUUID    uuidService(BLE_SENSORS_SERVICE_UUID);
static BLEUUID uuidCharAccelX();
static BLEUUID uuidCharAccelY();
static BLEUUID uuidCharAccelZ();
static BLEUUID uuidCharBarPsi();
static BLEUUID uuidCharPrecip();
static BLEUUID uuidCharProxim();
static BLEUUID uuidCharAirTmp();
static BLEUUID uuidCharH2OTmp();
static BLEUUID uuidCharWeight();

class Radio:
  public BLEDevice,
  public BLEAdvertisedDeviceCallbacks,
  public BLEClientCallbacks {
  using BLEDevice::BLEDevice;
  using Device = ESP323248S035C<MainView>;

private:
  Device &_dev;
  MainView &_view;

  bool _doScan;
  bool _doConn;
  bool _isConn;

protected:
  BLEAdvertisedDevice *_server;
  BLEClient *_client;
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
  static constexpr msec_t _refresh = msec_t{20};
public:
  static inline constexpr msec_t refresh() { return _refresh; }

protected:
  State _state;

  template <typename T>
  struct _BLEChar {
    BLERemoteCharacteristic *rc;
    const UUID &id;
    operator BLEUUID&()
      { return std::forward<BLEUUID&>(BLEUUID::fromString(id.c_str())); }
  };

public:
  Radio(Device &dev, MainView &root):
    _dev(dev), _view(root), _server(nullptr),
      _doScan(true), _doConn(false), _isConn(false),
      _state(dev.hw<RGB_PWM>()) {
    BLEDevice::init(BLE_DEVICE_NAME);
    BLEScan *s = getScan();
    s->setAdvertisedDeviceCallbacks(this);
    s->setInterval(1349);
    s->setWindow(449);
    s->setActiveScan(true);
    setState(State::Reset);
  }

  bool setState(State::Enum const state) {
    // Don't process unless state is changing
    if (_state == state) { return true; }

    switch (state) {
    case State::Disable:
      _state = State::Disable; // always permitted
      return true;

    case State::Disconnected:
      _state = State::Disconnected;
      return true;

    case State::Reset:
      _state = State::Reset; // always permitted
      return true;

    case State::ScanStart:
      _state = State::ScanStart;
      getScan()->start(0);
      return true;

    case State::ScanInProgress:
      _state = State::ScanInProgress;
      return true;

    case State::ScanSuccess:
      _state = State::ScanSuccess;
      getScan()->stop();
      return true;

    case State::ConnectStart:
      _state = State::ConnectStart;
      return true;

    case State::ConnectSuccess:
      _state = State::ConnectSuccess;
      return true;

    case State::ConnectFailure:
      _state = State::ConnectFailure;
      if (_client != nullptr) {
        if (_client->isConnected()) {
          _client->disconnect();
        } else {
          delete _client;
        }
      }
      return true;

    case State::RegisterStart:
      _state = State::RegisterStart;
      return true;

    case State::RegisterCallbacks:
      _state = State::RegisterCallbacks;
      return true;

    case State::RegisterSuccess:
      _state = State::RegisterSuccess;
      return true;

    case State::RegisterFailure:
      _state = State::RegisterFailure;
      if (_client != nullptr) {
        if (_client->isConnected()) {
          _client->disconnect();
        } else {
          delete _client;
        }
      }
      return true;

    case State::PairedAndReady:
      _state = State::PairedAndReady;
      return true;
    }
  }

  void update() {
    switch (_state) {
    case State::Disable:
      break;

    case State::Disconnected:
      delete _client;
      delete _server;
      setState(State::Reset);
      break;

    case State::Reset:
      setState(State::ScanStart);
      break;

    case State::ScanStart:
      setState(State::ScanInProgress);
      break;

    case State::ScanInProgress:
      break;

    case State::ScanSuccess:
      setState(State::ConnectStart);
      break;

    case State::ConnectStart:
      if (!connect()) {
        setState(State::ConnectFailure);
      }
      break;

    case State::ConnectSuccess:
      setState(State::RegisterStart);
      break;

    case State::ConnectFailure:
      if (_client == nullptr) {
        setState(State::Reset);
      }
      break;

    case State::RegisterStart:
      setState(State::RegisterCallbacks);
      break;

    case State::RegisterCallbacks:
      if (registerCallbacks()) {
        setState(State::RegisterSuccess);
      } else {
        setState(State::RegisterFailure);
      }
      break;

    case State::RegisterSuccess:
      setState(State::PairedAndReady);
      break;

    case State::RegisterFailure:
      if (_client == nullptr) {
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
    swritef("Found: %s\n", dev.toString().c_str());
    if (dev.getName() == BLE_DEVICE_NAME) {
      if (dev.haveServiceUUID() && dev.isAdvertisingService(uuidService)) {
        _server = new BLEAdvertisedDevice(dev);
        setState(State::ScanSuccess);
      }
    }
  }

  void onConnect(BLEClient *client) {
    setState(State::ConnectSuccess);
  }

  void onDisconnect(BLEClient *client) {
    setState(State::Disconnected);
  }

  bool connect() {
    _client = createClient();
    _client->setClientCallbacks(this);
    return _client->connect(_server);
  }

  template <typename T>
  bool install(BLERemoteService *rs, _BLEChar<T> *ch) {
    if ((ch->rc = rs->getCharacteristic(ch->id.c_str())) == nullptr ||
        !ch->rc->canNotify()) {
      return false;
    }
    ch->rc->registerForNotify(
      [&](BLERemoteCharacteristic *pChas,
          uint8_t *pData, size_t length, bool isNotify) {
        if (length >= 8) {
          TaggedData<T> td(pData, length);
        }
      }
    );
    return true;
  }

  bool registerCallbacks() {
    if (!_client->setMTU(517)) {
      return false;
    }
    BLERemoteService *svc;
    if ((svc = _client->getService(uuidService.toString().c_str())) == nullptr) {
      return false;
    }

    bool result = true;

#define _install(res, T, ch, uuid) \
    res && install<T>(svc, new _BLEChar<T>{ ch, uuid })

    result = _install(result, float,   _charAccelX, BLE_SENSORS_ACCELX_CHAR_UUID);
    result = _install(result, float,   _charAccelY, BLE_SENSORS_ACCELY_CHAR_UUID);
    result = _install(result, float,   _charAccelZ, BLE_SENSORS_ACCELZ_CHAR_UUID);
    result = _install(result, float,   _charBarPsi, BLE_SENSORS_BARPSI_CHAR_UUID);
    result = _install(result, float,   _charPrecip, BLE_SENSORS_PRECIP_CHAR_UUID);
    result = _install(result, int32_t, _charProxim, BLE_SENSORS_PROXIM_CHAR_UUID);
    result = _install(result, float,   _charAirTmp, BLE_SENSORS_AIRTMP_CHAR_UUID);
    result = _install(result, float,   _charH2OTmp, BLE_SENSORS_H2OTMP_CHAR_UUID);
    result = _install(result, float,   _charWeight, BLE_SENSORS_WEIGHT_CHAR_UUID);

    return result;
  }
};

#endif // radio_hpp
