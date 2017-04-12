#include <Arduino.h>
#include <SoftwareSerial.h>
#include <EEPROM.h>
#include <Heartbeat.h>
#include <MutilaDebug.h>
#include <stdint.h>

#include "Config.h"
#include "Matrix.h"
#include "Parser.h"
#include "Settings.h"

SoftwareSerial softSerial(SW_SERIAL_RX, SW_SERIAL_TX);
Heartbeat heartbeat(HEARTBEAT_PIN);

void setup() {
	Serial.begin(115200);
    heartbeat.begin();
    // Parser should be initialize before Matrix so that when Matrix
    // Enters DumpConfigMode, Parser wil already know it's ID
    Parser.begin();
    Matrix.begin(&softSerial);
    DBLN(F("setup:E"));
}

void loop () {
    heartbeat.update();
    Matrix.update();
    Parser.update();
}



