// Ref: docs/esp32-d1r32-calisma-plani.md
// Faz 2 — Modül 24: ESP32 DHT11 Sıcaklık & Nem
//
// Baglanti:
//   DHT11 VCC  → 3.3V  (🟠 turuncu)
//   DHT11 GND  → GND   (⚫ siyah)
//   DHT11 DATA → GPIO4 (🟡 sarı)  [D1 R32'de D15 etiketi]
//
// Kütüphane: DHT sensor library by Adafruit
// Board: WEMOS D1 MINI ESP32 | Baud: 115200

#include <DHT.h>

#define DHT_PIN  4      // GPIO4 = D1 R32'de D15 etiketi
#define DHT_TYPE DHT11

DHT dht(DHT_PIN, DHT_TYPE);

void setup() {
  Serial.begin(115200);
  dht.begin();
  Serial.println("DHT11 hazir — ESP32 D1 R32");
  Serial.println("Baglanti: VCC=3.3V, GND, DATA=GPIO4");
}

void loop() {
  float nem    = dht.readHumidity();
  float sicaklik = dht.readTemperature();

  if (isnan(nem) || isnan(sicaklik)) {
    Serial.println("HATA: DHT11 okunamadi — kablo kontrolu yap");
    delay(2000);
    return;
  }

  Serial.print("Sicaklik: ");
  Serial.print(sicaklik, 1);
  Serial.print(" C  |  Nem: %");
  Serial.println(nem, 1);

  // Ziraat yorumu
  if (sicaklik > 30) {
    Serial.println("  -> UYARI: Sera sicakligi yuksek (>30C)");
  } else if (sicaklik < 10) {
    Serial.println("  -> UYARI: Don riski! (<10C)");
  } else {
    Serial.println("  -> Sicaklik normal aralıkta");
  }

  delay(2000);
}
