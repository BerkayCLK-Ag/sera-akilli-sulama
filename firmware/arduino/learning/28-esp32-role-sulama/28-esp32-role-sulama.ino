// Ref: docs/esp32-d1r32-calisma-plani.md
// Faz 3 — Modül 28: ESP32 Röle + Otomatik Sulama (WiFi'sız)
//
// Bağlantı:
//   Röle VCC  → 5V      (🔴 kırmızı)   ← Röle bobini 5V ister!
//   Röle GND  → GND     (⚫ siyah)
//   Röle IN   → GPIO5   (⚪ beyaz)     [D1 R32'de D8 etiketi]
//
//   Röle COM  → 5V adaptör (+)
//   Röle NO   → Pompa (+)
//   Pompa (-) → GND
//
//   Tüm sensör bağlantıları 27-sensor-hub ile aynı.
//
// Sulama mantığı (ziraat perspektifi):
//   1. Toprak nemi ESIK altında → sulama gerekli
//   2. Su deposu MIN üzerinde  → pompa çalışabilir
//   3. COOLDOWN geçmiş         → çift sulama yok
//   Bu üçü sağlanırsa: pompa SURE saniye çalışır.
//
// Board: WEMOS D1 MINI ESP32 | Baud: 115200

#include <DHT.h>

// --- Pin tanımları ---
#define DHT_PIN     4      // GPIO4
#define TOPRAK_PIN  36     // GPIO36 = VP
#define SU_PIN      39     // GPIO39 = VN
#define ROLE_PIN    5      // GPIO5
#define DHT_TYPE    DHT11

// --- Sulama parametreleri (ayarlanabilir) ---
const int  TOPRAK_ESIK   = 30;   // % altında sulama tetiklenir
const int  SU_MIN        = 15;   // % altında pompa çalışmaz (depo koruması)
const int  SULAMA_SURE   = 5000; // ms (5 saniye)
const long COOLDOWN_MS   = 60000; // ms (1 dakika — aynı anda 2 sulama yok)

// --- Kalibrasyon ---
const int TOPRAK_KURU  = 20;    // Havada (kuru) — 25-esp32-toprak ile ayni
const int TOPRAK_ISLAK = 200;   // Islak
const int SU_BOS       = 0;
const int SU_DOLU      = 2200;

// --- Durum değişkenleri ---
DHT dht(DHT_PIN, DHT_TYPE);
bool pompaCalisiyor     = false;
unsigned long sonSulama = 0;

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
  Serial.println(">>> POMPA ACILIYOR <<<");
  digitalWrite(ROLE_PIN, LOW);    // Active-low röle: LOW = aktif
  pompaCalisiyor = true;
  delay(sure_ms);
  digitalWrite(ROLE_PIN, HIGH);   // Kapat
  pompaCalisiyor = false;
  sonSulama = millis();
  Serial.println(">>> POMPA KAPANDI <<<");
}

void setup() {
  Serial.begin(115200);
  dht.begin();
  pinMode(ROLE_PIN, OUTPUT);
  digitalWrite(ROLE_PIN, HIGH);   // Başlangıçta kapalı (active-low)
  Serial.println("Otomatik Sulama — ESP32 D1 R32");
  Serial.println("Kural: toprak<%30 AND su>%15 AND cooldown=1dk");
}

void loop() {
  int toprak = toprakOku();
  int su     = suSeviyeOku();
  float sic  = dht.readTemperature();
  float nem  = dht.readHumidity();

  Serial.print("Toprak:%"); Serial.print(toprak);
  Serial.print("  Su:%"); Serial.print(su);
  if (!isnan(sic)) {
    Serial.print("  Sic:"); Serial.print(sic,1); Serial.print("C");
    Serial.print("  Nem:%"); Serial.print(nem,1);
  }
  Serial.println();

  unsigned long gecen = millis() - sonSulama;
  bool cooldownOK = (sonSulama == 0) || (gecen > COOLDOWN_MS);

  // Sulama karar mantığı
  if (toprak < TOPRAK_ESIK && su > SU_MIN && cooldownOK) {
    Serial.println("-> SULAMA TETIKLENDI");
    pompaCalistir(SULAMA_SURE);
  } else if (toprak >= TOPRAK_ESIK) {
    Serial.println("-> Sulama gerekmez (toprak nemli)");
  } else if (su <= SU_MIN) {
    Serial.println("-> DEPO BOŞ: Pompa çalıştırılmıyor!");
  } else if (!cooldownOK) {
    Serial.print("-> Cooldown: ");
    Serial.print((COOLDOWN_MS - gecen) / 1000);
    Serial.println("sn kaldi");
  }

  delay(3000);
}
