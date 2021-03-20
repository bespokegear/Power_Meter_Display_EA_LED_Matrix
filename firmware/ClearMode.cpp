#include <MutilaDebug.h>
#include "ClearMode.h"
#include "Matrix.h"

ClearMode_ ClearMode;

void ClearMode_::start(const char* data)
{
    DBLN(F("ClearMode::start"));
    Matrix.clear();
    Matrix.paint();
}
