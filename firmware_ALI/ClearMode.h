#pragma once

#include "DisplayMode.h"

class ClearMode_ : public DisplayMode 
{
public:
    ClearMode_() {;}
    void start(const char* data);

};

extern ClearMode_ ClearMode;
