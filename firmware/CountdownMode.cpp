#include <MutilaDebug.h>
#include "CountdownMode.h"
#include "Matrix.h"

CountdownMode_ CountdownMode;

void CountdownMode_::start(const char* data)
{
    startMs = millis();
    n = String(data).toInt();
    DBLN(F("CountdownMode::start"));
    Matrix.clear();
    Matrix.paint();
    Matrix.setFont(5);
}

void CountdownMode_::update()
{
    unsigned long next = startMs + (i*2);
    if (millis() >= next && i < 18) {
        DB("updating, i=");
        DBLN(i);
        i+=1;
        Matrix.rectangle(MATRIX_RED, 17-i, 15+i, 30+(i*2), i*2);
        Matrix.rectangle(MATRIX_BLACK, 17-(i-3), 15+(i-3), 30+((i-3)*2), (i-3)*2);
        Matrix.text(MATRIX_ORANGE, 27, 23, String(n));
        Matrix.paint();
    }
}

