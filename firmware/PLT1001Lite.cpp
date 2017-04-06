#include "PLT1001Lite.h"
#include <stdio.h>
#include <MutilaDebug.h>
#include <string.h>

PLT1001Lite::PLT1001Lite(uint8_t rxPin, uint8_t txPin) :
    _serial(rxPin, txPin)
{
}

void PLT1001Lite::begin(long baud)
{
    _serial.begin(baud);
    setFont(8);
    clear(false);
    _serial.print(F("text "));
    _serial.print(COLOR_GREEN);
    _serial.print(F(" 10 29 \"Hello!\"\r"));
    paint();
}

void PLT1001Lite::update()
{
    // Does nothing at present
}

void PLT1001Lite::clear(bool now)
{
    _serial.print(F("clear\r"));
    if (now) paint();
}

void PLT1001Lite::paint()
{
    _serial.print(F("paint\r"));
}

void PLT1001Lite::setFont(uint8_t size)
{
    _serial.print(F("font "));
    _serial.print(size);
    _serial.print('\r');
}

void PLT1001Lite::timer(const char* data)
{
    displayTime(data, 11, 24, 1);
}

/* These functions imported from Matt's code */
void PLT1001Lite::displayTime(const char* data, int startx, int starty, int timerColour)
{
  // This displays the time onto the LED
  // Start position startx and starty
  // Timer colour 1= red, 2 = green, 3 = orange  
  String timeValueStr(data);
  DB("timeValueStr=");
  DBLN(timeValueStr);
  int timeValue;
  int timeValueInt;
  int timeValueDec;
  int startXOffset=0;
  
  timeValue = timeValueStr.toInt();
  timeValueInt=timeValue / 10;  // Find pre decimal point
  timeValueDec = timeValue % 10; // find post decimal point
  
  // Use font style 5 - all except 1's are 10 wide , 1s are 8 wide

  //Clear the display
  clear(false);
  // Set the font size
  setFont(5);

  // Serach through timeValueInt to see how many 1's are in the number
  int numberOfOnes = 0;
  for(int y=0;y<(timeValueStr.length()-1);y++)
  {
    if(timeValueStr[y]=='1')
    {
      numberOfOnes++;
    } 
  }
   
  if (timeValueInt < 10)
  {
    if (timeValueInt == 0)
    {
      // Plot starting at corect value
      _serial.print("text ");
      _serial.print(timerColour);
      _serial.print(' ');
      _serial.print(startx + 22);
      _serial.print(' ');
      _serial.print(starty);
      _serial.print(' ');
      _serial.print('"');
      _serial.print('0');
      _serial.print('"');
      _serial.print("\r");
    }
    else
    {
      // Plot starting at corect value
      _serial.print("text ");
      _serial.print(timerColour);
      _serial.print(' ');
      _serial.print(startx + 22 + (numberOfOnes*2));
      _serial.print(' ');
      _serial.print(starty);
      _serial.print(' ');
      _serial.print('"');
      _serial.print(timeValueInt);
      _serial.print('"');
      _serial.print("\r");
    }
  }
  else if (timeValueInt < 100)
  {
    // Plot starting at corect value
    _serial.print("text ");
    _serial.print(timerColour);
    _serial.print(' ');
    _serial.print(startx + 11+(numberOfOnes*2));
    _serial.print(' ');
    _serial.print(starty);
    _serial.print(' ');
    _serial.print('"');
    _serial.print(timeValueInt);
    _serial.print('"');
    _serial.print("\r");

  }
  else if (timeValueInt < 1000)
  {
    _serial.print("text ");
    _serial.print(timerColour);
    _serial.print(' ');
    _serial.print(startx + (numberOfOnes*2));
    _serial.print(' ');
    _serial.print(starty);
    _serial.print(' ');
    _serial.print('"');
    _serial.print(timeValueInt);
    _serial.print('"');
    _serial.print("\r");
  }
  
  // Plot the dot
  _serial.print("text ");
  _serial.print(timerColour);
  _serial.print(' ');
  _serial.print(startx + 33);
  _serial.print(' ');
  _serial.print(starty);
  _serial.print(' ');
  _serial.print('"');
  _serial.print('.');
  _serial.print('"');
  _serial.print("\r");


  // Plot starting at corect value
  _serial.print("text ");
  _serial.print(timerColour);
  _serial.print(' ');
  _serial.print(startx + 36);
  _serial.print(' ');
  _serial.print(starty);
  _serial.print(' ');
  _serial.print('"');
  _serial.print(timeValueDec);
  _serial.print('"');
  _serial.print("\r");

  // Then we display it all
  paint();
}


