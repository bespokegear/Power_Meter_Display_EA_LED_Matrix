#include "PLT1001Lite.h"

PLT1001Lite::PLT1001Lite(uint8_t rxPin, uint8_t txPin) :
    _serial(rxPin, txPin)
{
}

void PLT1001Lite::begin(long baud)
{
    _serial.begin(baud);
    // TODO: explicitly set state of PLT1001 module
    // like current font and so on
}

void PLT1001Lite::power(int16_t tenthWatt, bool paint)
{
    // TODO
    string("[watts]");
}

void PLT1001Lite::voltageAndCurrent(int16_t tenthVolt, int16_t tenthAmp, bool paint)
{
    // TODO
    string("[volts amps]");
}

void PLT1001Lite::maxGraphPower(uint16_t tenthWatt, bool paint)
{
    // TODO
    string("[max watts graph]");
}

void PLT1001Lite::timer(int16_t hundredMillis, bool paint)
{
    // TODO
    string("[seconds]");
}

void PLT1001Lite::countdown(uint8_t n, bool paint)
{
    // TODO
    string("[countdown]");
}

void PLT1001Lite::winner(uint8_t team, bool paint)
{
    // TODO
    string("[winner]");
}

void PLT1001Lite::clear(bool paint)
{
    _serial.print(CMD_CLEAR);
    if (paint)
        _serial.print(CMD_PAINT);
}

void PLT1001Lite::string(const char *msg, bool paint)
{
    _serial.print(CMD_CLEAR);
    _serial.print(F("text 2 3 24 \""));
    _serial.print(msg);
    _serial.print(F("\"\r"));
    if (paint)
        _serial.print(CMD_PAINT);
}

