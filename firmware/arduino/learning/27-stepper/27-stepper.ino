// Ref: docs/malzemeler.md#step-motor
// Ek Modül 27: Step Motor — havalandirma perdesi

const int IN1 = 8, IN2 = 9, IN3 = 10, IN4 = 11;
const int stepsPerRevolution = 2048;

void setup() {
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  Serial.begin(9600);
  Serial.println("Step motor perde kontrolu");
}

void stepMotor(int steps) {
  const int seq[4][4] = {
    {1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 1, 0}, {0, 0, 0, 1}
  };
  for (int i = 0; i < abs(steps); i++) {
    for (int phase = 0; phase < 4; phase++) {
      digitalWrite(IN1, seq[phase][0]);
      digitalWrite(IN2, seq[phase][1]);
      digitalWrite(IN3, seq[phase][2]);
      digitalWrite(IN4, seq[phase][3]);
      delay(2);
    }
  }
}

void loop() {
  Serial.println("Perde ACILIYOR");
  stepMotor(stepsPerRevolution / 4);
  delay(2000);
  Serial.println("Perde KAPANIYOR");
  stepMotor(-stepsPerRevolution / 4);
  delay(2000);
}
