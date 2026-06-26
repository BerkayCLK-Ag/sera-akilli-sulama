// Ref: docs/esp32-d1r32-calisma-plani.md
// Faz 2 — Modül 26: ESP32 Su Seviye Sensörü
//
// Baglanti:
//   Sensör VCC  → 3.3V   (🟠 turuncu)
//   Sensör GND  → GND    (⚫ siyah)
//   Sensör AO   → GPIO39 (🔵 mavi)   [D1 R32'de A1 etiketi = VN]
//
// Neden GPIO39?
//   ADC1, WiFi güvenli. Giriş-only pin — sensör okuma için yeterli.
//
// Ziraat bağlamı:
//   Su seviye sensörü = depo güvenlik sistemi.
//   Depo boşken pompa çalışırsa motor yanar (kuru çalışma).
//   Bu sensör o korumayı sağlar.
//
// Board: WEMOS D1 MINI ESP32 | Baud: 115200

const int SU_PIN = 39;    // GPIO39 = VN = D1 R32 A1

// Kalibrasyon — kendi probunla olc:
//   Havada: Ham ~0  |  Tam suda: Ham ~1500-2200
const int BOŞ_DEĞER = 0;      // Proba hava (depo bos)
const int DOLU_DEĞER = 2200;  // Proba tam suya batik (depo dolu)

void setup() {
  Serial.begin(115200);
  Serial.println("Su seviye sensoru — ESP32 D1 R32");
  Serial.println("VCC=3.3V, AO=GPIO39");
  Serial.println("Probu su kabina batir ve gozlemle");
}

void loop() {
  int ham = analogRead(SU_PIN);
  int yuzde = map(constrain(ham, BOŞ_DEĞER, DOLU_DEĞER),
                  BOŞ_DEĞER, DOLU_DEĞER, 0, 100);

  Serial.print("Ham: ");
  Serial.print(ham);
  Serial.print("  |  Su seviyesi: %");
  Serial.println(yuzde);

  if (yuzde < 10) {
    Serial.println("  -> DEPO BOŞ: Pompa kapatilmali!");
  } else if (yuzde < 30) {
    Serial.println("  -> Düsük: Su ekle");
  } else {
    Serial.println("  -> Yeterli su mevcut");
  }

  delay(2000);
}
