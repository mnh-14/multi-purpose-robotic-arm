#include <SoftwareSerial.h>

SoftwareSerial USerial(10, 11); // RX, TX

void setup() {
  Serial.begin(9600);      // PC USB serial monitor baud rate
  USerial.begin(9600);   // HC-05 default AT mode baud rate
}

void loop() {
  if (USerial.available()) {
    Serial.write(USerial.read());
  }
  if (Serial.available()) {
    USerial.write(Serial.read());
  }
}
