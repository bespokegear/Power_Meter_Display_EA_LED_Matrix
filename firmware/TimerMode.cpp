#include <MutilaDebug.h>
#include <stdint.h>
#include "TimerMode.h"
#include "Matrix.h"

TimerMode_ TimerMode;

void TimerMode_::start(const char* data)
{
    String s = data;
    // strip trailing '-' characters
    s.replace("-", "");

    // Work out width in pixels of whole part text
    // With font size 5, width of 1 is 8 pixels, 10 for other digits
    uint16_t wholePart = s.toInt()/10;
    uint8_t wholePartWidth = wholePart == 0 ? 10 : 0;
    for (uint16_t i=wholePart; i>0; i/=10) {
        wholePartWidth += (i%10==1) ? 8 : 10;
    }
    
    Matrix.setFont(5);
    Matrix.clear();
    // The whole part
    Matrix.text(MATRIX_RED, 40 - wholePartWidth, 24, String(s.toInt()/10));
    // Draw the decimal point
    Matrix.rectangle(MATRIX_RED, 43, 24, 2, 2);
    // The fractional part (1 d.p.)
    Matrix.text(MATRIX_RED, 46, 24, String(s.toInt()%10));
    Matrix.paint();
}

