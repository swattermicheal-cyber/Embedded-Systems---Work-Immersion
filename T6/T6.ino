#include <Servo.h>

Servo lockServo;

void setup() {
  lockServo.attach(6);
  lockServo.write(0);        // locked
}

void loop() {
  lockServo.write(90);       // unlocked
  delay(2000);
  lockServo.write(0);        // locked
  delay(2000);
}