#pragma once

#include "DisplayMode.h"

class PowerMode_ : public DisplayMode 
{
public:
    PowerMode_() {;}
    void start(const char* data);

};

extern PowerMode_ PowerMode;

