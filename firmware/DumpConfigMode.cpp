#include <MutilaDebug.h>
#include "DumpConfigMode.h"
#include "Settings.h"
#include "Parser.h"

DumpConfigMode_ DumpConfigMode;

void DumpConfigMode_::start(const char* data)
{
    Parser.dumpID();
    DBLN(F("DumpConfigMode::start"));
    Serial.print(F("Max Graph Power: "));
    Serial.println(MaxPowerWatts.get());
}

