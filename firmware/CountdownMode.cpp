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
    Matrix.setFont(6);
}

void CountdownMode_::update()
{
    unsigned long next = startMs + (i*2);
    if (millis() >= next && i < 18) {
        uint8_t bgCol[2] = { MATRIX_GREEN, MATRIX_RED };
        uint8_t txCol[2] = { MATRIX_RED, MATRIX_GREEN };
        uint8_t colIdx = n % 2;
        i+=1;
        Matrix.rectangle(bgCol[colIdx], 17-i, 15+i, 30+(i*2), i*2);

        // At the end of the sequence, draw the text
        if (i==17) {
            if (n==0) {
                Matrix.text(txCol[colIdx], 18, 23, "GO!");
            } else if (n==1) {
                // digit '1' is narrower than other digits
                Matrix.text(txCol[colIdx], 28, 23, String(n));
            } else {
                Matrix.text(txCol[colIdx], 27, 23, String(n));
            }
        }
        Matrix.paint();
    }
}

