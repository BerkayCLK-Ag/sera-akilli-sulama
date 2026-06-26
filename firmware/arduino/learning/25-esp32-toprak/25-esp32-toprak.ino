// Ref: docs/esp32-d1r32-calisma-plani.md
// Faz 2 — Modül 25: ESP32 Toprak Nem Sensörü
//
// Baglanti:
//   Sensör VCC  → 3.3V   (🟠 turuncu)  ← 5V DEGIL! ADC max 3.3V
//   Sensör GND  → GND    (⚫ siyah)
//   Sensör AO   → GPIO36 (🟢 yeşil)   [D1 R32'de A0 etiketi = VP]
//
// Neden GPIO36?
//   ADC1 pinleri (32-39 arası) WiFi acikken güvenilir okur.
//   ADC2 pinleri WiFi ile çakışır — yanliş okuma verir.
//
// Board: WEMOS D1 MINI ESP32 | Baud: 115200

const int TOPRAK_PIN = 36;    // GPIO36 = VP = D1 R32 A0

// Kalibrasyon — KENDI probunla olc, buraya yaz:
//   1) Probu havada tut → Ham deger = KURU_DEGER
//   2) Probu suya batir → Ham deger = ISLAK_DEGER
// Senin ilk olcumlerin: kuru ~0, islak ~186 (3.3V beslemede dusuk aralik normal)
const int KURU_DEGER   = 20;   // Havada (kuru) ham deger
const int ISLAK_DEGER  = 200;  // Suda / cok islak ham deger

void setup() {
  Serial.begin(115200);
  Serial.println("Toprak nem sensoru — ESP32 D1 R32");
  Serial.println("Sensör VCC = 3.3V, AO = GPIO36");
  Serial.println("Kalibrasyon: havada KURU_DEGER, suda ISLAK_DEGER duzenle");
}

void loop() {
  int ham = analogRead(TOPRAK_PIN);   // 0-4095 (12-bit ESP32)
  // Kuru = dusuk ham, islak = yuksek ham (3.3V'de bu modul boyle calisir)
  int yuzde = map(constrain(ham, KURU_DEGER, ISLAK_DEGER),
                  KURU_DEGER, ISLAK_DEGER, 0, 100);

  Serial.print("Ham (12-bit): ");
  Serial.print(ham);
  Serial.print("  |  Nem: %");
  Serial.println(yuzde);

  // Ziraat yorumu
  if (yuzde < 20) {
    Serial.println("  -> KURU: Sulama gerekli");
  } else if (yuzde < 40) {
    Serial.println("  -> NORMAL: Yakında sulama gerekebilir");
  } else if (yuzde < 70) {
    Serial.println("  -> NEMLI: Sulama gerekmiyor");
  } else {
    Serial.println("  -> ÇOK ISLAK: Drenaj kontrolü yap");
  }

  delay(2000);
}
