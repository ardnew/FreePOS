#include "Target.h"

Target::Target(void) {}

Target::~Target(void) {}

bool Target::init(void) {
  bool ok = _lcd.init();
  if (ok) {
    _lcd.display();
  }
  if (ok) {
    ok = _scale.init();
  }
  return ok;
}

LCD *Target::lcd(void) {
  return &_lcd;
}

Scale *Target::scale(void) {
  return &_scale;
}

void Target::update(msec_t const now) {
  throttle(updatePeriphs(now), now, MSECS_PER_TARGET_UPDATE);
}

void Target::updatePeriphs(msec_t const now) {
  _lcd.update(now);
  _scale.update(now);
}
