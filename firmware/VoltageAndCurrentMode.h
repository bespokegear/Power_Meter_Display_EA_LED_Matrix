#pragma once

#include "DisplayMode.h"

class VoltageAndCurrentMode_ : public DisplayMode 
{
public:
    VoltageAndCurrentMode_() {;}
    void start(const char* data);

};

extern VoltageAndCurrentMode_ VoltageAndCurrentMode;

