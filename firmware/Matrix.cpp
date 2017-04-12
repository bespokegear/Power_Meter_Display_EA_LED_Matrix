#include <stdio.h>
#include "Matrix.h"
#include "Config.h"
#include "DumpConfigMode.h"

Matrix_ Matrix;

Matrix_::Matrix_()
{
    mode = NULL;
}

void Matrix_::begin(SoftwareSerial* ss, unsigned long baud)
{
    PLT1001::begin(ss, baud);
    startMode(&DumpConfigMode);
}

void Matrix_::startMode(DisplayMode* newMode, const char* data)
{
    mode = newMode;
    if (newMode != NULL) {
        mode->start(data);
    }
}

void Matrix_::update()
{
    if (mode != NULL) {
        mode->update();
    }
}

uint8_t Matrix_::getCharacterHeight(const uint8_t fontSize)
{
    if (fontSize < 1 || fontSize > 7) {
        return 0;
    } else {
        return pgm_read_byte_near(CharacterHeight+fontSize-1);
    }
}

uint8_t Matrix_::getCharacterWidth(const uint8_t fontSize, const char c)
{
    if (fontSize < 1 || fontSize > 7) {
        return 0;
    } else {
        if (c == ' ') return 3;
        else return pgm_read_byte_near(CharacterWidth+((fontSize-1)*94)+c-('A'-'!'+1));
    }
}

uint8_t Matrix_::getGutterWidth(const uint8_t fontSize)
{
    return fontSize == 7 ? 2 : 1;
}

uint8_t Matrix_::getStringWidth(const uint8_t fontSize, const char* s)
{
    uint8_t w = 0;
    for (uint8_t i=0; s[i]!=0; i++) {
        w += getCharacterWidth(fontSize, s[i]);
        w += getGutterWidth(fontSize);
    }
    if (w > 0) 
        w -= getGutterWidth(fontSize);
    return w;
}

uint8_t Matrix_::getStringWidth(const uint8_t fontSize, String s)
{
    uint8_t w = 0;
    for (uint8_t i=0; i<s.length(); i++) {
        w += getCharacterWidth(fontSize, s[i]);
        w += getGutterWidth(fontSize);
    }
    if (w > 0) 
        w -= getGutterWidth(fontSize);
    return w;
}

void Matrix_::rtext(uns8 color, uns16 x, uns16 y, const char* s)
{
    text(color, x-getStringWidth(_currentFont, s)+1, y, s);
}

void Matrix_::rtext(uns8 color, uns16 x, uns16 y, String s)
{
    text(color, x-getStringWidth(_currentFont, s)+1, y, s);
}

