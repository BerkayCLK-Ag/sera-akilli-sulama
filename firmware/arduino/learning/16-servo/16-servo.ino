// Ref: docs/malzemeler.md#servo-sg90
// Faz 3 — Modül 16: Servo Motor
// Baglanti: signal→D3, VCC→5V, GND→GND

#include <Servo.h>

Servo valveServo;
const int SERVO_PIN = 3;

void setup() {
  valveServo.attach(SERVO_PIN);
  Serial.begin(9600);
  Serial.println("Servo vana kontrolu - 0-180 derece");
}

void loop() {
  Serial.println("Vana ACILIK (180°)");
  valveServo.write(180);
  delay(2000);

  Serial.println("Vana KAPALI (0°)");
  valveServo.write(0);
  delay(2000);

  Serial.println("Vana YARIM (90°)");
  valveServo.write(90);
  delay(2000);
}
