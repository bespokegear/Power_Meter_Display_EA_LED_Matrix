/*
  AnalogReadSerial
  Reads an analog input on pin 0, prints the result to the serial monitor.
  Graphical representation is available using serial plotter (Tools > Serial Plotter menu)
  Attach the center pin of a potentiometer to pin A0, and the outside pins to +5V and ground.

  This example code is in the public domain.
*/
int i=300;

// the setup routine runs once when you press reset:
void setup() {
  // initialize serial communication at 9600 bits per second:
  Serial.begin(115200);

  Serial.println("aAACD3------");
  delay(1000);
  Serial.println("aAACD2------");
  delay(1000);
  Serial.println("aAACD1------");
  delay(1000);
  Serial.println("aAACD0------");
  delay(1000);     
}

// the loop routine runs over and over again forever:
void loop() {

  
  // print out the value you read:
  Serial.print("aAAT");
  if(i<10)
  {
    Serial.print("000");    
  }
  else if(i<100)
  {
    Serial.print("00");    
  } 
  else if(i<1000)
  {
    Serial.print("0");    
  }  
  Serial.print(i);
  Serial.println("--");
  i--;
  if(i<0||digitalRead(A4)==HIGH)
  {
    Serial.println("aAAWN4------");
    delay(2000);  
    Serial.println("aAACD3------");
    delay(1000);
    Serial.println("aAACD2------");
    delay(1000);
    Serial.println("aAACD1------");
    delay(1000);
    Serial.println("aAACD0------");
    delay(1000);          
    i=300;
  }
  delay(80);        // delay in between reads for stability
}
