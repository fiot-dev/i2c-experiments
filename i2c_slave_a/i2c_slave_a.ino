// Simple I2C protocol for Arduino
// Slave side program
// (c) 2014 Ignas Gramba
//
#include <Wire.h>
 
#define XSensorPin A1
#define YSensorPin A2
 
const byte SlaveDeviceId = 0x04;
byte LastMasterCommand = 0;
 
void setup(){
  Wire.begin(SlaveDeviceId);      // join i2c bus with Slave ID
  Wire.onReceive(receiveCommand); // register talk event
  Wire.onRequest(slavesRespond);  // register callback event
 
  pinMode(XSensorPin, INPUT);
  pinMode(YSensorPin, INPUT);
}
 
void loop(){
  delay(100);
}
 
void receiveCommand(int howMany){
  LastMasterCommand = Wire.read(); // 1 byte (maximum 256 commands)
}
 
void slavesRespond(){
 
  int returnValue = 0;
 
  switch(LastMasterCommand){
    case 0:   // No new command was received
      Wire.write("NA");
    break;
    
    case 1:   // Return X sensor value
      returnValue = GetXSensorValue();
    break;
 
    case 2:   // Return Y sensor value
      returnValue = GetYSensorValue();
    case 3:   // Return Y sensor value
      returnValue = 5;
      
    break;
    
  }
 
  byte buffer[2];                 // split int value into two bytes buffer
  buffer[0] = returnValue >> 8;
  buffer[1] = returnValue & 255;
  Wire.write(buffer, 2);          // return response to last command
  LastMasterCommand = 0;          // null last Master's command
}
 
int GetXSensorValue(){
  int val = analogRead(XSensorPin);
  return val;
}
 
int GetYSensorValue(){
  int val = analogRead(YSensorPin);
  return val;
}
