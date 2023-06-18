#include "Target.h"
#include "Scale.h"

Scale::Scale(void):
  _read(0.0F), _zero(0.0F) {}

Scale::~Scale(void) {}

bool Scale::init(void) {
  begin(SCALE_DATA_PIN, SCALE_CLOCK_PIN);
  set_scale(SCALE_FACTOR);
  set_offset(SCALE_OFFSET);
  tare();
  return true;
}

void Scale::update(msec_t const now) {
  throttle(set(get_units(SCALE_SAMPLE)), now, MSECS_PER_SCALE_UPDATE);
}

float Scale::read(void) {
  return _read - _zero;
}

void Scale::zero(float val) {
  _zero = val;
}

void Scale::set(float val) {
  _read = val;
  target.lcd()->updateScale(read());
}
