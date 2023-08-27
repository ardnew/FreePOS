#include "target.hpp"

Target target;

void setup(void) {
  native::log::init();
  swritef("%s", "initializing target");
  if (!target.init()) {
		spanicf("%s: failed to initialize target", "target.init()");
	}
  swritef("%s", "target entering run-state");
}

void loop(void) {
  target.update();
}
