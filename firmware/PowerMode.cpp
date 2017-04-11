#include <MutilaDebug.h>
#include <stdint.h>
#include <string.h>
#include "PowerMode.h"
#include "Matrix.h"
#include "Config.h"
#include "Settings.h"

PowerMode_ PowerMode;

PowerMode_::PowerMode_()
{
    reset();
}

void PowerMode_::reset()
{
    memset((void*)graphData, 0, sizeof(uint32_t)*GRAPH_DATA_ITEMS);
    lastGraphUpdate = 0;
}

void PowerMode_::start(const char* data)
{
    DB(F("PowerMode::start "));
    DBLN(data);
    String s = data;
    // strip trailing '-' characters
    s.replace("-", "");
    lastValue = s.toInt();
    draw();
}

void PowerMode_::update()
{
    // Scrolly scrolly plot plot
    if (millis() >= lastGraphUpdate + GRAPH_PLOT_MS) {
        updateGraph();
        draw();
    }
}

void PowerMode_::draw()
{
    Matrix.clear();
    drawGraph();
    drawValue();
    Matrix.paint();
}

void PowerMode_::drawGraph()
{
    DBLN(F("PowerMode::drawGraph"));

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

void PowerMode_::updateGraph()
{
    DBLN(F("PowerMode::updateGraph"));
    lastGraphUpdate = millis();
    // Add a value to the graph data & change the pointer
    graphData[graphPos] = lastValue;
    graphPos = (graphPos+1)%GRAPH_DATA_ITEMS;
}

void PowerMode_::drawValue()
{
    // Work out width in pixels of whole part text
    uint16_t wholePart = lastValue/10;
    uint8_t wholePartWidth = wholePart == 0 ? otherWidth+1 : 0;
    for (uint16_t i=wholePart; i>0; i/=10) {
        wholePartWidth += (i%10==1) ? oneWidth : otherWidth;
        wholePartWidth++;
    }

    Matrix.setFont(fontSize);
    // Write the whole part
    Matrix.text(MATRIX_GREEN, xpos-wholePartWidth, ypos, String(wholePart));
    // Write the fractional part (1 d.p.)
    Matrix.setFont(fontSize-2);
    Matrix.text(MATRIX_ORANGE, xpos-1, ypos-3, "W");
}



