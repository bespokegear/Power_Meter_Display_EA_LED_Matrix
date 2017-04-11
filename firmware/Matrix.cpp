#include "Matrix.h"
#include "ClearMode.h"
#include <stdio.h>

Matrix_ Matrix;

Matrix_::Matrix_()
{
    mode = NULL;
}

void Matrix_::begin(SoftwareSerial* ss, unsigned long baud)
{
    PLT1001::begin(ss, baud);
    startMode(&ClearMode);
}

void Matrix_::startMode(DisplayMode* newMode, const char* data)
{
    mode = newMode;
    mode->start(data);
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
        return pgm_read_byte_near(CharacterWidth+((fontSize-1)*94)+c-('A'-'!'+1));
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


