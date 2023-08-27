#ifndef view_hpp
#define view_hpp

#include "spec.hpp"
#include "ipc.hpp"
#include "ui/tabview.hpp"

#if (LV_USE_LOG)
const TPC_LCD::Log TPC_LCD::log = [](const char *message) {
  swritef("%s", message);
};
#endif

class MainView: public View {
protected:
  TabView _main;
  IPC *_chan;

public:
  MainView() = delete;
  MainView(IPC *chan): _chan(chan) {}
  bool init(lv_obj_t *root) override {
    _main.connect(_chan);
    _main.init(root);
    return true;
  }
  void update(msecu32_t const now) override {
    _main.update(now);
  }
  inline std::string title() override { return "<root>"; }
};

#endif // view_hpp
