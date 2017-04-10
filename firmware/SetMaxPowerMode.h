#pragma once

#include "DisplayMode.h"

class SetMaxPowerMode_ : public DisplayMode 
{
public:
    SetMaxPowerMode_() {;}
    void start(const char* data);

};

extern SetMaxPowerMode_ SetMaxPowerMode;

