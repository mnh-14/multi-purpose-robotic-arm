#include <SoftwareSerial.h>

SoftwareSerial USerial(10, 11); // RX, TX

void setup() {
  Serial.begin(9600);      // PC USB serial monitor baud rate
  USerial.begin(9600);   // HC-05 default AT mode baud rate
  Serial.println("Hello fuckers");
}

void loop() {
  if (USerial.available()) {
    char c = USerial.read();
      Serial.write(c);
  }
  // if (USerial.available()) {
  //   char c = USerial.read();

  //   Serial.print("READ: ");
  //   Serial.print(c);
  //   Serial.print(" (HEX: ");
  //   Serial.print((int)c, HEX);
  //   Serial.println(")");

  //   if (c == '$') {
  //     Serial.println("<newline>");
  //     Serial.print("\r\n");
  //   } else {
  //     Serial.write(c);
  //   }
  // }
  if (Serial.available()) {
    USerial.write(Serial.read());
  }
}
