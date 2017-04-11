#pragma once

#include "DisplayMode.h"

class SetIDMode_ : public DisplayMode 
{
public:
    SetIDMode_() {;}
    void start(const char* data);

};

extern SetIDMode_ SetIDMode;

