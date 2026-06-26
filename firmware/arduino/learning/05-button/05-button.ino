// Ref: docs/malzemeler.md#push-buton, docs/malzemeler.md#led-yesil
// Faz 1 — Modül 05: Push Buton
// Buton: bir bacak D2, diger GND. D2'ye 10K pull-down (veya INPUT_PULLUP)
// LED: D8 → 330Ω → GND

const int BUTTON_PIN = 2;
const int LED_PIN = 8;
bool ledState = false;

void setup() {
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  pinMode(LED_PIN, OUTPUT);
  Serial.begin(9600);
  Serial.println("Butona basin - LED degisecek");
}

void loop() {
  if (digitalRead(BUTTON_PIN) == LOW) {
    delay(50); // debounce
    if (digitalRead(BUTTON_PIN) == LOW) {
      ledState = !ledState;
      digitalWrite(LED_PIN, ledState ? HIGH : LOW);
      Serial.println(ledState ? "LED ACILDI" : "LED KAPANDI");
      while (digitalRead(BUTTON_PIN) == LOW) delay(10); // buton birakilana kadar bekle
    }
  }
}
