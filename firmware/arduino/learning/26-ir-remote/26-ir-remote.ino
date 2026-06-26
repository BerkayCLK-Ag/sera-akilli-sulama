// Ref: docs/malzemeler.md#ir-kumanda
// Ek Modül 26: IR Kumanda ile pompa kontrolu
// Kurulum: IRremote kutuphanesi

#include <IRremote.h>

const int RECV_PIN = 11;
const int RELAY_PIN = 4;
const bool RELAY_ACTIVE_LOW = true;

IRrecv irrecv(RECV_PIN);
decode_results results;

void setup() {
  pinMode(RELAY_PIN, OUTPUT);
  deactivatePump();
  Serial.begin(9600);
  irrecv.enableIRIn();
  Serial.println("IR kumanda hazir");
}

void activatePump() {
  digitalWrite(RELAY_PIN, RELAY_ACTIVE_LOW ? LOW : HIGH);
}

void deactivatePump() {
  digitalWrite(RELAY_PIN, RELAY_ACTIVE_LOW ? HIGH : LOW);
}

void loop() {
  if (irrecv.decode(&results)) {
    Serial.print("Kod: 0x");
    Serial.println(results.value, HEX);
    // Kumanda tusuna gore kodu ogrenin ve asagiya yazin
    if (results.value == 0xFFA25D) activatePump();
    if (results.value == 0xFF629D) deactivatePump();
    irrecv.resume();
  }
}
