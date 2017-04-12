#include <MutilaDebug.h>
#include <stdint.h>
#include "TextMode.h"
#include "Matrix.h"
#include "Config.h"

TextMode_ TextMode;

TextMode_::TextMode_() :
    _fontSize(1),
    _justify('L'),
    _color(MATRIX_ORANGE)
{
}

void TextMode_::start(const char* data)
{
    DB(F("TextMode::start "));
    DBLN(data);
    // min line height = 5 + 1 line of gutter
    // display height is 32, therefore max possible lines = 32/6 = 5.33..
    String lineText[5];

    Matrix.clear();
    Matrix.setFont(_fontSize);

    // But this actually depends on the font
    uint8_t maxLines = Matrix.getCharacterHeight(MATRIX_SIZE_Y / (_fontSize+1));
    uint8_t lines = 0;
    int8_t lastSpace = -1; // The last space character in the current line
    for (uint8_t i=0; data[i]!=0; i++) {
        if (data[i] == '\r' || data[i] == '\n' || data[i] == '|') {  
            // explicit line break
            if (lines == 4) {
                // if we're out of lines, stop adding more letters
                break;
            } else {
                // continue on next line
                lines++;
                lastSpace = -1;
                continue;
            }
        } else if (Matrix.getStringWidth(_fontSize, lineText[lines]) + Matrix.getCharacterWidth(_fontSize, data[i]) >= MATRIX_SIZE_X) {
            // word wrap
            if (lines == 4) {
                // if we're out of lines, stop adding more letters
                break;
            }
            if (lastSpace>=0) {
                // 1. copy last partial word from the end of this line to the next line
                lineText[lines+1] = lineText[lines].substring(lastSpace+1);
                // 2. remove last partial word from end of this line
                lineText[lines].remove(lastSpace);
                lastSpace = -1;
            }
            // 3. text from here goes on next line
            lines++;
        }
        lineText[lines] += data[i];
        if (data[i] == ' ') {
            lastSpace = lineText[lines].length() - 1;
            DB("Current line=\"");
            DB(lineText[lines]);
            DB("\" lastSpace idx=");
            DBLN(lastSpace);
        }
    }
    lines++;

    // Work out where to put it
    uint8_t ysize = lines * (1 + Matrix.getCharacterHeight(_fontSize));
    uint8_t ybottom = MATRIX_SIZE_Y - ((MATRIX_SIZE_Y - ysize)/2);
    for (uint8_t i = 0; i<lines; i++) {
        uint8_t ypos = ybottom - ((1 + Matrix.getCharacterHeight(_fontSize))*(lines-i-1));
        switch(_justify) {
        case 'R':
            Matrix.rtext(_color, MATRIX_SIZE_X-1, ypos, lineText[i]);
            break;
        default:
            Matrix.text(_color, 0, ypos, lineText[i]);
            break;
        }
    }
    Matrix.paint();
    
}

void TextMode_::setFont(uint8_t fontSize)
{
    if (fontSize >= 1 && fontSize <= 7) {
        _fontSize = fontSize;
    } else {
        DB(F("TextMode::setFont bad size "));
        DBLN(fontSize);
    }
}

void TextMode_::setJustify(char j)
{
    if (j != 'L' && j != 'C' && j != 'R') {
        DB(F("TextMode::setJustify bad j "));
        DBLN(j);
    } else {
        _justify = j;
    }
}

void TextMode_::setColor(uint8_t c)
{
    _color = c;
}

