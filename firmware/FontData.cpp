#include "FontData.h"

uint8_t getCharacterHeight(const uint8_t fontSize)
{
    if (fontSize < 1 || fontSize > 7) {
        return 0;
    } else {
        return pgm_read_byte_near(CharacterHeight+fontSize-1);
    }
}

uint8_t getCharacterWidth(const uint8_t fontSize, const char c)
{
    if (fontSize < 1 || fontSize > 7) {
        return 0;
    } else {
        return pgm_read_byte_near(CharacterWidth+((fontSize-1)*94)+c-('A'-'!'+1));
    }
}

uint8_t getGutterWidth(const uint8_t fontSize)
{
    return fontSize == 7 ? 2 : 1;
}

uint8_t getStringWidth(const uint8_t fontSize, const char* s)
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


