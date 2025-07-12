#include <Servo.h>

Servo myServo;  // create servo object

// Constants
const int servoPin = 9;  // You can change this to any PWM-capable pin (3, 5, 6, 9, 10, 11)

void setup() {
  myServo.attach(servoPin);  // attaches the servo to the pin
}

void loop() {
  // Sweep from 0 to 180
  for (int angle = 0; angle <= 180; angle += 1) {
    myServo.write(angle);     // send angle to servo
    delay(15);                // wait for it to reach the position
  }

  // Sweep from 180 to 0
  for (int angle = 180; angle >= 0; angle -= 1) {
    myServo.write(angle);
    delay(15);
  }
}