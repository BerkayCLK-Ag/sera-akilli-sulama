// Ref: docs/malzemeler.md#toprak-nem
// Faz 2 — Modül 11: Toprak Nem Sensörü
// Baglanti: VCC→5V, GND→GND, AO→A0
// ONEMLI: BC547 yoksa SOIL_POWER -1 birak (VCC dogrudan 5V hattina)

const int SOIL_PIN = A0;
const int SOIL_POWER = -1; // BC547 varsa 7 yap

void setup() {
  if (SOIL_POWER >= 0) pinMode(SOIL_POWER, OUTPUT);
  Serial.begin(9600);
  Serial.println("Toprak nem okuma");
}

int readSoilMoisture() {
  if (SOIL_POWER >= 0) {
    digitalWrite(SOIL_POWER, HIGH);
    delay(10);
  }
  int raw = analogRead(SOIL_PIN);
  if (SOIL_POWER >= 0) digitalWrite(SOIL_POWER, LOW);
  return raw;
}

void loop() {
  int raw = readSoilMoisture();
  // Kuru ~1023, Islak ~300 — map ters
  int percent = map(constrain(raw, 300, 1023), 1023, 300, 0, 100);

  Serial.print("Ham: ");
  Serial.print(raw);
  Serial.print(" | Toprak nemi: %");
  Serial.println(percent);

  if (percent < 30) Serial.println("  -> SULAMA GEREKLI!");
  else if (percent < 50) Serial.println("  -> Nem dusuk");
  else Serial.println("  -> Nem yeterli");

  delay(3000);
}
