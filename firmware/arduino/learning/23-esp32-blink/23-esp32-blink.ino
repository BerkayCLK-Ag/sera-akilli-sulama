// Ref: docs/esp32-d1r32-calisma-plani.md
// Faz 1 — Modül 23: ESP32 D1 R32 İlk Test (Blink + Serial)
//
// Hedef: D1 R32 kartının Arduino IDE'de tanındığını doğrula.
// Board: WEMOS D1 MINI ESP32  veya  ESP32 Dev Module
// Baud:  115200  (UNO'dan farklı — ESP32 standartı)
//
// D1 R32'de built-in LED GPIO2'ye bağlı.

const int LED_PIN = 2;   // D1 R32 dahili LED

void setup() {
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);

  Serial.println("=================================");
  Serial.println("ESP32 D1 R32 — Sera v2 Basliyor");
  Serial.println("=================================");
  Serial.print("CPU Frekans: ");
  Serial.print(getCpuFrequencyMhz());
  Serial.println(" MHz");
  Serial.print("Flash Boyut: ");
  Serial.print(ESP.getFlashChipSize() / 1024 / 1024);
  Serial.println(" MB");
  Serial.print("Serbest RAM: ");
  Serial.print(ESP.getFreeHeap());
  Serial.println(" bayt");
  Serial.println("---------------------------------");
  Serial.println("LED yanik sondi — kart hazir!");
}

void loop() {
  digitalWrite(LED_PIN, HIGH);
  Serial.println("LED: ACIK");
  delay(500);

  digitalWrite(LED_PIN, LOW);
  Serial.println("LED: KAPALI");
  delay(500);
}
