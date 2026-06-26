// Ref: docs/malzemeler.md#buzzer
// Faz 3 — Modül 18: Buzzer Alarm
// Baglanti: +→D5, -→GND (aktif buzzer) veya PWM ile ton (pasif)

const int BUZZER_PIN = 5;

void setup() {
  pinMode(BUZZER_PIN, OUTPUT);
  Serial.begin(9600);
  Serial.println("Buzzer alarm testi");
}

void alarmShort() {
  for (int i = 0; i < 3; i++) {
    tone(BUZZER_PIN, 1000, 200);
    delay(300);
  }
}

void alarmLong() {
  tone(BUZZER_PIN, 500, 1000);
  delay(1200);
}

void loop() {
  Serial.println("Kisa alarm (nem dusuk)");
  alarmShort();
  delay(2000);

  Serial.println("Uzun alarm (kritik)");
  alarmLong();
  delay(2000);
}
