#pragma once

#include <stdint.h>
#include "Config.h"
#include "DisplayMode.h"

class PowerMode_ : public DisplayMode 
{
public:
    PowerMode_();
    // Adds a value, and plots
    void start(const char* data);

    // When time period is done, update graph data with new value
    void update();

    // Clear the graph history and last value printed, and reset timers
    void reset();

protected:
    // Draw the value of the max power in this time period
    // and plot the graph underneith
    void draw();

    // Draw individual components
    void drawGraph();
    void drawValue();

    // Add a new value to the graph
    void updateGraph();

private:
    // Some constants we might want to tweak
    static const uint8_t fontSize = 6;
    static const uint8_t oneWidth = 9;
    static const uint8_t otherWidth = 12;
    static const uint8_t ypos = 23;
    static const uint8_t xpos = 55; // the right hand side of the number

    unsigned long lastGraphUpdate; 
    uint32_t lastValue;
    uint32_t graphData[GRAPH_DATA_ITEMS];
    uint8_t graphPos;

};

extern PowerMode_ PowerMode;

