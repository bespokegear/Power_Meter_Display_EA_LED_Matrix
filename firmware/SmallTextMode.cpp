#include <MutilaDebug.h>
#include <stdint.h>
#include "SmallTextMode.h"
#include "Matrix.h"

SmallTextMode_ SmallTextMode;

void SmallTextMode_::start(const char* data)
{
  DB(F("SmallTextMode::start "));
  DBLN(data);
  String line[4];
  uint8_t lineCount = 1;
  for (uint8_t i = 0; i < strlen(data); i++) {
    DB(F("in: "));
    DB(data[i]);
    DB(F(" (int)"));
    DBLN((int)data[i]);
    if ((data[i] == '\r' || data[i] == '\n' || data[i] == '|') && lineCount < 4) {
      DBLN(F("newline"));
      lineCount++;
      continue;
    }
    DB(F("append:"));
    DB(data[i]);
    DB(F(" (int)"));
    DBLN((int)data[i]);
    line[lineCount - 1] += data[i];
  }
  DB(F("lines="));
  DBLN(lineCount);
  for (uint8_t i = 0; i < lineCount; i++) {
    DB(i);
    DB('=');
    DBLN(line[i]);
  }

  uint8_t y, x;
  Matrix.clear();
  Matrix.setFont(2);
  for (uint8_t i = 0; i < lineCount; i++) {
    if (i == 0) {
      y = 6;
      x = 72;
    }
    else if (i == 1) {
      y = 14;
      x = 72;
    }
    else if (i == 2) {
      y = 6;
      x = 8;
    }
    else {
      y = 14;
      x = 8;
    };
    Matrix.text(MATRIX_ORANGE, x, y, line[i]);
  }
  Matrix.paint();
}
