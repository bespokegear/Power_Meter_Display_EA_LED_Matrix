#pragma once

#include <stdint.h>
#include "DisplayMode.h"

class CountdownMode_ : public DisplayMode 
{
public:
    CountdownMode_() {;}
    void start(const char* data);
    void update();

private:
    unsigned long startMs;
    uint8_t n; // digit to display
    uint8_t i; // used for drawing rectangles

};

extern CountdownMode_ CountdownMode;
