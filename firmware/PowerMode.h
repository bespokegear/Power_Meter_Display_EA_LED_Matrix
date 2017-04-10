#pragma once

#include <stdint.h>
#include "Config.h"
#include "DisplayMode.h"

class PowerMode_ : public DisplayMode 
{
public:
    PowerMode_() {;}
    void start(const char* data);
    void stop();
    void writeValue(uint32_t tenthsWatt);
    void update();
    //void clearGraph();
    //void plotGraph();
    //void addValue;

private:
   // Some constants we might want to tweak
    static const uint8_t fontSize = 4;
    static const uint8_t oneWidth = 3;
    static const uint8_t otherWidth = 6;
    static const uint8_t ypos = 18;
    static const uint8_t xpos = 37; // the position of the decimal point

    unsigned long nextGraphUpdate; 
    uint32_t graphData;
    uint8_t graphPos;
    uint32_t graphMax;

};

extern PowerMode_ PowerMode;

