#include "PLT1001Lite.h"
#include <stdio.h>
#include <MutilaDebug.h>

PLT1001Lite::PLT1001Lite(uint8_t rxPin, uint8_t txPin) :
    _serial(rxPin, txPin)
{
}

void PLT1001Lite::begin(long baud)
{
    _serial.begin(baud);
    clearBuf();
    // TODO: explicitly set state of PLT1001 module
    // like current font and so on
}

void PLT1001Lite::update()
{
}

void PLT1001Lite::power(int16_t tenthWatt, bool paint)
{
    // TODO
    DB(F("power="));
    DB(tenthWatt);
    DBLN(F("/10 W"));
    clearBuf();
    snprintf(_buf, STR_BUF_LEN, "%d.%d Watts", tenthWatt/10, tenthWatt%10); 
    string(_buf);
}

void PLT1001Lite::voltageAndCurrent(int16_t tenthVolt, int16_t tenthAmp, bool paint)
{
    // TODO
    DB(F("volt="));
    DB(tenthVolt);
    DB(F("/10 V, amp="));
    DB(tenthAmp);
    DBLN(F("/10 A"));
    clearBuf();
    snprintf(_buf, STR_BUF_LEN, 
             "%d.%dV %d.%dA", 
                tenthVolt/10, tenthVolt%10,
                tenthAmp/10, tenthAmp%10); 
    string(_buf);
}

void PLT1001Lite::maxGraphPower(uint16_t tenthWatt, bool paint)
{
    // TODO
    string("(max watts graph)");
}

void PLT1001Lite::timer(int16_t hundredMillis, bool paint)
{
    // TODO
    string("(seconds)");
}

void PLT1001Lite::countdown(uint8_t n, bool paint)
{
    // TODO
    string("(countdown)");
}

void PLT1001Lite::winner(uint8_t team, bool paint)
{
    // TODO
    string("(winner)");
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

void PLT1001Lite::clearBuf()
{
    memset(_buf, 0, STR_BUF_LEN);
    _bufPtr = _buf;
}

