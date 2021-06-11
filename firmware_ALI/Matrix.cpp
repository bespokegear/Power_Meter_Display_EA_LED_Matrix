#include <stdio.h>
#include "Matrix.h"
#include "Config.h"
#include "DumpConfigMode.h"

#include "LEDMatrix.h"
LEDMatrix led;

Matrix_ Matrix;

Matrix_::Matrix_()
{
  mode = NULL;
}

void Matrix_::begin()
{
  startMode(&DumpConfigMode);
}

void Matrix_::reset()
{
  led.clear();
  led.setBrightness(100);       // adjust brightness. i: min:0; max:100.
  //Matrix.title();
}

void Matrix_::clear()
{
  led.clear();
}

void Matrix_::paint()
{
  // This is no longer needed - it just happens!
}

void Matrix_::rectangle(uint16_t _x, uint16_t _y, uint16_t _w, uint16_t _h, uint8_t _colour)
{
  led.drawLine(_x, _y, _x, _y + _h, RED);        // draw a Line
  led.drawLine(_x, _y, _x + _w, _y, RED);      // draw a Line
  led.drawLine(_x + _w, _y, _x, _y + _h, RED);      // draw a Line
  led.drawLine(_x, _y + _h, _x + _w, _y + _h, RED);    // draw a Line
}

void Matrix_::line(uint16_t _x1, uint16_t _y1, uint16_t _x2, uint16_t _y2, uint8_t _colour)
{
  led.drawLine(_x1, _y1, _x2, _y2, RED);         // draw a Line
}

void Matrix_::pixel(uint16_t _x, uint16_t _y, uint8_t _colour)
{
  led.drawPoint(_x, _y, GREEN);     // Turn on a LED in LED Matrix
}

void Matrix_::text(uint8_t _colour, uint16_t _x, uint16_t _y, String _text)
{
  char _cString[_text.length()+1];
  strcpy(_cString, _text.c_str());
  Serial.print("Text:");
  Serial.print(_cString);
  Serial.print(" LEN:");
  Serial.println(_text.length());
  //void LEDMatrix::displayString(uint8_t x, uint8_t y, char* charString, uint8_t length, font_size font, led_color color)
  led.displayString(_x, _y, _cString, _text.length(), FONT_6X8, GREEN);       // display an string
}

void Matrix_::setFont(uint8_t _font_size)
{

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

void Matrix_::title()
{
  led.displayString(0, 10, "HELLO", 5, FONT_10X16, GREEN);
}

uint8_t Matrix_::getCharacterHeight(const uint8_t fontSize)
{
  if (fontSize < 1 || fontSize > 7) {
    return 0;
  } else {
    return pgm_read_byte_near(CharacterHeight + fontSize - 1);
  }
}

uint8_t Matrix_::getCharacterWidth(const uint8_t fontSize, const char c)
{
  if (fontSize < 1 || fontSize > 7) {
    return 0;
  } else {
    if (c == ' ') return 3;
    else return pgm_read_byte_near(CharacterWidth + ((fontSize - 1) * 94) + c - ('A' - '!' + 1));
  }
}

uint8_t Matrix_::getGutterWidth(const uint8_t fontSize)
{
  return fontSize == 7 ? 2 : 1;
}

uint8_t Matrix_::getStringWidth(const uint8_t fontSize, const char* s)
{
  uint8_t w = 0;
  for (uint8_t i = 0; s[i] != 0; i++) {
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
  for (uint8_t i = 0; i < s.length(); i++) {
    w += getCharacterWidth(fontSize, s[i]);
    w += getGutterWidth(fontSize);
  }
  if (w > 0)
    w -= getGutterWidth(fontSize);
  return w;
}

void Matrix_::rtext(uint8_t  color, uint16_t  x, uint16_t y, const char* s)
{
  //text(color, x - getStringWidth(_currentFont, s) + 1, y, s);
}

void Matrix_::rtext(uint8_t  color, uint16_t x, uint16_t y, String s)
{
  //text(color, x - getStringWidth(_currentFont, s) + 1, y, s);
}

void Matrix_::ctext(uint8_t  color, uint16_t x, uint16_t y, const char* s)
{
  //text(color, x - (getStringWidth(_currentFont, s) / 2) + 1, y, s);
}

void Matrix_::ctext(uint8_t  color, uint16_t x, uint16_t y, String s)
{
  //text(color, x - (getStringWidth(_currentFont, s) / 2) + 1, y, s);
}
