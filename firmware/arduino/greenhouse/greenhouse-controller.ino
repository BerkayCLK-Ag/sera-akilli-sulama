// Ref: docs/malzemeler.md — Akilli Sera Prototipi (Faz 7)
// Entegre firmware: sensor okuma + otomatik sulama + MQTT + LCD + alarm
//
// Pin ozeti: docs/malzemeler.md#arduino-uno-r3
// Mimari: docs/mimari.md

#include <SoftwareSerial.h>
#include <LiquidCrystal.h>
#include <DHT.h>
#include <Servo.h>

// --- Pin tanimlari ---
#define DHT_PIN 2
#define SERVO_PIN 3
#define RELAY_PIN 4
#define BUZZER_PIN 5
#define TRIG_PIN 6
#define ECHO_PIN 7
#define SOIL_POWER 7
#define SOIL_PIN A0
#define LDR_PIN A1
#define LM35_PIN A2
#define WATER_PIN A3

#define ESP_RX 0
#define ESP_TX 1

// --- WiFi / MQTT (duzenleyin) ---
const char* WIFI_SSID = "WIFI_ADINIZ";
const char* WIFI_PASS = "WIFI_SIFRENIZ";
const char* MQTT_BROKER = "192.168.1.100";
const char* DEVICE_ID = "greenhouse-01";

// --- Sulama kurallari ---
const int MOISTURE_THRESHOLD = 30;
const int PUMP_DURATION_MS = 5000;
const int TEMP_ALARM = 35;
const bool RELAY_ACTIVE_LOW = true;

// --- Nesneler ---
LiquidCrystal lcd(8, 9, 10, 11, 12, 13);
DHT dht(DHT_PIN, DHT11);
Servo valveServo;
SoftwareSerial esp(ESP_RX, ESP_TX);

unsigned long lastSensorRead = 0;
unsigned long lastMqttPublish = 0;
unsigned long lastStatusPublish = 0;
const unsigned long SENSOR_INTERVAL = 5000;
const unsigned long MQTT_INTERVAL = 30000;
const unsigned long STATUS_INTERVAL = 60000;

bool pumpRunning = false;
bool wifiConnected = false;

void setup() {
  pinMode(RELAY_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(SOIL_POWER, OUTPUT);
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  deactivatePump();
  valveServo.attach(SERVO_PIN);
  valveServo.write(0);

  Serial.begin(9600);
  esp.begin(115200);
  dht.begin();
  lcd.begin(16, 2);
  lcd.print("Akilli Sera");
  lcd.setCursor(0, 1);
  lcd.print("Baslatiliyor...");

  initESP8266();

  Serial.println("=== Akilli Sera Controller ===");
  Serial.println("Ref: docs/malzemeler.md");
}

void initESP8266() {
  delay(1000);
  sendAT("AT+RST", 3000);
  sendAT("AT+CWMODE=1", 2000);
  String cmd = String("AT+CWJAP=\"") + WIFI_SSID + "\",\"" + WIFI_PASS + "\"";
  esp.println(cmd);
  waitESP(10000);
  wifiConnected = true;
  lcd.setCursor(0, 1);
  lcd.print("WiFi OK         ");
}

void loop() {
  unsigned long now = millis();

  if (now - lastSensorRead >= SENSOR_INTERVAL) {
    lastSensorRead = now;
    readAndDisplaySensors();
    checkIrrigation();
  }

  if (now - lastMqttPublish >= MQTT_INTERVAL) {
    lastMqttPublish = now;
    publishAllSensors();
  }

  if (now - lastStatusPublish >= STATUS_INTERVAL) {
    lastStatusPublish = now;
    publishStatus();
  }

  checkIncomingCommands();
  delay(50);
}

// --- Sensor okuma ---

int readSoilMoisture() {
  digitalWrite(SOIL_POWER, HIGH);
  delay(10);
  int raw = analogRead(SOIL_PIN);
  digitalWrite(SOIL_POWER, LOW);
  return map(constrain(raw, 300, 1023), 1023, 300, 0, 100);
}

int readWaterLevel() {
  int raw = analogRead(WATER_PIN);
  return map(constrain(raw, 0, 600), 0, 600, 0, 100);
}

void readAndDisplaySensors() {
  float temp = dht.readTemperature();
  float hum = dht.readHumidity();
  int soil = readSoilMoisture();
  int water = readWaterLevel();
  int light = analogRead(LDR_PIN);

  if (isnan(temp)) temp = 0;
  if (isnan(hum)) hum = 0;

  Serial.print("T:"); Serial.print(temp);
  Serial.print(" H:"); Serial.print(hum);
  Serial.print(" Toprak:"); Serial.print(soil);
  Serial.print(" Su:"); Serial.println(water);

  lcd.setCursor(0, 0);
  lcd.print("T:");
  lcd.print(temp, 0);
  lcd.print(" H:");
  lcd.print(hum, 0);
  lcd.print("%    ");
  lcd.setCursor(0, 1);
  lcd.print("Toprak:");
  lcd.print(soil);
  lcd.print("% Su:");
  lcd.print(water);
  lcd.print("%  ");

  if (temp > TEMP_ALARM) {
    tone(BUZZER_PIN, 1000, 500);
  }
  if (water < 10) {
    tone(BUZZER_PIN, 500, 1000);
  }
}

// --- Otomatik sulama ---

void checkIrrigation() {
  if (pumpRunning) return;

  int soil = readSoilMoisture();
  if (soil < MOISTURE_THRESHOLD) {
    Serial.println("OTOMATIK SULAMA BASLIYOR");
    runPump(PUMP_DURATION_MS);
    valveServo.write(180);
    delay(1000);
    valveServo.write(0);
  }
}

void runPump(int durationMs) {
  pumpRunning = true;
  activatePump();
  lcd.setCursor(0, 1);
  lcd.print("SULAMA AKTIF    ");
  delay(durationMs);
  deactivatePump();
  pumpRunning = false;
}

void activatePump() {
  digitalWrite(RELAY_PIN, RELAY_ACTIVE_LOW ? LOW : HIGH);
}

void deactivatePump() {
  digitalWrite(RELAY_PIN, RELAY_ACTIVE_LOW ? HIGH : LOW);
}

// --- MQTT (ESP8266 AT modu, basitlestirilmis) ---

void publishAllSensors() {
  float temp = dht.readTemperature();
  float hum = dht.readHumidity();
  int soil = readSoilMoisture();
  int light = analogRead(LDR_PIN);
  int water = readWaterLevel();

  if (!isnan(temp)) mqttPublish("temperature", temp, "C");
  if (!isnan(hum)) mqttPublish("humidity", hum, "%");
  mqttPublish("soil_moisture", soil, "%");
  mqttPublish("light", light, "raw");
  mqttPublish("water_level", water, "%");
}

void mqttPublish(const char* sensor, float value, const char* unit) {
  String topic = String("sera/") + DEVICE_ID + "/sensor/" + sensor;
  String payload = String("{\"value\":") + value + ",\"unit\":\"" + unit + "\"}";
  Serial.print("MQTT >> ");
  Serial.print(topic);
  Serial.print(" ");
  Serial.println(payload);
  // Gercek ortamda: PubSubClient veya ESP8266 Arduino core ile publish
}

void publishStatus() {
  String payload = String("{\"online\":") + (wifiConnected ? "true" : "false");
  payload += ",\"uptime\":" + String(millis() / 1000) + "}";
  Serial.print("MQTT status: ");
  Serial.println(payload);
}

void checkIncomingCommands() {
  static String buffer = "";
  while (esp.available()) {
    char c = esp.read();
    buffer += c;
    if (buffer.length() > 200) buffer = buffer.substring(buffer.length() - 100);

    if (buffer.indexOf("\"state\":\"on\"") >= 0) {
      int dur = 5;
      int idx = buffer.indexOf("\"duration\":");
      if (idx >= 0) dur = buffer.substring(idx + 11, idx + 13).toInt();
      if (dur <= 0) dur = 5;
      runPump(dur * 1000);
      buffer = "";
    } else if (buffer.indexOf("\"state\":\"off\"") >= 0) {
      deactivatePump();
      pumpRunning = false;
      buffer = "";
    }

    int angleIdx = buffer.indexOf("\"angle\":");
    if (angleIdx >= 0) {
      int angle = buffer.substring(angleIdx + 8, angleIdx + 11).toInt();
      valveServo.write(constrain(angle, 0, 180));
      buffer = "";
    }
  }
}

void sendAT(const char* cmd, int timeout) {
  esp.println(cmd);
  waitESP(timeout);
}

void waitESP(int timeout) {
  unsigned long start = millis();
  while (millis() - start < (unsigned long)timeout) {
    if (esp.available()) Serial.write(esp.read());
  }
}
