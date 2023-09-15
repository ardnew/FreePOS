#ifndef ui_tabview_hpp
#define ui_tabview_hpp

#include "../spec.hpp"
#include "../ipc.hpp"
#include "../icons.h"

#include "chart.hpp"
#include "weight.hpp"

class TabView: public View {
private:
  typedef void (*scroll_begin_t)(lv_event_t *);
  static inline void scroll_begin(lv_event_t *e) {
    // Disable the scroll animations. Triggered when a tab button is clicked
    if (lv_event_get_code(e) == LV_EVENT_SCROLL_BEGIN) {
      lv_anim_t *a = (lv_anim_t *)lv_event_get_param(e);
      if (a) { a->time = 0; }
    }
  }

protected:
  IPC *_chan;
  lv_obj_t *_base;
  Chart _chart;
  Weight _weight;

public:

  bool init(lv_obj_t *root) override {
    _base = lv_tabview_create(root, LV_DIR_LEFT, 80);

    lv_obj_add_event_cb(
      lv_tabview_get_content(_base),
      static_cast<scroll_begin_t>(&TabView::scroll_begin),
      LV_EVENT_SCROLL_BEGIN, NULL);

    lv_obj_set_style_bg_color(_base, lv_palette_lighten(LV_PALETTE_RED, 2), 0);

    lv_obj_t *tab_btns = lv_tabview_get_tab_btns(_base);
    lv_obj_set_style_bg_color(tab_btns, lv_palette_darken(LV_PALETTE_GREY, 3), 0);
    lv_obj_set_style_text_color(tab_btns, lv_palette_lighten(LV_PALETTE_GREY, 5), 0);
    lv_obj_set_style_border_side(tab_btns, LV_BORDER_SIDE_RIGHT, LV_PART_ITEMS | LV_STATE_CHECKED);

    // lv_obj_set_style_text_font(_base, &mug_24, 0);

    _chart.init(lv_tabview_add_tab(_base, "Chart"));
    _weight.init(lv_tabview_add_tab(_base, "Weight"));

    lv_obj_t *tab2 = lv_tabview_add_tab(_base, "Tab 2");
    lv_obj_t *tab3 = lv_tabview_add_tab(_base, "Tab 3");
    lv_obj_t *tab4 = lv_tabview_add_tab(_base, "Tab 4");
    lv_obj_t *tab5 = lv_tabview_add_tab(_base, "Tab 5");

    lv_obj_t *label = lv_label_create(tab2);
    // lv_obj_set_style_text_font(label, &beans_16, 0);
    lv_label_set_text(label, "Second tab");

    label = lv_label_create(tab3);
    lv_label_set_text(label, "Third tab");

    label = lv_label_create(tab4);
    lv_label_set_text(label, "Forth tab");

    label = lv_label_create(tab5);
    lv_label_set_text(label, "Fifth tab");

    lv_obj_clear_flag(lv_tabview_get_content(_base), LV_OBJ_FLAG_SCROLLABLE);

    return true;
  }
  void connect(IPC *chan) {
    _chan = chan;
    _chart.connect(chan);
    _weight.connect(chan);
  }
  void update(msecu32_t const now) override {
    _chart.update(now);
    _weight.update(now);
  }
  inline std::string title() override { return "TabView"; }
  // operator lv_obj_t*() { return _base; }
};

#endif // ui_tabview_hpp
