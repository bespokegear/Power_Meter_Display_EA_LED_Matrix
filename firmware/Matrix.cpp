#include "Matrix.h"
#include "ClearMode.h"
#include <stdio.h>

Matrix_ Matrix;

Matrix_::Matrix_()
{
    mode = NULL;
}

void Matrix_::begin(SoftwareSerial* ss, unsigned long baud)
{
    PLT1001::begin(ss, baud);
    startMode(&ClearMode);
}

void Matrix_::startMode(DisplayMode* newMode, const char* data)
{
    mode = newMode;
    mode->start(data);
}

void Matrix_::update()
{
    if (mode != NULL) {
        mode->update();
    }
}

