#include "PLT1001Lite.h"
#include <stdio.h>
#include <MutilaDebug.h>
#include <string.h>

PLT1001Lite::PLT1001Lite(uint8_t rxPin, uint8_t txPin) :
    _serial(rxPin, txPin)
{
}

void PLT1001Lite::begin(long baud)
{
    _serial.begin(baud);
    setFont(8);
    clear(false);
    _serial.print(F("text "));
    _serial.print(COLOR_GREEN);
    _serial.print(F(" 10 29 \"Hello!\"\r"));
    paint();
}

void PLT1001Lite::update()
{
    // Does nothing at present
}

void PLT1001Lite::clear(bool now)
{
    _serial.print(F("clear\r"));
    if (now) paint();
}

void PLT1001Lite::paint()
{
    _serial.print(F("paint\r"));
}

void PLT1001Lite::setFont(uint8_t size)
{
    _serial.print(F("font "));
    _serial.print(size);
    _serial.print('\r');
}

