// Ref: docs/sera-v3-prototip-kurulum.md
// Sera v3 — Tam prototip: OLED + sensörler + pompa + WiFi/backend
//
// Kutuphaneler: DHT, Adafruit SSD1306, Adafruit GFX, WiFi, HTTPClient
// Board: WEMOS D1 MINI ESP32 | Baud: 115200

#include <Wire.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <WiFiUdp.h>
#include <esp_wifi.h>
#include <soc/rtc_cntl_reg.h>
#include <soc/soc.h>
#include <DHT.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// ========== PINLER ==========
#define DHT_PIN     4
#define TOPRAK_PIN  36
#define SU_PIN      39
#define ROLE_PIN    5
#define OLED_SDA    21
#define OLED_SCL    22
#define DHT_TYPE    DHT11

// ========== WiFi — DUZENLE ==========
// ESP32 sadece 2.4 GHz WiFi'ye baglanir. 5 GHz aglari gormez.
// Brownout riskini azaltmak icin tek SSID'ye sakin baglaniyoruz:
// tarama yok, agresif reconnect yok, WiFi TX gucu dusuk.
const char* WIFI_SSID = "WIFI_ADINIZ";
const char* WIFI_PASS = "WIFI_SIFRENIZ";
const int   BACKEND_PORT = 8000;
// PC IP — ipconfig ile kontrol et. UDP bulamazsa bu kullanilir.
const char* PC_IP_SABIT = "192.168.1.140";

// ========== SULAMA ==========
const int  TOPRAK_ESIK = 30;
const int  TOPRAK_HEDEF = 50;        // Bu neme ulasinca pompa HEMEN kapanir
const int  SU_MIN      = 15;
const int  SULAMA_MS   = 45000;      // Guvenlik ust siniri: max 45 sn (hedef %50 oncelikli)
const long COOLDOWN_MS = 60000;
const long COOLDOWN_TEKRAR_MS = 15000; // Hedefe ulasmadan max sure biterse 15 sn sonra tekrar
const int  KURU_OKUMA_GEREKLI = 3;  // 3 olcum ust uste kuruysa sulama baslar
const bool POMPA_AKTIF = true;       // Pompa/role aktif

// ========== KALIBRASYON (senin probun) ==========
// Toprak sensörü kalibrasyonu:
//   Bu dirençli probda genelde kuru -> ham yüksek, ıslak -> ham düşük.
//   Yani su verdikçe ToprakHam azalır, Toprak % artmalıdır.
const int TOPRAK_KURU  = 3000;
const int TOPRAK_ISLAK = 0;
const int SU_BOS       = 0;
const int SU_DOLU      = 2200;

// ========== OLED ==========
#define OLED_W 128
#define OLED_H 64
Adafruit_SSD1306 display(OLED_W, OLED_H, &Wire, -1);

DHT dht(DHT_PIN, DHT_TYPE);
WiFiUDP udp;

String pcIP = "";
String durumMesaji = "Basliyor...";
String aktifSSID = "";
unsigned long sonSulama = 0;
unsigned long sonOlcum = 0;
unsigned long sonUDP = 0;
unsigned long sonWifiKontrol = 0;
unsigned long wifiDenemeBaslangic = 0;
unsigned long wifiIlkDenemeZamani = 0;
unsigned long pompaBitis = 0;
bool pompaCalisiyor = false;
int kuruOkumaSayisi = 0;
int wifiDenemeSayisi = 0;
bool wifiIlkDenemeYapildi = false;
float sonGecerliSicaklik = 0;
float sonGecerliNem = 0;
bool dhtIlkOkumaAlindi = false;

struct Veri {
  float sicaklik;
  float nem;
  int toprakHam;
  int toprak;
  int suHam;
  int su;
  bool dhtOk;
};

int analogOrtalamaOku(int pin) {
  long toplam = 0;
  const int ornekSayisi = 15;
  for (int i = 0; i < ornekSayisi; i++) {
    toplam += analogRead(pin);
    delay(2);
  }
  return toplam / ornekSayisi;
}

int toprakYuzdeOku() {
  int ham = analogOrtalamaOku(TOPRAK_PIN);
  return map(constrain(ham, TOPRAK_ISLAK, TOPRAK_KURU),
             TOPRAK_KURU, TOPRAK_ISLAK, 0, 100);
}

Veri okuSensorler() {
  Veri v;
  float okunanSicaklik = dht.readTemperature();
  float okunanNem = dht.readHumidity();
  v.dhtOk = !isnan(okunanSicaklik) && !isnan(okunanNem);

  if (v.dhtOk) {
    sonGecerliSicaklik = okunanSicaklik;
    sonGecerliNem = okunanNem;
    dhtIlkOkumaAlindi = true;
  }

  // DHT11 ara sira nan dondurur. Dashboard'a -1 gondermek yerine
  // son saglam olcumu koruyoruz.
  v.sicaklik = sonGecerliSicaklik;
  v.nem = sonGecerliNem;

  v.toprakHam = analogOrtalamaOku(TOPRAK_PIN);
  v.toprak = map(constrain(v.toprakHam, TOPRAK_ISLAK, TOPRAK_KURU),
                 TOPRAK_KURU, TOPRAK_ISLAK, 0, 100);

  v.suHam = analogOrtalamaOku(SU_PIN);
  v.su = map(constrain(v.suHam, SU_BOS, SU_DOLU), SU_BOS, SU_DOLU, 0, 100);
  return v;
}

void oledCiz(const Veri& v) {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);

  display.setCursor(0, 0);
  if (v.dhtOk) {
    display.print("T:");
    display.print(v.sicaklik, 0);
    display.print("C H:");
    display.print(v.nem, 0);
    display.println("%");
  } else {
    display.println("DHT hata");
  }

  display.setCursor(0, 12);
  display.print("Toprak:");
  display.print(v.toprak);
  display.print("% Su:");
  display.print(v.su);
  display.println("%");

  display.setCursor(0, 24);
  display.println(durumMesaji);

  display.setCursor(0, 36);
  if (pompaCalisiyor) {
    display.println(">> POMPA ACIK <<");
  } else if (v.toprak < TOPRAK_ESIK && v.su > SU_MIN) {
    display.println("Sulama hazir");
  } else if (v.su <= SU_MIN) {
    display.println("! Depo dusuk");
  } else {
    display.println("Sulama beklemede");
  }

  display.setCursor(0, 52);
  if (WiFi.status() == WL_CONNECTED) {
    display.print("WiFi OK");
    if (pcIP.length()) {
      display.print(" PC:");
      display.print(pcIP.substring(pcIP.lastIndexOf('.') + 1));
    }
  } else {
    display.print("WiFi yok");
  }
  display.display();
}

void pompaBaslat(int ms) {
  if (!POMPA_AKTIF) {
    durumMesaji = "Pompa kilitli";
    Serial.println("POMPA KILITLI - stabil dashboard modu");
    return;
  }
  digitalWrite(ROLE_PIN, LOW);
  pompaCalisiyor = true;
  pompaBitis = millis() + ms;
  durumMesaji = "Sulama hedef 50";
  Serial.println(">>> POMPA AC");
}

void pompaGuncelle() {
  // Pompa çalışmıyorsa kontrol etme
  if (!pompaCalisiyor) return;

  // Her loop'ta toprak oku ve eşiği geçtiyse HEMEN kapat
  int anlikToprak = toprakYuzdeOku();
  Serial.print("Pompa calisiyor | Toprak:%"); Serial.println(anlikToprak);

  if (anlikToprak >= TOPRAK_HEDEF) {
    digitalWrite(ROLE_PIN, HIGH);
    pompaCalisiyor = false;
    sonSulama = millis();
    durumMesaji = "Hedef nem OK";
    Serial.print(">>> POMPA KAPAT - hedef nem: %");
    Serial.println(anlikToprak);
    return;
  }

  if (millis() >= pompaBitis) {
    digitalWrite(ROLE_PIN, HIGH);
    pompaCalisiyor = false;
    if (anlikToprak < TOPRAK_HEDEF) {
      // Hedefe ulasmadi — kisa sure sonra tekrar dene
      sonSulama = millis() - COOLDOWN_MS + COOLDOWN_TEKRAR_MS;
      durumMesaji = "Hedefe ulasmadi";
      Serial.print(">>> POMPA KAPAT - max sure, nem dusuk: %");
    } else {
      sonSulama = millis();
      durumMesaji = "Max sure bitti";
      Serial.print(">>> POMPA KAPAT - max sure, nem: %");
    }
    Serial.println(anlikToprak);
  }
}

const char* wifiDurumAdi(wl_status_t durum) {
  switch (durum) {
    case WL_IDLE_STATUS: return "IDLE";
    case WL_NO_SSID_AVAIL: return "SSID_YOK";
    case WL_SCAN_COMPLETED: return "TARAMA_OK";
    case WL_CONNECTED: return "BAGLI";
    case WL_CONNECT_FAILED: return "BAGLANTI_HATA";
    case WL_CONNECTION_LOST: return "KOPTU";
    case WL_DISCONNECTED: return "BAGLI_DEGIL";
    default: return "BILINMEYEN";
  }
}

bool wifiBaglanmayiDene(const char* ssid, unsigned long timeoutMs) {
  aktifSSID = ssid;
  wifiDenemeSayisi++;
  wifiDenemeBaslangic = millis();

  Serial.print("WiFi deneme ");
  Serial.print(wifiDenemeSayisi);
  Serial.print(": ");
  Serial.println(ssid);

  WiFi.mode(WIFI_STA);
  WiFi.persistent(false);
  WiFi.setSleep(false);
  WiFi.setAutoReconnect(false);
  WiFi.setTxPower(WIFI_POWER_2dBm);
  WiFi.begin(ssid, WIFI_PASS);

  unsigned long basla = millis();
  while (WiFi.status() != WL_CONNECTED && millis() - basla < timeoutMs) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();

  if (WiFi.status() == WL_CONNECTED) {
    udp.begin(9999);
    if (pcIP.length() == 0) {
      pcIP = PC_IP_SABIT;
      Serial.print("PC IP (sabit): ");
      Serial.println(pcIP);
    }
    durumMesaji = "WiFi baglandi";
    Serial.print("WiFi OK: ");
    Serial.println(WiFi.SSID());
    Serial.print("ESP IP: ");
    Serial.println(WiFi.localIP());
    return true;
  }

  Serial.print("WiFi basarisiz. Durum: ");
  Serial.println(wifiDurumAdi(WiFi.status()));
  durumMesaji = "WiFi yok-lokal";
  return false;
}

void wifiBaglan() {
  // Brownout'u azaltmak icin WiFi'yi boot aninda degil,
  // sistem 5 sn oturduktan sonra baslatiyoruz.
  durumMesaji = "Lokal basladi";
  wifiIlkDenemeZamani = millis() + 5000;
  Serial.println("WiFi 5 sn sonra denenecek (brownout onlemi).");
}

void wifiKontrolEt() {
  if (!wifiIlkDenemeYapildi && millis() >= wifiIlkDenemeZamani) {
    wifiIlkDenemeYapildi = true;
    durumMesaji = "WiFi...";
    WiFi.mode(WIFI_STA);
    WiFi.setSleep(false);
    WiFi.setTxPower(WIFI_POWER_2dBm);
    wifiBaglanmayiDene(WIFI_SSID, 15000);
    return;
  }

  if (!wifiIlkDenemeYapildi) return;
  if (millis() - sonWifiKontrol < 60000) return;
  sonWifiKontrol = millis();

  if (WiFi.status() == WL_CONNECTED) return;

  durumMesaji = "WiFi tekrar...";
  Serial.print("WiFi bagli degil. Durum: ");
  Serial.print(wifiDurumAdi(WiFi.status()));
  Serial.println(" -> 60 sn sonra tekrar denenecek.");

  const char* yenidenSSID = aktifSSID.length() ? aktifSSID.c_str() : WIFI_SSID;
  WiFi.reconnect();
  delay(1000);
  if (WiFi.status() != WL_CONNECTED) {
    wifiBaglanmayiDene(yenidenSSID, 10000);
  }
}

bool udpPCBul() {
  bool bulundu = false;
  while (true) {
    int n = udp.parsePacket();
    if (n <= 0) break;
    char buf[64];
    int len = udp.read(buf, sizeof(buf) - 1);
    buf[len] = 0;
    String m = String(buf);
    if (m.startsWith("SERADASH|")) {
      int a = m.indexOf('|');
      int b = m.indexOf('|', a + 1);
      pcIP = m.substring(a + 1, b);
      bulundu = true;
    }
  }
  return bulundu;
}

bool backendGonder(const Veri& v) {
  if (WiFi.status() != WL_CONNECTED) return false;
  if (pcIP.length() == 0) pcIP = PC_IP_SABIT;

  HTTPClient http;
  String url = "http://" + pcIP + ":" + String(BACKEND_PORT)
             + "/api/sensors/ingest/simple"
             + "?temperature=" + String(v.sicaklik, 1)
             + "&humidity=" + String(v.nem, 1)
             + "&soil_moisture=" + String(v.toprak)
             + "&water_level=" + String(v.su)
             + "&ip=" + WiFi.localIP().toString()
             + "&wifi=" + WiFi.SSID();

  http.setTimeout(8000);
  http.begin(url);
  int kod = http.GET();
  String cevap = http.getString();
  http.end();

  Serial.print("HTTP ");
  Serial.print(kod);
  Serial.print(" -> ");
  Serial.print(pcIP);
  Serial.print(" ");
  Serial.println(cevap);

  if (kod <= 0) {
    // Baglanti hatasi — UDP'den yeni IP dene
    udpPCBul();
    if (pcIP != PC_IP_SABIT && pcIP.length() > 0) {
      Serial.println("PC IP guncellendi, sonraki dongude tekrar");
    }
    return false;
  }

  if (cevap.indexOf("OK PUMP") >= 0) {
    if (v.toprak >= TOPRAK_HEDEF) {
      durumMesaji = "Nem yeterli";
      Serial.print("Backend pompa komutu engellendi - nem: %");
      Serial.println(v.toprak);
      return false;
    }

    int sure = SULAMA_MS;
    int idx = cevap.indexOf("OK PUMP ");
    if (idx >= 0) {
      sure = cevap.substring(idx + 8).toInt() * 1000;
      if (sure <= 0 || sure > SULAMA_MS) sure = SULAMA_MS;
    }
    if (!pompaCalisiyor) pompaBaslat(sure);
    durumMesaji = "Backend sulama";
    return true;
  }
  return false;
}

void lokalSulamaKarar(const Veri& v) {
  if (pompaCalisiyor) return;
  bool cd = (sonSulama == 0) || (millis() - sonSulama > COOLDOWN_MS);

  if (v.toprak < TOPRAK_ESIK) {
    kuruOkumaSayisi++;
  } else {
    kuruOkumaSayisi = 0;
  }

  if (kuruOkumaSayisi >= KURU_OKUMA_GEREKLI && v.su > SU_MIN && cd) {
    pompaBaslat(SULAMA_MS);
    durumMesaji = "Otomatik sulama";
    kuruOkumaSayisi = 0;
    Serial.println("-> Lokal sulama");
  } else if (v.su <= SU_MIN && v.toprak < TOPRAK_ESIK) {
    durumMesaji = "Depo bos!";
  } else if (v.toprak >= TOPRAK_ESIK) {
    durumMesaji = "Toprak OK";
  }
}

void setup() {
  // D1 R32 + USB + WiFi anlik akim cekislerinde BOD reset atabiliyor.
  // Stabil prototip testi icin brownout resetini kapatiyoruz.
  WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0);

  Serial.begin(115200);
  pinMode(ROLE_PIN, OUTPUT);
  digitalWrite(ROLE_PIN, HIGH);

  Wire.begin(OLED_SDA, OLED_SCL);
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("OLED yok — devam ediliyor");
  }
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.println("Sera v3");
  display.println("Yukleniyor...");
  display.display();

  // ESP32 ADC 11dB atenuasyon — 0-3.3V tam aralik (default 1V'a kadar)
  analogSetAttenuation(ADC_11db);

  dht.begin();
  wifiBaglan();
  Serial.println("=== Sera v3 Prototip ===");
}

void loop() {
  pompaGuncelle();
  wifiKontrolEt();

  if (millis() - sonUDP > 2000) {
    udpPCBul();
    sonUDP = millis();
  }

  if (millis() - sonOlcum >= 3000) {
    sonOlcum = millis();
    Veri v = okuSensorler();
    oledCiz(v);

    Serial.print("T:"); Serial.print(v.sicaklik);
    Serial.print(" Toprak:"); Serial.print(v.toprak);
    Serial.print(" ToprakHam:"); Serial.print(v.toprakHam);
    Serial.print(" Su:"); Serial.print(v.su);
    Serial.print(" SuHam:"); Serial.println(v.suHam);

    if (!backendGonder(v)) {
      lokalSulamaKarar(v);
    }
  }
}
