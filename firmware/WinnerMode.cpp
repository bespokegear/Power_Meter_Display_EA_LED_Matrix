#include <MutilaDebug.h>
#include "WinnerMode.h"
#include "Matrix.h"

WinnerMode_ WinnerMode;

void WinnerMode_::start(const char* data)
{
    DBLN(F("WinnerMode::start"));
    Matrix.clear();
    Matrix.paint();
    // draw a pretty border
    uint8_t i;
    i=16; Matrix.rectangle(MATRIX_RED, 17-i, 15+i, 30+(i*2), i*2);
    i=15; Matrix.rectangle(MATRIX_GREEN, 17-i, 15+i, 30+(i*2), i*2);
    i=14; Matrix.rectangle(MATRIX_BLACK, 17-i, 15+i, 30+(i*2), i*2);

    // draw "Winner:" with small font
    Matrix.setFont(2);
    Matrix.text(MATRIX_ORANGE, 16, 8, "WINNER");
    
    // draw who is the winner in big text
    Matrix.setFont(5);
    switch(data[0]) {
    case '0':
        Matrix.text(MATRIX_ORANGE, 15, 26, "TIE!");
        break;
    case '1':
        Matrix.text(MATRIX_ORANGE, 29, 26, String(data[0]));
    case '2':
        Matrix.text(MATRIX_ORANGE, 27, 26, String(data[0]));
        break;
    default:
        DB(F("CountdownMode ERR bad param: "));
        DBLN(data);
        break;
    }
    Matrix.paint();
}

