#ifndef Scale_h
#define Scale_h

#include <HX711.h>

#include "spec.h"

class Scale: private HX711 {
private:
  float _read;
  float _zero;
  void set(float val);
public:
  Scale(void);
  virtual ~Scale(void);
  bool init(void);
  void update(msec_t const now);
  float read(void);
  void zero(float val = 0.0F);
};

#endif // Scale_h
