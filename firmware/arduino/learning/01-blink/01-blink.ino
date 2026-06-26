// Modul 01 - Blink
// Ref: docs/malzemeler.md
// Kart uzerindeki L LED'i (pin 13) yanip soner

const int LED_PIN = LED_BUILTIN;

void setup() {
  pinMode(LED_PIN, OUTPUT);
}

void loop() {
  digitalWrite(LED_PIN, HIGH);
  delay(1000);
  digitalWrite(LED_PIN, LOW);
  delay(1000);
}
