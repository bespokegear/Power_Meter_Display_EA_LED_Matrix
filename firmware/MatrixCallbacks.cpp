#include <stdio.h>
#include <stdint.h>
#include <MutilaDebug.h>
#include "TimerMode.h"
#include "ClearMode.h"
#include "CountdownMode.h"
#include "Matrix.h"

void cbPower(const char* data)
{
    DBLN("cbPower");
}

void cbVoltageAndCurrent(const char* data)
{
    DBLN("cbVoltageAndCurrent");
}

void cbTimer(const char* data)
{
    DBLN("cbTimer");
    Matrix.startMode(&TimerMode, data);
}

void cbClear(const char* data)
{
    DBLN("cbClear");
    Matrix.startMode(&ClearMode, data);
}

void cbMaxGraphPower(const char* data)
{
    DBLN("cbMaxGraphPower");
}

void cbString(const char* data)
{
    DBLN("cbString");
}

void cbWinner(const char* data)
{
    DBLN("cbWinner");
}

void cbCountdown(const char* data)
{
    DBLN("cbCountdown");
    Matrix.startMode(&CountdownMode, data);
}


