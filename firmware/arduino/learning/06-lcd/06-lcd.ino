// Ref: docs/malzemeler.md#lcd-2x16
// Faz 1 — Modül 06: LCD Ekran (4-bit mod)
// RS→D8, E→D9, D4→D10, D5→D11, D6→D12, D7→D13
// VSS→GND, VDD→5V, V0→pot ortanca (kontrast), A→5V, K→GND

#include <LiquidCrystal.h>

LiquidCrystal lcd(8, 9, 10, 11, 12, 13);

void setup() {
  lcd.begin(16, 2);
  lcd.print("Merhaba Sera!");
  lcd.setCursor(0, 1);
  lcd.print("IoT Prototip v1");
  Serial.begin(9600);
  Serial.println("LCD hazir");
}

void loop() {
  lcd.setCursor(0, 1);
  lcd.print("Uptime: ");
  lcd.print(millis() / 1000);
  lcd.print(" sn  ");
  delay(1000);
}
