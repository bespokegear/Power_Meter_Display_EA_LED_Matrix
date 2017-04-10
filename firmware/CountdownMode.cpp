#include <MutilaDebug.h>
#include "CountdownMode.h"
#include "Matrix.h"

CountdownMode_ CountdownMode;

void CountdownMode_::start(const char* data)
{
    startMs = millis();
    n = String(data).toInt();
    i = 0;
    DB(F("CountdownMode::start "));
    DBLN(data);
    Matrix.clear();
    Matrix.paint();
    Matrix.setFont(5);
}

void CountdownMode_::update()
{
    unsigned long next = startMs + (i*2);
    if (millis() >= next && i < 18) {
        i+=1;
        Matrix.rectangle(MATRIX_RED, 17-i, 15+i, 30+(i*2), i*2);
        Matrix.rectangle(MATRIX_BLACK, 17-(i-3), 15+(i-3), 30+((i-3)*2), (i-3)*2);
        if (n==0) {
            Matrix.text(MATRIX_ORANGE, 20, 23, "Go!");
        } else {
            Matrix.text(MATRIX_ORANGE, 27, 23, String(n));
        }
        Matrix.paint();
    }
}

