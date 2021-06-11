#pragma once

#include <Mode.h>

class DisplayMode
{
public:
    DisplayMode() {;}
    virtual void start(const char* data) {;}
    virtual void update() {;}
    
};
