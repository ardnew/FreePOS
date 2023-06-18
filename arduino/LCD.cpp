#include <lvgl.h>

#include "LCD.h"
#include "Target.h"

// Plain C static functions are used to simplify callback semantics.
//
// Use the sole Target instance declared in Target.h to obtain a reference to
// our instantiated objects such as the LCD.

#if (LV_USE_LOG)
static void c_log(const char *buf) {
  logf("%s", buf);
}
#endif

static void c_tick(void) {
  lv_tick_inc(MSECS_PER_LCD_UPDATE);
}

static void c_flush(lv_disp_drv_t *drv, const lv_area_t *area, lv_color_t *color) {
  target.lcd()->flush(drv, area, color);
}

static void c_read(lv_indev_drv_t *drv, lv_indev_data_t *data) {
  target.lcd()->read(drv, data);
}

lv_disp_t          *LCD::_disp;
lv_disp_drv_t       LCD::_ddrv;
lv_disp_draw_buf_t  LCD::_draw;
lv_color_t          LCD::_fb[LCD_WIDTH_PX * 10];
lv_indev_t         *LCD::_inpt;
lv_indev_drv_t      LCD::_idrv;

LCD::LCD():
  TFT_eSPI(
    LCD_ROTATION & 1 ?
      LCD_HEIGHT_PX : LCD_WIDTH_PX,
    LCD_ROTATION & 1 ?
      LCD_WIDTH_PX : LCD_HEIGHT_PX
  ),
  _scaleChart(nullptr),
  _scaleSeries(nullptr) {}

LCD::~LCD(void) {}

LCD::Error LCD::init(void) {
  lv_init();
#if (LV_USE_LOG)
  lv_log_register_print_cb(c_log); // Register debug print function
#endif
  begin();
  if (!initTimer()) {
    return LCD::Error::InitTimer;
  }
  if (!initDisplay()) {
    return LCD::Error::InitDisplay;
  }
  if (!initInput()) {
    return LCD::Error::InitInput;
  }
  return LCD::Error::None;
}

void LCD::update(msec_t const now) {
  throttle(lv_timer_handler(), now, MSECS_PER_TARGET_UPDATE);
}

void LCD::flush(lv_disp_drv_t *drv, const lv_area_t *area, lv_color_t *color) {
  uint32_t w = area->x2 - area->x1 + 1;
  uint32_t h = area->y2 - area->y1 + 1;
  if (w == 0 || h == 0) { return; }
  startWrite();
#if LCD_LAZY_ASET
  static uint32_t pw = ~w; // initialize the static values so that it is
  static uint32_t ph = ~h; // guaranteed to call setAddrWindow on first flush.
  if (w != pw || h != ph) {
    setAddrWindow(area->x1, area->y1, w, h);
    pw = w;
    ph = h;
  }
#else
  setAddrWindow(area->x1, area->y1, w, h);
#endif
  pushColors((uint16_t *)&color->full, w * h, true);
  endWrite();
  lv_disp_flush_ready(drv);
}

void LCD::read(lv_indev_drv_t *drv, lv_indev_data_t *data) {
  static uint16_t const tol = 600;
  uint16_t x, y;
  if (!getTouch(&x, &y, tol)) {
    data->state = LV_INDEV_STATE_REL;
  } else {
    data->state = LV_INDEV_STATE_PR;
    data->point.x = x;
    data->point.y = y;
  }
}

void LCD::updateScale(float value) {
  if (nullptr != _scaleChart && nullptr != _scaleSeries) {
    lv_chart_set_next_value(_scaleChart, _scaleSeries, value);
  }
}


static void scroll_begin_event(lv_event_t * e)
{
    /*Disable the scroll animations. Triggered when a tab button is clicked */
    if(lv_event_get_code(e) == LV_EVENT_SCROLL_BEGIN) {
        lv_anim_t * a = (lv_anim_t *)lv_event_get_param(e);
        if(a)  a->time = 0;
    }
}


void LCD::tabs(void) {
    /*Create a Tab view object*/
    lv_obj_t *tabview;
    tabview = lv_tabview_create(lv_scr_act(), LV_DIR_LEFT, 80);
    lv_obj_add_event_cb(lv_tabview_get_content(tabview), scroll_begin_event, LV_EVENT_SCROLL_BEGIN, NULL);

    lv_obj_set_style_bg_color(tabview, lv_palette_lighten(LV_PALETTE_RED, 2), 0);

    lv_obj_t * tab_btns = lv_tabview_get_tab_btns(tabview);
    lv_obj_set_style_bg_color(tab_btns, lv_palette_darken(LV_PALETTE_GREY, 3), 0);
    lv_obj_set_style_text_color(tab_btns, lv_palette_lighten(LV_PALETTE_GREY, 5), 0);
    lv_obj_set_style_border_side(tab_btns, LV_BORDER_SIDE_RIGHT, LV_PART_ITEMS | LV_STATE_CHECKED);


    /*Add 3 tabs (the tabs are page (lv_page) and can be scrolled*/
    lv_obj_t *tab1 = lv_tabview_add_tab(tabview, "Tab 1");
    lv_obj_t *tab2 = lv_tabview_add_tab(tabview, "Tab 2");
    lv_obj_t *tab3 = lv_tabview_add_tab(tabview, "Tab 3");
    lv_obj_t *tab4 = lv_tabview_add_tab(tabview, "Tab 4");
    lv_obj_t *tab5 = lv_tabview_add_tab(tabview, "Tab 5");

    lv_obj_set_style_bg_color(tab2, lv_palette_lighten(LV_PALETTE_AMBER, 3), 0);
    lv_obj_set_style_bg_opa(tab2, LV_OPA_COVER, 0);

    /*Add content to the tabs*/
//    lv_obj_t * label = lv_label_create(tab1);
//    lv_label_set_text(label, "First tab");

    _scaleChart = lv_chart_create(tab1);
    lv_obj_set_size(_scaleChart, LCD_WIDTH_PX - 80 - 10, LCD_HEIGHT_PX - 10);
    lv_obj_center(_scaleChart);
    lv_chart_set_type(_scaleChart, LV_CHART_TYPE_LINE);   /*Show lines and points too*/

    /*Add two data series*/
    _scaleSeries = lv_chart_add_series(_scaleChart, lv_palette_main(LV_PALETTE_RED), LV_CHART_AXIS_PRIMARY_Y);
    lv_chart_set_point_count(_scaleChart, 100);
    lv_chart_set_update_mode(_scaleChart, LV_CHART_UPDATE_MODE_SHIFT);


    lv_obj_t *label = lv_label_create(tab2);
    lv_obj_set_style_text_font(label, &beans_16, 0);
    lv_label_set_text(label, "Second tab");

    label = lv_label_create(tab3);
    lv_label_set_text(label, "Third tab");

    label = lv_label_create(tab4);
    lv_label_set_text(label, "Forth tab");

    label = lv_label_create(tab5);
    lv_label_set_text(label, "Fifth tab");

    lv_obj_clear_flag(lv_tabview_get_content(tabview), LV_OBJ_FLAG_SCROLLABLE);
}

static void fonts(void) {
  lv_obj_t *cont = lv_obj_create(lv_scr_act());
  lv_obj_set_size(cont, 240, 320);
  lv_obj_center(cont);
  lv_obj_set_flex_flow(cont, LV_FLEX_FLOW_COLUMN_WRAP);


  lv_obj_t *label = lv_label_create(cont);
  lv_label_set_text(label, "This is size 12");
  lv_obj_align(label, LV_ALIGN_CENTER, 0, 0);
  lv_obj_set_style_text_font(label, &lv_font_montserrat_12, 0);

  label = lv_label_create(cont);
  lv_label_set_text(label, "This is size 14");
  lv_obj_align(label, LV_ALIGN_CENTER, 0, 0);
  lv_obj_set_style_text_font(label, &lv_font_montserrat_14, 0);

  label = lv_label_create(cont);
  lv_label_set_text(label, "This is size 16");
  lv_obj_align(label, LV_ALIGN_CENTER, 0, 0);
  lv_obj_set_style_text_font(label, &lv_font_montserrat_16, 0);

  label = lv_label_create(cont);
  lv_label_set_text(label, "This is size 18");
  lv_obj_align(label, LV_ALIGN_CENTER, 0, 0);
  lv_obj_set_style_text_font(label, &lv_font_montserrat_18, 0);

  label = lv_label_create(cont);
  lv_label_set_text(label, "This is size 20");
  lv_obj_align(label, LV_ALIGN_CENTER, 0, 0);
  lv_obj_set_style_text_font(label, &lv_font_montserrat_20, 0);
}

void LCD::display(void) {
  unused(fonts);
  tabs();
}

bool LCD::initTimer(void) {
  _tick.attach_ms(MSECS_PER_LCD_UPDATE, c_tick);
  return true;
}

bool LCD::initDisplay(void) {
  setRotation(LCD_ROTATION);
  lv_disp_draw_buf_init(&_draw, _fb, NULL, LCD_WIDTH_PX * 10);
  lv_disp_drv_init(&_ddrv);
  _ddrv.hor_res = LCD_WIDTH_PX;
  _ddrv.ver_res = LCD_HEIGHT_PX;
  _ddrv.flush_cb = c_flush;
  _ddrv.draw_buf = &_draw;
  _disp = lv_disp_drv_register(&_ddrv);
  return nullptr != _disp;
}

bool LCD::initInput(void) {
  setTouch((uint16_t *)LCD_TOUCH_CAL);
  lv_indev_drv_init(&_idrv);
  _idrv.type = LV_INDEV_TYPE_POINTER;
  _idrv.read_cb = c_read;
  _inpt = lv_indev_drv_register(&_idrv);
  return nullptr != _inpt;
}
Aw
