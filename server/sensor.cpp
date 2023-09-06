#include <periph/APDS9960.h>
#include <periph/HTS221.h>
#include <periph/LPS22HB.h>
#include <periph/LSM9DS1.h>
#include <periph/MP34DT05.h>

// Provide instances that are defined extern in the nanosensors library.
//
// You must also ensure NO_GLOBAL_NANOSENSORS is defined if using the library
// provided as a submodule. If the library is installed via the Arduino IDE
// sketchbook, then NO_GLOBAL_NANOSENSORS is not required.
#ifdef NO_GLOBAL_NANOSENSORS
PeriphAPDS9960     _APDS9960(Wire1, PIN_INT_APDS);
PeriphHTS221       _HTS221(Wire1);
PeriphLPS22HB      _LPS22HB(Wire1);
PeriphLSM9DS1      _LSM9DS1(Wire1);
PeriphMP34DT05<>   _MP34DT05(PIN_PDM_DIN, PIN_PDM_CLK, PIN_PDM_PWR);
#endif
