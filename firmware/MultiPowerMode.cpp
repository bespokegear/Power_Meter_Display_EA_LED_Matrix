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
  memset((void*)graphAData, 0, sizeof(uint16_t)*GRAPH_DATA_ITEMS_SM);
  memset((void*)graphBData, 0, sizeof(uint16_t)*GRAPH_DATA_ITEMS_SM);
  memset((void*)graphCData, 0, sizeof(uint16_t)*GRAPH_DATA_ITEMS_SM);  
  memset((void*)graphDData, 0, sizeof(uint16_t)*GRAPH_DATA_ITEMS_SM);  
  lastGraphUpdate = 0;
}

void MultiPowerMode_::start(const char* data)
{
  DB(F("MultiPowerMode::start "));
  DBLN(data);
  String s = data;
  // strip trailing '-' characters

  // Test data
  Serial.println(s);
  // The data we get here will be the end part of the string.
  // Need to parse the data here
  int start_index = s.indexOf(':');
  int end_index;
  String s_substring = "";

  for (int i = 0; i < 4; i++)
  {
    end_index = s.indexOf(':', start_index + 1);
    if (end_index == -1)
    {
      // This sorts out the final teminating char '#'
      end_index = s.indexOf('#', start_index + 1);
    }
    s_substring = s.substring(start_index + 1, end_index);
    switch (i)
    {
      case 0:
        lastAValue = s_substring.toInt();
        break;
      case 1:
        lastBValue = s_substring.toInt();
        break;
      case 2:
        lastCValue = s_substring.toInt();
        break;
      case 3:
        lastDValue = s_substring.toInt();
        break;
    }
    start_index = end_index;  // For the next instance
  }
  DB(F("A: "));
  DB(lastAValue);
  DB(F(" B: "));
  DB(lastBValue);
  DB(F(" C: "));
  DB(lastCValue);
  DB(F(" D: "));
  DBLN(lastDValue);
  // Here we have the data saved in the A,B,C,D values.
  // Now need to draw the graph
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
  // Need to draw 4 graphs here: one for each bit of data

  // Plot up to the pointer
  int8_t idx = graphPos;
  for (uint8_t i = 0; i < GRAPH_DATA_ITEMS_SM; i++) {
    idx = idx == 0 ? GRAPH_DATA_ITEMS_SM - 1 : idx - 1;
    // get the index in the graph in reverse chronological order (most recent first)
    // We use the max graph power / 4 for the max graph power for these smaller graphs
    uint8_t height = (graphAData[idx] * MATRIX_SIZE_Y) / ((10 * MaxPowerWatts.get()) / 4);
    if (graphAData[idx] > 0) {
      Matrix.line(MATRIX_GREEN, 64+GRAPH_DATA_ITEMS_SM - i - 1, MATRIX_SIZE_Y - 1, GRAPH_DATA_ITEMS_SM - i - 1, height < MATRIX_SIZE_Y ? MATRIX_SIZE_Y - 1 - height : 0);
    }
    height = (graphBData[idx] * MATRIX_SIZE_Y) / ((10 * MaxPowerWatts.get()) / 4);
    if (graphBData[idx] > 0) {
      Matrix.line(MATRIX_RED, 128+GRAPH_DATA_ITEMS_SM - i - 1, MATRIX_SIZE_Y - 1, GRAPH_DATA_ITEMS_SM - i - 1, height < MATRIX_SIZE_Y ? MATRIX_SIZE_Y - 1 - height : 0);
    }
    height = (graphCData[idx] * MATRIX_SIZE_Y) / ((10 * MaxPowerWatts.get()) / 4);
    if (graphCData[idx] > 0) {
      Matrix.line(MATRIX_RED, GRAPH_DATA_ITEMS_SM - i - 1, MATRIX_SIZE_Y - 1, GRAPH_DATA_ITEMS_SM - i - 1, height < MATRIX_SIZE_Y ? MATRIX_SIZE_Y - 1 - height : 0);
    }
    height = (graphDData[idx] * MATRIX_SIZE_Y) / ((10 * MaxPowerWatts.get()) / 4);
    if (graphDData[idx] > 0) {
      Matrix.line(MATRIX_GREEN, 32+GRAPH_DATA_ITEMS_SM - i - 1, MATRIX_SIZE_Y - 1, GRAPH_DATA_ITEMS_SM - i - 1, height < MATRIX_SIZE_Y ? MATRIX_SIZE_Y - 1 - height : 0);
    }    
    
  }
}

void MultiPowerMode_::updateGraph()
{
  DBLN(F("MultiPowerMode::updateGraph"));
  lastGraphUpdate = millis();
  // Add a value to the graph data & change the pointer
  graphAData[graphPos] = lastAValue;
  graphBData[graphPos] = lastBValue;
  graphCData[graphPos] = lastCValue;  
  graphDData[graphPos] = lastDValue;
  graphPos = (graphPos + 1) % GRAPH_DATA_ITEMS_SM;
}

void MultiPowerMode_::drawValue()
{

  // Need to draw all 4 values here:
  // A is at 96, B is at 128, C is at 64 and D is at 32 (right justified)
  Matrix.setFont(4);
  // Write the whole part
  Matrix.rtext(MATRIX_RED, 96, 15, String(lastAValue / 10));
  Matrix.rtext(MATRIX_GREEN, 128, 15, String(lastBValue / 10));
  Matrix.rtext(MATRIX_GREEN, 32, 15, String(lastCValue / 10));
  Matrix.rtext(MATRIX_RED, 64, 15, String(lastDValue / 10));
  // Write the fractional part (1 d.p.)
//  Matrix.setFont(2);
//  Matrix.text(MATRIX_RED, 64 + 3, 15, "W");
}
