#pragma once

#include "DisplayMode.h"

class SmallTextMode_ : public DisplayMode 
{
public:
    SmallTextMode_() {;}
    void start(const char* data);

};

extern SmallTextMode_ SmallTextMode;
