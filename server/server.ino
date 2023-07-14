#include <Arduino.h>

#include "target.hpp"

Target target;

void setup(void) {
#ifdef DEBUG
  SIO.begin(115200);
#ifdef DEBUG_SYNC
  while (!SIO) { delay(10); }
#endif // DEBUG_SYNC
#endif // DEBUG
  swritef("%s", "initializing target");
  if (!target.init()) {
		spanicf("%s: failed to initialize target", "target.init()");
	}
  swritef("%s", "target entering run-state");
}

void loop(void) {
  target.update();
}
