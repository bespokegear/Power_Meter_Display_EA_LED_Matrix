#include <MutilaDebug.h>
#include "TextControlMode.h"
#include "TextMode.h"
#include "Matrix.h"

TextControlMode_ TextControlMode;

void TextControlMode_::start(const char* data)
{
    DBLN(F("TextControlMode::start"));
    switch(data[0]) {
    case 'F':
        TextMode.setFont(data[1]-'0');
        break;
    case 'C':
        char color;
        if (data[1] == 'R') {
            color = MATRIX_RED;
        } else if (data[1] == 'G') {
            color = MATRIX_GREEN;
        } else if (data[1] == 'O') {
            color = MATRIX_ORANGE;
        } else {
            color = data[1]-'0';
        }
        TextMode.setColor(color);
        break;
    case 'J':
        TextMode.setJustify(data[1]);
        break;
    default:
        DBLN(F("TextControlMode ERR cmd"));
        return;
    }
}
