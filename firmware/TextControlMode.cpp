#include <MutilaDebug.h>
#include "TextControlMode.h"
#include "TextMode.h"

TextControlMode_ TextControlMode;

void TextControlMode_::start(const char* data)
{
    DBLN(F("TextControlMode::start"));
    switch(data[0]) {
    case 'F':
        TextMode.setFont(data[1]-'0');
        break;
    case 'C':
        TextMode.setColor(data[1]-'0');
        break;
    case 'J':
        TextMode.setJustify(data[1]);
        break;
    default:
        DBLN(F("TextControlMode ERR cmd"));
        return;
    }
}

