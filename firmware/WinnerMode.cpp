#include <MutilaDebug.h>
#include "WinnerMode.h"
#include "Matrix.h"

WinnerMode_ WinnerMode;

void WinnerMode_::start(const char* data)
{
    DB(F("WinnerMode::start "));
    DBLN(data);
    Matrix.clear();
    Matrix.paint();
    // draw a pretty border
    uint8_t i;
    i=16; Matrix.rectangle(MATRIX_RED, 17-i, 15+i, 30+(i*2), i*2);
    i=15; Matrix.rectangle(MATRIX_GREEN, 17-i, 15+i, 30+(i*2), i*2);
    i=14; Matrix.rectangle(MATRIX_BLACK, 17-i, 15+i, 30+(i*2), i*2);

    // draw "Winner:" with small font
    Matrix.setFont(2);
    Matrix.text(MATRIX_RED, 15, 8, "WINNER");
    // draw a colon with pixels - nicer than font colon
    Matrix.pixel(MATRIX_RED, 49, 5);
    Matrix.pixel(MATRIX_RED, 49, 7);
    
    // draw who is the winner in big text
    Matrix.setFont(5);
    switch(data[0]) {
    case '0':
        Matrix.text(MATRIX_ORANGE, 16, 27, "TIE!");
        break;
    case '1':
        Matrix.text(MATRIX_ORANGE, 29, 27, String(data[0]));
    case '2':
        Matrix.text(MATRIX_ORANGE, 27, 27, String(data[0]));
        break;
    default:
        DB(F("CountdownMode ERR bad param: "));
        DBLN(data);
        break;
    }
    Matrix.paint();
}

