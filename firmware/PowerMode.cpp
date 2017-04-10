#include <MutilaDebug.h>
#include <stdint.h>
#include <string.h>
#include "PowerMode.h"
#include "Matrix.h"
#include "Config.h"
#include "Settings.h"

PowerMode_ PowerMode;

PowerMode_::PowerMode_() :
    lastPlot(0),
    graphPos(0),
    maxSincePlot(0)
{
    clearGraph();
}

void PowerMode_::start(const char* data)
{
    DB(F("PowerMode::start "));
    DBLN(data);
    String s = data;
    // strip trailing '-' characters
    s.replace("-", "");
    Matrix.clear();
    // TODO: determine if we need to clear the graph - if we have had no value for some number of seconds

    // Update maxSincePlot
    if (s.toInt() > maxSincePlot) {
        maxSincePlot = s.toInt();
    }

    // Plot the graph if we need to
    if (millis() >= lastPlot + GRAPH_PLOT_MS) {
        lastPlot = millis();
        plotGraph();
    }

    writeValue(s.toInt());
    Matrix.paint();
}

void PowerMode_::writeValue(uint32_t tenthsWatt)
{
    // Work out width in pixels of whole part text
    uint16_t wholePart = tenthsWatt/10;
    uint8_t wholePartWidth = wholePart == 0 ? otherWidth+1 : 0;
    for (uint16_t i=wholePart; i>0; i/=10) {
        wholePartWidth += (i%10==1) ? oneWidth : otherWidth;
        wholePartWidth++;
    }

    Matrix.setFont(fontSize);
    // Write the whole part
    Matrix.text(MATRIX_GREEN, xpos - wholePartWidth, ypos, String(wholePart));
    // Draw the decimal point
    Matrix.rectangle(MATRIX_GREEN, xpos, ypos, 2, 2);
    // Write the fractional part (1 d.p.)
    Matrix.text(MATRIX_GREEN, xpos+3, ypos, String(tenthsWatt%10));
    // Write the units symbol (Watts)
    Matrix.text(MATRIX_GREEN, xpos+otherWidth+4, ypos, "W");
}

void PowerMode_::update()
{
}

void PowerMode_::clearGraph()
{
    DBLN(F("PowerMode::clearGraph"));
    memset((void*)graphData, 0, sizeof(uint32_t)*GRAPH_DATA_ITEMS);
}

void PowerMode_::plotGraph()
{
    DB(F("PowerMode::plotGraph 10ths="));
    DBLN(maxSincePlot);
    // Add a value to the graph data & change the pointer
    graphData[graphPos] = maxSincePlot;
    // Incremement the pointer
    graphPos = (graphPos+1)%GRAPH_DATA_ITEMS;

    // Plot up to the pointer
    int8_t idx = graphPos; 
    for (uint8_t i=0; i<GRAPH_DATA_ITEMS; i++) {
        idx = idx == 0 ? GRAPH_DATA_ITEMS-1 : idx-1;
        // get the index in the graph in reverse chronological order (most recent first)
        uint8_t height = (graphData[idx]*MATRIX_SIZE_Y)/(10*MaxPowerWatts.get());
        DB(F("PLOT idx="));
        DB(idx);
        DB(F(" 10ths="));
        DB(graphData[idx]);
        DB(F(" max="));
        DB(MaxPowerWatts.get());
        DB(F(" height="));
        DBLN(height);
        if (graphData[idx]>0) {
            Matrix.line(MATRIX_RED, MATRIX_SIZE_X-i-1, MATRIX_SIZE_Y-1, MATRIX_SIZE_X-i-1, height < MATRIX_SIZE_Y ? MATRIX_SIZE_Y-1-height : 0);
        }
    }
    maxSincePlot = 0;
}



