/********************************************************
/****** LED Power Meter Display CODE ********************
/****** by Matt Little **********************************
/****** Date: 9/6/16 ************************************
/****** info@re-innovation.co.uk ************************
/****** www.re-innovation.co.uk *************************
/********************************************************

  See www.re-innovation.co.uk for information and 
  construction details
  
  This is basic code for the Pedalog Version 2.

/*************Details of Code*****************************
  
   The PCB/schematic files are available from: www.re-innovation.co.uk
   
  This code is for an Arduino Nano

  It reads serial data (on D0/D1, Tx/Rx).
  It parses that data and displays the power on a large Embedded Adventures serial LED display.

  Display connections:
  EA controller is on Tx = A2, Rx = A1

  Serial Input is on Tx = D0, Rx = D1

  Two switches are included on D5 and D9
   
   Power data is read on the serial port at 115200 baud
   ****protocol for straming data to serial****
   
  There are a number of functions which can be adjusted via software commands. The main protocol is based upon LLAP (Lightweight Local Automation Protocol), with some additional features.
  http://openmicros.org/index.php/articles/85-llap-lightweight-local-automation-protocol/101-llap-starter
  This uses a 12 character message in the format:
  “aXXDDDDDDDDD”
  Where a is the start signifier and XX is the device ID, from AA to ZZ.
  Everything else is in upper case.
  
  Power
  “aXXPDDDDD---“
  This is the power output (0000.0W to 9999.9W). This will be an output approximately every second.
  
  Voltage and Current
  “aXXVDDDIDDDD“
  This is the voltage and current output (V is 00.0 to 99.9V DC) (I is 00.00A to 99.99A). This will be an output approximately every second.
  
  Energy
  “aXXEDDDDDD--“
  This is the Energy from 0000.00Wh to 9999.99Wh (10kWh – is this enough?)
  Max power and average power are done by the receiving device. This will be an output approximately every second.

 
 
  Updates: 
  06/06/2016 Code started - Matt Little

**********************************************************************************************************/
/************ External Libraries*****************************/

#include <stdlib.h>
#include <avr/pgmspace.h>  // Library for putting data into program memory
#include <EEPROM.h>        // For writing values to the EEPROM
#include <avr/eeprom.h>    // For writing values to EEPROM
#include <SoftwareSerial.h>

/************User variables and hardware allocation**********************************************/

// **********Pedalog v2 Hardware information********************************************
// These constants won't change.  They're used to give names to the pins used:
//const int analogCurrent = A1;  // I read in on A1
//const int analogVoltage = A0;  // V read in on A0
const int dump1 = 9;   // Analog output pin that the DUMP LOAD 1 is attached to via FET (PWM)
const int dump2 = 10;  // Analog output pin that the DUMP LOAD 2 is attached to via FET (PWM)
const int fan = 4;     // Fan control transistor is on pin 4
const int LEDred = 3;  // RGB LED on 3/6/5, as the have PWM outputs
const int LEDgrn = 6;
const int LEDblu = 5;
const int buzzer = 8;  // Buzzer attached to D8

// This is for the serial shifted output data
const int sLatch = A3;   //Pin connected to ST_CP of 74HC595
const int sData = A4;    //Pin connected to DS of 74HC595
const int sClk =  A5;    //Pin connected to SH_CP of 74HC595

// Set up the software serial line to put data to the display
SoftwareSerial mySerial(A1, A2); // RX, TX

//*********END of hardware information****************************************

// Useful variables
int voltageInt = 0;
int currentInt = 0;
int fanValue = LOW;

char deviceID[3]; // A buffer to hold the device ID

unsigned int graphDisplay[64];  // A buffer to hold the graphical bar graph display
int maxGraphPower = 1000;  // Power in Watts which is max power to display (=16 on output)
String maxGraphPowerStr;

unsigned long int voltSetPoint;  // The voltage setpoint as a long int. This is the voltage as a value 000-999 = 00.0V to 99.9V
String voltSetPointStr;  // A buffer to hold th voltgae set-point as a sting

unsigned long int currentShunt;  // The current shunt value as a long int. In milliOhms
String currentShuntStr;  // A buffer to hold the current Shunt value as a sting

String outputString;    // This is the holder for the data as a string. Start as blank

// ****** Serial Data Read***********
// Variables for the serial data read
char inByte;         // incoming serial char
String str_buffer = "";  // This is the holder for the string which we will display
#define MAX_STRING 80      // Sets the maximum length of string probably could be lower
char stringBuffer[MAX_STRING];  // A buffer to hold the string when pulled from program memory

// Varibales for writing to EEPROM
int hiByte;      // These are used to store longer variables into EERPRPROM
int loByte;
long int number;

// Variables for the serial shift output display:
// This includes an 3 value 7 segment LED display and a 10 segment bar graph
int displayArray[5]; // An array to store the data to display

long maxPower = 100;  // This is the value to light all 10 LEDs in Watts
long powerInterval = maxPower/10;   // This is a holder for the steps in power (powerInterval = maxPower/10)
int graphLights = 0;  // This is the number of bar graph LEDs to light

// Variables for the PI control loop
long currentint = 0;  // To hold the ADC value
long voltageint = 0;

long currentave = 0;  // To hold the average values
long voltageave = 0;  
long powerave = 0;  // DC power in
long poweravedisplay = 0;  // This is a smoothed value of power, which is used for the display
long remainder = 0;    // This is for the remainder of the LED calculation

int averagecounter = 1000;  // The number of samples to average over
int displaycounter = 20;   // When to update the display
int displaycountermax = 20;  // This is when to display the data ( x 10mS)

long FETcontrol = 0;   // This is the PWM level to send to the FETs
                      // Both FETs work in parallel
long proportional = 0;    // The proportional element
long integral = 0;        // The integral element
long error = 0;           // The error signal
//long vsetpoint = 12000;   // The voltage setpoint for the PICD controller  

long int oldTime = 0;  // This is for outputting the data every 1S or so

// Variables for the RGB output LED
unsigned long int oldtime = 0;  // To store the previous time
int redPWMint = 0;  // This is the colour level for the R
int greenPWMint = 0;  // This is the colour level for the G
int bluePWMint = 0;  // This is the colour level for the B

// Flags to control if things are attached
// LOW = OFF, HIGH = ON. These can be changes with serial control functions
int beepFlag = LOW;  // This is a flag to set beeps on/off
int bargraphFlag = LOW;  // This is for the huge bar graph
int LEDdisplayFlag = LOW;  // This is for the seven segment array

unsigned long int voltageAve = 0;  // This holds the average calculation value
long int averageCounter = 100;  // This counts the number of times an average has been taken.
unsigned long int time = 0;  // This holds the averaging 

int displayOffset = 0;  // This is for putting the power data in the middle of the display.

// Commands for writing to serial port (put into const to save RAM)
const char barGraph[] PROGMEM = "The bar graph is:";
const char beep[] PROGMEM = "The buzzer is:";
const char LEDdisplay[] PROGMEM = "The LED is:";
const char clearEA[] PROGMEM = "clear";


//****************INITIALISE ROUTINE******************************
void setup()
{
  Serial.begin(115200);    // Set up a serial output for data display and changing parameters
  
  pinMode(A2, OUTPUT); 
  // set the data rate for the SoftwareSerial port
  mySerial.begin(115200);  

//  // declare correct inputs and outputs:
//  pinMode(analogVoltage,INPUT);
//  pinMode(analogCurrent,INPUT);  

  pinMode(dump1, OUTPUT);
  pinMode(dump2, OUTPUT);  
  digitalWrite(dump1,LOW);
  digitalWrite(dump2,LOW); 
  
  pinMode(fan, OUTPUT); 
  pinMode(LEDred, OUTPUT);
  pinMode(LEDgrn, OUTPUT);   
  pinMode(LEDblu, OUTPUT);   
  digitalWrite(LEDred,LOW);  // Set pin HIGH to switch it OFF
  digitalWrite(LEDgrn,LOW);  // Set pin HIGH to switch it OFF 
  digitalWrite(LEDblu,LOW);  // Set pin HIGH to switch it OFF 

  
  //set pins to output so you can control the shift register
  pinMode(sLatch, OUTPUT);
  pinMode(sClk, OUTPUT);
  pinMode(sData, OUTPUT);  
  
  // Read in the values from EEPROM
  // Read the device ID
  deviceID[0] = char(EEPROM.read(0));
  deviceID[1] = char(EEPROM.read(1)); 
  Serial.print("Device ID is: ");
  Serial.print(deviceID[0]);
  Serial.println(deviceID[1]);
  
  // Read in the Voltage Set-point
  hiByte = EEPROM.read(2);
  loByte = EEPROM.read(3);
  voltSetPoint = (hiByte << 8)+loByte;  // Get the sensor calibrate value 
  Serial.print("Voltage Set Point is: ");
  Serial.print(voltSetPoint);
  Serial.println("00 mV"); 
  
  // Read in the Current Shunt Value
  hiByte = EEPROM.read(4);
  loByte = EEPROM.read(5);
  currentShunt = (hiByte << 8)+loByte;  // Get the sensor calibrate value 
  Serial.print("Current Shunt Offset: ");
  Serial.println(currentShunt);  
  //Serial.println("/10 milliOhms"); 
  

  // Check the BEEP flag - is it required?
  beepFlag = EEPROM.read(6); 
  Serial.print(getString(beep));
  if(beepFlag==HIGH)
  {
    Serial.println(" ON");  
  }
  else  
  {
    Serial.println("OFF");  
  }
  
  // Check the LED bar graph flag - is it required?
  bargraphFlag = EEPROM.read(7); 
  Serial.print(getString(barGraph));
  if(bargraphFlag==HIGH)
  {
    Serial.println(" ON");  
  }
  else  
  {
    Serial.println("OFF");  
  }
  
    // Check the BEEP flag - is it required?
  LEDdisplayFlag = EEPROM.read(8); 
  Serial.print(getString(LEDdisplay));
  if(LEDdisplayFlag==HIGH)
  {
    Serial.println(" ON");  
  }
  else  
  {
    Serial.println("OFF");  
  }

  // Read in the max Graph Power
  hiByte = EEPROM.read(9);
  loByte = EEPROM.read(10);
  maxGraphPower = (hiByte << 8)+loByte;  
  Serial.print("Max Graph Power: ");
  Serial.println(maxGraphPower); 

  
  for(int z=0;z<64;z++)
  {
    // Ensure the graphDisplay buffer is clear
    graphDisplay[z]=0;
  }
  
}

void loop()
{
  // This is the main loop. 
  
//  // Firstly lets take an average of the voltage and current values
//  // This is doen in a sub-routine:
//  readVI();
//  
//  //*************POWER CACULATION********
//  // This is not extremly accurate - as it only take the average values
//  // But good enough for high powers and quick for this item 
//  powerave = voltageint*currentint;  
//  powerave = powerave/1000000;  // Gets rid of sampling freq

  powerave = 100;
//  // Display the output voltage on the RGB LED
//  ledRGBdisplay();
  
//  currentave = 0;  // Reset the average values
//  voltageave = 0;   
  
  //*********Here we check to see if any serial data has been seen***********
  getData();  // Check the serial port for data
  
  delay(5);   // Short pause to stop everything running too quick
  
  // ***********Display data********************************
  // Here we keep a count of the milliseconds
  // If more than 1000mS has passed then send out the data
  // Output data on serial:
  if((millis())>=(oldTime+1000))
  {
    oldTime = millis();  // Save the time ready for the next output display    
    // We enter this point every 1000mS (approx, it might be out by a bit (+/-5mS))
    
//        //*************POWER CACULATION********
//    // This is not extremly accurate - as it only take the average values
//    // But good enough for high powers and quick for this item 
//    if(voltageint>0&&currentint>0)
//    {
//      powerave = voltageint*currentint;  
//      powerave = powerave/1000000;  // Gets rid of sampling freq
//    }
//    else
//    {
//      powerave=0;
//    }
       
    // Output the voltage phrase:
    displayVoltage();  
    displayCurrent();
    displayPower();
    
    // Sort out the graphDisplay with the power values
    // First move all the power values along
    // Then fit the new powerAve into the buffer
    for(int z=1;z<64;z++)
    {
      // Shift all the graph display values along
      graphDisplay[z-1]=graphDisplay[z];
    }
    graphDisplay[63]=powerave;
   
     // Output things to the Embedded Adventures display unit (serial)
     // More info can be found here: http://www.embeddedadventures.com/datasheets/PLT-1001_hw_v4_doc_v3.pdf
      
    // Here we calculate the offset for the power text so that it stays to one side
    // The display is 64 px wide.
    // Each number is 10+1(space) px wide
    // So if power is 0-9W, Offset is 38 
    // So if power is 10-99W, Offset is 27 
    // So if power is 100-999W, Offset is 16 
    // So if power is 1000-9999W, Offset is 5 
    
    if(powerave>=0&&powerave<10)
    {
      displayOffset=38;
    }
    else if(powerave>=10&&powerave<20)
    {
      displayOffset=29;
    } 
    else if(powerave>=20&&powerave<100)
    {
      displayOffset=27;
    } 
    else if(powerave>=100&&powerave<200)
    {
      displayOffset=18;
    }
    else if(powerave>=200&&powerave<1000)
    {
      displayOffset=16;
    } 
    else if(powerave>=1000&&powerave<2000)
    {
      displayOffset=7;
    } 
    else if(powerave>=2000&&powerave<10000)
    {
      displayOffset=5;
    }    
    
    // Clear the LED display (just in case)
    mySerial.print("clear\r");
    delay(5); 
    mySerial.print("clear\r");
    delay(5); 
    mySerial.print("clear\r");
    delay(5);     
    mySerial.print("font 5\r");
    delay(5);
    
    
    // Then also want to display a scrolling bar graph of power along the bottom
    // This takes the values in graphDisplay and creates a line from 0 to value all along the bottom
    
    // To draw a line we need to write the command:
    //  "line colour x1 y1 x2 y2" for every data point
    
    // Power values is converted into a graph.
    // The power value is also displayed on top of the graph.
    
    for(int y=0;y<64;y++)
    {   
      if(graphDisplay[y]>0)
      {
        mySerial.print("line 1 ");
        mySerial.print(y);
        mySerial.print(" 31 ");
        //mySerial.print(graphDisplay[y]);
        mySerial.print(y); 
        mySerial.print(' ');
        // This is where we draw the bar graph section:
        // We have maxGraphPower = 16 LEDs lit.
        // So to convert from power reading to LEDs to light we use the calculation:
        // (power now / maxGraphPower) *16
        int LEDtoLight = (((float)graphDisplay[y]/(float)maxGraphPower)*16);
        mySerial.print(31-LEDtoLight);
        mySerial.print('\r');
        mySerial.print('\r'); 
      }
      else
      {
      }
      // Need to slow this down a bit
      delay(5);     
    }   
 
     mySerial.print("text 2 ");
    mySerial.print(displayOffset);  // This ensure the numbers are in the middle of the display
    mySerial.print(" 23 ");
    mySerial.print('"');
    // Want to display the power here:
    mySerial.print(powerave);
    mySerial.print('w');
    mySerial.print('"');  
    mySerial.print('\r');
    delay(2);
    
    // Then we display it all
    mySerial.print("paint");
    mySerial.print('\r');   
   
  }

}


// This routine pulls the string stored in program memory so we can use it
// It is temporaily stored in the stringBuffer
char* getString(const char* str) {
	strcpy_P(stringBuffer, (char*)str);
	return stringBuffer;
}

// **********************GET DATA SUBROUTINE*****************************************
// This sub-routine picks up and serial string sent to the device and sorts out a power string if there is one
// All values are global, hence nothing is sent/returned

void getData()
{
  // **********GET DATA*******************************************
  // We want to find the bit of interesting data in the serial data stream
  // As mentioned above, we are using LLAP for the data.
  // All the data arrives as serial commands via the serial interface.
  // All data is in format aXXDDDDDDDDD where XX is the device ID
  if (Serial.available() > 0) 
  {
    inByte = Serial.read(); // Read whatever is happening on the serial port
  
    if(inByte=='a')    // If we see an 'a' then read in the next 11 chars into a buffer.
    {   
      str_buffer+=inByte;
      for(int i = 0; i<11;i++)  // Read in the next 11 chars - this is the data
      {
        inByte = Serial.read(); 
        str_buffer+=inByte;
      }
      Serial.println(str_buffer);  // TEST - print the str_buffer data (if it has arrived)
      sortData();
      str_buffer="";  // Reset the buffer to be filled again
    }
  }
}

// **********************SORT DATA SUBROUTINE*****************************************
// This sub-routine takes the read-in data string (12 char, starting with a) and does what is required with it
// The str-buffer is global so we do not need to send it to the routine

void sortData()
{ 
  // We first want to check if the device ID matches.
  // If it does not then we disregard the command (as it was not meant for this device      
  if(str_buffer.substring(1,3) == deviceID)
  {
    // If yes then we can do further checks on ths data
    // This is where we do all of the checks on the incomming serial command:
    
    Serial.println("Matches Device ID");  // TEST - got into this routine
    
    // Change device ID:
    // Device ID
    // “aXXCHIDXXE--“
    // Where the last two values (XX) are the new device ID (from AA to ZZ).
    if(str_buffer.substring(3,7) == "CHID")
    {
      // First check if the NEW device ID is within the allowable range (AA-ZZ)
      // to do this we can convert to an int and check if it is within the OK levels
      // A -> int is 65, Z -. int is 90
      // So our new device ID as an int must be between 65 and 90 for it to be valid
      if(65<=int(str_buffer[7])&&int(str_buffer[7])<=90&&65<=int(str_buffer[8])&&int(str_buffer[8])<=90)   
      { // If is all OK then write the data
        // Change device ID
        Serial.print("Change Device ID to ");
        Serial.println(str_buffer.substring(7,9));  // This will change the device ID
        deviceID[0] = str_buffer[7];
        deviceID[1] = str_buffer[8];
        // Also want to store this into EEPROM
        EEPROM.write(0, deviceID[0]);    // Do this seperately
        EEPROM.write(1, deviceID[1]);
      }
      else
      {
        Serial.println("Invalid device ID"); 
     }      
    } 
    
     // Ask voltage set-point
    //  To ask the set-point we can send:
    //  “aXXVSP?-----“ 
    else if(str_buffer.substring(3,7) == "VSP?")
    {
      // Show voltage Set Point
      Serial.print("Voltage Set Point is:");
      // ***TO DO*** add the voltage set-point
      Serial.println(voltSetPoint);
    } 
    
    //  Voltage regulation set-point change
    //  The voltage regulation set-point controls when the shunt load will switch on. The device will try (as much as possible) to maintain the voltage at this point. Proportional –Integral (PI) control is used to do this.
    //  “aXXVSPDDD-—“
    //  This sends the command to device DD. The set-point (VSP) is DD.D volts.
    else if(str_buffer.substring(3,6) == "VSP")
    {
      // Change Voltage Set-Point 
      // DO NOT CHANGE if outside of max/min values or rubbish data
      // Need to convert the substring into a long int
      voltSetPointStr = str_buffer.substring(6,9);
      voltSetPoint = voltSetPointStr.toInt();
      // DO NOT CHANGE if outside of max/min values or rubbish data 
      // Setpoint is only allowed up to 40V DC.    
      if(voltSetPoint>=0&&voltSetPoint<=400)
      {
        Serial.print("New Voltage Set Point is:");
        Serial.println(voltSetPoint); 
        // Also want to store this into EEPROM
        EEPROM.write(2, voltSetPoint >> 8);    // Do this seperately
        EEPROM.write(3, voltSetPoint & 0xff);
      }
      else
      {
        Serial.println("Invalid Volt Set Point"); 
     }        
      
    }  
    
    // Current Shunt value read
    //  To read the value from the device we can ask the command:
    //  “aXXISH?E----“
    else if(str_buffer.substring(3,7) == "ISH?")
    {
      // Show Current Shunt value 
      Serial.print("Current shunt offset is:");
      Serial.println(currentShunt); 
      //Serial.println("/10 milliOhms");
    } 

    //  Current Shunt Value
    //  To change this we use the command:
    //  “aXXISHDDDDE-“
    //  Where the value DDDD is the resistance in milliOhms
    else if(str_buffer.substring(3,6) == "ISH")
    {
      
      // Change Voltage Set-Point 
      // DO NOT CHANGE if outside of max/min values or rubbish data
      // Need to convert the substring into a long int
      currentShuntStr = str_buffer.substring(6,11);
      currentShunt = currentShuntStr.toInt();
  
      // DO NOT CHANGE if outside of max/min values or rubbish data     
      if(currentShunt>=0&&currentShunt<=1024)
      {
        // Change Current Shunt Value
        Serial.print("Current Shunt changed to: ");
        Serial.println(currentShunt);  
        //Serial.println("/10 milliOhms");
      
        // Also want to store this into EEPROM
        EEPROM.write(4, currentShunt >> 8);    // Do this seperately
        EEPROM.write(5, currentShunt & 0xff);
      }
      else
      {
        Serial.println("Invalid Current Shunt"); 
      }         
    } 


    //  Maximum Graph Power
    //  To change this we use the command:
    //  “aXXMGPDDDDE-“
    //  Where the value DDDD is the maximum graph power in watts
    // This corresponds to 16 red LEDs being lit.
    else if(str_buffer.substring(3,6) == "MGP")
    {
      maxGraphPowerStr = str_buffer.substring(6,10);
      maxGraphPower = maxGraphPowerStr.toInt();      
      // DO NOT CHANGE if outside of max/min values or rubbish data     
      if(maxGraphPower>=0&&maxGraphPower<=3000)
      {
        // Change max Graph Power
        Serial.print("maxGraphPower changed to: ");
        Serial.println(maxGraphPower);  
      
        // Also want to store this into EEPOM
        EEPROM.write(9, maxGraphPower >> 8);    // Do this seperately
        EEPROM.write(10, maxGraphPower & 0xff);
      }
      else
      {
        Serial.println("Invalid Graph Power"); 
      }         
    }
    
    // Control BEEP on/off
    //  “aXXBEEPON---“ “aXXBEEPOFF--“ 
    // This will be saved in EEPROM
    else if(str_buffer.substring(3,7) == "BEEP")
    { 
      if(str_buffer.substring(7,10) == "OFF")
      {
        Serial.println("Set beep OFF");
        beepFlag = LOW;
        EEPROM.write(6, beepFlag);    // Write the beep flag to EEPROM
      }  
      else if(str_buffer.substring(7,9) == "ON")
      {
        Serial.println("Set beep ON");
        beepFlag = HIGH;
        EEPROM.write(6, beepFlag);    // Write the beep flag to EEPROM
      }       
    }     

    // Control Bar Graph on/off
    //  “aXXBARON----“ “aXXBAROFF---“ 
    // This will be saved in EEPROM
    else if(str_buffer.substring(3,6) == "BAR")
    { 
      if(str_buffer.substring(6,9) == "OFF")
      {
        Serial.println("Bar Graph OFF");
       bargraphFlag = LOW;
        EEPROM.write(7, bargraphFlag);    // Write the beep flag to EEPROM
      }  
      else if(str_buffer.substring(6,8) == "ON")
      {
        Serial.println("Bar Graph ON");
        bargraphFlag = HIGH;
        EEPROM.write(7, bargraphFlag);    // Write the beep flag to EEPROM
      }       
    }
    
    // Control LED Display on/off
    //  “aXXLEDON----“ “aXXLEDOFF---“ 
    // This will be saved in EEPROM
    else if(str_buffer.substring(3,6) == "LED")
    { 
      if(str_buffer.substring(6,9) == "OFF")
      {
        Serial.println("LED Display OFF");
        LEDdisplayFlag = LOW;
        EEPROM.write(8, LEDdisplayFlag);    // Write the beep flag to EEPROM
      }  
      else if(str_buffer.substring(6,8) == "ON")
      {
        Serial.println("LED Display ON");
        LEDdisplayFlag = HIGH;
        EEPROM.write(8, LEDdisplayFlag);    // Write the beep flag to EEPROM
      }       
    }



    
  }
  else
  {
    Serial.println("Device ID does not match");
  }
}

//********** This subroutine is used for outputting the voltage value
void displayVoltage()
{
  // **************DISPLAY VOLTAGE************************
     //  Voltage and Current
    //  “aXXVDDD-----"
    //  "aXXIDDDD----“
    //  This is the voltage and current output (V is 00.0 to 99.9V DC) (I is 00.00A to 99.99A). 
    outputString = "a";
    outputString += deviceID[0];
    outputString += deviceID[1];
    outputString += "V";
    // Here we add the voltage as a value from 00.0V to 99.9V
    // voltageint is in mV so we need to sort that out
    // If voltageint is <1000 then add a zero, if <100 then add two zeros, if <10 the all zeros
    if(voltageint<100)
    {
       outputString += "000";     
    }
    else if(voltageint<1000)
    {
      outputString += "00";
      outputString += String(voltageint);
    }
    else if (voltageint<10000)
    {
      outputString += "0";
      outputString += String(voltageint);
    }    
    else
    {
      outputString += String(voltageint);
    }
    outputString = outputString.substring(0,7);
    outputString += "-----";
    Serial.println(outputString); 
}

//********** This subroutine is used for outputting the current value
void displayCurrent()
{
  // **************DISPLAY VOLTAGE************************
     //  Voltage and Current
    //  “aXXVDDD-----"
    //  "aXXIDDDD----“
    //  This is the voltage and current output (V is 00.0 to 99.9V DC) (I is 000.0A to 999.9A). 
    outputString = "a";
    outputString += deviceID[0];
    outputString += deviceID[1];
    outputString += "I";
    
    // THIS HAS BEEN REWRITTEN FOR THE 50A VERSION
    // Here we add the voltage as a value from 000.0A to 999.9A
    // currentint is in milliamps, but we want to display 000.0A to 999.9A
    
    // *************TO DO**************************************************
    // Getting a value from 1 = 0.001A to 999999 = 999.999A
    // Must convert this into the display value of 000.0A to 999.9A
    
    // If the value is <99 then output is zero
    // If the value is 100< <999999 then output is value/100 A
    
    
    // If currentint is <1000 then add a zero, if <100 then add two zeros, if <10 the all zeros
    if(currentint<100)
    {
       outputString += "0000";     
    }
    else if(currentint<1000)
    {
      outputString += "000";
      outputString += String(currentint/100);
    }
    else if (currentint<10000)
    {
      outputString += "00";
      outputString += String(currentint/100);
    }    
    else if (currentint<100000)
    {
      outputString += "0";
      outputString += String(currentint/100);
    }
    else
    {
      outputString += String(currentint/100);
    }
    outputString = outputString.substring(0,8);
    outputString += "----";
    Serial.println(outputString); 
}

//********** This subroutine is used for outputting the power value
void displayPower()
{
  // **************DISPLAY POWER************************  
    //  These values will be printed to output when required.
    //  Power
    //  “aXXPDDDDE---“
    //  This is the power output (0000W to 9999W). This will be an output approximately every second.
    //  
    outputString = "a";
    outputString += deviceID[0];
    outputString += deviceID[1];
    outputString += "P";
    // Here we add the power in W from 0000W to 9999W
    // powerave is in Wso we need to sort that out
    // If powerave is <1000 then add a zero, if <100 then add two zeros, if <10 the all zeros
    if(powerave<10)
    {
       outputString += "000";
       outputString += String(powerave);     
    }
    else if(powerave<100)
    {
      outputString += "00";
      outputString += String(powerave);
    }
    else if (powerave<1000)
    {
      outputString += "0";
      outputString += String(powerave);
    }    
    else
    {
      outputString += String(powerave);
    }
    outputString += "----";
    Serial.println(outputString); 
}


////********** This subroutine is controls the RGB output LED
//void ledRGBdisplay()
//{
//  // This depends upon the voltage level:
//  //  With a set-point of 13V DC
//  //  <10V		Flash RED (beep?)
//  //  10V to 11.5V	RED constant
//  //  11.5V to 12.5V	RED fade to YELLOW
//  //  12.5V to 13.0V	YELLOW fade to GREEN
//  //  13.0V to 14.0V	GREEN fade to BLUE
//  //  >14.0V		Flash BLUE (Beep)
//  
//  // We call this routine often and irregularly.
//  // Need to keep track of the time using the millis() function
//  
//  //// Variables for the RGB output LED
//  //unsigned long int oldtime = 0;  // To store the previous time
//  //int redPWMint = 0;  // This is the colour level for the R
//  //int greenPWMint = 0;  // This is the colour level for the G
//  //int bluePWMint = 0;  // This is the colour level for the B 
// 
//  // This is fully controlled by the voltageint (000-999 = 000V to 99.9V)
//  // and the voltSetPoint, which is the level we are trying to shunt at
//  // The voltSetPoint is higher than the correct running setpoint 
//  
//  //voltageint>=(voltSetPoint*100)
//  // voltSetPoint is when the system starts to dump power
//  // This is bad, so switch to blue there
//  // For a 12V system green is from 12-14V, for 24V system green is from 24-28, which is 2000mv for 12V and 4000mV for 24V
//  // so need green zone for voltSetPoint - ((voltSetPoint/12000) *2000)
//  
//  long int fetONvoltage = voltSetPoint*100;
//  // ***** CHANGED FOR NOTTS UNI *****
//  //long int redONvoltage = fetONvoltage-2500; //(fetONvoltage-((fetONvoltage/12000)*4000));  // Below this Red is ON
//  long int redONvoltage = 10000;  // 10V in milli volts
//  
//  //long int yellowONvoltage = (fetONvoltage-((fetONvoltage/12000)*2000)); // Below this Yellow is ON
//  //long int greenONvoltage = fetONvoltage-500;  // We use the setpoint minus 1V as the transition between green and blue 
//  long int greenONvoltage = fetONvoltage;
//  
//  // A problem is that the Tone() command used affect the PWM outputs. This could cause problems.
//  
//  // We also want to average the data a bit here
//  // We take averages over 1 second (approx)
//  // We sum all the voltages and increment a counter
//  // The average is then the total sum (voltageAve) divided by the total counter (averageCounter)
// 
//  voltageAve = voltageAve + voltageint;  // This increase the average
//  averageCounter++;  // Increment the average counter
//  if(millis()>time+500)
//  {  
//    // If we enter here then we update the RGB LED and Beep (if required)
//    voltageAve = voltageAve/averageCounter;  // works out the average for the previous 1 second
//    
//    if(voltageAve<redONvoltage)
//    {
//      redPWMint = 255;
//      greenPWMint = 255;
//      bluePWMint = 0;
//    
//      if(beepFlag==HIGH)
//      {
//         tone(buzzer, 420, 30);
//         delay(30);
//         tone(buzzer, 460, 30);
//         delay(30);
//         tone(buzzer, 700, 30);         
//         delay(30);
//      }	
//    }  
//
//    else if (voltageAve>redONvoltage&&voltageAve<greenONvoltage)
//    {
//      redPWMint = 255;
//      greenPWMint = 0;
//      bluePWMint = 255;
//    }  
//    else if (voltageAve>greenONvoltage)
//    {
//      redPWMint = 0;
//      greenPWMint = 255;
//      bluePWMint = 255; 
//      
//      if(beepFlag==HIGH)
//      {
//        // Want to beep to go slower
//        // Do a freq lowering beep....
//        tone(buzzer, 700, 30);
//        delay(30);
//        tone(buzzer, 460, 30);
//        delay(30);
//        tone(buzzer, 420, 30);         
//        delay(30);
//      }
//    }
//      
//    analogWrite(LEDred,redPWMint);  // Set the output correctly   
//    analogWrite(LEDgrn,greenPWMint);  // Set the output correctly  
//    analogWrite(LEDblu,bluePWMint);  // Set the output correctly  
//    
//    voltageAve=0;      // Reset the average for next second
//    averageCounter=0;
//    time=millis();  // Reset the timer  
//  }
//} 


//// Subroutine to read current and voltage
//void readVI()
//{
//    // Firstly lets take an average of the voltage and current values:
//   for (int i = 0; i < averagecounter; i++) 
//  {  
//    currentave += analogRead(analogCurrent);
//    voltageave += analogRead(analogVoltage);
//  } 
//  // Then calculate the average values:
//  currentint = currentave/averagecounter;
//  voltageint = voltageave/averagecounter;
//  
//  // *********Current conversion************************
//  // In this version the current is measured with an ACS756 200A hall effect sensor.
//  // This gives an output voltage of 0.6V + 0.02V x Current
//  // Need to convert this.
////  Serial.print("Input value = ");
////  Serial.println(currentint);    
//  // Remove the offset, using the current shunt vlaue.
//  currentint = currentint-currentShunt;
//  
////  Serial.print("Input value 2 = ");
////  Serial.println(currentint); 
//  
//  if(currentint<0)
//  {
//    currentint=0;
//  }
//  else
//  {
//    // We use a figure of 50 for the 0.02V/A and 1000 to convert from A to  mA
//    currentint = (currentint*5*50*1000)/1024;
//  }
//   
//  
//  //*******Voltage Conversion**************************
//  // Voltage is measured with a potential divider
//  // If the values are 680k and 100k, so max voltage is 35V
//  // 5V = 1023 reading. Vin * (100k / (100k + 680k)) = Vout
//  // Vout x 15.468 = Vin. Vout = voltageint x 4.88mV
//  // So Vin = voltageint x 4.88 x ((100k+680k)/100k) = voltageint x 38 (in mV)
//  
//  // This depends upon the supply voltage, along with Ra and Rb (46k and 680k repecively)
//  // With pedalog v2, the supply goes via a 1N4001 diode, hence it is around 4.2V
//  // We also use a higher voltage range: 46k and 680k, which gives 60V max range.
//  // So the calc becomes:
//  
//  // Vin = voltageint x 4.2 x ((46k+680k)/46k) = voltageint x 66 (in mV)  
//  voltageint = voltageint * 66;  // This is going to loose the decimal place so not super accurate
// 
//}
