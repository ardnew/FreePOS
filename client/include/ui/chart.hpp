#ifndef ui_chart_hpp
#define ui_chart_hpp

#include "../spec.hpp"
#include "../ipc.hpp"

class Chart: public View {
protected:
  IPC *_chan;
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
  void connect(IPC *chan) {
    _chan = chan;
  }
  bool init(lv_obj_t *root) override {
    _base = lv_chart_create(root);

    lv_obj_set_size(_base, LV_PCT(100), LV_PCT(100));
    lv_chart_set_type(_base, LV_CHART_TYPE_LINE);
    lv_chart_set_point_count(_base, 100);

    _accelXSeries = lv_chart_add_series(
      _base, lv_palette_main(LV_PALETTE_RED), LV_CHART_AXIS_PRIMARY_Y);
    _accelYSeries = lv_chart_add_series(
      _base, lv_palette_main(LV_PALETTE_GREEN), LV_CHART_AXIS_PRIMARY_Y);
    _accelZSeries = lv_chart_add_series(
      _base, lv_palette_main(LV_PALETTE_BLUE), LV_CHART_AXIS_PRIMARY_Y);
    //_barPsiSeries = lv_chart_add_series(
    //  _base, lv_palette_main(LV_PALETTE_RED), LV_CHART_AXIS_PRIMARY_Y);
    //_precipSeries = lv_chart_add_series(
    //  _base, lv_palette_main(LV_PALETTE_RED), LV_CHART_AXIS_PRIMARY_Y);
    //_proximSeries = lv_chart_add_series(
    //  _base, lv_palette_main(LV_PALETTE_RED), LV_CHART_AXIS_PRIMARY_Y);
    //_airTmpSeries = lv_chart_add_series(
    //  _base, lv_palette_main(LV_PALETTE_RED), LV_CHART_AXIS_PRIMARY_Y);
    //_h2OTmpSeries = lv_chart_add_series(
    //  _base, lv_palette_main(LV_PALETTE_RED), LV_CHART_AXIS_PRIMARY_Y);
    //_weightSeries = lv_chart_add_series(
    //  _base, lv_palette_main(LV_PALETTE_RED), LV_CHART_AXIS_PRIMARY_Y);

    // _scaleSeries = lv_chart_add_series(
    //   _base, lv_palette_main(LV_PALETTE_RED), LV_CHART_AXIS_PRIMARY_Y);

    lv_chart_set_point_count(_base, 100);
    lv_chart_set_update_mode(_base, LV_CHART_UPDATE_MODE_SHIFT);

    lv_chart_set_range(_base, LV_CHART_AXIS_PRIMARY_Y, -100.0, 100.0);

    return true;
  }
  void update(msecu32_t const) override {
    if (_chan) {
      static Packet<std::array<float, 3U>> AccXyzPkt;
      if (_chan->AccXyz.pop(AccXyzPkt)) {
        lv_chart_set_next_value(_base, _accelXSeries, 100.0 * AccXyzPkt.value()[0]);
        lv_chart_set_next_value(_base, _accelYSeries, 100.0 * AccXyzPkt.value()[1]);
        lv_chart_set_next_value(_base, _accelZSeries, 100.0 * AccXyzPkt.value()[2]);
      }
      // Packet<float> BarPsiPkt;
      // if (_chan->BarPsi.pop(BarPsiPkt)) {
      //   //lv_chart_set_next_value(_base, _barPsiSeries, BarPsiPkt.value());
      // }
      // Packet<float> PrecipPkt;
      // if (_chan->Precip.pop(PrecipPkt)) {
      //   //lv_chart_set_next_value(_base, _precipSeries, PrecipPkt.value());
      // }
      // Packet<int32_t> ProximPkt;
      // if (_chan->Proxim.pop(ProximPkt)) {
      //   //lv_chart_set_next_value(_base, _proximSeries, ProximPkt.value());
      // }
      // Packet<float> AirTmpPkt;
      // if (_chan->AirTmp.pop(AirTmpPkt)) {
      //   //lv_chart_set_next_value(_base, _airTmpSeries, AirTmpPkt.value());
      // }
      // Packet<float> H2OTmpPkt;
      // if (_chan->H2OTmp.pop(H2OTmpPkt)) {
      //   //lv_chart_set_next_value(_base, _h2OTmpSeries, H2OTmpPkt.value());
      // }
      // Packet<float> WeightPkt;
      // if (_chan->Weight.pop(WeightPkt)) {
      //   //lv_chart_set_next_value(_base, _weightSeries, WeightPkt.value());
      // }
      // _scaleSeries->points[0] = _chan->scale();
      // lv_chart_refresh(_base);
    }
  }
  inline std::string title() override { return "Chart"; }
  // operator lv_obj_t*() { return  _base; }
};

#endif // ui_chart_hpp
