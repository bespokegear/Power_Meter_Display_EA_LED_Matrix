#pragma once

#include "DisplayMode.h"

class TextControlMode_ : public DisplayMode 
{
public:
    TextControlMode_() {;}
    void start(const char* data);

};

extern TextControlMode_ TextControlMode;

