#pragma once

#include <stdint.h>
#include "Config.h"
#include "DisplayMode.h"

class EnergyMode_ : public DisplayMode 
{
public:
    EnergyMode_();
    // Adds a value, and plots
    void start(const char* data);

    // Clear the graph history and last value printed, and reset timers
    void reset();
    // Draw individual components
    void drawValue();
    
private:
    uint32_t lastValue;
    
};

extern EnergyMode_ EnergyMode;
