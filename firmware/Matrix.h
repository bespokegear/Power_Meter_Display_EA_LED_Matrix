#pragma once

#include "DisplayMode.h"
#include "PLT1001.h"

/*! \class Matrix
 *
 * Display modes for Re-innovation projects
 * A display mode is a sequence of events which are used to animate an
 * effect. Effects which require events into the future (animations) are 
 * implemented using Mutils Modes.
 */

#define MATRIX_BLACK    0
#define MATRIX_RED      1
#define MATRIX_GREEN    2
#define MATRIX_ORANGE   3

class Matrix_ : public PLT1001 {
public:
    Matrix_();
    void begin(SoftwareSerial* ss, unsigned long baud=115200);
    void startMode(DisplayMode* newMode, const char* data=NULL);
    void update();

private:
    DisplayMode* mode;

};

// a global object called Matrix
extern Matrix_ Matrix;


