// Ref: docs/esp32-d1r32-calisma-plani.md
// Faz 4 — Modül 29: ESP32 WiFi + Backend Entegrasyonu
//
// Bu sketch = sera v2 ana firmware.
// AT komutu YOK — direkt HTTPClient kütüphanesi.
// Sensörler + röle + WiFi tek kart.
//
// Bağlantılar: 28-esp32-role-sulama ile aynı.
//
// Board: WEMOS D1 MINI ESP32 | Baud: 115200

#include <WiFi.h>
#include <HTTPClient.h>
#include <DHT.h>
#include "../../secrets.h"

// --- Pin tanımları ---
#define DHT_PIN     4
#define TOPRAK_PIN  36
#define SU_PIN      39
#define ROLE_PIN    5
#define DHT_TYPE    DHT11

// --- WiFi + Backend (secrets.h) ---
const char* BACKEND_IP   = PC_IP_SABIT;
const int   BACKEND_PORT = 8000;

// --- Sulama parametreleri ---
const int  TOPRAK_ESIK  = 30;
const int  SU_MIN       = 15;
const int  SULAMA_SURE  = 5000;
const long COOLDOWN_MS  = 60000;

// --- Kalibrasyon ---
const int TOPRAK_KURU   = 20;
const int TOPRAK_ISLAK  = 200;
const int SU_BOS        = 0;
const int SU_DOLU       = 2200;

DHT dht(DHT_PIN, DHT_TYPE);
unsigned long sonSulama = 0;
bool pompaCalisiyor     = false;

// --- UDP ile PC keşfi (v6 mantığı korunuyor) ---
WiFiUDP udp;
String pcIP = "";
const int UDP_PORT = 9999;
unsigned long sonUDP = 0;

void wifiBaglan() {
  Serial.print("WiFi baglaniyor: ");
  Serial.println(WIFI_SSID);
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  int deneme = 0;
  while (WiFi.status() != WL_CONNECTED && deneme < 30) {
    delay(500);
    Serial.print(".");
    deneme++;
  }
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\nWiFi OK!");
    Serial.print("ESP32 IP: ");
    Serial.println(WiFi.localIP());
    udp.begin(UDP_PORT);
  } else {
    Serial.println("\nWiFi BAGLANAMADI — seri ile devam");
  }
}

bool udpPCBul() {
  // Backend'den "SERADASH|IP|8000" beacon bekle
  int n = udp.parsePacket();
  if (n > 0) {
    char buf[64];
    udp.read(buf, sizeof(buf) - 1);
    buf[n] = 0;
    String mesaj = String(buf);
    if (mesaj.startsWith("SERADASH|")) {
      int b1 = mesaj.indexOf('|');
      int b2 = mesaj.indexOf('|', b1 + 1);
      pcIP = mesaj.substring(b1 + 1, b2);
      Serial.print("PC bulundu: ");
      Serial.println(pcIP);
      return true;
    }
  }
  return false;
}

int toprakOku() {
  int ham = analogRead(TOPRAK_PIN);
  return map(constrain(ham, TOPRAK_KURU, TOPRAK_ISLAK),
             TOPRAK_KURU, TOPRAK_ISLAK, 0, 100);
}

int suSeviyeOku() {
  int ham = analogRead(SU_PIN);
  return map(constrain(ham, SU_BOS, SU_DOLU),
             SU_BOS, SU_DOLU, 0, 100);
}

void pompaCalistir(int sure_ms) {
  Serial.println(">>> POMPA AC <<<");
  digitalWrite(ROLE_PIN, LOW);
  pompaCalisiyor = true;
  delay(sure_ms);
  digitalWrite(ROLE_PIN, HIGH);
  pompaCalisiyor = false;
  sonSulama = millis();
  Serial.println(">>> POMPA KAPAT <<<");
}

bool veriGonder(float sic, float nem, int toprak, int su) {
  if (pcIP.length() == 0) return false;
  if (WiFi.status() != WL_CONNECTED) return false;

  HTTPClient http;
  String url = "http://" + pcIP + ":" + String(BACKEND_PORT)
             + "/api/sensors/ingest/simple"
             + "?temperature=" + String(sic, 1)
             + "&humidity=" + String(nem, 1)
             + "&soil_moisture=" + String(toprak)
             + "&water_level=" + String(su);

  http.begin(url);
  int kod = http.GET();
  String cevap = http.getString();
  http.end();

  Serial.print("HTTP ");
  Serial.print(kod);
  Serial.print(" -> ");
  Serial.println(cevap);

  // Backend "OK PUMP X Y" veya "OK NONE" döner
  if (cevap.indexOf("OK PUMP") >= 0) {
    int sure = SULAMA_SURE;
    // Opsiyonel: backend'den süre parse
    int idx = cevap.indexOf("OK PUMP ");
    if (idx >= 0) {
      String parca = cevap.substring(idx + 8);
      sure = parca.toInt() * 1000;
      if (sure <= 0 || sure > 60000) sure = SULAMA_SURE;
    }
    pompaCalistir(sure);
    return true;
  }
  return false;
}

void setup() {
  Serial.begin(115200);
  dht.begin();
  pinMode(ROLE_PIN, OUTPUT);
  digitalWrite(ROLE_PIN, HIGH);
  Serial.println("=== Sera v2 — ESP32 D1 R32 ===");
  wifiBaglan();
}

void loop() {
  // UDP ile PC ara (15 sn'de bir)
  if (millis() - sonUDP > 15000) {
    udpPCBul();
    sonUDP = millis();
  }
  // Anlık UDP mesajı da kontrol et
  udpPCBul();

  int toprak = toprakOku();
  int su     = suSeviyeOku();
  float sic  = dht.readTemperature();
  float nem  = dht.readHumidity();

  if (isnan(sic)) { sic = -1; nem = -1; }

  Serial.print("Toprak:%"); Serial.print(toprak);
  Serial.print(" Su:%"); Serial.print(su);
  Serial.print(" Sic:"); Serial.print(sic,1);
  Serial.print("C Nem:%"); Serial.println(nem,1);

  // Backend'e gönder — otomatik sulama kararı backend'den gelir
  bool sulamaBackend = veriGonder(sic, nem, toprak, su);

  // Backend cevap vermediyse / WiFi yoksa: lokal yedek mantık
  if (!sulamaBackend) {
    bool cooldownOK = (sonSulama == 0) || ((millis() - sonSulama) > COOLDOWN_MS);
    if (toprak < TOPRAK_ESIK && su > SU_MIN && cooldownOK) {
      Serial.println("-> Lokal sulama tetiklendi (backend yok)");
      pompaCalistir(SULAMA_SURE);
    }
  }

  delay(3000);
}
