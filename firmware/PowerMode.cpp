#include <MutilaDebug.h>
#include <stdint.h>
#include "PowerMode.h"
#include "Matrix.h"

PowerMode_ PowerMode;

void PowerMode_::start(const char* data)
{
    DB(F("PowerMode::start "));
    DBLN(data);

    // Some constants we might want to tweak
    const uint8_t fontSize = 4;
    const uint8_t oneWidth = 3;
    const uint8_t otherWidth = 6;
    const uint8_t ypos = 18;
    const uint8_t xpos = 37; // the position of the decimal point
    
    String s = data;
    // strip trailing '-' characters
    s.replace("-", "");

    // Work out width in pixels of whole part text
    // With font size 5, width of 1 is 8 pixels, 10 for other digits
    uint16_t wholePart = s.toInt()/10;
    uint8_t wholePartWidth = wholePart == 0 ? otherWidth+1 : 0;
    for (uint16_t i=wholePart; i>0; i/=10) {
        wholePartWidth += (i%10==1) ? oneWidth : otherWidth;
        wholePartWidth++;
    }

    Matrix.setFont(fontSize);
    Matrix.clear();
    // Write the whole part
    Matrix.text(MATRIX_ORANGE, xpos - wholePartWidth, ypos, String(s.toInt()/10));
    // Draw the decimal point
    Matrix.rectangle(MATRIX_ORANGE, xpos, ypos, 2, 2);
    // Write the fractional part (1 d.p.)
    Matrix.text(MATRIX_ORANGE, xpos+3, ypos, String(s.toInt()%10));
    // Write the units symbol (Watts)
    Matrix.text(MATRIX_ORANGE, xpos+12, ypos, "W");
    Matrix.paint();
}

