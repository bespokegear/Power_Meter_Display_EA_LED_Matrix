#pragma once

#include "DisplayMode.h"

class DumpConfigMode_ : public DisplayMode 
{
public:
    DumpConfigMode_() {;}
    void start(const char* data);
};

extern DumpConfigMode_ DumpConfigMode;
