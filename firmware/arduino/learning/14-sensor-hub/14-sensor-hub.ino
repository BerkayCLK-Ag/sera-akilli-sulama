// Ref: docs/malzemeler.md — tum sensorler
// Faz 2 — Sensor Hub: Tum sensorleri entegre okuma
// LCD + seri monitör cikti

#include <LiquidCrystal.h>
#include <DHT.h>

#define DHT_PIN 2
#define DHT_TYPE DHT11

LiquidCrystal lcd(8, 9, 10, 11, 12, 13);
DHT dht(DHT_PIN, DHT_TYPE);

const int SOIL_PIN = A0;
const int LDR_PIN = A1;
const int LM35_PIN = A2;
const int WATER_PIN = A3;
const int SOIL_POWER = 7;

void setup() {
  pinMode(SOIL_POWER, OUTPUT);
  Serial.begin(9600);
  dht.begin();
  lcd.begin(16, 2);
  lcd.print("Sensor Hub");
  delay(1000);
  Serial.println("=== Sera Sensor Hub ===");
}

int readSoil() {
  digitalWrite(SOIL_POWER, HIGH);
  delay(10);
  int raw = analogRead(SOIL_PIN);
  digitalWrite(SOIL_POWER, LOW);
  return map(constrain(raw, 300, 1023), 1023, 300, 0, 100);
}

void loop() {
  float temp = dht.readTemperature();
  float hum = dht.readHumidity();
  int soil = readSoil();
  int light = analogRead(LDR_PIN);
  int water = map(constrain(analogRead(WATER_PIN), 0, 600), 0, 600, 0, 100);

  // Seri monitör
  Serial.println("--- Olcum ---");
  Serial.print("Sicaklik: "); Serial.print(temp); Serial.println(" C");
  Serial.print("Nem: %"); Serial.println(hum);
  Serial.print("Toprak nemi: %"); Serial.println(soil);
  Serial.print("Isik (ham): "); Serial.println(light);
  Serial.print("Su seviyesi: %"); Serial.println(water);

  // LCD
  lcd.setCursor(0, 0);
  lcd.print("T:");
  lcd.print(temp, 0);
  lcd.print("C H:%");
  lcd.print(hum, 0);
  lcd.print("   ");
  lcd.setCursor(0, 1);
  lcd.print("Toprak:%");
  lcd.print(soil);
  lcd.print(" Su:%");
  lcd.print(water);
  lcd.print(" ");

  delay(3000);
}
