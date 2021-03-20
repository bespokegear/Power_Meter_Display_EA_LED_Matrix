#include <MutilaDebug.h>
#include "WinnerMode.h"
#include "Matrix.h"
#include "Config.h"

WinnerMode_ WinnerMode;

void WinnerMode_::start(const char* data)
{
  DB(F("WinnerMode::start "));
  DBLN(data);
  Matrix.clear();
  Matrix.paint();
  // draw a pretty border - using lines on the larger display
  Matrix.line(MATRIX_RED, 64, 0, 127, 0);
  Matrix.line(MATRIX_RED, 64, 12, 127, 12);
  Matrix.line(MATRIX_RED, 64, 0, 64, 12);
  Matrix.line(MATRIX_RED, 127, 0, 127, 12);
  Matrix.line(MATRIX_GREEN, 65, 1, 126, 1);
  Matrix.line(MATRIX_GREEN, 65, 11, 126, 11);
  Matrix.line(MATRIX_GREEN, 65, 1, 65, 11);
  Matrix.line(MATRIX_GREEN, 126, 1, 126, 11);

  
//  uint8_t i;
//  i = 16; Matrix.rectangle(MATRIX_RED, 17 - i, 15 + i, 30 + (i * 2), i * 2);
//  i = 15; Matrix.rectangle(MATRIX_GREEN, 17 - i, 15 + i, 30 + (i * 2), i * 2);
//  i = 14; Matrix.rectangle(MATRIX_BLACK, 17 - i, 15 + i, 30 + (i * 2), i * 2);

  // draw "Winner:" with small font
  Matrix.setFont(2);
  Matrix.text(MATRIX_RED, 15 + 64, 8, "WINNER");
  // draw a colon with pixels - nicer than font colon
  Matrix.pixel(MATRIX_RED, 49 + 64, 5);
  Matrix.pixel(MATRIX_RED, 49 + 64, 7);

  // draw who is the winner in big text
  Matrix.setFont(6);
  switch (data[0]) {
    case '0':
      Matrix.ctext(MATRIX_ORANGE, (MATRIX_SIZE_X / 2) - 2, 15, "TIE!");
      break;
    case '1':
      Matrix.ctext(MATRIX_ORANGE, (MATRIX_SIZE_X / 2) - 2, 15, String(data[0]));
    case '2':
      Matrix.ctext(MATRIX_ORANGE, (MATRIX_SIZE_X / 2) - 2, 15, String(data[0]));
      break;
    case '3':
      Matrix.ctext(MATRIX_ORANGE, (MATRIX_SIZE_X / 2) - 2, 15, String(data[0]));
      break;
    case '4':
      Matrix.ctext(MATRIX_ORANGE, (MATRIX_SIZE_X / 2) - 2, 15, String(data[0]));
      break;
    default:
      DB(F("CountdownMode ERR bad param: "));
      DBLN(data);
      break;
  }
  Matrix.paint();
}
