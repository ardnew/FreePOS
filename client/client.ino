#include <ESP323248S035.h>

#include "radio.hpp"
#include "view.hpp"

static MainView root;

// Task encapsulates a thread of execution on a single SMP core.
//
// Each Task has a single divergent TaskFunction_t member _func that defines the
// execution logic of the core.
// Tasks should never terminate. In Arduino-land, this is typified by the
// structure of the main() function (or similar) in most Arduino cores, e.g.,
// the following infinite loop implemented in the ESP32 Arduino core (main.cpp):
//
//    40╶┤║ void loopTask(void *pvParameters)
//    41╶┤║ {
//    42╶┤║     setup();
//    43╶┤║     for(;;) {
//    44╶┤║ #if CONFIG_FREERTOS_UNICORE
//    45╶┤║         yieldIfNecessary();
//    46╶┤║ #endif
//    47╶┤║         if(loopTaskWDTEnabled){
//    48╶┤║             esp_task_wdt_reset();
//    49╶┤║         }
//    50╶┤║         loop();
//    51╶┤║         if (serialEventRun) serialEventRun();
//    52╶┤║     }
//    53╶┤║ }
//
// Thus, we assign exactly 2 tasks to the dual-core ESP32; i.e., one task per
// core, executed in parallel. These tasks and their respective root functions
// are defined below in class Target.
template <typename T>
struct Task {
public:
  Task() = delete;
  Task(const Task &) = delete;
  Task(const std::string id, const BaseType_t core,
    const TaskFunction_t func, const T *args):
      _hand(nullptr), _name(id), _func(func), _args(args), _core(core) {}
  bool init() {
    _hand = xTaskCreateStaticPinnedToCore(_func, _name.c_str(), _size,
      (void *)_args, _prio, _buff, &_ctrl, _core);
    return nullptr != _hand;
  }
private:
  static constexpr size_t      _size = 8192U; // stack size (bytes)
  static constexpr UBaseType_t _prio = 7U;    // task priority

  TaskHandle_t _hand;
  StaticTask_t _ctrl;
  StackType_t  _buff[_size];

  const std::string    _name;
  const TaskFunction_t _func;
  const T             *_args;
  const BaseType_t     _core;
};

class Target {
  using Device = ESP323248S035C<decltype(root)>;

public:
  Target():
    _dev(root), // Both cores receive a pointer to the exact same member _dev.
    _tsk{
      // HMI driver (BSP) runs on the Arduino sketch core.
      Task<Device>("HMI", ARDUINO_RUNNING_CORE,
        [](void *pArg) {
          auto dev = static_cast<Device *>(pArg);
          if (!dev->init())
            { spanicf("failed to initialize device(=<0x%p>)", dev); }
          static constexpr uint32_t period = Device::refresh().count();
          govern<period>( [&](){
            dev->update();
          } );
        },
        &_dev),
      // BLE handlers run on the opposite core.
      Task<Device>("BLE", 1-ARDUINO_RUNNING_CORE,
        [](void *pArg) {
          auto dev = static_cast<Device *>(pArg);
          auto rad = Radio(*dev, root);
          static constexpr uint32_t period = Radio::refresh().count();
          govern<period>( [&](){ rad.update(); } );
        },
        &_dev),
    } {}

  bool init() {
    bool res = true;
    for (Task<Device> &t : _tsk) {
      res = res && t.init();
    }
    return res;
  }

private:
  Device                      _dev;
  std::array<Task<Device>, 2> _tsk;
};

Target target;

void setup() {
  // NOOP unless DEBUG is defined non-zero (see debug.hpp)
  Debug::init();

  target.init();

  // Suspend ourselves so that only the tasks created above are allocated time.
  vTaskSuspend(nullptr);
}

void loop() { yield(); }
