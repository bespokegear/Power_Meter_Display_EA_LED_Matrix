#pragma once

#include "DisplayMode.h"

class TimerMode_ : public DisplayMode 
{
public:
    TimerMode_() {;}
    void start(const char* data);

};

extern TimerMode_ TimerMode;

