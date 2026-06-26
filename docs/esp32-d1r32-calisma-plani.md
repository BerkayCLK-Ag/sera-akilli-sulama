# ESP32 D1 R32 — Sera Akıllı Sulama Geçiş Çalışma Planı

> **Kart:** LOLIN D1 R32 (ESP32-WROOM-32 çipli, Arduino UNO form factor)  
> **Tarih:** 2026-06-25  
> **Hedef:** Arduino UNO + ESP8266 (AT modu) → Tek kart ESP32 akıllı sulama sistemi

---

## Neden ESP32? (Ziraat Mühendisi Perspektifi)

Arduino UNO + ESP8266 ikili setup, sahada kullanılan "eski nesil logger" gibiydi:
- 2 kart, 2 USB, SoftwareSerial köprüsü
- AT komutları → ayrıştırma hataları
- 32KB flash → küçük program alanı

**ESP32 = modern saha bilgisayarı:**

| Özellik | Arduino UNO + ESP8266 | ESP32 (D1 R32) |
|---------|-----------------------|----------------|
| WiFi | AT komutlu köprü | Dahili, doğrudan |
| Flash | 32 KB | **4 MB** |
| RAM | 2 KB | **520 KB** |
| ADC kanalı | 6 (A0-A5) | **18 kanal** |
| I2C | Yazılım | **Donanım (GPIO21/22)** |
| Çalışma gerilimi | 5V logic | **3.3V logic** |
| Fiyat | 2 kart ~500 TL | Tek kart ~300 TL |

> **Ziraat notu:** Gerçek saha sensörleri (pH, EC, tüm sulama kontrol üniteleri) bugün ESP32 veya Raspberry Pi üzerinde çalışır. Bu geçiş, endüstriyel sisteme yaklaşmak demektir.

---

## KRİTİK: 3.3V Mantık — Bilmesi Gereken #1

ESP32'de GPIO pinleri **3.3V** çıkış verir. Arduino'da 5V'tu.

### Bu ne demek?

```
Arduino UNO:          ESP32 D1 R32:
GPIO HIGH = 5.0V      GPIO HIGH = 3.3V
GPIO LOW  = 0V        GPIO LOW  = 0V
ADC max   = 5.0V      ADC max   = 3.3V  ← ÖNEMLİ!
```

### Sensörler için kural:

| Sensör | VCC bağlantısı | AO/Sinyal | Güvenli mi? |
|--------|----------------|-----------|-------------|
| DHT11 | **3.3V** (3-5.5V arası çalışır) | DATA → GPIO | ✅ |
| Toprak nem modülü | **3.3V** (ya da 5V ama AO → 5V olur, tehlikeli!) | AO → ADC1 | ✅ 3.3V ile |
| Su seviye modülü | **3.3V** | AO → ADC1 | ✅ 3.3V ile |
| Röle modülü | **5V** (bobin 5V ister) | IN → GPIO (3.3V çeker) | ✅ |

> **Kural:** Sensör modüllerini **3.3V** besle → hem güvenli hem ADC için uygun.  
> **İstisna:** Röle **VCC = 5V**, ancak IN pini 3.3V ile tetiklenir (transistörlü modüllerde).

---

## Pin Haritası — D1 R32 ESP32

### D1 R32 Fiziksel Layout

```
         [USB Micro]      [DC Jack]
              |                |
    ┌─────────┴────────────────┘
    │  RESET    [ESP32-WROOM-32]    │
    │                               │
    │  3V3  [ ]    [ ] GND          │
    │  RST  [ ]    [ ] TX  (GPIO1)  │
    │  GND  [ ]    [ ] RX  (GPIO3)  │
    │  D15  [ ] ← GPIO4   [ ] D14   │
    │  D13  [ ] ← GPIO2   [ ] D12   │
    │  D12  [ ]           [ ] D11   │
    │  D11  [ ]           [ ] D10   │
    │  D10  [ ]           [ ] D9    │
    │  D9   [ ]           [ ] D8    │
    │  D8   [ ]           [ ] D7    │
    │  D7   [ ]           [ ] D6    │
    │  D6   [ ]           [ ] D5    │
    │  D5   [ ]           [ ] D4    │
    │  D4   [ ]           [ ] D3    │
    │  D3   [ ]           [ ] D2    │
    │  D2   [ ]           [ ] D1    │
    │  D1   [ ]           [ ] D0    │
    │  D0   [ ]           [ ] GND   │
    │  GND  [ ]           [ ] GND   │
    │  GND  [ ]    5V  [ ] [ ] A0   │
    │  A5   [ ]    3V3 [ ] [ ] A1   │
    │  A4   [ ]           [ ] A2    │
    │  A3   [ ]           [ ] A3    │
    │                               │
    └───────────────────────────────┘
```

### Sera Projesi Pin Atamaları

| Bileşen | D1 R32 Pin | Gerçek GPIO | Renk (jumper) | Not |
|---------|-----------|-------------|---------------|-----|
| DHT11 DATA | **D15** | GPIO4 | 🟡 Sarı | Dijital, pullup |
| Toprak nem AO | **A0** | GPIO36 (VP) | 🟢 Yeşil | ADC1, input-only |
| Su seviye AO | **A1** | GPIO39 (VN) | 🔵 Mavi | ADC1, input-only |
| Röle IN | **D8** | GPIO5 | ⚪ Beyaz | Dijital çıkış |
| (OLED SDA) | **A4** | GPIO21 | — | Sipariş gelince |
| (OLED SCL) | **A5** | GPIO22 | — | Sipariş gelince |
| (2. röle) | **D5** | GPIO18 | — | Sipariş gelince |
| **3.3V** | 3V3 | — | 🟠 Turuncu | Sensör VCC |
| **5V** | 5V | — | 🔴 Kırmızı | Röle VCC |
| **GND** | GND | — | ⚫ Siyah | Ortak toprak |

> **Neden GPIO36 ve GPIO39?**  
> ADC1 pinleri — WiFi açıkken ADC2 pinleri bozuk okuma verir.  
> GPIO36 (VP) ve GPIO39 (VN) giriş-only ama sensörler için zaten sadece okuma lazım.

---

## Renk Kodu Standardı

Her kablo renginin anlamı sabit kalır — karışıklık olmaz:

| Renk | Anlam | Kullanım |
|------|-------|---------|
| 🔴 Kırmızı | 5V güç | Röle VCC, genel 5V hattı |
| 🟠 Turuncu | 3.3V güç | Sensör VCC (DHT, toprak, su) |
| ⚫ Siyah | GND | Tüm topraklar |
| 🟡 Sarı | Dijital sinyal | DHT11 DATA |
| 🟢 Yeşil | Analog sinyal | Toprak nem AO |
| 🔵 Mavi | Analog sinyal | Su seviye AO |
| ⚪ Beyaz | Kontrol sinyali | Röle IN |
| 🟣 Mor | Gelecek kullanım | 2. röle, OLED, vs. |

> **Ziraat notu:** Endüstriyel sulama panolarında da renk standardı zorunludur.  
> IEC 60446: kırmızı=faz, mavi=nötr, yeşil-sarı=toprak. Sen de kendi standardını oluşturdun.

---

## Fazlar — Öğretici Adım Adım Plan

### FAZ 1: Tanıma (Bugün, ~30 dk)

**Hedef:** D1 R32'yi Arduino IDE'de tanıt, blink çalıştır, seri monitör test et.

**Ziraat bağlamı:** Sahaya yeni ekipman geldiğinde önce çalışıp çalışmadığını doğrularsın.

**Adımlar:**
1. Arduino IDE → Board Manager → "ESP32 by Espressif" yükle
2. Board: **"WEMOS D1 MINI ESP32"** veya **"ESP32 Dev Module"**
3. Port seç → `23-esp32-blink.ino` yükle → LED yanıp sönüyor mu?
4. Seri monitör 115200 baud (ESP32'de değişti!)

**Başarı kriteri:** Built-in LED (GPIO2) yanıp sönüyor + Serial.println çıktısı görünüyor.

---

### FAZ 2: Sensörler (1-2 gün)

**Hedef:** DHT11, toprak nem, su seviye → ESP32'den okunuyor.

**Ziraat bağlamı:** Sensör kalibrasyonu = tarla ölçümünden önce cihaz doğrulama.

**Alt adımlar:**
1. `24-esp32-dht.ino` → Sıcaklık + nem ekrana
2. `25-esp32-toprak.ino` → Ham değer + % ekrana
3. `26-esp32-su-seviye.ino` → Ham değer + % ekrana
4. `27-esp32-sensor-hub.ino` → Üçü birden tek sketch

---

### FAZ 3: Röle + Güvenli Sulama (1 gün)

**Hedef:** Toprak nemi düşünce pompa tetiklenir, su azalınca DURUR.

**Ziraat bağlamı:** Tarımsal sulama sistemlerinde "interlocking":
- Su deposu bitmişse pompa çalıştırma (motor yanar)
- Toprak zaten ıslaksa tekrar sulama (kök çürümesi)

**Logic:**
```
SULAMA = (toprak_nem < ESIK) AND (su_seviye > MIN) AND (son_sulama + COOLDOWN < simdi)
```

---

### FAZ 4: WiFi + Dashboard (2-3 gün)

**Hedef:** ESP32 → WiFi → PC backend → dashboard'da canlı veriler.

**Fark:** AT komutları YOK. Direkt `HTTPClient` kütüphanesi.

---

### FAZ 5: Sipariş Gelince (2. röle + 2 pompa + OLED)

**Hedef:** 2 bağımsız sulama zonu, OLED lokal ekran, flyback diyot koruması.

---

## Devre Şeması (Metin)

```
D1 R32 ESP32
┌─────────────────────────────────────┐
│                                     │
│  3V3 ──[🟠]──── DHT11 VCC           │
│               ├── Toprak VCC        │
│               └── Su Seviye VCC     │
│                                     │
│  5V  ──[🔴]──── Röle VCC            │
│                                     │
│  GND ──[⚫]──── DHT11 GND           │
│               ├── Toprak GND        │
│               ├── Su Seviye GND     │
│               └── Röle GND          │
│                                     │
│  GPIO4  ──[🟡]── DHT11 DATA         │
│  GPIO36 ──[🟢]── Toprak nem AO      │
│  GPIO39 ──[🔵]── Su seviye AO       │
│  GPIO5  ──[⚪]── Röle IN            │
│                                     │
│           Röle COM ─── 5V adaptör + │
│           Röle NO  ─── Pompa +      │
│                        Pompa - ─── GND │
└─────────────────────────────────────┘
```

---

## Arduino IDE Ayarları

```
Board:           WEMOS D1 MINI ESP32  (ya da ESP32 Dev Module)
Upload Speed:    921600
CPU Frequency:   240MHz
Flash Size:      4MB
Partition:       Default 4MB
Port:            COMx (Device Manager'dan bak)
Baud Rate:       115200  ← UNO'dakinden farklı!
```

**Board Manager URL** (henüz eklemediysen):
```
https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json
```

---

## Teknik Borçlar

- [ ] Sensör ADC kalibrasyonu (ESP32 ADC doğrusal değil — `analogReadMilliVolts()`)
- [ ] OLED lokal ekran (sipariş gelince)
- [ ] 2. röle + 2. pompa (sipariş gelince)
- [ ] OTA (Over-the-Air) güncelleme
- [ ] Flyback diyot montajı (1N5819 gelince)

---

## Sonraki Mantıklı Görev

1. Arduino IDE'de ESP32 board package kur
2. `23-esp32-blink.ino` yükle — kartın tanındığını doğrula
3. Sensörleri renk kodu ile breadboard'a bağla
4. `27-esp32-sensor-hub.ino` → tüm sensörler birlikte

*Her adımda seri monitörde veri görürsen bir sonrakine geç.*
