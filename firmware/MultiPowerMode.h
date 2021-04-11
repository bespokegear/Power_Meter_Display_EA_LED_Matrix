#pragma once

#include <stdint.h>
#include "Config.h"
#include "DisplayMode.h"

class MultiPowerMode_ : public DisplayMode
{
  public:
    MultiPowerMode_();
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
    unsigned long lastGraphUpdate;
    uint16_t lastValue;   // This wont be used?
    uint16_t lastAValue;
    uint16_t lastBValue;
    uint16_t lastCValue;
    uint16_t lastDValue;
    uint16_t graphAData[GRAPH_DATA_ITEMS_SM];
    uint16_t graphBData[GRAPH_DATA_ITEMS_SM];
    uint16_t graphCData[GRAPH_DATA_ITEMS_SM];
    uint16_t graphDData[GRAPH_DATA_ITEMS_SM];
    uint8_t graphPos;

};


extern MultiPowerMode_ MultiPowerMode;
