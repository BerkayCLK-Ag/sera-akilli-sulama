// Ref: docs/malzemeler.md#esp8266, docs/malzemeler.md#role-5v
// Faz 4 — Modül 22b: MQTT Subscribe — Pompa komutu alma
// Topic: sera/greenhouse-01/actuator/pump
// Payload: {"state":"on","duration":5}

#include <SoftwareSerial.h>

SoftwareSerial esp(0, 1);

const char* WIFI_SSID = "WIFI_ADINIZ";
const char* WIFI_PASS = "WIFI_SIFRENIZ";
const char* MQTT_BROKER = "192.168.1.100";
const char* DEVICE_ID = "greenhouse-01";

const int RELAY_PIN = 4;
const bool RELAY_ACTIVE_LOW = true;

void setup() {
  pinMode(RELAY_PIN, OUTPUT);
  deactivatePump();
  Serial.begin(9600);
  esp.begin(115200);
  delay(1000);

  initESP8266();
  Serial.println("MQTT subscribe hazir - pompa komutu bekleniyor");
  Serial.println("Test: mosquitto_pub -h localhost -t sera/greenhouse-01/actuator/pump -m '{\"state\":\"on\",\"duration\":5}'");
}

void initESP8266() {
  sendAT("AT+RST", 3000);
  sendAT("AT+CWMODE=1", 2000);
  String cmd = String("AT+CWJAP=\"") + WIFI_SSID + "\",\"" + WIFI_PASS + "\"";
  esp.println(cmd);
  waitResponse(10000);
}

void activatePump(int durationSec) {
  Serial.println("POMPA ACILDI");
  digitalWrite(RELAY_PIN, RELAY_ACTIVE_LOW ? LOW : HIGH);
  delay(durationSec * 1000);
  deactivatePump();
  Serial.println("POMPA KAPANDI");
}

void deactivatePump() {
  digitalWrite(RELAY_PIN, RELAY_ACTIVE_LOW ? HIGH : LOW);
}

void sendAT(const char* cmd, int timeout) {
  esp.println(cmd);
  waitResponse(timeout);
}

void waitResponse(int timeout) {
  unsigned long start = millis();
  while (millis() - start < (unsigned long)timeout) {
    if (esp.available()) {
      char c = esp.read();
      Serial.write(c);
      // Basit JSON parse: "on" algilama
      static String buffer = "";
      buffer += c;
      if (buffer.indexOf("\"state\":\"on\"") >= 0) {
        int dur = 5;
        if (buffer.indexOf("\"duration\":") >= 0) {
          int idx = buffer.indexOf("\"duration\":") + 11;
          dur = buffer.substring(idx, idx + 2).toInt();
          if (dur <= 0) dur = 5;
        }
        activatePump(dur);
        buffer = "";
      }
    }
  }
}

void loop() {
  if (esp.available()) {
    Serial.write(esp.read());
  }
  delay(10);
}
