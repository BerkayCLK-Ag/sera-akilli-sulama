// Ref: docs/malzemeler.md#esp8266
// ESP TX -> D2, ESP RX <- D3 (direnc hatti), VCC+EN+IO0 -> 3.3V
// WiFi basarili -> IP kaydedilir, durum periyodik yazilir

#include <SoftwareSerial.h>
#include "../../secrets.h"

const bool SWAP_PINS = false;

SoftwareSerial esp(SWAP_PINS ? 3 : 2, SWAP_PINS ? 2 : 3);

const char* DEVICE_ID = "greenhouse-01";

long espBaud = 9600;
String espBuffer;
String deviceIp = "";
bool wifiOk = false;

const long BAUD_LIST[] = {9600, 115200, 57600, 74880};
const int BAUD_COUNT = 4;

unsigned long lastStatus = 0;
const unsigned long STATUS_INTERVAL = 30000;

void setup() {
  Serial.begin(9600);
  delay(2000);

  Serial.println(F("=== Sera ESP8266 Baglantisi ==="));
  Serial.println(F("Dashboard: sera/greenhouse-01/status"));

  espBaud = findWorkingBaud();
  if (espBaud == 0) {
    Serial.println(F("HATA: ESP iletisimi kurulamadi"));
    return;
  }

  if (connectWiFi()) {
    readDeviceIp();
    printStatusJson();
  }
}

bool tryAtOnce(int timeout) {
  flushEsp();
  esp.print(F("AT\r\n"));
  readEsp(timeout);
  return espBuffer.indexOf(F("OK")) >= 0;
}

long findWorkingBaud() {
  for (int i = 0; i < BAUD_COUNT; i++) {
    esp.begin(BAUD_LIST[i]);
    esp.listen();
    delay(1000);
    if (tryAtOnce(2000)) {
      espBaud = BAUD_LIST[i];
      return espBaud;
    }
  }
  return 0;
}

bool connectWiFi() {
  sendAT("AT+RST", 5000);
  delay(3000);
  sendAT("AT+CWMODE=1", 2000);
  sendAT("AT+CWQAP", 2000);

  Serial.println(F("WiFi baglaniyor..."));
  String cmd = String(F("AT+CWJAP=\"")) + WIFI_SSID + F("\",\"") + WIFI_PASS + F("\"");
  sendAT(cmd.c_str(), 25000);

  wifiOk = espBuffer.indexOf(F("WIFI GOT IP")) >= 0
        || espBuffer.indexOf(F("WIFI CONNECTED")) >= 0;

  if (wifiOk) {
    Serial.println(F("WiFi: BASARILI"));
  } else {
    Serial.println(F("WiFi: BASARISIZ"));
  }
  return wifiOk;
}

void readDeviceIp() {
  sendAT("AT+CIFSR", 3000);
  int start = espBuffer.indexOf(F("STAIP,\""));
  if (start < 0) return;
  start += 7;
  int end = espBuffer.indexOf('"', start);
  if (end > start) {
    deviceIp = espBuffer.substring(start, end);
    Serial.print(F("IP: "));
    Serial.println(deviceIp);
  }
}

void printStatusJson() {
  Serial.print(F("{\"device_id\":\""));
  Serial.print(DEVICE_ID);
  Serial.print(F("\",\"online\":"));
  Serial.print(wifiOk ? F("true") : F("false"));
  Serial.print(F(",\"ip\":\""));
  Serial.print(deviceIp);
  Serial.print(F("\",\"wifi\":\""));
  Serial.print(WIFI_SSID);
  Serial.print(F("\",\"uptime\":"));
  Serial.print(millis() / 1000);
  Serial.println(F("}"));
}

void sendAT(const char* cmd, int timeout) {
  flushEsp();
  esp.listen();
  esp.print(cmd);
  esp.print(F("\r\n"));
  readEsp(timeout);
}

void flushEsp() {
  while (esp.available()) esp.read();
  espBuffer = "";
}

int readEsp(int timeout) {
  espBuffer = "";
  unsigned long start = millis();
  int total = 0;

  while (millis() - start < (unsigned long)timeout) {
    esp.listen();
    while (esp.available()) {
      char c = esp.read();
      espBuffer += c;
      total++;
      if (espBuffer.length() > 200) {
        espBuffer.remove(0, espBuffer.length() - 200);
      }
    }
  }
  return total;
}

void loop() {
  if (!wifiOk || espBaud == 0) {
    delay(5000);
    return;
  }

  if (millis() - lastStatus >= STATUS_INTERVAL) {
    lastStatus = millis();
    if (!tryAtOnce(2000)) {
      wifiOk = false;
      Serial.println(F("Baglanti koptu, yeniden deneniyor..."));
      wifiOk = connectWiFi();
      if (wifiOk) readDeviceIp();
    }
    printStatusJson();
  }
  delay(100);
}
