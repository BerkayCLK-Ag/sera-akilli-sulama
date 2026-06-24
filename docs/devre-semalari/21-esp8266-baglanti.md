# ESP8266 (ESP-01, 8 pin) — Arduino UNO Bağlantısı

> Ref: docs/malzemeler.md#esp8266

## 8 pin düzeni

Modülü **anteni yukarıda**, **yazılar sana dönük** tut:

```
           [ Anten ]
       ┌─────────────┐
       │   ESP-01    │
       └─────────────┘

  SOL (1-4)     SAĞ (5-8)
  ─────────     ─────────
  1 GND         5 TX
  2 IO2         6 EN  (CH_PD)
  3 IO0         7 RST
  4 RX          8 VCC
```

| Pin | Yazı | Bağlantı |
|-----|------|----------|
| 1 | GND | Arduino **GND** |
| 2 | IO2 / GPIO2 | Boş bırak (veya 3.3V) |
| 3 | IO0 / GPIO0 | **3.3V** (normal çalışma) |
| 4 | RX | Arduino **D3** (voltaj bölücü ile) |
| 5 | TX | Arduino **D2** |
| 6 | EN / CH_PD | **3.3V** (modül açık kalsın) |
| 7 | RST | Boş bırak |
| 8 | VCC | Arduino **3.3V** |

**5V ASLA** — sadece **3.3V**.

---

## Voltaj bölücü (D3 → pin 4 RX)

```
Arduino D3 ── 1KΩ ──┬── ESP RX (pin 4)
                    │
                   2KΩ
                    │
                   GND
```

---

## Tam tablo

| ESP-01 | Arduino |
|--------|---------|
| 8 VCC | **3.3V** |
| 6 EN | **3.3V** |
| 3 IO0 | **3.3V** |
| 1 GND | GND |
| 5 TX | **D2** |
| 4 RX | **D3** (1K+2K bölücü) |
| 7 RST | boş |
| 2 IO2 | boş |

---

## Breadboard ipucu

ESP breadboard'a **ortadan ikiye yay** tak (sol 4 + sağ 4 pin).  
3.3V ve GND jumper'ları breadboard **+ / -** hattından da gidebilir.

---

## Güç

Arduino 3.3V pini zayıf kalabilir → WiFi'de reset. İlk test için dene; reset olursa harici 3.3V gerekir.

Testte **röle/sensör sök** — sadece ESP + Arduino.

---

## Kod

`21-esp8266-serial.ino` — WiFi ad/şifre yaz → yükle → seri monitör 9600.

Pinler: **D2 + D3** (USB yükleme bozulmaz).

---

## Voltaj bölücü (D3 → ESP RX) — ZORUNLU

Arduino D3 = 5V sinyal, ESP RX = 3.3V max.

```
Arduino D3 ── 1KΩ ──┬── ESP RX
                    │
                   2KΩ
                    │
                   GND
```

Kitinde 1K + 2K direnç var.

---

## Tam bağlantı tablosu

| ESP8266 | Arduino UNO |
|---------|-------------|
| VCC | **3.3V** |
| GND | GND |
| EN (CH_PD) | **3.3V** |
| IO0 | **3.3V** (veya 10K ile 3.3V) |
| TX | **D2** |
| RX | **D3** → (1K+2K bölücü) |

**Çapraz bağlantı:** ESP TX → Arduino RX (D2), Arduino TX (D3) → ESP RX

---

## Güç uyarısı

Arduino **3.3V pini ~50mA** verir. WiFi bağlanırken ESP daha fazla çeker — **reset atabilir**.

- İlk test için 3.3V pin yeterli olabilir
- Takılı kalırsa / reset olursa → harici 3.3V regülatör gerekir
- Test sırasında **sadece ESP + Arduino** bağlı olsun (röle vs. sök)

---

## Koda yüklemeden önce

Sketch **D2/D3** kullanır (D0/D1 değil) → USB ile yükleme sorunsuz.

1. WiFi adı/şifre: `21-esp8266-serial.ino` içinde düzenle
2. Yükle
3. Seri monitör **9600**
4. `OK` ve WiFi IP görürsen tamam

---

## Pin tanıyamazsan

Modülün üstündeki yazıları fotoğrafla — pin eşlemesini birlikte yaparız.
