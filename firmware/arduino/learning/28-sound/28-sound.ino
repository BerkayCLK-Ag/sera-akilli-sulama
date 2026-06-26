// Ref: docs/malzemeler.md#ses-sensoru
// Ek Modül 28: Ses sensörü

const int SOUND_PIN = A0;
const int THRESHOLD = 600;

void setup() {
  Serial.begin(9600);
  Serial.println("Ses sensoru - anormal ses algilama");
}

void loop() {
  int level = analogRead(SOUND_PIN);
  Serial.print("Ses seviyesi: ");
  Serial.println(level);

  if (level > THRESHOLD) {
    Serial.println("  -> ANORMAL SES ALGILANDI!");
  }
  delay(200);
}
