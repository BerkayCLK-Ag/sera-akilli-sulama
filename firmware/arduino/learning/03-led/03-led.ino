// Ref: docs/malzemeler.md#led-kirmizi, docs/malzemeler.md#led-yesil, docs/malzemeler.md#led-sari
// Faz 1 — Modül 03: LED Kontrol
// Bağlantı: LED anot → 330Ω direnç → D8/D9/D10, katot → GND

const int LED_RED   = 8;
const int LED_GREEN = 9;
const int LED_YELLOW = 10;

void setup() {
  pinMode(LED_RED, OUTPUT);
  pinMode(LED_GREEN, OUTPUT);
  pinMode(LED_YELLOW, OUTPUT);
  Serial.begin(9600);
  Serial.println("LED testi basladi");
}

void loop() {
  // Kirmizi: alarm durumu simulasyonu
  digitalWrite(LED_RED, HIGH);
  digitalWrite(LED_GREEN, LOW);
  digitalWrite(LED_YELLOW, LOW);
  Serial.println("Durum: ALARM (kirmizi)");
  delay(1000);

  // Sari: uyari
  digitalWrite(LED_RED, LOW);
  digitalWrite(LED_GREEN, LOW);
  digitalWrite(LED_YELLOW, HIGH);
  Serial.println("Durum: UYARI (sari)");
  delay(1000);

  // Yesil: normal
  digitalWrite(LED_RED, LOW);
  digitalWrite(LED_GREEN, HIGH);
  digitalWrite(LED_YELLOW, LOW);
  Serial.println("Durum: NORMAL (yesil)");
  delay(1000);
}
