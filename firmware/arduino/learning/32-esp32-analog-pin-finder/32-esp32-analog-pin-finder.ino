// ESP32 D1 R32 analog pin bulucu
// Amac: Toprak nem sensörünün AO kablosu gerçekte hangi GPIO'ya gidiyor bulmak.
//
// Board: WEMOS D1 MINI ESP32
// Seri monitor: 115200
//
// Test:
// 1) Toprak sensörü VCC -> 3V3
// 2) Toprak sensörü GND -> GND
// 3) Toprak sensörü AO kablosu neredeyse orada kalsın
// 4) Bu sketch'i yükle
// 5) Probu havada ve suda tut; hangi pinin değeri değişiyor bak

const int ADC_PINS[] = {32, 33, 34, 35, 36, 39};
const int ADC_COUNT = sizeof(ADC_PINS) / sizeof(ADC_PINS[0]);

int analogOrtalamaOku(int pin) {
  long toplam = 0;
  for (int i = 0; i < 20; i++) {
    toplam += analogRead(pin);
    delay(2);
  }
  return toplam / 20;
}

void setup() {
  Serial.begin(115200);
  analogSetAttenuation(ADC_11db);

  Serial.println("=== ESP32 analog pin bulucu ===");
  Serial.println("Toprak sensoru AO hangi GPIO'daysa o deger degisecek.");
  Serial.println("Havada ve suda test et.");
}

void loop() {
  Serial.println("---- ADC OKUMALARI ----");
  for (int i = 0; i < ADC_COUNT; i++) {
    int pin = ADC_PINS[i];
    int deger = analogOrtalamaOku(pin);
    Serial.print("GPIO");
    Serial.print(pin);
    Serial.print(": ");
    Serial.println(deger);
  }
  delay(1500);
}
