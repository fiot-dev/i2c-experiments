#include <Wire.h>

#include <OneWire.h>

const int DEV_CNT = 4, I2C_ADDR = 0x04;

OneWire ds(10);  // 1wire bus on pin 10
int j = 0;
float celsius[DEV_CNT];
byte dev[DEV_CNT][8];

void setup(void) {
  Wire.begin(I2C_ADDR);
  Wire.onRequest(requestEvent); //Register i2c request handler 
}

void loop(void) {
  byte i, present = 0, type_s = 0, data[12], addr[8];
  if(!ds.search(addr)) {
    ds.reset_search();
    delay(250);
    return;
  } 
  // Read 8-byte device ID
  for( i = 0; i < 8; i++) { dev[j][i] = addr[i]; }
  ds.reset();
  ds.select(addr);
  ds.write(0x44,1);   // start conversion, parasite power on   
  delay(750);      
  present = ds.reset();
  ds.select(addr);    
  ds.write(0xBE);     // Read Scratchpad
  for ( i = 0; i < 9; i++) {  data[i] = ds.read(); }
  // convert the data to actual temperature
  unsigned int raw = (data[1] << 8) | data[0];
  celsius[j] = (float) raw / 16.0;
  Serial.println(celsius);
  ++j %= DEV_CNT;  // Increment j but keep within range of device count
}

void requestEvent() // Handle i2c requests
{
  // Buffer to hold temp data, 6 bytes for each device
  byte buffer[DEV_CNT*4];
  // Populate buffer with temp. data
  for(int a = 0; a < DEV_CNT; a++) {
    byte* b = (byte*) &celsius[a];
    // 4 bytes for each float
    for(int i = 0; i < 4; i++) {
      buffer[a*DEV_CNT+i] = b[i];
    }
  }
  // Send data over i2c connection
  Wire.write(buffer, DEV_CNT*4);
}
