// 2'li role modulu test sketch'i
// Kart: WEMOS D1 MINI ESP32
// Seri monitor: 115200
//
// POMPA TAKMA. Sadece role kartinin isigi ve TIK sesi test edilir.
//
// Senin 2'li role sag 4 pin sirasi:
//   GND  IN1  IN2  VCC
//
// Baglanti:
//   GND -> ESP/Breadboard GND
//   VCC -> ESP/Breadboard 5V
//   IN1 -> ESP IO5
//   IN2 -> bos
//
// Cogu role active-low calisir:
//   HIGH = kapali
//   LOW  = acik

const int ROLE_IN1 = 5;  // ESP32 IO5

void setup() {
  Serial.begin(115200);
  pinMode(ROLE_IN1, OUTPUT);
  digitalWrite(ROLE_IN1, HIGH); // baslangicta kapali

  Serial.println("=== 2'li role IN1 test ===");
  Serial.println("POMPA TAKMA. Sadece role isigi/TIK testi.");
  Serial.println("GND->GND, VCC->5V, IN1->IO5, IN2 bos");
  Serial.println("10 sn kapali, 10 sn acik. Multimetre olcumu icin yavaslatildi.");
}

void loop() {
  Serial.println("ROLE KAPALI - 10 sn");
  digitalWrite(ROLE_IN1, HIGH);
  delay(10000);

  Serial.println("ROLE ACIK - 10 sn - LED yanmali / TIK gelmeli");
  digitalWrite(ROLE_IN1, LOW);
  delay(10000);
}
