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
    Matrix.begin(&softSerial);
    Parser.begin();
    Serial.print(F("Max Graph Power: "));
    Serial.println(MaxPowerWatts.get());
    delay(100);
    DBLN(F("setup:E"));
}

void loop () {
    heartbeat.update();
    Matrix.update();
    Parser.update();
}



