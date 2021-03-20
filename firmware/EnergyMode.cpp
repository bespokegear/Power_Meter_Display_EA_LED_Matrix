#include <MutilaDebug.h>
#include <stdint.h>
#include <string.h>
#include "EnergyMode.h"
#include "Matrix.h"
#include "Config.h"
#include "Settings.h"

EnergyMode_ EnergyMode;

EnergyMode_::EnergyMode_()
{
    reset();
}

void EnergyMode_::reset()
{
  
}

void EnergyMode_::start(const char* data)
{
    DB(F("EnergyMode::start "));
    DBLN(data);
    String s = data;
    // strip trailing '-' characters
    s.replace("-", "");
    lastValue = s.toInt();
    drawValue();
}

void EnergyMode_::drawValue()
{
    Matrix.setFont(6);
    // Write the whole part
    Matrix.rtext(MATRIX_GREEN, 50+64, 15, String(lastValue/10));
    // Write the fractional part (1 d.p.)
    Matrix.setFont(4);
    Matrix.text(MATRIX_GREEN, 50+3+64, 15, "Wh");
}
