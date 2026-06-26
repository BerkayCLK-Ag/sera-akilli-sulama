// Ref: docs/malzemeler.md#ldr
// Faz 2 — Modül 10: LDR Isik Sensörü
// Baglanti: 5V → LDR → A1 → 10K direnc → GND (voltaj bolucu)

const int LDR_PIN = A1;

void setup() {
  Serial.begin(9600);
  Serial.println("LDR isik seviyesi okuma");
}

void loop() {
  int raw = analogRead(LDR_PIN);
  int percent = map(raw, 0, 1023, 100, 0); // karanlik=100%, aydinlik=0%

  Serial.print("Ham deger: ");
  Serial.print(raw);
  Serial.print(" | Isik seviyesi: %");
  Serial.println(percent);

  if (percent > 70) Serial.println("  -> Karanlik (bitki isigi dusuk)");
  else if (percent > 40) Serial.println("  -> Orta isik");
  else Serial.println("  -> Aydinlik");

  delay(1000);
}
