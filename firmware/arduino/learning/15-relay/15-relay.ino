// Faz 3 — Modül 15: Role TEST sketch'i
// !!! DIKKAT: Bu kod 3 sn acik / 3 sn kapali DONGU yapar — surekli TIK normal!
// Normal sera icin 22-mqtt-publish.ino yukleyin.
// Baglanti: VCC→5V, GND→GND, IN→D5 (D4=DHT11)// UYARI: Pompa/motor ayri guc kaynagindan beslenmeli

const int RELAY_PIN = 5;
const bool RELAY_ACTIVE_LOW = true; // cogu kart LOW=aktif

void setup() {
  pinMode(RELAY_PIN, OUTPUT);
  deactivateRelay();
  Serial.begin(9600);
  Serial.println("!!! ROLE TEST MODU — 3sn ac/kapa dongusu");
  Serial.println("!!! Bitince 22-mqtt-publish.ino yukle!");
}

void activateRelay() {
  digitalWrite(RELAY_PIN, RELAY_ACTIVE_LOW ? LOW : HIGH);
}

void deactivateRelay() {
  digitalWrite(RELAY_PIN, RELAY_ACTIVE_LOW ? HIGH : LOW);
}

void loop() {
  Serial.println("Pompa ACILIYOR");
  activateRelay();
  delay(3000);

  Serial.println("Pompa KAPANIYOR");
  deactivateRelay();
  delay(3000);
}
