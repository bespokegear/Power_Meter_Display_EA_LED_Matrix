#pragma once

#include <stdint.h>
#include "Config.h"
#include "DisplayMode.h"

class PowerMode_ : public DisplayMode 
{
public:
    PowerMode_();
    void start(const char* data);
    void stop();
    void writeValue(uint32_t tenthsWatt);
    void update();
    void clearGraph();
    void plotGraph();

private:
   // Some constants we might want to tweak
    //static const uint8_t fontSize = 4;
    //static const uint8_t oneWidth = 3;
    //static const uint8_t otherWidth = 6;
    //static const uint8_t ypos = 18;
    //static const uint8_t xpos = 37; // the position of the decimal point
    static const uint8_t fontSize = 5;
    static const uint8_t oneWidth = 8;
    static const uint8_t otherWidth = 10;
    static const uint8_t ypos = 18;
    static const uint8_t xpos = 45; // the position of the decimal point

    unsigned long lastPlot; 
    uint32_t graphData[GRAPH_DATA_ITEMS];
    uint8_t graphPos;
    uint32_t maxSincePlot;

};

extern PowerMode_ PowerMode;

