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
    static const uint8_t fontSize = 5;
    static const uint8_t oneWidth = 8;
    static const uint8_t otherWidth = 10;
    static const uint8_t ypos = 18;
    static const uint8_t xpos = 45; // the position of the decimal point

    unsigned long lastGraphUpdate; 
    unsigned long lastValueUpdate; 
    unsigned long lastDrawGraph;
    unsigned long lastDrawValue;
    uint32_t maxSinceGraph;
    uint32_t maxSinceValue;
    uint32_t graphData[GRAPH_DATA_ITEMS];
    uint8_t graphPos;

};

extern PowerMode_ PowerMode;

