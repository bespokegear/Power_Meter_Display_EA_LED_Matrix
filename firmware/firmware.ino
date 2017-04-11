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
#include "FontData.h"

SoftwareSerial softSerial(SW_SERIAL_RX, SW_SERIAL_TX);
Heartbeat heartbeat(HEARTBEAT_PIN);

void setup() {
	Serial.begin(115200);
    heartbeat.begin();
    Matrix.begin(&softSerial);
    Parser.begin();
    Serial.print(F("Max Graph Power: "));
    Serial.println(MaxPowerWatts.get());
    for (uint8_t fontSize=0; fontSize<=8; fontSize++) {
        DB(F("FontSize "));
        DB(fontSize);
        DB(F(" pixels high: "));
        DB(getCharacterHeight(fontSize));
        DB(F(" characters:"));
        for (char c='A'; c<'K'; c++) {
            DB(' ');
            DB(c);
            DB('=');
            DB(getCharacterWidth(fontSize, c));
        }
        DBLN('.');
    }
    delay(100);
    DBLN(F("setup:E"));
}

void loop () {
    heartbeat.update();
    Matrix.update();
    Parser.update();
}



