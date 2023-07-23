#ifndef view_hpp
#define view_hpp

#include <ESP323248S035.h>
#include <lvgl.h>

#include "spec.hpp"
#include "ui/tabview.hpp"

class MainView: public View {
protected:
  TabView _main;

public:
  bool init(lv_obj_t *root) override {
    _main.init(root);
    return true;
  }
  void update(msec_t const) override {}
  inline std::string title() override { return "<root>"; }
};

#endif // view_hpp
