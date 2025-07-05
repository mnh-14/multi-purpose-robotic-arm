#include <SoftwareSerial.h>

SoftwareSerial BTSerial(10, 11); // RX, TX

void setup() {
  Serial.begin(9600);      // PC USB serial monitor baud rate
  BTSerial.begin(38400);   // HC-05 default AT mode baud rate
}

void loop() {
  if (Serial.available()) {
    BTSerial.write(Serial.read());
  }
  if (BTSerial.available()) {
    Serial.write(BTSerial.read());
  }
}


// +ADDR:0022:03:010039 addr of slave module