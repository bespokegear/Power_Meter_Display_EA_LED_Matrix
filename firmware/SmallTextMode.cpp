#include <MutilaDebug.h>
#include <stdint.h>
#include "SmallTextMode.h"
#include "Matrix.h"

SmallTextMode_ SmallTextMode;

void SmallTextMode_::start(const char* data)
{
    DB(F("SmallTextMode::start "));
    DBLN(data);
    String line[3];
    uint8_t lineCount = 1;
    for (uint8_t i=0; i<strlen(data); i++) {
        DB("in: ");
        DB(data[i]);
        DB(" (int)");
        DBLN((int)data[i]);
        if (data[i] == '\r' && lineCount<3) {
            DBLN("newline");
            lineCount++;
            continue;
        }
        DB("append:");
        DB(data[i]);
        DB(" (int)");
        DBLN((int)data[i]);
        line[lineCount-1] += data[i];
    }
    DB("lines=");
    DBLN(lineCount);
    for (uint8_t i=0; i<lineCount; i++) {
        DB(i);
        DB('=');
        DBLN(line[i]);
    }
    
    uint8_t y;
    if (lineCount == 1) y = 18;
    else if (lineCount == 2) y = 14;
    else y = 10;

    Matrix.clear();
    Matrix.setFont(2);
    for (uint8_t i=0; i<lineCount; i++) {
        Matrix.text(MATRIX_ORANGE, 1, y, line[i]);
        y += 8;
    }
    Matrix.paint();
}

