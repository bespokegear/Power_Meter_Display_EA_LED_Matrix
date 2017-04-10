#include <MutilaDebug.h>
#include <stdint.h>
#include "PowerMode.h"
#include "Matrix.h"

PowerMode_ PowerMode;

void PowerMode_::start(const char* data)
{
    DB(F("PowerMode::start "));
    DBLN(data);
    String s = data;
    // strip trailing '-' characters
    s.replace("-", "");
    Matrix.clear();
    // TODO: determine if we need to clear the graph
    // TODO: plot graph 
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
    Matrix.text(MATRIX_GREEN, xpos+12, ypos, "W");
}

void PowerMode_::update()
{
}
