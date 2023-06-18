#ifndef Target_h
#define Target_h

#include "LCD.h"
#include "Scale.h"

class Target {
private:
  LCD _lcd;
  Scale _scale;
  void updatePeriphs(msec_t const now);
public:
  Target(void);
  virtual ~Target(void);
  bool init(void);
  LCD *lcd(void);
  Scale *scale(void);
  void update(msec_t const now);
};

extern Target target;

#endif // Target_h
