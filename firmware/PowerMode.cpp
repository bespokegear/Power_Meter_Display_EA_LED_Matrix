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
  // Need to parse the data here
  // strip trailing '-' characters
  s.replace("-", "");
  int start_index = -1;
  int end_index;
  String s_substring = "";
  for (int i = 0; i < 2; i++)
  {
    end_index = s.indexOf('E', start_index + 1);
    if (end_index == -1)
    {
      // This sorts out the final teminating char '#'
      end_index = s.indexOf('#', start_index + 1);
    }
    s_substring = s.substring(start_index + 1, end_index);
    switch (i)
    {
      case 0:
        lastPValue = s_substring.toInt();
        break;
      case 1:
        if (s_substring.toInt() > 0)
        {
          lastEValue = s_substring.toInt();
        }
        break;
    }
    start_index = end_index;  // For the next instance
  }
  DB(F("P: "));
  DB(lastPValue);
  DB(F(" E: "));
  DBLN(lastEValue);
  // Here we have the data saved in the A,B,C,D values.
  // Now need to draw the graph
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
  for (uint8_t i = 0; i < GRAPH_DATA_ITEMS; i++) {
    idx = idx == 0 ? GRAPH_DATA_ITEMS - 1 : idx - 1;
    // get the index in the graph in reverse chronological order (most recent first)
    uint32_t height = ((uint32_t)graphData[idx] * MATRIX_SIZE_Y) / (10 * MaxPowerWatts.get());
    if (graphData[idx] > 0) {
      Matrix.line(MATRIX_RED, MATRIX_SIZE_X - i - 1, MATRIX_SIZE_Y - 1, MATRIX_SIZE_X - i - 1, height < MATRIX_SIZE_Y ? MATRIX_SIZE_Y - 1 - height : 0);
    }
  }
}

void PowerMode_::updateGraph()
{
  DBLN(F("PowerMode::updateGraph"));
  lastGraphUpdate = millis();
  // Add a value to the graph data & change the pointer
  graphData[graphPos] = lastPValue;
  graphPos = (graphPos + 1) % GRAPH_DATA_ITEMS;
}

void PowerMode_::drawValue()
{
  Matrix.setFont(6);
  // Write the whole part
  Matrix.rtext(MATRIX_GREEN, 50, 15, String(lastPValue / 10));

  // Need to sort ouyt Ws to Wh?
  Matrix.rtext(MATRIX_GREEN, 50 + 64, 15, String(lastEValue / 3600));
  // Write the fractional part (1 d.p.)
  Matrix.setFont(4);
  Matrix.text(MATRIX_GREEN, 50 + 3, 15, "W");
  Matrix.text(MATRIX_GREEN, 50 + 3 + 64, 15, "Wh");
}
