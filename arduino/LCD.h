#ifndef LCD_h
#define LCD_h

#include <lvgl.h>
#include <TFT_eSPI.h>
#include <Ticker.h>

#include "spec.h"

class LCD: public TFT_eSPI {
private:
  Ticker                     _tick;
  static lv_disp_t          *_disp;
  static lv_disp_drv_t       _ddrv;
  static lv_disp_draw_buf_t  _draw;
  static lv_color_t          _fb[LCD_WIDTH_PX * 10];
  static lv_indev_t         *_inpt;
  static lv_indev_drv_t      _idrv;
  lv_obj_t                  *_scaleChart;
  lv_chart_series_t         *_scaleSeries;
  void tabs(void);
  bool initTimer(void);
  bool initDisplay(void);
  bool initInput(void);
public:
  class Error {
  public:
    enum Level: int {
      None            = 0,
      InitTimer,   // = 1,
      InitDisplay, // = 2,
      InitInput,   // = 3,
      COUNT        // = 4,
    };
    Error() = default;
    constexpr Error(Level level): _level(level) {}
    constexpr operator Level() const { return _level; }
    constexpr operator bool() const { return _level == None; }
    operator char const *() const {
      switch (_level) {
        case None:
          return "";
        case InitTimer:
          return "LCD: failed to initialize timer";
        case InitDisplay:
          return "LCD: failed to initialize display";
        case InitInput:
          return "LCD: failed to initialize input";
        default:
          break;
      }
      return "LCD: unknown error";
    }
  private:
    Level _level;
  };
  LCD();
  virtual ~LCD(void);
  Error init(void);
  void update(msec_t const now);
  void flush(lv_disp_drv_t *drv, const lv_area_t *area, lv_color_t *color);
  void read(lv_indev_drv_t *drv, lv_indev_data_t *data);
  void updateScale(float value);
  void display(void);
};

#endif // LCD_h
