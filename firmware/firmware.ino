#include <Arduino.h>
#include <Heartbeat.h>
#include <MutilaDebug.h>
#include <SoftwareSerial.h>
#include <stdint.h>

#include "Config.h"
#include "Matrix.h"
#include "Parser.h"

SoftwareSerial softSerial(SW_SERIAL_RX, SW_SERIAL_TX);
Heartbeat heartbeat(HEARTBEAT_PIN);

void setup() {
	Serial.begin(115200);
    heartbeat.begin();
    Matrix.begin(&softSerial);
    Parser.begin();
    delay(100);
    DBLN(F("setup:E"));
}

void loop () {
    heartbeat.update();
    Matrix.update();
    Parser.update();
}



