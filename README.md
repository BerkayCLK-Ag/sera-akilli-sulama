# Akıllı Sera IoT — Ziraat + IoT Portföy Projesi

[![GitHub](https://img.shields.io/badge/GitHub-BerkayCLK--Ag-181717?logo=github)](https://github.com/BerkayCLK-Ag/sera-akilli-sulama)

ESP32 tabanlı **akıllı sulama ve sera izleme prototipi**. Ziraat mühendisliği perspektifiyle tasarlandı; sensör okuma, otomatik pompa kontrolü, FastAPI backend ve canlı dashboard içerir.

> **Hedef kitle:** e-sular, Ubicro ve benzeri tarım teknolojisi firmalarına yönelik portföy / saha IoT deneyimi.

## Öne Çıkan Özellikler

- **ESP32 D1 R32** tek kart mimari (DHT11, toprak nem, su seviyesi)
- **Otomatik sulama:** toprak nem eşiği, hedef nemde durma, depo boş kilidi
- **Röle + dalgıç pompa** kontrolü (active-low, güvenlik süresi)
- **OLED** lokal ekran + **web dashboard** (canlı veri, pompa kontrolü)
- **FastAPI** backend, UDP ile PC otomatik keşfi
- **32 adet** Arduino/ESP32 öğrenme modülü + devre şemaları

## Hızlı Başlangıç

```bash
# Dashboard (Windows)
start-dashboard.bat
# → http://localhost:8000

# Firmware (Arduino IDE)
firmware/arduino/greenhouse/sera-v3-prototip/sera-v3-prototip.ino
# Board: WEMOS D1 MINI ESP32 | Baud: 115200
# WiFi ayarlari: firmware/arduino/secrets.example.h -> secrets.h kopyala
```

## Proje Yapısı

```
sera-akilli-sulama/
├── docs/                    # Dokümantasyon, portföy, devre şemaları
├── firmware/arduino/
│   ├── learning/            # 01-32 öğrenme modülleri
│   └── greenhouse/          # sera-v3-prototip (ana firmware)
├── backend/                 # FastAPI + MQTT
├── frontend/                # Web dashboard
└── docker-compose.yml       # Mosquitto
```

## Dokümantasyon

| Dosya | Açıklama |
|-------|----------|
| [portfolio-yetkinlikler.md](docs/portfolio-yetkinlikler.md) | CV / iş başvurusu özeti |
| [sera-v3-prototip-kurulum.md](docs/sera-v3-prototip-kurulum.md) | Tam kurulum rehberi |
| [esp32-d1r32-calisma-plani.md](docs/esp32-d1r32-calisma-plani.md) | ESP32 geçiş planı |
| [ogrenme-yol-haritasi.md](docs/ogrenme-yol-haritasi.md) | Öğrenme sırası |
| [malzemeler.md](docs/malzemeler.md) | Donanım envanteri |

## Teknoloji Stack

`ESP32` · `Arduino` · `C++` · `Python` · `FastAPI` · `MQTT` · `SQLite` · `HTML/CSS/JS`

## Geliştirici

**Berkay CLK** — [GitHub @BerkayCLK-Ag](https://github.com/BerkayCLK-Ag)

Kişisel öğrenme ve prototip projesi. Her milestone GitHub'a dokümante edilir.
