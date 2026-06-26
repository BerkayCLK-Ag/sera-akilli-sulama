// Ref: docs/malzemeler.md#dht11
// Faz 2 — Modül 08: DHT11 Sicaklik ve Nem
// Kurulum: Arduino IDE → Kutuphane Yoneticisi → "DHT sensor library" (Adafruit)
// Baglanti: VCC→5V, GND→GND, DATA→D4 (ESP D2/D3 kullandigi icin)

#include <DHT.h>

#define DHT_PIN 4
#define DHT_TYPE DHT11

DHT dht(DHT_PIN, DHT_TYPE);

void setup() {
  Serial.begin(9600);
  dht.begin();
  Serial.println("DHT11 baslatildi");
}

void loop() {
  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();

  if (isnan(humidity) || isnan(temperature)) {
    Serial.println("DHT11 okuma hatasi!");
    delay(2000);
    return;
  }

  Serial.print("Sicaklik: ");
  Serial.print(temperature);
  Serial.print(" C | Nem: %");
  Serial.println(humidity);

  delay(2000);
}
