#ifndef ui_weight_hpp
#define ui_weight_hpp

#include "../spec.hpp"
#include "../ipc.hpp"

static lv_obj_t *_meter;
static lv_meter_scale_t *_scale;
static lv_meter_indicator_t *_indic;

static void adjustWeight(void *var, int32_t value) {
  lv_meter_set_indicator_value(_meter, _indic, value);
}

class Weight: public View {
protected:
  IPC *_chan;
  lv_anim_t _needleMotion;

public:
  void connect(IPC *chan) {
    _chan = chan;
  }

  bool init(lv_obj_t *root) override {

    _meter = lv_meter_create(root);
    lv_obj_center(_meter);
    lv_obj_set_size(_meter, 200, 200);

    /*Add a scale first*/
    _scale = lv_meter_add_scale(_meter);
    lv_meter_set_scale_ticks(_meter, _scale, 41, 2, 10, lv_palette_main(LV_PALETTE_GREY));
    lv_meter_set_scale_major_ticks(_meter, _scale, 8, 4, 15, lv_color_black(), 10);
    lv_meter_set_scale_range(_meter, _scale, 0, 1600, 270, 90);

    /*Add a blue arc to the start*/
    _indic = lv_meter_add_arc(_meter, _scale, 3, lv_palette_main(LV_PALETTE_BLUE), 0);
    lv_meter_set_indicator_start_value(_meter, _indic, 0);
    lv_meter_set_indicator_end_value(_meter, _indic, 320);

    /*Make the tick lines blue at the start of the scale*/
    _indic = lv_meter_add_scale_lines(_meter, _scale, lv_palette_main(LV_PALETTE_BLUE), lv_palette_main(LV_PALETTE_BLUE),
                                     false, 0);
    lv_meter_set_indicator_start_value(_meter, _indic, 0);
    lv_meter_set_indicator_end_value(_meter, _indic, 320);

    /*Add a red arc to the end*/
    _indic = lv_meter_add_arc(_meter, _scale, 3, lv_palette_main(LV_PALETTE_RED), 0);
    lv_meter_set_indicator_start_value(_meter, _indic, 1280);
    lv_meter_set_indicator_end_value(_meter, _indic, 1600);

    /*Make the tick lines red at the end of the scale*/
    _indic = lv_meter_add_scale_lines(_meter, _scale, lv_palette_main(LV_PALETTE_RED), lv_palette_main(LV_PALETTE_RED), false,
                                     0);
    lv_meter_set_indicator_start_value(_meter, _indic, 1280);
    lv_meter_set_indicator_end_value(_meter, _indic, 1600);

    /*Add a needle line indicator*/
    _indic = lv_meter_add_needle_line(_meter, _scale, 4, lv_palette_main(LV_PALETTE_GREY), -10);

    lv_anim_init(&_needleMotion);
    lv_anim_set_exec_cb(&_needleMotion, (lv_anim_exec_xcb_t)adjustWeight);
    lv_anim_set_var(&_needleMotion, _meter);
    lv_anim_set_time(&_needleMotion, 150);

    return true;
  }

  void update(msecu32_t const) override {
    static float prev = 0.0F;
    sputf("%s", "update");
    if (_chan) {
      sputf(" → %s", "_chan");
      Packet<float> pwt;
      if (_chan->Weight.pop(pwt)) {
        sputf(" → %s", "pop");
        swritef("update weight | %p |", _chan);
        pwt.describe();

        float wt = pwt.value();
        lv_anim_set_values(&_needleMotion, prev, wt);
        lv_anim_start(&_needleMotion);
        prev = wt;
      }
    }
    swritef(" %s", ".");
  }
  inline std::string title() override { return "Weight"; }
  // operator lv_obj_t*() { return  _base; }
};

#endif // ui_weight_hpp
