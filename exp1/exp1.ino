#include <Wire.h>
#define SLAVE_ADDRESS 0x2a
#define pot1pin A2
#define pot2pin A3

byte pot1byte;
byte pot2byte;
void setup()
{
    Wire.begin(SLAVE_ADDRESS);
    Wire.onRequest(requestEvent);
}

void loop() {
    int pot1int = analogRead(pot1pin);
    int pot2int = analogRead(pot2pin);
    pot1byte = map(pot1int, 0, 1024, 0, 255);
    pot2byte = map(pot2int, 0, 1024, 0, 255);
}

void requestEvent()
{
    Wire.write(pot1byte);
    delay(30);
    Wire.write(pot2byte);
}
