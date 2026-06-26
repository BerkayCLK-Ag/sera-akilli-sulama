// Ref: docs/malzemeler.md#dc-motor, docs/malzemeler.md#bc547
// Faz 3 — Modül 17: DC Motor (BC547 ile)
// Baglanti: Baz→D4 (1K direnc ile), Kollektor→motor-, Emiter→GND
// Motor+: ayri guc (5V), flyback diyot paralel (1N4007 onerilir)

const int MOTOR_PIN = 4;

void setup() {
  pinMode(MOTOR_PIN, OUTPUT);
  Serial.begin(9600);
  Serial.println("DC motor pompa simulasyonu");
}

void loop() {
  Serial.println("Motor CALISIYOR (3 sn)");
  analogWrite(MOTOR_PIN, 200);
  delay(3000);

  Serial.println("Motor DURDU");
  analogWrite(MOTOR_PIN, 0);
  delay(3000);
}
