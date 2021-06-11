#pragma once

#include "DisplayMode.h"

class TextMode_ : public DisplayMode 
{
public:
    TextMode_();
    void start(const char* data);
    void setFont(uint8_t fontSize);
    // 'L', 'C', or 'R'
    void setJustify(char j);
    // MATRIX_RED, MATRIX_GREEN, MATRIX_ORANGE
    void setColor(uint8_t c);

private:
    uint8_t _fontSize;
    char _justify;
    uint8_t _color;

};

extern TextMode_ TextMode;
