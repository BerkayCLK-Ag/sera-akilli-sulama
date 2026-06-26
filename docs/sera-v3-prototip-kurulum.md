# Sera v3 — Tam Prototip Kurulum Rehberi

> **Hedef:** Gerçek saksıya su veren, nemi ölçen, OLED'de gösteren, depo korumalı akıllı sulama.  
> **Kart:** ESP32 D1 R32 | **Firmware:** `firmware/arduino/greenhouse/sera-v3-prototip.ino`

---

## Sistem görünümü (ziraat perspektifi)

```
                    ┌──────────────────┐
                    │  Mini sera / kutu │
                    │  DHT11 + OLED     │
                    └────────┬─────────┘
                             │ ortam nem/sıcaklık
┌──────────────┐    hortum    ┌──────────────┐
│ Su deposu    │─────────────→│ Saksı + bitki │
│ pompa + prob │              │ toprak probu  │
└──────────────┘              └──────────────┘
       ↑
  su seviye sensörü (depo boş → pompa dur)
```

**Karar zinciri:**
1. Toprak nemi < %30 → sulama gerekli  
2. Depo su > %15 → pompa çalışabilir  
3. Her iki koşul sağlanırsa → röle 5 sn → pompa su verir  

---

## Fiziksel kurulum (5 adım)

### 1 — Su deposu

- 5–10 L plastik kova veya derin kap  
- **Dalgıç pompa** deponun **içine** (tamamen su altında)  
- **Su seviye probu** deponun **iç duvarına** — metal uç suya değsin  

### 2 — Saksı

- Orta boy saksı, torf/toprak  
- **Toprak nem probu** saksının **yanına** dikey batır (kök bölgesi)  
- Hortum ucu saksı kenarına — su doğrudan köke değil kenara (erozyon azalır)  

### 3 — Hortum hattı

```
Pompa çıkışı → silikon hortum (4–6 mm) → saksı
```

İlk test: hortum ucunu saksı üstüne sabitle (bant/yaylı).

### 4 — Elektrik güvenliği (pompa)

```
5V adaptör (+) ──→ Röle COM
Röle NO        ──→ Pompa (+)
Pompa (-)      ──→ Adaptör (-) / GND

Flyback diyot (1N5819 veya 1N4007):
  Diyot ── pompa üzerinde paralel
  Bantlı uç (+) → Katot (çizgili)
  Bantlı uç (-) → Anot
```

> **ESP USB'sinden pompa çalıştırma.** Pompa 200–500 mA çeker — ayrı 5V adaptör şart.

### 5 — Ortak toprak (GND)

ESP32 GND, adaptör GND, röle GND **birbirine bağlı** olmalı.

---

## Pin planı (D1 R32 — mevcut devre + OLED)

| Bileşen | GPIO | Kablo |
|---------|------|-------|
| DHT11 DATA | IO4 | Sarı |
| Toprak AO | IO36 | Yeşil |
| Su AO | IO39 | Mavi |
| Röle IN | IO5 | Beyaz |
| OLED SDA | IO21 | Mor |
| OLED SCL | IO22 | Gri |
| Sensör VCC | 3V3 şerit | Turuncu |
| Röle VCC | 5V şerit | Kırmızı |
| GND | GND şerit | Siyah |

OLED: **3.3V** (5V değil — I2C ekran).

---

## Arduino IDE kütüphaneleri

Araçlar → Kütüphaneleri Yönet:

1. **Adafruit GFX Library**
2. **Adafruit SSD1306**
3. **DHT sensor library** (Adafruit)

---

## Kurulum sırası (bugün)

| Sıra | Görev | Sketch / dosya |
|------|-------|----------------|
| 1 | OLED tek başına test | `30-esp32-oled-test.ino` |
| 2 | Pompa + röle (su yok) TIK testi | `28-esp32-role-sulama.ino` |
| 3 | Depo + saksı fiziksel montaj | — |
| 4 | Tam sistem | `sera-v3-prototip.ino` |
| 5 | PC dashboard | `start-dashboard.bat` + WiFi ayarı |

---

## Kalibrasyon (senin probun)

```cpp
TOPRAK_KURU  = 20    // havada
TOPRAK_ISLAK = 200   // suda/ıslak
SU_BOS       = 0     // havada
SU_DOLU      = 2200  // tam suda
```

Saksıda hedef bant: **%40–70** nem (bitkiye göre değişir).

---

## Demo senaryosu

1. Backend aç → http://localhost:8000  
2. `sera-v3-prototip.ino` yükle (WiFi + PC IP düzenle)  
3. OLED: sıcaklık, nem, toprak, su görünür  
4. Saksıyı kurut (veya probu çıkar) → otomatik sulama  
5. Depo probunu havaya al → pompa **durur** (koruma)  
6. Dashboard'da canlı grafikler  

---

## Teknik borçlar

- [ ] 2. saksı / 2. röle (2 zone)
- [ ] OTA güncelleme
- [ ] pH/EC sensörü (ileri seviye)

---

*Ref: docs/devre-semalari/30-sera-v3-pompa-oled-baglanti.md*
