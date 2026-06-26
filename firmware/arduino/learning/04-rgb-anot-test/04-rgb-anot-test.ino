// ORTAK ANOT RGB testi
// Uzun bacak 5V (+) kirmizi hatta, renk bacaklari D8 D9 D10 (direnc ile)
// Bu kodde pin LOW = LED yanar

const int D_R = 8;
const int D_G = 9;
const int D_B = 10;

void setup() {
  pinMode(D_R, OUTPUT);
  pinMode(D_G, OUTPUT);
  pinMode(D_B, OUTPUT);
  digitalWrite(D_R, HIGH);
  digitalWrite(D_G, HIGH);
  digitalWrite(D_B, HIGH);
  Serial.begin(9600);
  Serial.println("ORTAK ANOT test - uzun bacak 5V (+) hatta olmali");
}

void loop() {
  digitalWrite(D_R, HIGH);
  digitalWrite(D_G, HIGH);
  digitalWrite(D_B, HIGH);

  Serial.println("D8 - bir renk yanmali");
  digitalWrite(D_R, LOW);
  delay(4000);
  digitalWrite(D_R, HIGH);

  Serial.println("D9 - bir renk yanmali");
  digitalWrite(D_G, LOW);
  delay(4000);
  digitalWrite(D_G, HIGH);

  Serial.println("D10 - bir renk yanmali");
  digitalWrite(D_B, LOW);
  delay(4000);
  digitalWrite(D_B, HIGH);
}
