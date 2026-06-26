// Ref: docs/malzemeler.md#joystick
// Ek Modül 25: Joystick ile Servo kontrolu

#include <Servo.h>

const int VRX = A0;
const int VRY = A1;
const int SW = 2;
const int SERVO_PIN = 3;

Servo servo;

void setup() {
  pinMode(SW, INPUT_PULLUP);
  servo.attach(SERVO_PIN);
  Serial.begin(9600);
}

void loop() {
  int x = analogRead(VRX);
  int angle = map(x, 0, 1023, 0, 180);
  servo.write(angle);

  if (digitalRead(SW) == LOW) {
    Serial.println("Joystick buton - vana acildi");
    servo.write(180);
    delay(500);
  }

  Serial.print("Servo acisi: ");
  Serial.println(angle);
  delay(200);
}
