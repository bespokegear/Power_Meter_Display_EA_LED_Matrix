#pragma once

#include <Arduino.h>
#include <avr/pgmspace.h>
#include <stdint.h>
#include "DisplayMode.h"
#include "PLT1001.h"

/*! \class Matrix
 *
 * Display modes for Re-innovation projects
 * A display mode is a sequence of events which are used to animate an
 * effect. Effects which require events into the future (animations) are 
 * implemented using Mutils Modes.
 */

#define MATRIX_BLACK    0
#define MATRIX_RED      1
#define MATRIX_GREEN    2
#define MATRIX_ORANGE   3

class Matrix_ : public PLT1001 {
public:
    Matrix_();
    void begin(SoftwareSerial* ss, unsigned long baud=115200);
    void startMode(DisplayMode* newMode, const char* data=NULL);
    void update();

    // Information about character sizes
    uint8_t getCharacterHeight(const uint8_t fontSize);
    uint8_t getCharacterWidth(const uint8_t fontSize, const char c);
    uint8_t getGutterWidth(const uint8_t fontSize);
    uint8_t getStringWidth(const uint8_t, const char* s);
    uint8_t getStringWidth(const uint8_t, String s);

    // right justified text
	void rtext(uns8 color, uns16 x, uns16 y, const char* s);
	void rtext(uns8 color, uns16 x, uns16 y, String text);

private:
    DisplayMode* mode;

};

// Font heights for fonts size 1 through 7
const uint8_t CharacterHeight[] PROGMEM = {5, 5, 7, 8, 16, 16, 24};

// Fonts 1 through 7, ASCII codes 33 through 126, pixel widths for each character
const uint8_t CharacterWidth[] PROGMEM = {
      1, 3, 5, 5, 5, 5, 1, 2, 2, 5, 5, 2, 4, 2, 3, 4, 2, 4, 4, 4, 
      4, 4, 4, 4, 4, 1, 2, 3, 4, 3, 5, 5, 4, 4, 4, 4, 4, 4, 4, 4, 
      3, 4, 4, 4, 5, 5, 4, 4, 5, 4, 4, 5, 4, 5, 5, 5, 5, 5, 0, 0, 
      0, 0, 0, 0, 4, 4, 4, 4, 4, 4, 4, 4, 3, 4, 4, 4, 5, 5, 4, 4, 
      5, 4, 4, 5, 4, 5, 5, 5, 5, 5, 0, 0, 0, 0,

      1, 5, 5, 5, 5, 5, 2, 2, 2, 5, 5, 2, 4, 2, 5, 5, 2, 5, 5, 5, 
      5, 5, 4, 5, 5, 2, 2, 3, 4, 3, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 
      3, 5, 4, 4, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 0, 0, 
      0, 0, 0, 0, 5, 5, 5, 5, 5, 5, 5, 5, 3, 5, 4, 4, 5, 5, 5, 5, 
      5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 0, 0, 0, 0,

      1, 3, 5, 5, 5, 5, 2, 3, 3, 5, 5, 2, 5, 2, 5, 5, 3, 5, 5, 5, 
      5, 5, 5, 5, 5, 1, 2, 4, 5, 4, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 
      3, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 3, 0, 
      3, 0, 5, 0, 5, 5, 5, 5, 5, 4, 5, 4, 1, 3, 4, 3, 5, 4, 5, 5, 
      5, 4, 5, 4, 5, 5, 5, 5, 4, 4, 0, 0, 0, 0,

      1, 3, 5, 5, 5, 6, 2, 3, 3, 5, 5, 2, 5, 2, 5, 6, 3, 6, 6, 6, 
      6, 6, 6, 6, 6, 1, 2, 4, 5, 4, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 
      3, 4, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 6, 3, 0, 
      3, 0, 0, 0, 5, 4, 5, 4, 5, 4, 4, 4, 1, 3, 4, 2, 5, 4, 5, 4, 
      5, 4, 5, 4, 4, 5, 5, 5, 4, 4, 0, 0, 0, 0,

      2, 5, 10, 10, 10, 10, 4, 5, 5, 10, 10, 4, 8, 2, 10, 10, 8, 
      10, 10, 10, 10, 10, 10, 10, 10, 2, 4, 8, 8, 8, 10, 10, 10, 
      10, 10, 10, 10, 10, 10, 10, 6, 10, 10, 10, 10, 10, 10, 10, 
      10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 4, 10, 4, 4, 6, 2, 
      8, 8, 8, 8, 8, 6, 8, 8, 2, 4, 8, 6, 10, 8, 8, 8, 8, 6, 8, 
      7, 8, 8, 10, 8, 7, 8, 0, 0, 0, 10,

      3, 7, 12, 13, 13, 10, 4, 6, 6, 11, 11, 4, 8, 3, 13, 11, 9, 
      11, 11, 12, 11, 11, 11, 11, 11, 3, 4, 8, 8, 11, 10, 10, 11, 
      11, 11, 11, 11, 11, 11, 11, 7, 12, 12, 11, 12, 12, 11, 11, 
      12, 11, 11, 11, 11, 11, 12, 12, 11, 11, 5, 13, 5, 11, 10, 
      3, 8, 8, 8, 8, 8, 6, 8, 8, 2, 5, 8, 6, 10, 8, 8, 8, 8, 6, 
      8, 7, 8, 8, 10, 8, 7, 8, 0, 0, 0, 10,

      0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 13, 13, 13, 
      13, 13, 13, 13, 13, 13, 13, 2, 0, 0, 0, 0, 0, 0, 0, 0, 13, 
      0, 0, 12, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
      0, 0
};

// a global object called Matrix
extern Matrix_ Matrix;


