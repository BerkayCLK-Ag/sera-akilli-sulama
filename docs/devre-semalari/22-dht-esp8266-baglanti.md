# DHT11 + ESP8266 — Birlikte Bağlantı

> Ref: docs/malzemeler.md · Modül 22

ESP8266 **D2 ve D3** pinlerini kullandığı için DHT11 **D4**'e bağlanır.

---

## DHT11 (3 pin)

| DHT11 | Arduino |
|-------|---------|
| **+** (VCC) | **5V** |
| **-** (GND) | **GND** |
| **S** (DATA) | **D4** |

Modülde direnç varsa ek direnç gerekmez. Yoksa DATA ile 5V arası **10K** pull-up.

---

## ESP8266 (aynı kalır)

| ESP-01 | Arduino |
|--------|---------|
| VCC, EN, IO0 | **3.3V** |
| GND | GND |
| TX (pin 5) | **D2** |
| RX (pin 4) | **D3** → direnç hattı |

---

## PC (Dashboard)

| | |
|-|-|
| PC IP | `192.168.1.106` (ipconfig ile doğrula) |
| ESP IP | `192.168.1.195` |
| Dashboard | http://localhost:8000 |
| Backend | `0.0.0.0:8000` (ağdan erişim için) |

---

## Yükleme sırası

1. **DHT11** + **ESP8266** kablolarını bağla
2. Arduino IDE → **DHT sensor library** (Adafruit) yükle
3. `22-mqtt-publish.ino` aç → **Yükle**
4. PC'de `start-dashboard.bat` çalıştır
5. Seri monitör **9600** → `Dashboard'a gonderildi` gör
6. Tarayıcıda sıcaklık/nem güncellenir

**Sonraki adım:** Toprak nem + pompa → [23-toprak-role-esp8266-baglanti.md](23-toprak-role-esp8266-baglanti.md)

---

## Sorun giderme

| Sorun | Çözüm |
|-------|--------|
| DHT11 okuma hatasi | DATA → D4, 5V/GND kontrol |
| Gonderilemedi | PC'de backend çalışıyor mu? Firewall 8000 |
| WiFi hatasi | 2.4 GHz SSID/sifre |
