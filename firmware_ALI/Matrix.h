#pragma once

#include <Arduino.h>
#include <avr/pgmspace.h>
#include <stdint.h>
#include "DisplayMode.h"
// #include "PLT1001.h"
#include "LEDMatrix.h"


/*! \class Matrix

   Display modes for Re-innovation projects
   A display mode is a sequence of events which are used to animate an
   effect. Effects which require events into the future (animations) are
   implemented using Mutils Modes.
*/

#define MATRIX_BLACK    0
#define MATRIX_RED      1
#define MATRIX_GREEN    2
#define MATRIX_ORANGE   3


class Matrix_ {
  public:
    Matrix_();
    void begin();
    void reset();
    void clear();
    void paint();
    void startMode(DisplayMode* newMode, const char* data = NULL);
    void update();
    void title();
    void text(uint8_t my_colour, uint16_t my_x, uint16_t my_y, String my_text);
    void rectangle(uint16_t my_x, uint16_t my_y, uint16_t my_w, uint16_t my_h, uint8_t my_colour);
    void line(uint16_t my_x1, uint16_t my_y1, uint16_t my_x2, uint16_t my_y2, uint8_t my_colour);
    void pixel(uint16_t my_x, uint16_t my_y, uint8_t my_colour);

    void setFont(uint8_t my_font_size);

    // Information about character sizes
    uint8_t getCharacterHeight(const uint8_t fontSize);
    uint8_t getCharacterWidth(const uint8_t fontSize, const char c);
    uint8_t getGutterWidth(const uint8_t fontSize);
    uint8_t getStringWidth(const uint8_t, const char* s);
    uint8_t getStringWidth(const uint8_t, String s);

    //! Right justified text
    //! \param color text color
    //! \param x right-most x-pixel of text
    //! \param y bottom-most pixel of text
    //! \param s string to be printed. Note '|' or '\r' or '\n' embedded
    //!          in a s will force a new-line to be started at that point
    void rtext(uint8_t  color, uint16_t x, uint16_t y, const char* s);
    void rtext(uint8_t  color, uint16_t x, uint16_t y, String text);
    //! Center justified text
    //! \param color text color
    //! \param x center x-pixel of text
    //! \param y bottom-most pixel of font
    //! \param s string to be printed. Note '|' or '\r' or '\n' embedded
    //!          in a s will force a new-line to be started at that point
    void ctext(uint8_t color, uint16_t x, uint16_t y, const char* s);
    void ctext(uint8_t color, uint16_t x, uint16_t y, String text);

  private:
    DisplayMode* mode;

};

// Font heights for fonts size 1 through 7
const uint8_t CharacterHeight[] PROGMEM = {5, 5, 7, 8, 16, 16, 24};

// Fonts 1 through 7, ASCII codes 33 through 126, pixel widths for each character
const uint8_t CharacterWidth[] PROGMEM = {
  //  font 1
  //  !   "   #   $   %   &   '   (   )   *   +   ,   -   .   /   0   1   2   3   4
  1,  3,  5,  5,  5,  5,  1,  2,  2,  5,  5,  2,  4,  2,  3,  4,  2,  4,  4,  4,
  //  5   6   7   8   9   :   ;   <   =   >   ?   @   A   B   C   D   E   F   G   H
  4,  4,  4,  4,  4,  1,  2,  3,  4,  3,  5,  5,  4,  4,  4,  4,  4,  4,  4,  4,
  //  I   J   K   L   M   N   O   P   Q   R   S   T   U   V   W   X   Y   Z   [   backslash
  3,  4,  4,  4,  5,  5,  4,  4,  5,  4,  4,  5,  4,  5,  5,  5,  5,  5,  0,  0,
  //  ]   ^   _   `   a   b   c   d   e   f   g   h   i   j   k   l   m   n   o   p
  0,  0,  0,  0,  4,  4,  4,  4,  4,  4,  4,  4,  3,  4,  4,  4,  5,  5,  4,  4,
  //  q   r   s   t   u   v   w   x   y   z   {   |   }   ~
  5,  4,  4,  5,  4,  5,  5,  5,  5,  5,  0,  0,  0,  0,

  //  font 2
  //  !   "   #   $   %   &   '   (   )   *   +   ,   -   .   /   0   1   2   3   4
  1,  5,  5,  5,  5,  5,  2,  2,  2,  5,  5,  2,  4,  2,  5,  5,  2,  5,  5,  5,
  //  5   6   7   8   9   :   ;   <   =   >   ?   @   A   B   C   D   E   F   G   H
  5,  5,  4,  5,  5,  2,  2,  3,  4,  3,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,
  //  I   J   K   L   M   N   O   P   Q   R   S   T   U   V   W   X   Y   Z   [   backslash
  3,  5,  4,  4,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  0,  0,
  //  ]   ^   _   `   a   b   c   d   e   f   g   h   i   j   k   l   m   n   o   p
  0,  0,  0,  0,  5,  5,  5,  5,  5,  5,  5,  5,  3,  5,  4,  4,  5,  5,  5,  5,
  //  q   r   s   t   u   v   w   x   y   z   {   |   }   ~
  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  0,  0,  0,  0,

  //  font 3
  //  !   "   #   $   %   &   '   (   )   *   +   ,   -   .   /   0   1   2   3   4
  1,  3,  5,  5,  5,  5,  2,  3,  3,  5,  5,  2,  5,  2,  5,  5,  3,  5,  5,  5,
  //  5   6   7   8   9   :   ;   <   =   >   ?   @   A   B   C   D   E   F   G   H
  5,  5,  5,  5,  5,  1,  2,  4,  5,  4,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,
  //  I   J   K   L   M   N   O   P   Q   R   S   T   U   V   W   X   Y   Z   [   backslash
  3,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  3,  0,
  //  ]   ^   _   `   a   b   c   d   e   f   g   h   i   j   k   l   m   n   o   p
  3,  0,  5,  0,  5,  5,  5,  5,  5,  4,  5,  4,  1,  3,  4,  3,  5,  4,  5,  5,
  //  q   r   s   t   u   v   w   x   y   z   {   |   }   ~
  5,  4,  5,  4,  5,  5,  5,  5,  4,  4,  0,  0,  0,  0,

  //  font 4
  //  !   "   #   $   %   &   '   (   )   *   +   ,   -   .   /   0   1   2   3   4
  1,  3,  5,  5,  5,  6,  2,  3,  3,  5,  5,  2,  5,  2,  5,  6,  3,  6,  6,  6,
  //  5   6   7   8   9   :   ;   <   =   >   ?   @   A   B   C   D   E   F   G   H
  6,  6,  6,  6,  6,  1,  2,  4,  5,  4,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,
  //  I   J   K   L   M   N   O   P   Q   R   S   T   U   V   W   X   Y   Z   [   backslash
  3,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  6,  3,  0,
  //  ]   ^   _   `   a   b   c   d   e   f   g   h   i   j   k   l   m   n   o   p
  3,  0,  0,  0,  5,  4,  5,  4,  5,  4,  4,  4,  1,  3,  4,  2,  5,  4,  5,  4,
  //  q   r   s   t   u   v   w   x   y   z   {   |   }   ~
  5,  4,  5,  4,  4,  5,  5,  5,  4,  4,  0,  0,  0,  0,

  //  font 5
  //  !   "   #   $   %   &   '   (   )   *   +   ,   -   .   /   0   1   2   3   4
  2,  5,  10, 10, 10, 10, 4,  5,  5,  10, 10, 4,  8,  2,  10, 10, 8,  10, 10, 10,
  //  5   6   7   8   9   :   ;   <   =   >   ?   @   A   B   C   D   E   F   G   H
  10, 10, 10, 10, 10, 2,  4,  9,  8,  9,  10, 10, 10, 10, 10, 10, 10, 10, 10, 10,
  //  I   J   K   L   M   N   O   P   Q   R   S   T   U   V   W   X   Y   Z   [   backslash
  6,  10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 4,  10,
  //  ]   ^   _   `   a   b   c   d   e   f   g   h   i   j   k   l   m   n   o   p
  4,  4,  6,  2,  8,  8,  8,  8,  8,  6,  8,  8,  2,  5,  8,  6,  10, 8,  8,  8,
  //  q   r   s   t   u   v   w   x   y   z   {   |   }   ~
  8,  6,  8,  7,  8,  8,  10, 8,  7,  8,  0,  0,  0,  10,

  //  font 6
  //  !   "   #   $   %   &   '   (   )   *   +   ,   -   .   /   0   1   2   3   4
  3,  7,  12, 13, 13, 10, 4,  6,  6,  11, 11, 4,  8,  3,  13, 11, 9,  11, 11, 12,
  //  5   6   7   8   9   :   ;   <   =   >   ?   @   A   B   C   D   E   F   G   H
  11, 11, 11, 11, 11, 3,  4, 11,  8,  11, 10, 10, 11, 11, 11, 11, 11, 11, 11, 11,
  //  I   J   K   L   M   N   O   P   Q   R   S   T   U   V   W   X   Y   Z   [   backslash
  7,  12, 12, 11, 12, 12, 11, 11, 12, 11, 11, 11, 11, 11, 12, 12, 11, 11, 5,  13,
  //  ]   ^   _   `   a   b   c   d   e   f   g   h   i   j   k   l   m   n   o   p
  5,  11, 10, 3,  8,  8,  8,  8,  8,  6,  8,  8,  2,  5,  8,  6,  10, 8,  8,  8,
  //  q   r   s   t   u   v   w   x   y   z   {   |   }   ~
  8,  6,  8,  7,  8,  8,  10, 8,  7,  8,  0,  0,  0,  10,

  //  font 7
  //  !   "   #   $   %   &   '   (   )   *   +   ,   -   .   /   0   1   2   3   4
  0,  0,  0,  0,  0,  0,  3,  0,  0,  0,  0,  0,  0,  0,  0,  13, 13, 13, 13, 13,
  //  5   6   7   8   9   :   ;   <   =   >   ?   @   A   B   C   D   E   F   G   H
  13, 13, 13, 13, 13, 2,  0,  0,  0,  0,  0,  0,  0,  0,  13, 0,  0,  12, 0,  0,
  //  I   J   K   L   M   N   O   P   Q   R   S   T   U   V   W   X   Y   Z   [   backslash
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  //  ]   ^   _   `   a   b   c   d   e   f   g   h   i   j   k   l   m   n   o   p
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  //  q   r   s   t   u   v   w   x   y   z   {   |   }   ~
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0
};

// a global object called Matrix
extern Matrix_ Matrix;
