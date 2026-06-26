// Ref: docs/malzemeler.md#usb-kablosu
// Faz 1 — Modül 02: Serial Monitor
// Hedef: Seri monitör ile bilgisayar-Arduino iletişimi

void setup() {
  Serial.begin(9600);
  Serial.println("=== Akilli Sera IoT ===");
  Serial.println("Seri monitör calisiyor!");
  Serial.println("Baud rate: 9600");
}

void loop() {
  Serial.print("Zaman (ms): ");
  Serial.println(millis());
  delay(2000);
}
