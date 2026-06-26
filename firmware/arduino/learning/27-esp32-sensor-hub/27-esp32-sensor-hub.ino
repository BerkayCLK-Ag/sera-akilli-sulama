// Ref: docs/esp32-d1r32-calisma-plani.md
// Faz 2 — Modül 27: ESP32 Tüm Sensörler (Sensor Hub)
//
// Tüm sensörler tek sketch'te — WiFi olmadan lokal test.
//
// Bağlantı özeti:
//   DHT11  VCC → 3.3V (🟠)  GND → GND (⚫)  DATA → GPIO4 (🟡)
//   Toprak VCC → 3.3V (🟠)  GND → GND (⚫)  AO   → GPIO36 (🟢)
//   Su Sev VCC → 3.3V (🟠)  GND → GND (⚫)  AO   → GPIO39 (🔵)
//
// Board: WEMOS D1 MINI ESP32 | Baud: 115200

#include <DHT.h>

#define DHT_PIN      4     // GPIO4
#define TOPRAK_PIN   36    // GPIO36 = VP
#define SU_PIN       39    // GPIO39 = VN
#define DHT_TYPE     DHT11

DHT dht(DHT_PIN, DHT_TYPE);

// Kalibrasyon (25-esp32-toprak'tan ölçerek doldur)
const int TOPRAK_KURU  = 20;
const int TOPRAK_ISLAK = 200;
const int SU_BOS       = 0;
const int SU_DOLU      = 2200;

struct SensorVerisi {
  float sicaklik;
  float nem;
  int toprakHam;
  int toprakYuzde;
  int suHam;
  int suYuzde;
  bool dhtHata;
};

SensorVerisi oku() {
  SensorVerisi v;
  v.sicaklik = dht.readTemperature();
  v.nem      = dht.readHumidity();
  v.dhtHata  = isnan(v.sicaklik) || isnan(v.nem);

  v.toprakHam   = analogRead(TOPRAK_PIN);
  v.toprakYuzde = map(constrain(v.toprakHam, TOPRAK_KURU, TOPRAK_ISLAK),
                      TOPRAK_KURU, TOPRAK_ISLAK, 0, 100);

  v.suHam   = analogRead(SU_PIN);
  v.suYuzde = map(constrain(v.suHam, SU_BOS, SU_DOLU),
                  SU_BOS, SU_DOLU, 0, 100);
  return v;
}

void yazdir(const SensorVerisi& v) {
  Serial.println("===== SENSOR HUB =====");
  if (v.dhtHata) {
    Serial.println("DHT11: HATA — kablo kontrol");
  } else {
    Serial.print("Sicaklik : "); Serial.print(v.sicaklik, 1); Serial.println(" C");
    Serial.print("Hava nem : %"); Serial.println(v.nem, 1);
  }
  Serial.print("Toprak   : %"); Serial.print(v.toprakYuzde);
  Serial.print("  (ham:"); Serial.print(v.toprakHam); Serial.println(")");
  Serial.print("Su dep.  : %"); Serial.print(v.suYuzde);
  Serial.print("  (ham:"); Serial.print(v.suHam); Serial.println(")");
  Serial.println("======================");
}

void setup() {
  Serial.begin(115200);
  dht.begin();
  Serial.println("Sensor Hub — ESP32 D1 R32");
  Serial.println("Tum sensorler 3.3V besleniyor");
  delay(1000);
}

void loop() {
  SensorVerisi veri = oku();
  yazdir(veri);
  delay(2000);
}
