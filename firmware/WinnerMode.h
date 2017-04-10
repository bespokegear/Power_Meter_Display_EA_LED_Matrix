#pragma once

#include <stdint.h>
#include "DisplayMode.h"

class WinnerMode_ : public DisplayMode 
{
public:
    WinnerMode_() {;}
    void start(const char* data);

};

extern WinnerMode_ WinnerMode;

