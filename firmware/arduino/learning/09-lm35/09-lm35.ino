// Ref: docs/malzemeler.md#lm35
// Faz 2 — Modül 09: LM35 Sicaklik Sensörü
// Baglanti: VCC→5V, GND→GND, OUT→A2
// LM35: 10 mV/°C lineer cikis

const int LM35_PIN = A2;

void setup() {
  Serial.begin(9600);
  Serial.println("LM35 sicaklik okuma");
}

void loop() {
  int raw = analogRead(LM35_PIN);
  float voltage = raw * (5.0 / 1023.0);
  float temperature = voltage * 100.0; // 10 mV/°C

  Serial.print("Ham: ");
  Serial.print(raw);
  Serial.print(" | Sicaklik: ");
  Serial.print(temperature, 1);
  Serial.println(" C");

  delay(1000);
}
