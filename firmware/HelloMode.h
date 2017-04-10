#pragma once

#include "DisplayMode.h"

class HelloMode_ : public DisplayMode 
{
public:
    HelloMode_() {;}
    void start(const char* data);

};

extern HelloMode_ HelloMode;

