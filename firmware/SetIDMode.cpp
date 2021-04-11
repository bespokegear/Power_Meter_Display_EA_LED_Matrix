#include <MutilaDebug.h>
#include <string.h>
#include "Matrix.h"
#include "SetIDMode.h"
#include "Parser.h"
#include "Settings.h"

SetIDMode_ SetIDMode;

void SetIDMode_::start(const char* data)
{
    DB(F("SetIDMode::start "));
    DBLN(data);
    Matrix.clear();
    Matrix.setFont(2);
    
    bool ok = ((data[0] >= 'A' && data[0] <= 'Z') || data[0] == '*') && ((data[1] >= 'A' && data[1] <= 'Z') || data[1] == '*') && strlen(data) == 2;
    Matrix.clear();
    Matrix.text(MATRIX_ORANGE, 80, 9, "Set ID");

    Matrix.setFont(6);
    if (ok) {
        DisplayID0.set(data[0]);
        DisplayID0.save();
        DisplayID1.set(data[1]);
        DisplayID1.save();
        Parser.setID(data[0], data[1]);
        Matrix.text(MATRIX_ORANGE, 16, 15, data);
    } else {
        Matrix.text(MATRIX_ORANGE, 16, 15, "Error");
    }
    Matrix.paint();
}
