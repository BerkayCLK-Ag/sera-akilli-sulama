// Ref: docs/malzemeler.md#hc-sr04
// Faz 2 — Modül 13: HC-SR04 Ultrasonik Mesafe
// Baglanti: VCC→5V, GND→GND, TRIG→D6, ECHO→D7

const int TRIG_PIN = 6;
const int ECHO_PIN = 7;

void setup() {
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  Serial.begin(9600);
  Serial.println("HC-SR04 mesafe olcumu (su seviyesi alternatifi)");
}

long readDistanceCM() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  long duration = pulseIn(ECHO_PIN, HIGH, 30000);
  return duration * 0.034 / 2;
}

void loop() {
  long distance = readDistanceCM();

  if (distance <= 0 || distance > 400) {
    Serial.println("Mesafe okunamadi");
  } else {
    Serial.print("Mesafe: ");
    Serial.print(distance);
    Serial.println(" cm");
    // Depo yuksekligi 30 cm ise: seviye = (30 - distance) / 30 * 100
    int level = constrain(map(distance, 5, 30, 100, 0), 0, 100);
    Serial.print("Tahmini su seviyesi: %");
    Serial.println(level);
  }

  delay(1000);
}
