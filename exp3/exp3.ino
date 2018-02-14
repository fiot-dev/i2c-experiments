#include <Wire.h>

#define SLAVE_ADDRESS 0x04
#define FLOATS_SENT 3

float data[FLOATS_SENT];

void setup() {
  pinMode(13, OUTPUT);
  Serial.begin(9600); // start serial for output
  // initialize i2c as slave
  Wire.begin(SLAVE_ADDRESS);

  // define callbacks for i2c communication
  //Wire.onReceive(receiveData);
  Wire.onRequest(sendData);

  Serial.println("Ready!");
}

void loop() {
  delay(1000);
}

/* callback for received data */
void receiveData(int byteCount) {
  while (Wire.available()) {
    status = Wire.read();
    Serial.print("data received: ");
    Serial.println(status);

    if (status == 1) {

      if (state == 0) {
        digitalWrite(13, HIGH); // set the LED on
        state = 1;
      }
      else {
        digitalWrite(13, LOW); // set the LED off
        state = 0;
      }
    }
  }
}

// callback for sending data
void sendData() {
  data[0] = getWindspeed(A0);
  data[1] = getLuminosity(A1);
  data[2] = getRainRes(A2);
  Wire.write((byte*) &data, FLOATS_SENT*sizeof(float));
}

float getWindspeed(byte pin) {
  int sensorValue = analogRead(pin);
  float outVoltage = sensorValue * (5.0 / 1023);
  return outVoltage*6;
}

int getLuminosity(byte pin) {
  int sensorValue = analogRead(pin);
  return sensorValue;
}

int getRainRes(byte pin) {
  int sensorValue = analogRead(pin);
  return sensorValue;  
}
