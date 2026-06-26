# ESP32 D1 R32 — Renk Kodlu Breadboard Bağlantı Şeması

> **Kart:** LOLIN D1 R32 (ESP32-WROOM-32)  
> **Kritik:** GPIO logic = **3.3V** (Arduino'dan farklı!)  
> **Ref:** docs/esp32-d1r32-calisma-plani.md

---

## Renk Kodu Standardı

| Renk | Anlam |
|------|-------|
| 🔴 Kırmızı | 5V güç (röle bobini) |
| 🟠 Turuncu | 3.3V güç (tüm sensörler) |
| ⚫ Siyah | GND |
| 🟡 Sarı | DHT11 DATA (dijital) |
| 🟢 Yeşil | Toprak nem AO (analog) |
| 🔵 Mavi | Su seviye AO (analog) |
| ⚪ Beyaz | Röle IN (kontrol) |

---

## Breadboard Bağlantı Tablosu

### DHT11

| DHT11 Pini | D1 R32 Pini | Jumper Rengi |
|-----------|-------------|--------------|
| VCC (+) | **3V3** | 🟠 Turuncu |
| GND (-) | **GND** | ⚫ Siyah |
| DATA (S) | **GPIO4** (D15 etiketi) | 🟡 Sarı |

> NOT: DHT11 DATA pini ile 3.3V arasına 10kΩ pull-up direnci önerilir (genelde sketch çalışır; sorun olursa ekle).

---

### Toprak Nem Sensörü

| Sensör Pini | D1 R32 Pini | Jumper Rengi |
|------------|-------------|--------------|
| VCC (+) | **3V3** | 🟠 Turuncu |
| GND (-) | **GND** | ⚫ Siyah |
| AO | **GPIO36** (A0 / VP etiketi) | 🟢 Yeşil |
| DO | — (takma) | — |

> UYARI: VCC'yi **5V'a bağlama** — AO çıkışı 5V olur, ESP32 ADC'si 3.3V max → hasar!

---

### Su Seviye Sensörü

| Sensör Pini | D1 R32 Pini | Jumper Rengi |
|------------|-------------|--------------|
| VCC (+) | **3V3** | 🟠 Turuncu |
| GND (-) | **GND** | ⚫ Siyah |
| AO | **GPIO39** (A1 / VN etiketi) | 🔵 Mavi |
| DO | — (takma) | — |

---

### Tek Kanal Röle Modülü

| Röle Pini | Bağlantı | Jumper Rengi |
|----------|---------|--------------|
| VCC (+) | **5V** | 🔴 Kırmızı |
| GND (-) | **GND** | ⚫ Siyah |
| IN | **GPIO5** (D8 etiketi) | ⚪ Beyaz |

**Pompa hattı:**

| Röle terminali | Bağlantı |
|---------------|---------|
| COM | 5V adaptör (+) |
| NO (normally open) | Pompa (+) |
| Pompa (-) | GND |

> Dalgıç pompa gelince bu şema geçerlidir. Şimdilik röle TIK/KAPA sesi ile test edilir.

---

## Fiziksel Breadboard Yerleşimi

```
Breadboard üst rail:
  [+] ── 3V3 (🟠) ── DHT VCC, Toprak VCC, Su VCC
  [-] ── GND  (⚫) ── DHT GND, Toprak GND, Su GND, Röle GND

Breadboard alt rail (ayrı, röle için):
  [+] ── 5V  (🔴) ── Röle VCC
  [-] ── GND (⚫) ── bağlı kalır

D1 R32:
  3V3 ── üst [+] rail (🟠)
  GND ── üst [-] rail (⚫)
  5V  ── alt [+] rail (🔴)
  GND ── alt [-] rail (⚫)
  GPIO4  ── DHT DATA (🟡)
  GPIO36 ── Toprak AO (🟢)
  GPIO39 ── Su AO    (🔵)
  GPIO5  ── Röle IN  (⚪)
```

---

## Neden Bu Pinler?

### GPIO36 ve GPIO39 — ADC1

ESP32'de iki ADC grubu var:
- **ADC1** (GPIO32–39): WiFi açıkken güvenilir çalışır ✅
- **ADC2** (GPIO0,2,4,12-15,25-27): WiFi açınca çakışır, yanlış okuma ❌

Sensör analog çıkışları ADC1'e bağlanır → WiFi ile eş zamanlı çalışır.

### GPIO4 — DHT11

Dijital I/O, interrupt destekli, DHT protokolü için uygun.

### GPIO5 — Röle

Sağlam dijital çıkış, 3.3V HIGH → transistörlü röle modülü tetiklenir.

---

## İleride Eklenecek (Sipariş Gelince)

| Bileşen | Pin |
|---------|-----|
| OLED SDA | GPIO21 |
| OLED SCL | GPIO22 |
| 2. röle | GPIO18 |
| 2. toprak nem | GPIO34 |

---

## Güç Özeti

```
USB → D1 R32 → dahili regülatör → 3.3V (sensörler)
                               → 5V  (USB'den pasif, röle için)

5V adaptör (pompa gelince) → Röle COM → Pompa (+)
                                         Pompa (-) → GND
```

D1 R32 USB'den sağlıklı 5V üretir → röle modülü için yeterli.  
Dalgıç pompa için **ayrı adaptör** kullan (D1 R32 USB'si pompa akımını kaldıramaz).
