#ifndef keepalive_hpp
#define keepalive_hpp

#include "spec.hpp"

template <typename T = uint32_t>
class KeepAlive {
  using pulse_type = T;

public:
  KeepAlive() = delete;
  KeepAlive(const std::chrono::seconds timeout): _pulse{} {
    // Initialize the TWDT with a timeout of 5 seconds; else panic (noreturn).
    // This only needs to be initialized once, unless you wish to change the
    // timeout or panic behavior.
    // The TWDT can then be enabled/disabled using
    esp_err_t err = esp_task_wdt_init(timeout.count(), true);
    if (ESP_OK != err) {
      _esp_error_check_failed(err, __FILE__, __LINE__, __PRETTY_FUNCTION__,
        "ESP_OK != esp_task_wdt_init");
    }
  }

  virtual ~KeepAlive() {
    esp_err_t err;
  DEINIT:
    err = esp_task_wdt_deinit();
    if (ESP_ERR_INVALID_STATE == err && ESP_OK == esp_task_wdt_status(nullptr)) {
      err = esp_task_wdt_delete(nullptr);
      if (ESP_OK == err) {
        goto DEINIT;
      }
    }
    if (ESP_OK != err) {
      _esp_error_check_failed(err, __FILE__, __LINE__, __PRETTY_FUNCTION__,
        "ESP_OK != esp_task_wdt_(deinit|delete)");
    }
  }

  void enable(const bool &enable) {
    esp_err_t err = ESP_OK;
    if (enable) {
      err = esp_task_wdt_add(nullptr);
    } else {
      err = esp_task_wdt_delete(nullptr);
      // Ignore the error returned when the task is already deleted.
      if (err == ESP_ERR_INVALID_ARG) { err = ESP_OK; }
    }
    if (ESP_OK != err) {
      _esp_error_check_failed(err, __FILE__, __LINE__, __PRETTY_FUNCTION__,
        "ESP_OK != esp_task_wdt_add");
    }
  }

  bool update(const pulse_type pulse) {
    if (pulse != _pulse) {
      _pulse = pulse;
      esp_err_t err = esp_task_wdt_reset();
      if (ESP_OK != err) {
        _esp_error_check_failed(err, __FILE__, __LINE__, __PRETTY_FUNCTION__,
          "ESP_OK != esp_task_wdt_reset");
      }
      return true;
    } else {
      return false;
    }
  }

  bool update(const uint8_t *ptr, size_t len) {
    if (len <= 0) { return false; }
    if (len > sizeof(pulse_type)) { len = sizeof(pulse_type); }
    pulse_type pulse = 0;
    for (size_t i = 0; i < len; ++i) {
      pulse = (pulse << 8) | ptr[i];
    }
    return update(pulse);
  }

  // Force the watchdog timer to reset by calling update without an argument.
  // This simulates a change in _pulse by inverting the current bits of _pulse.
  bool update() { return update(~_pulse); }

private:
  pulse_type _pulse;
};

#endif // keepalive_hpp
