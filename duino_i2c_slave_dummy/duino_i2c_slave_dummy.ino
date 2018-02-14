#include <Wire.h>

void setup()
{
int arduinoI2CAddress = 33; // set the slave address for the Arduino on the I2C buss

Wire.begin(arduinoI2CAddress); // join i2c bus with specified address
Wire.onRequest(requestEvent); // register wire.request interrupt event
Wire.onReceive(receiveEvent); // register wire.write interrupt event
}

char sendStatus[31] = “000000000000000000000000000000”; // initialize the container variable
int index = 0; // initialize the index variable
char pwm[15] = “00000000000000”; // initialize the PWM flag container

void loop()
{

String pinStatus=””; // initialize pinStatus variable

for(int digitalPin = 0; digitalPin <= 13; digitalPin++) // loop through 14 digital pins 0 – 13
{
if (pwm[digitalPin] == 0) // in case PWM is off for the pin, read the pin status
{
pinStatus += String (digitalRead(digitalPin)); // read the pin status & add it to the container variable
}
else
{
pinStatus += “P”; // in case PWM is on for the pin, add P to the pin status container string
}
}

for(int analogPin = 0; analogPin <= 3; analogPin++) // loop through the 4 (unused) analog pins 0 – 3
{
pinStatus += String (1000+analogRead(analogPin)); // read the analog value from the pin, add 1000 to make it 4 digit & add it to the container variable
}

pinStatus.toCharArray(sendStatus, 31); // convert the container variable pinStatus to a char array which can be send over i2c

delay(1000); // wait for an interrupt event
}

//——————————————————————————–
// function that executes whenever a status update is requested by master
// this function is registered as an event, see setup()

void requestEvent() {
Wire.write(sendStatus[index]);
++index;
if (index >= 30) {
index = 0;
}
}

//——————————————————————————–
// function that executes whenever a message is received from master
// this function is registered as an event, see setup()

void receiveEvent(int howMany)
{
int receiveByte = 0; // set index to 0
char command[7]; // expect 7 char + 1 end byte
String mode = “”; // initialize mode variable for holding the mode
String pin = “”; // initialize pin variable for holding the pin number as a String
String awValue = “”; // intitalize the variable for holding the analogWrite value
int pinVal; // inititalize the variable for holding the pin number as integer
int awValueVal; // initialize the variable for holding the analog write value as integer (only PWM pins!)

while(Wire.available()) // loop through all incoming bytes
{
command[receiveByte] = Wire.read(); // receive byte as a character
receiveByte++; // increase index by 1
}

pin = String(command[1]) + String(command[2]); // combine byte 2 and 3 in order to get the pin number
awValue = String(command[4]) + String(command[5]) + String(command[6]); // combine byte 5, 6 and 7 in order to get the analogWrite value
awValueVal = awValue.toInt(); // convert the awValue string to a value

if (String(command[1]) != “A” ) { pinVal = pin.toInt();} // in case of not an analog pin assignment convert into digital pin number
if (String(command[1]) != “A” ) { pwm[pinVal] = 0;} // in case of not an analog pin assignment set PWM flag to 0

// incase of analog pin assignment determine analog pin to be set
if (String(command[1]) == “A” && String(command[2]) == “0”) { pinVal = A0;}
if (String(command[1]) == “A” && String(command[2]) == “1”) { pinVal = A1;}
if (String(command[1]) == “A” && String(command[2]) == “2”) { pinVal = A2;}
if (String(command[1]) == “A” && String(command[2]) == “3”) { pinVal = A3;}

// if requested set pinmode
if (String(command[0]) == “S” && String(command[3]) == “I”) { pinMode(pinVal, INPUT);}
if (String(command[0]) == “S” && String(command[3]) == “O”) { pinMode(pinVal, OUTPUT);}
if (String(command[0]) == “S” && String(command[3]) == “P”) { pinMode(pinVal, INPUT_PULLUP);}

// if requested perform digital write
if (String(command[0]) == “W” && String(command[3]) == “H”) { digitalWrite(pinVal, HIGH);}
if (String(command[0]) == “W” && String(command[3]) == “L”) { digitalWrite(pinVal, LOW);}
// if requested perform analog write
if (String(command[0]) == “A” && pinVal == 3 || pinVal == 5 || pinVal == 6 || pinVal == 9 || pinVal == 10 || pinVal == 11 )
{
analogWrite(pinVal, awValueVal);
pwm[pinVal] = 1;
}

}
