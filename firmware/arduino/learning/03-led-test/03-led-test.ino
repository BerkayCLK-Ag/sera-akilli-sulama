// Modul 03 - LED test (hangi pin hangi LED?)
// Her pin 3 saniye yanar - hangi renk yandigini not edin

const int PIN_KIRMIZI = 8;
const int PIN_YESIL   = 9;
const int PIN_SARI    = 10;

void setup() {
  pinMode(PIN_KIRMIZI, OUTPUT);
  pinMode(PIN_YESIL, OUTPUT);
  pinMode(PIN_SARI, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  digitalWrite(PIN_KIRMIZI, LOW);
  digitalWrite(PIN_YESIL, LOW);
  digitalWrite(PIN_SARI, LOW);

  Serial.println(">>> D8 - Kirmizi yanmali");
  digitalWrite(PIN_KIRMIZI, HIGH);
  delay(3000);

  digitalWrite(PIN_KIRMIZI, LOW);
  Serial.println(">>> D9 - Yesil yanmali");
  digitalWrite(PIN_YESIL, HIGH);
  delay(3000);

  digitalWrite(PIN_YESIL, LOW);
  Serial.println(">>> D10 - Sari yanmali");
  digitalWrite(PIN_SARI, HIGH);
  delay(3000);
}
