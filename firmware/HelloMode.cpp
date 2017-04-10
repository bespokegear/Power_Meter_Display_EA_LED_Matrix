#include <MutilaDebug.h>
#include "HelloMode.h"
#include "Matrix.h"

HelloMode_ HelloMode;

void HelloMode_::start(const char* data)
{
    DBLN(F("HelloMode::start"));
    Matrix.clear();
    Matrix.setFont(8);
    Matrix.text(MATRIX_GREEN, 10, 29, "Hello");
    Matrix.paint();
}

