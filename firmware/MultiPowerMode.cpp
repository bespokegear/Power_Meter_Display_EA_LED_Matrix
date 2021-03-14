#include <MutilaDebug.h>
#include <stdint.h>
#include <string.h>
#include "MultiPowerMode.h"
#include "Matrix.h"
#include "Config.h"
#include "Settings.h"

MultiPowerMode_ MultiPowerMode;

MultiPowerMode_::MultiPowerMode_()
{
    reset();
}

void MultiPowerMode_::reset()
{
    memset((void*)graphData, 0, sizeof(uint32_t)*GRAPH_DATA_ITEMS);
    lastGraphUpdate = 0;
}

void MultiPowerMode_::start(const char* data)
{
    DB(F("MultiPowerMode::start "));
    DBLN(data);
    String s = data;
    // strip trailing '-' characters
    s.replace("-", "");
    lastValue = s.toInt();
    draw();
}

void MultiPowerMode_::update()
{
    // Scrolly scrolly plot plot
    if (millis() >= lastGraphUpdate + GRAPH_PLOT_MS) {
        updateGraph();
        draw();
    }
}

void MultiPowerMode_::draw()
{
    Matrix.clear();
    drawGraph();
    drawValue();
    Matrix.paint();
}

void MultiPowerMode_::drawGraph()
{
    DBLN(F("MultiPowerMode::drawGraph"));

    // Plot up to the pointer
    int8_t idx = graphPos; 
    for (uint8_t i=0; i<GRAPH_DATA_ITEMS; i++) {
        idx = idx == 0 ? GRAPH_DATA_ITEMS-1 : idx-1;
        // get the index in the graph in reverse chronological order (most recent first)
        uint8_t height = (graphData[idx]*MATRIX_SIZE_Y)/(10*MaxPowerWatts.get());
        if (graphData[idx]>0) {
            Matrix.line(MATRIX_RED, MATRIX_SIZE_X-i-1, MATRIX_SIZE_Y-1, MATRIX_SIZE_X-i-1, height < MATRIX_SIZE_Y ? MATRIX_SIZE_Y-1-height : 0);
        }
    }
}

void MultiPowerMode_::updateGraph()
{
    DBLN(F("MultiPowerMode::updateGraph"));
    lastGraphUpdate = millis();
    // Add a value to the graph data & change the pointer
    graphData[graphPos] = lastValue;
    graphPos = (graphPos+1)%GRAPH_DATA_ITEMS;
}

void MultiPowerMode_::drawValue()
{
    Matrix.setFont(6);
    // Write the whole part
    Matrix.rtext(MATRIX_GREEN, 50, 24, String(lastValue/10));
    // Write the fractional part (1 d.p.)
    Matrix.setFont(4);
    Matrix.text(MATRIX_GREEN, 50+3, 21, "W");
}
