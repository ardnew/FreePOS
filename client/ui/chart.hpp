#ifndef chart_hpp
#define chart_hpp

#include <ESP323248S035.h>
#include <lvgl.h>

class Chart: public View {
protected:
  lv_obj_t *_base;

  lv_chart_series_t *_accelXSeries;
  lv_chart_series_t *_accelYSeries;
  lv_chart_series_t *_accelZSeries;
  lv_chart_series_t *_barPsiSeries;
  lv_chart_series_t *_precipSeries;
  lv_chart_series_t *_proximSeries;
  lv_chart_series_t *_airTmpSeries;
  lv_chart_series_t *_h2OTmpSeries;
  lv_chart_series_t *_weightSeries;

public:
  bool init(lv_obj_t *root) override {
    _base = lv_chart_create(root);

    lv_obj_set_size(_base, LV_PCT(100), LV_PCT(100));
    lv_chart_set_type(_base, LV_CHART_TYPE_LINE);

    // _scaleSeries = lv_chart_add_series(
    //   _base, lv_palette_main(LV_PALETTE_RED), LV_CHART_AXIS_PRIMARY_Y);

    lv_chart_set_point_count(_base, 100);
    lv_chart_set_update_mode(_base, LV_CHART_UPDATE_MODE_SHIFT);

    return true;
  }
  void update(msec_t const) override {}
  inline std::string title() override { return "Chart"; }
  // operator lv_obj_t*() { return  _base; }
};

#endif // chart_hpp
