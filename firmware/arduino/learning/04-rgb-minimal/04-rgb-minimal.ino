// EN BASIT RGB TEST
// Ref: docs/devre-semalari/04-rgb-baglanti.md
// Oncelik: devre-semalari dosyasindaki baglantiyi BIREBIR yap
// D8 acik = bir renk, D9 acik = bir renk, D10 acik = bir renk

const int D8 = 8;
const int D9 = 9;
const int D10 = 10;

void setup() {
  pinMode(D8, OUTPUT);
  pinMode(D9, OUTPUT);
  pinMode(D10, OUTPUT);
  Serial.begin(9600);
  Serial.println("RGB minimal test basladi");
}

void loop() {
  digitalWrite(D8, LOW);
  digitalWrite(D9, LOW);
  digitalWrite(D10, LOW);

  Serial.println(">>> D8 ACILDI (4 sn)");
  digitalWrite(D8, HIGH);
  delay(4000);

  digitalWrite(D8, LOW);
  Serial.println(">>> D9 ACILDI (4 sn)");
  digitalWrite(D9, HIGH);
  delay(4000);

  digitalWrite(D9, LOW);
  Serial.println(">>> D10 ACILDI (4 sn)");
  digitalWrite(D10, HIGH);
  delay(4000);
}
