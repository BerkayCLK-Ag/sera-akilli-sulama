// Ref: docs/malzemeler.md#pir
// Faz 3 — Modül 19: PIR Hareket Sensörü
// Baglanti: VCC→5V, GND→GND, OUT→D2
// Isinma: ~30 saniye

const int PIR_PIN = 2;
const int LED_PIN = 13;

void setup() {
  pinMode(PIR_PIN, INPUT);
  pinMode(LED_PIN, OUTPUT);
  Serial.begin(9600);
  Serial.println("PIR isiniyor... 30 saniye");
  delay(30000);
  Serial.println("PIR hazir - hareket bekleniyor");
}

void loop() {
  int motion = digitalRead(PIR_PIN);
  digitalWrite(LED_PIN, motion);

  if (motion == HIGH) {
    Serial.println("HAREKET ALGILANDI!");
  }
  delay(500);
}
