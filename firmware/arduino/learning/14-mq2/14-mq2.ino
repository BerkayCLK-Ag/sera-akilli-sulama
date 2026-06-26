// Ref: docs/malzemeler.md#mq2
// Faz 2 — Modül 14: MQ-2 Gaz Sensörü
// Baglanti: VCC→5V, GND→GND, AO→A4
// ONEMLI: 20 saniye isinma suresi gerekli

const int MQ2_PIN = A4;
const int WARMUP_MS = 20000;

void setup() {
  Serial.begin(9600);
  Serial.println("MQ-2 isiniyor... 20 saniye bekleyin");
  delay(WARMUP_MS);
  Serial.println("MQ-2 hazir");
}

void loop() {
  int raw = analogRead(MQ2_PIN);
  int percent = map(raw, 0, 1023, 0, 100);

  Serial.print("Ham: ");
  Serial.print(raw);
  Serial.print(" | Gaz seviyesi: %");
  Serial.println(percent);

  if (percent > 60) Serial.println("  -> UYARI: Yuksek gaz/duman!");
  else if (percent > 30) Serial.println("  -> Orta seviye");
  else Serial.println("  -> Normal");

  delay(2000);
}
