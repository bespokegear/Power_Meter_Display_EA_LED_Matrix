#include "Settings.h"

//                                                  EEROM,  Min,    Max,    Default,    Incremenet
PersistentSetting<uint16_t> MaxPowerWatts(          0,      1,     9999,    500,        50);
PersistentSetting<uint8_t> DisplayID0(              4,      '*',    'Z',    '*',        1);
PersistentSetting<uint8_t> DisplayID1(              8,      '*',    'Z',    '*',        1);
