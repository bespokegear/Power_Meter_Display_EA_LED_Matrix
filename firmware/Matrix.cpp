#include "Matrix.h"
#include "HelloMode.h"
#include <stdio.h>

Matrix_ Matrix;

Matrix_::Matrix_()
{
    mode = NULL;
}

void Matrix_::begin(SoftwareSerial* ss, unsigned long baud)
{
    PLT1001::begin(ss, baud);
    // Wait for display board to boot
    delay(1000);
    startMode(&HelloMode);
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

