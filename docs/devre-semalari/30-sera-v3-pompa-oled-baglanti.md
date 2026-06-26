# Sera v3 — Pompa + OLED + Sensör Bağlantı Şeması

> **Ref:** docs/sera-v3-prototip-kurulum.md

---

## Güç hatları (breadboard)

```
ESP 3V3 ──🟠── BB üst [+] ── DHT, Toprak, Su, OLED VCC
ESP GND ──⚫── BB üst [-] ── DHT, Toprak, Su, OLED GND
ESP 5V  ──🔴── BB alt [+] ── Röle VCC
ESP GND ──⚫── BB alt [-] ── Röle GND
Adaptör GND ──⚫── ESP GND (ortak!)
```

---

## OLED 0.96" I2C (SSD1306)

| OLED | D1 R32 | Kablo |
|------|--------|-------|
| VCC | 3V3 şerit | Mor/turuncu |
| GND | GND şerit | Siyah |
| SDA | **IO21** | Mor |
| SCL | **IO22** | Gri |

I2C adresi: **0x3C** (çoğu modül). Çalışmazsa 0x3D dene.

---

## Dalgıç pompa + röle (5V adaptör)

```
                    ┌─────────────┐
  5V Adaptör (+) ───┤ Röle COM    │
                    │             │
  5V Adaptör (-) ───┼── GND ortak ├── ESP GND
                    │             │
                    │  Röle NO ───┼──→ Pompa (+)
                    └─────────────┘    Pompa (-) ──→ GND

  ESP IO5 ──⚪── Röle IN
  ESP 5V  ──🔴── Röle VCC (modül bobini)
  ESP GND ──⚫── Röle GND
```

### Flyback diyot (pompa üzerinde)

```
Pompa (+) ────|>|──── Pompa (-)
              diyot
        (çizgi = katot → + tarafa)
```

Röle kapanınca pompa bobini geri voltaj üretir — diyot korur.

---

## Sensörler (değişmedi)

| Sensör | Sinyal pini |
|--------|-------------|
| DHT11 | IO4 |
| Toprak | IO36 |
| Su seviye | IO39 |

Toprak probu → **saksı toprağında**  
Su probu → **depo içinde**

---

## Fiziksel yerleşim önerisi

```
[Masa]
  Sol: D1 R32 + breadboard
  Orta: OLED (yukarı bakacak şekilde)
  Sağ: Depo (pompa içerde) + saksı (1 m hortum)
```

DHT11 mini sera kutusunda veya saksı yanında — gerçek ortamı ölçer.
