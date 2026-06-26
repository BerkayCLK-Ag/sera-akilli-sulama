// Ref: docs/devre-semalari/23-toprak-role-esp8266-baglanti.md
// v6 — PC IP otomatik (backend UDP yayini), elle IP gerekmez

#include <SoftwareSerial.h>
#include <DHT.h>
#include "../../secrets.h"

SoftwareSerial esp(2, 3);
DHT dht(4, DHT11);

const char* PC_HOST_FALLBACK = PC_IP_SABIT;
const int PC_PORT = 8000;
const int DISCOVERY_PORT = 9999;

const int SOIL_PIN = A0;
const int RELAY_PIN = 5;
const bool RELAY_ACTIVE_LOW = true;

long espBaud = 9600;
const long BAUD_LIST[] = {9600, 115200};

String espBuffer;
String deviceIp = "";
String pcHost = "";
bool wifiOk = false;
bool pumpRunning = false;
byte connectFailCount = 0;

unsigned long lastSend = 0;
unsigned long lastEspCheck = 0;
const unsigned long SEND_INTERVAL = 1500;
const unsigned long ESP_CHECK_INTERVAL = 60000;

bool readDht(float& temp, float& hum);
void markEspReset();
bool espAlive();
int readSoilRaw();
bool httpRequest(const char* method, const char* path, const char* body, String& response);
bool waitForHttpDone(String& response);
bool waitForPrompt(char prompt, int timeout);
void sendAT(const char* cmd, int timeout);
void flushEsp();
void readEspActive(unsigned long timeoutMs);
long findEspBaud();
bool connectWiFi();
void readDeviceIp();
void okuVeGonder();
void handlePlainResponse(const String& resp);
bool sendPumpAck(int seq);
void runPump(unsigned long durationMs);
void activatePump();
void deactivatePump();
void warnIfPcHostIsEspIp();
void testRelayPin();
bool discoverPcHost();
const char* activePcHost();

void setup() {
  pinMode(RELAY_PIN, OUTPUT);
  deactivatePump();
  delay(50);
  deactivatePump();

  Serial.begin(9600);
  dht.begin();
  delay(2000);

  Serial.println(F("=== Sera v6 — otomatik PC IP ==="));
  Serial.println(F("Backend acikken UDP ile PC bulunur"));
  testRelayPin();

  espBaud = findEspBaud();
  if (espBaud == 0) {
    Serial.println(F("HATA: ESP yok"));
    return;
  }

  if (!connectWiFi()) {
    Serial.println(F("HATA: WiFi"));
    return;
  }
  readDeviceIp();
  if (!discoverPcHost()) {
    pcHost = PC_HOST_FALLBACK;
    Serial.print(F("Yedek IP: "));
    Serial.println(pcHost);
  }
  warnIfPcHostIsEspIp();
  Serial.print(F("PC hedef: "));
  Serial.println(activePcHost());

  delay(2000);
  okuVeGonder();
  lastSend = millis();

  Serial.println(F("Hazir — dashboard: http://localhost:8000"));
  Serial.println(F("Seri monitore P = pompa test"));
}

void warnIfPcHostIsEspIp() {
  if (deviceIp.length() == 0 || pcHost.length() == 0) return;
  if (deviceIp != pcHost) return;

  Serial.println();
  Serial.println(F("!!! HATA: PC IP ile ESP IP AYNI !!!"));
  Serial.println(F("Backend calismiyor veya UDP yanlis IP verdi."));
  Serial.println();
}

const char* activePcHost() {
  return pcHost.length() > 0 ? pcHost.c_str() : PC_HOST_FALLBACK;
}

bool discoverPcHost() {
  Serial.println(F("PC araniyor (UDP 9999)..."));
  flushEsp();
  sendAT("AT+CIPCLOSE", 1000);

  String udpCmd = String(F("AT+CIPSTART=\"UDP\",\"0.0.0.0\","))
                + DISCOVERY_PORT + F(",") + DISCOVERY_PORT + F(",2");
  sendAT(udpCmd.c_str(), 4000);

  espBuffer = "";
  unsigned long start = millis();
  while (millis() - start < 12000UL) {
    esp.listen();
    while (esp.available()) {
      espBuffer += (char)esp.read();
      if (espBuffer.length() > 900) {
        espBuffer.remove(0, espBuffer.length() - 900);
      }
    }
    int tag = espBuffer.indexOf(F("SERADASH|"));
    if (tag >= 0) {
      int ipStart = tag + 9;
      int ipEnd = espBuffer.indexOf('|', ipStart);
      if (ipEnd > ipStart) {
        pcHost = espBuffer.substring(ipStart, ipEnd);
        pcHost.trim();
        sendAT("AT+CIPCLOSE", 2000);
        if (pcHost.length() >= 7 && pcHost != deviceIp) {
          Serial.print(F("PC bulundu: "));
          Serial.println(pcHost);
          connectFailCount = 0;
          return true;
        }
      }
    }
    delay(30);
  }

  sendAT("AT+CIPCLOSE", 2000);
  Serial.println(F("UDP: PC bulunamadi (start-dashboard.bat acik mi?)"));
  return false;
}

void testRelayPin() {
  Serial.println(F("-- Role D5 test --"));
  activatePump();
  delay(400);
  deactivatePump();
}

void loop() {
  if (Serial.available()) {
    char c = Serial.read();
    if (c == 'P' || c == 'p') runPump(500);
  }

  if (!wifiOk) {
    wifiOk = connectWiFi();
    if (wifiOk) readDeviceIp();
    delay(3000);
    return;
  }

  if (millis() - lastEspCheck >= ESP_CHECK_INTERVAL) {
    if (!espAlive()) {
      markEspReset();
      wifiOk = false;
    }
    lastEspCheck = millis();
  }

  if (!pumpRunning && millis() - lastSend >= SEND_INTERVAL) {
    okuVeGonder();
    lastSend = millis();
  }
  delay(30);
}

long findEspBaud() {
  for (int i = 0; i < 2; i++) {
    esp.begin(BAUD_LIST[i]);
    esp.listen();
    delay(800);
    flushEsp();
    esp.print(F("AT\r\n"));
    readEspActive(2000);
    if (espBuffer.indexOf(F("OK")) >= 0) return BAUD_LIST[i];
  }
  return 0;
}

bool connectWiFi() {
  sendAT("AT+RST", 5000);
  delay(3000);
  sendAT("AT+CWMODE=1", 2000);
  String cmd = String(F("AT+CWJAP=\"")) + WIFI_SSID + F("\",\"") + WIFI_PASS + F("\"");
  sendAT(cmd.c_str(), 25000);
  wifiOk = espBuffer.indexOf(F("WIFI GOT IP")) >= 0
        || espBuffer.indexOf(F("WIFI CONNECTED")) >= 0;
  if (wifiOk) delay(2000);
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
    Serial.print(F("ESP IP: "));
    Serial.println(deviceIp);
  }
}

void markEspReset() {
  wifiOk = false;
}

bool espAlive() {
  flushEsp();
  esp.listen();
  esp.print(F("AT\r\n"));
  readEspActive(1500);
  return espBuffer.indexOf(F("OK")) >= 0 && espBuffer.indexOf(F("Ai-Thinker")) < 0;
}

bool readDht(float& temp, float& hum) {
  flushEsp();
  delay(300);
  hum = dht.readHumidity();
  temp = dht.readTemperature();
  if (!isnan(temp) && !isnan(hum) && temp > 1.0f && hum >= 1.0f) return true;
  delay(1500);
  hum = dht.readHumidity();
  temp = dht.readTemperature();
  return !isnan(temp) && !isnan(hum) && temp > 1.0f && hum >= 1.0f;
}

int readSoilRaw() {
  return analogRead(SOIL_PIN);
}

void activatePump() {
  digitalWrite(RELAY_PIN, RELAY_ACTIVE_LOW ? LOW : HIGH);
}

void deactivatePump() {
  digitalWrite(RELAY_PIN, RELAY_ACTIVE_LOW ? HIGH : LOW);
}

void runPump(unsigned long durationMs) {
  if (pumpRunning) return;
  pumpRunning = true;
  activatePump();
  Serial.println(F("*** TIK AC ***"));
  delay(durationMs);
  deactivatePump();
  Serial.println(F("*** TIK KAPA ***"));
  pumpRunning = false;
  lastSend = 0;
}

bool responseBodyReady(const String& raw) {
  return raw.indexOf(F("OK PUMP")) >= 0
      || raw.indexOf(F("OK NONE")) >= 0
      || raw.indexOf(F("ACK OK")) >= 0
      || raw.indexOf(F("ACK NONE")) >= 0;
}

bool extractPlainText(const String& raw, String& out) {
  // ONEMLI: "HTTP/1.1 200 OK" ve "SEND OK" icindeki OK'yi alma!
  int pump = raw.indexOf(F("OK PUMP"));
  if (pump >= 0) {
    out = raw.substring(pump);
    int end = out.indexOf('\r');
    if (end < 0) end = out.indexOf('\n');
    if (end > 0) out = out.substring(0, end);
    out.trim();
    return true;
  }

  int none = raw.indexOf(F("OK NONE"));
  if (none >= 0) {
    out = raw.substring(none);
    int end = out.indexOf('\r');
    if (end < 0) end = out.indexOf('\n');
    if (end > 0) out = out.substring(0, end);
    out.trim();
    return true;
  }

  int ackOk = raw.indexOf(F("ACK OK"));
  if (ackOk >= 0) {
    out = F("ACK OK");
    return true;
  }

  int ackNone = raw.indexOf(F("ACK NONE"));
  if (ackNone >= 0) {
    out = F("ACK NONE");
    return true;
  }

  return false;
}

bool waitForHttpDone(String& response) {
  espBuffer = "";
  unsigned long start = millis();
  while (millis() - start < 15000UL) {
    esp.listen();
    while (esp.available()) {
      espBuffer += (char)esp.read();
      if (espBuffer.length() > 900) {
        espBuffer.remove(0, espBuffer.length() - 900);
      }
    }
    if (responseBodyReady(espBuffer)) {
      delay(300);
      while (esp.available()) {
        espBuffer += (char)esp.read();
      }
      break;
    }
    if (espBuffer.indexOf(F("CLOSED")) >= 0) break;
    delay(20);
  }
  return extractPlainText(espBuffer, response);
}

bool httpRequest(const char* method, const char* path, const char* body, String& response) {
  response = "";
  const char* host = activePcHost();
  sendAT("AT+CIPCLOSE", 1000);
  String startCmd = String(F("AT+CIPSTART=\"TCP\",\"")) + host + F("\",") + PC_PORT;
  sendAT(startCmd.c_str(), 8000);
  if (espBuffer.indexOf(F("CONNECT")) < 0 && espBuffer.indexOf(F("OK")) < 0
      && espBuffer.indexOf(F("ALREADY")) < 0) {
    connectFailCount++;
    Serial.print(F("PC baglanamadi: "));
    Serial.println(host);
    if (connectFailCount >= 2) {
      Serial.println(F("PC yeniden araniyor..."));
      discoverPcHost();
    }
    Serial.println(F("Kontrol: start-dashboard.bat + allow-firewall.bat"));
    if (deviceIp.length() > 0) {
      Serial.print(F("ESP IP: "));
      Serial.println(deviceIp);
    }
    return false;
  }

  connectFailCount = 0;
  String req = String(method) + F(" ") + path + F(" HTTP/1.1\r\n")
             + F("Host: ") + host + F(":") + String(PC_PORT) + F("\r\n")
             + F("Connection: close\r\n");
  if (body && body[0]) {
    req += F("Content-Type: application/json\r\n");
    req += F("Content-Length: ");
    req += String(strlen(body));
    req += F("\r\n\r\n");
    req += body;
  } else {
    req += F("\r\n");
  }

  flushEsp();
  esp.listen();
  esp.print(F("AT+CIPSEND="));
  esp.println(req.length());
  if (!waitForPrompt('>', 5000)) {
    Serial.println(F("CIPSEND hatasi"));
    return false;
  }

  esp.print(req);

  if (!waitForHttpDone(response)) {
    Serial.println(F("HTTP cevap yok"));
    Serial.print(F("Ham("));
    Serial.print(espBuffer.length());
    Serial.println(F("):"));
    Serial.println(espBuffer);
    return false;
  }

  sendAT("AT+CIPCLOSE", 1000);
  if (espBuffer.indexOf(F("Ai-Thinker")) >= 0) markEspReset();
  return true;
}

void handlePlainResponse(const String& resp) {
  Serial.print(F("PC: "));
  Serial.println(resp);

  if (resp.startsWith(F("OK PUMP"))) {
    int duration = 5;
    int seq = 0;
    int p1 = resp.indexOf(' ');
    int p2 = resp.indexOf(' ', p1 + 1);
    int p3 = resp.indexOf(' ', p2 + 1);
    if (p2 > 0) duration = resp.substring(p2 + 1, p3 > 0 ? p3 : resp.length()).toInt();
    if (p3 > 0) seq = resp.substring(p3 + 1).toInt();
    if (duration < 1) duration = 5;

    Serial.print(F(">>> POMPA "));
    Serial.print(duration);
    Serial.println(F(" sn"));
    runPump((unsigned long)duration * 1000UL);
    sendPumpAck(seq);
  }
}

bool sendPumpAck(int seq) {
  String path = String(F("/api/actuators/pump/ack-simple?seq=")) + seq;
  String resp;
  if (httpRequest("GET", path.c_str(), nullptr, resp)) {
    Serial.println(resp);
    return resp.indexOf(F("ACK OK")) >= 0;
  }
  return false;
}

void okuVeGonder() {
  float temp = 0;
  float hum = 0;
  if (!readDht(temp, hum)) {
    Serial.println(F("DHT hatasi"));
    return;
  }

  int rawSoil = readSoilRaw();
  int soil = map(constrain(rawSoil, 300, 1023), 1023, 300, 0, 100);

  Serial.print(F("DHT:"));
  Serial.print(temp, 1);
  Serial.print(F("C %"));
  Serial.print(hum, 0);
  Serial.print(F(" Toprak:"));
  Serial.println(soil);

  String path = String(F("/api/sensors/ingest/simple?temperature=")) + String(temp, 1)
              + F("&humidity=") + String((int)hum)
              + F("&soil_moisture=") + soil
              + F("&ip=") + deviceIp
              + F("&device_id=greenhouse-01&wifi=") + WIFI_SSID;

  String resp;
  if (httpRequest("GET", path.c_str(), nullptr, resp)) {
    Serial.println(F("sensor OK"));
    handlePlainResponse(resp);
  } else {
    Serial.println(F("sensor GONDERILEMEDI"));
  }
}

bool waitForPrompt(char prompt, int timeout) {
  unsigned long start = millis();
  while (millis() - start < (unsigned long)timeout) {
    esp.listen();
    while (esp.available()) {
      if (esp.read() == prompt) return true;
    }
  }
  return false;
}

void sendAT(const char* cmd, int timeout) {
  flushEsp();
  esp.listen();
  esp.print(cmd);
  esp.print(F("\r\n"));
  readEspActive(timeout);
}

void flushEsp() {
  while (esp.available()) esp.read();
  espBuffer = "";
}

void readEspActive(unsigned long timeoutMs) {
  espBuffer = "";
  unsigned long start = millis();
  while (millis() - start < timeoutMs) {
    esp.listen();
    while (esp.available()) {
      espBuffer += (char)esp.read();
      if (espBuffer.length() > 600) {
        espBuffer.remove(0, espBuffer.length() - 600);
      }
    }
  }
}
