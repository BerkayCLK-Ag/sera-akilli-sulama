// Ref: docs/malzemeler.md#hc06-bluetooth
// Ek Modül 23: HC-06 Bluetooth yerel kontrol

#include <SoftwareSerial.h>

SoftwareSerial bt(2, 3); // RX, TX

const int RELAY_PIN = 4;
const bool RELAY_ACTIVE_LOW = true;

void setup() {
  pinMode(RELAY_PIN, OUTPUT);
  deactivatePump();
  Serial.begin(9600);
  bt.begin(9600);
  Serial.println("HC-06 Bluetooth hazir - '1'=pompa ac, '0'=kapat");
}

void activatePump() {
  digitalWrite(RELAY_PIN, RELAY_ACTIVE_LOW ? LOW : HIGH);
}

void deactivatePump() {
  digitalWrite(RELAY_PIN, RELAY_ACTIVE_LOW ? HIGH : LOW);
}

void loop() {
  if (bt.available()) {
    char cmd = bt.read();
    if (cmd == '1') {
      activatePump();
      bt.println("Pompa ACILDI");
    } else if (cmd == '0') {
      deactivatePump();
      bt.println("Pompa KAPANDI");
    }
  }
}
