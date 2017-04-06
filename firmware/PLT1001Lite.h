#pragma once

/*! \class PLT1001Lite
 * \brief Lightweight class to control the PLT1001 matrix display from Embedded Adventures
 * Uses a SoftwareSerial object to communicate with the device
 */

#include <stdint.h>
#include <Arduino.h>
#include <SoftwareSerial.h>

#define CMD_CLEAR       F("clear\r")
#define CMD_PAINT       F("paint\r")

class PLT1001Lite {
public:
    PLT1001Lite(uint8_t rxPin, uint8_t txPin);
    void begin(long baud=115200);

    // aXXP????-
    void power(int16_t tenthWatt, bool paint=true);
    // aXXMGP????--
    void maxGraphPower(uint16_t tenthWatt, bool paint=true);
    // aXXVDDDIDDDD
    void voltageAndCurrent(int16_t tenthVolt, int16_t tenthAmp, bool paint=true);
    // aXXT????-
    void timer(int16_t hundredMillis, bool paint=true);
    // aXXCD?------
    void countdown(uint8_t n, bool paint=true);
    // aXXWN?------
    void winner(uint8_t team, bool paint=true);
    // aXXCL-------
    void clear(bool paint=true);
    // aXXST------------
    void string(const char* buf, bool paint=true);

    // Ideas:
    // Long scrolling messages

private:
    SoftwareSerial _serial;

};

