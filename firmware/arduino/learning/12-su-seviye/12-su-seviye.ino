// Ref: docs/malzemeler.md#su-seviye
// Faz 2 — Modül 12: Su Seviye Sensörü
// Baglanti: VCC→5V, GND→GND, AO→A3

const int WATER_PIN = A3;

void setup() {
  Serial.begin(9600);
  Serial.println("Su seviye okuma");
}

void loop() {
  int raw = analogRead(WATER_PIN);
  int percent = map(constrain(raw, 0, 600), 0, 600, 0, 100);

  Serial.print("Ham: ");
  Serial.print(raw);
  Serial.print(" | Su seviyesi: %");
  Serial.println(percent);

  if (percent < 10) Serial.println("  -> UYARI: Su deposu bos!");
  else if (percent < 30) Serial.println("  -> Su seviyesi dusuk");
  else Serial.println("  -> Su seviyesi OK");

  delay(2000);
}
