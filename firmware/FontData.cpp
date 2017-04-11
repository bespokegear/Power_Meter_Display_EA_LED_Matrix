#include "FontData.h"

uint8_t getCharacterHeight(uint8_t fontSize)
{
    if (fontSize < 1 || fontSize > 7) {
        return 0;
    } else {
        return pgm_read_byte_near(CharacterHeight+fontSize-1);
    }
}

uint8_t getCharacterWidth(uint8_t fontSize, char c)
{
    if (fontSize < 1 || fontSize > 7) {
        return 0;
    } else {
        return pgm_read_byte_near((CharacterHeight+(fontSize-1)*94)+c-33);
    }
}

