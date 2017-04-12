#include <MutilaDebug.h>
#include <stdint.h>
#include "VoltageAndCurrentMode.h"
#include "Matrix.h"

VoltageAndCurrentMode_ VoltageAndCurrentMode;

void VoltageAndCurrentMode_::start(const char* data)
{
    DB(F("VoltageAndCurrentMode::start "));
    DBLN(data);
    String s = data;
    // strip trailing '-' characters
    s.replace("-", "");

    Matrix.setFont(4);
    Matrix.clear();

    const uint8_t xpos = 17;

    Matrix.rtext(MATRIX_RED,       xpos,    12, String(s.substring(0, 2).toInt()));
    Matrix.rectangle(MATRIX_RED,   xpos+2,  12, 2, 2);
    Matrix.text(MATRIX_RED,        xpos+5,  12, s.substring(2, 3));
    Matrix.text(MATRIX_RED,        xpos+15, 12, "Volts");

    Matrix.rtext(MATRIX_GREEN,     xpos,    26, String(s.substring(4, 6).toInt()));
    Matrix.rectangle(MATRIX_GREEN, xpos+2,  26, 2, 2);
    Matrix.text(MATRIX_GREEN,      xpos+5,  26, s.substring(6, 7));
    Matrix.text(MATRIX_GREEN,      xpos+15, 26, "Amps");

    Matrix.paint();
}

