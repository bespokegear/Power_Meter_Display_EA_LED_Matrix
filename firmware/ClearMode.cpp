#include <MutilaDebug.h>
#include "ClearMode.h"
#include "Matrix.h"

ClearMode_ ClearMode;

void ClearMode_::start(const char* data)
{
    DBLN(F("ClearMode::start"));
    Matrix.reset(); // Reset the matrix if we get this command
    Matrix.clear();
    Matrix.paint();
}
