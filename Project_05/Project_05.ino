#include <Servo.h>

Servo myServo;
int const potPin = A0;
int const servoPin = 9;
int const comRate = 9600;
int potValue;
int angleValue;

void setup() {
  // put your setup code here, to run once:
  myServo.attach(servoPin);
  Serial.begin(comRate);
}

void loop() {
  // put your main code here, to run repeatedly:
  potValue = analogRead(potPin);
  Serial.print("Pot value: ");
  Serial.print(potValue);
  Serial.print("\n");

  angleValue = map(potValue, 0, 1023, 0, 179);
  Serial.print("Angle value: ");
  Serial.print(angleValue);
  Serial.print("\n");

  myServo.write(angleValue);

  delay(15);
}
