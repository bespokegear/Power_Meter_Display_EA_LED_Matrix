#pragma once

/*! \class PLT1001Lite
 * \brief Lightweight class to control the PLT1001 matrix display from Embedded Adventures
 * Uses a SoftwareSerial object to communicate with the device
 */

#include <stdint.h>
#include <Arduino.h>
#include <SoftwareSerial.h>

#define COLOR_RED       1
#define COLOR_GREEN     2
#define COLOR_ORANGE    3

class PLT1001Lite {
public:
    PLT1001Lite(uint8_t rxPin, uint8_t txPin);
    void begin(long baud=115200);
    void update();

    void timer(const char* data);

    void clear(bool now=true);
    void paint();
    void setFont(uint8_t size);

    // Weird to have this public, but it makes porting code way quicker
    SoftwareSerial _serial;

private:
    void displayTime(const char* data, int startx, int starty, int timerColour);

};

