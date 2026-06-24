# Toprak Nem + Röle + DHT11 + ESP8266 — Tam Bağlantı

> Ref: docs/malzemeler.md · Modül 23

Mevcut çalışan DHT11 + ESP8266 kurulumuna **toprak nem** ve **pompa rölesi** eklenir.

---

## Pin özeti

| Bileşen | Arduino pini |
|---------|--------------|
| ESP8266 TX | **D2** |
| ESP8266 RX | **D3** (+ 3×1K direnç) |
| DHT11 DATA | **D4** |
| Röle IN | **D5** |
| Toprak nem AO | **A0** |
| Toprak guc (BC547 varsa) | **D7** |

> **D4'e röle bağlamayın** — DHT11 orada.

---

## Toprak nem sensörü (4 pin modül)

| Sensör | Arduino |
|--------|---------|
| **VCC** | **5V** |
| **GND** | **GND** |
| **AO** | **A0** |
| DO | bağlanmaz |

Probu toprağa / ıslak bez parçasına batırın. BC547 güç anahtarınız yoksa `SOIL_POWER_PIN = -1` bırakın (sketch'te varsayılan D7; BC547 yoksa kodu `-1` yapın).

---

## 5V röle kartı

| Röle | Arduino |
|------|---------|
| **VCC** | **5V** |
| **GND** | **GND** |
| **IN** | **D5** |

### Dalgiç pompa + röle (ayrı güç)

Pompa **Arduino'dan değil**, adaptorden beslenir. Röle sadece **anahtar** gibi davranır.

| Bağlantı | Nereye |
|----------|--------|
| Adaptör **+** | Röle **COM** |
| Pompa **+** (kırmızı) | Röle **NO** |
| Pompa **-** (siyah) | Adaptör **-** (GND) |
| Röle VCC/GND/IN | Arduino 5V, GND, **D5** |

```
  [Saksı / toprak]  ←── hortum ──┐
                                 │
  ┌──────────────────────────────┴── Su deposu (kap, kova)
  │  ~~~~~~  dalgiç pompa (TAMAMEN suyun icinde)
  │  ──→ hortum cikisi
  └──────────────────────────────
         ↑ elektrik: röle NO/COM uzerinden
```

| Adim | Ne yaparsin |
|------|-------------|
| 1 | Plastik kap doldur (pompa en az su icinde kalmali) |
| 2 | Dalgiç pompayi suya batir; hortumu saksiya yonlendir |
| 3 | Voltaj: pompa etiketi **5V** ise 5V adaptör, **12V** ise 12V |
| 4 | Ilk test: röle TIK (susuz); sonra su ile 5 sn dashboard testi |

**Ziraat notu:** Fitil pasif emer; dalgiç pompa **aktif basinc** verir — ne zaman, kac saniye sulayacagini sen (veya otomatik kural) secersin.

**Uyari:** Susuz calistirma motoru yakar. Sonraki adim: **su seviye sensörü (A3)** — depo bosken pompa blok.

---

## ESP8266 + DHT11

Değişmez — bkz. [22-dht-esp8266-baglanti.md](22-dht-esp8266-baglanti.md)

---

## Yükleme sırası

1. **Önce** `11-toprak-nem.ino` ile sadece toprak sensörünü test edin (AO→A0).
2. **Sonra** `15-relay.ino` ile röleyi test edin — IN pinini geçici **D5** yapın.
3. Dalgiç pompayi röle hattina baglayin (once susuz TIK, sonra su).
4. `22-mqtt-publish.ino` yükleyin.
5. PC'de `start-dashboard.bat` çalıştırın.
6. Seri monitör **9600** → `sensor OK`, `Toprak: %XX`.
7. Dashboard **Pompa Aç (5 sn)** veya otomatik sulama (nem <%30).

---

## Dashboard pompa kontrolü

```
Dashboard → POST /api/actuators/pump
ESP       → GET  /api/sensors/ingest/simple → OK PUMP 5 ...
```

---

## Sorun giderme

| Sorun | Çözüm |
|-------|--------|
| Toprak hep %0 veya %100 | Probu toprağa batırın; A0 kablosunu kontrol edin |
| Röle tik, pompa donuk | Voltaj uyumu; NO/COM kabloları; pompa su icinde mi |
| Pompa calisiyor su yok | Hortum tikanik veya pompa cok yuksek (hava emiyor) |
| Röle tıklamıyor | IN → D5; `15-relay` ayri test |
| DHT hatası | DATA D4'te; röle D5'te |
