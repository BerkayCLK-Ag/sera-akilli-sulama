// RGB basit test - once bunu yukle
// Her pin 4 saniye acik kalir, seri monitore bak

const int D_R = 8;
const int D_G = 9;
const int D_B = 10;

void setup() {
  pinMode(D_R, OUTPUT);
  pinMode(D_G, OUTPUT);
  pinMode(D_B, OUTPUT);
  Serial.begin(9600);
  Serial.println("RGB test - ortak katot: COM=GND, renkler D8 D9 D10");
}

void loop() {
  digitalWrite(D_R, LOW);
  digitalWrite(D_G, LOW);
  digitalWrite(D_B, LOW);

  Serial.println("D8 ACILDI");
  digitalWrite(D_R, HIGH);
  delay(4000);

  digitalWrite(D_R, LOW);
  Serial.println("D9 ACILDI");
  digitalWrite(D_G, HIGH);
  delay(4000);

  digitalWrite(D_G, LOW);
  Serial.println("D10 ACILDI");
  digitalWrite(D_B, HIGH);
  delay(4000);
}
