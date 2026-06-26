// Ref: docs/malzemeler.md#toprak-nem, docs/malzemeler.md#role-5v, docs/malzemeler.md#buzzer
// Faz 3 — Otomatik Sulama: Nem < esik → pompa 5 sn
// Potansiyometre A0 ile esik ayari (0-100%)

const int SOIL_PIN = A0;
const int SOIL_POWER = 7;
const int RELAY_PIN = 4;
const int BUZZER_PIN = 5;
const int MOISTURE_THRESHOLD = 30; // %30 altinda sulama (pot ile ayar icin A5 kullanilabilir)
const int PUMP_DURATION_MS = 5000;
const bool RELAY_ACTIVE_LOW = true;

void setup() {
  pinMode(SOIL_POWER, OUTPUT);
  pinMode(RELAY_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  deactivatePump();
  Serial.begin(9600);
  Serial.println("=== Otomatik Sulama Sistemi ===");
  Serial.print("Esik degeri: %");
  Serial.println(MOISTURE_THRESHOLD);
}

void activatePump() {
  digitalWrite(RELAY_PIN, RELAY_ACTIVE_LOW ? LOW : HIGH);
}

void deactivatePump() {
  digitalWrite(RELAY_PIN, RELAY_ACTIVE_LOW ? HIGH : LOW);
}

int readSoilMoisture() {
  digitalWrite(SOIL_POWER, HIGH);
  delay(10);
  int raw = analogRead(SOIL_PIN);
  digitalWrite(SOIL_POWER, LOW);
  return map(constrain(raw, 300, 1023), 1023, 300, 0, 100);
}

void loop() {
  int moisture = readSoilMoisture();

  Serial.print("Toprak nemi: %");
  Serial.println(moisture);

  if (moisture < MOISTURE_THRESHOLD) {
    Serial.println("SULAMA BASLIYOR!");
    tone(BUZZER_PIN, 800, 300);
    activatePump();
    delay(PUMP_DURATION_MS);
    deactivatePump();
    Serial.println("Sulama tamamlandi");
    delay(10000); // tekrar sulamadan once bekle
  } else {
    Serial.println("Nem yeterli - sulama gerekmiyor");
  }

  delay(5000);
}
