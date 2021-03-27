#include <Arduino.h>
#include <SoftwareSerial.h>
#include <EEPROM.h>
#include <Heartbeat.h>
#include <MutilaDebug.h>
#include <stdint.h>
#include <DebouncedButton.h>


#include "Config.h"
#include "Matrix.h"
#include "Parser.h"
#include "Settings.h"
#include "ClearMode.h"
#include "PowerMode.h"

SoftwareSerial softSerial(SW_SERIAL_RX, SW_SERIAL_TX);
Heartbeat heartbeat(HEARTBEAT_PIN);

// Sort out reset button
DebouncedButton ResetButton(RESET_BUTTON_PIN, true);

void setup() {
  Serial.begin(SERIAL_BAUD);
  heartbeat.begin();
  // Parser should be initialize before Matrix so that when Matrix
  // Enters DumpConfigMode, Parser wil already know it's ID
  Parser.begin();
  Matrix.begin(&softSerial);
  // Initialize button objects
  ResetButton.begin();
  DBLN(F("setup:E"));
}

void loop () {
  heartbeat.update();
  Matrix.update();
  Parser.update();
    // Check Buttons
  ResetButton.update();
  if (ResetButton.pushed() == true)
  {
    //energyWs = 0;  // Reset the energy value
    DBLN(F("Pressed"));
    PowerMode.lastEValue = 0;
    Matrix.startMode(&ClearMode);
  }
}
