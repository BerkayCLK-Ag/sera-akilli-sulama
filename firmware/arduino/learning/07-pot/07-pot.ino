// Ref: docs/malzemeler.md#potansiyometre-10k
// Faz 1 — Modül 07: Potansiyometre
// Pot: dis uclar VCC/GND, ortanca → A0
// Seri monitörde 0-1023 ham deger ve 0-100% yuzde gosterir

const int POT_PIN = A0;

void setup() {
  Serial.begin(9600);
  Serial.println("Potansiyometre okuma - Sulama esik simulasyonu");
  Serial.println("Cevirerek nem esigi ayarlayin (0-100%)");
}

void loop() {
  int raw = analogRead(POT_PIN);
  int percent = map(raw, 0, 1023, 0, 100);

  Serial.print("Ham deger: ");
  Serial.print(raw);
  Serial.print(" | Nem esigi: %");
  Serial.println(percent);

  delay(500);
}
