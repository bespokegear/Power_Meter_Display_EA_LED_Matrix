#include <MutilaDebug.h>
#include <stdint.h>
#include "TimerMode.h"
#include "Matrix.h"

TimerMode_ TimerMode;

void TimerMode_::start(const char* data)
{
    DB(F("TimerMode::start "));
    DBLN(data);
    // Some constants we might want to tweak
    const uint8_t fontSize = 5;
    const uint8_t oneWidth = 8;
    const uint8_t otherWidth = 10;
    const uint8_t ypos = 15;
    const uint8_t xpos = 41; // the position of the decimal point
    
    String s = data;
    // strip trailing '-' characters
    s.replace("-", "");

    // Work out width in pixels of whole part text
    uint16_t wholePart = s.toInt()/10;
    uint8_t wholePartWidth = wholePart == 0 ? otherWidth+1 : 0;
    for (uint16_t i=wholePart; i>0; i/=10) {
        wholePartWidth += (i%10==1) ? oneWidth : otherWidth;
        wholePartWidth++;
    }
    Matrix.setFont(4);
    Matrix.clear();
    Matrix.text(MATRIX_RED, 84, 11, "TIME:");
    Matrix.setFont(fontSize);
    // Write the whole part
    Matrix.text(MATRIX_RED, xpos - wholePartWidth, ypos, String(s.toInt()/10));
    // Draw the decimal point
    Matrix.rectangle(MATRIX_RED, xpos, ypos, 2, 2);
    // Write the fractional part (1 d.p.)
    Matrix.text(MATRIX_RED, xpos+3, ypos, String(s.toInt()%10));
    Matrix.paint();
}
