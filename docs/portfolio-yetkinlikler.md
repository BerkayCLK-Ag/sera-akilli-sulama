# Portfolio — GitHub ve Is Basvurulari Icin

> Bu dosya her ogrenme gorevinde guncellenir. E-sular, Ubicro gibi tarim teknolojisi firmalarinin dikkatini cekecek sekilde yazilir.
> Son guncelleme: 2026-06-26

## Kimlik

**Berkay CLK** — Ziraat muhendisligi altyapisini IoT, gomulu sistemler ve tarimsal otomasyonla birlestiren gelistirici adayi.

- GitHub: [BerkayCLK-Ag](https://github.com/BerkayCLK-Ag)
- Ana proje: [sera-akilli-sulama](https://github.com/BerkayCLK-Ag/sera-akilli-sulama)

## One Cikan Yetkinlikler

| Alan | Seviye | Kanit |
|------|--------|-------|
| ESP32 / Arduino firmware | Orta | 32 ogrenme modulu + sera-v3 prototip |
| Sensor okuma ve kalibrasyon | Orta | DHT11, toprak nem, su seviye, ADC kalibrasyonu |
| Otomatik sulama mantigi | Orta | Esik, hedef nem, cooldown, depo kilidi |
| Röle ve pompa kontrolu | Orta | Active-low röle, NO/NC, ayri guc beslemesi |
| FastAPI backend | Baslangic-Orta | REST ingest, UDP discovery, WebSocket |
| Web dashboard | Baslangic-Orta | Canli sensor, pompa kontrolu, trend grafikleri |
| MQTT / IoT mimari | Baslangic | Mosquitto, publish/subscribe, cihaz durumu |
| Dokumantasyon | Iyi | Devre semalari, kurulum rehberleri, karar gunlugu |

## Tamamlanan Milestone'lar

### 2026-06 — Temel Akilli Sulama Prototipi (Sera v3)

- ESP32 D1 R32 uzerinde tek kart mimari (Arduino UNO + ESP8266 yerine)
- DHT11, toprak nem, su seviye sensorleri entegre
- SSD1306 OLED lokal ekran
- 2 kanalli röle ile dalgic pompa kontrolu
- WiFi + FastAPI backend + canli dashboard
- Otomatik sulama: toprak nem esigi, hedef nemde durma, depo bos kilidi
- UDP ile PC otomatik kesfi (elle IP yazmadan)
- Renk kodlu breadboard semalari ve kurulum dokumantasyonu

### 2026-06 — Ogrenme Altyapisi

- 01-32 arasi Arduino/ESP32 ogrenme modulleri
- Sistemli dokumantasyon: ogrenme gunlugu, karar gunlugu, research notes
- GitHub bilgi koruma ve otomatik push akisi

## Teknoloji Stack

```
Firmware:  C++ / Arduino, ESP32, DHT, I2C OLED, ADC kalibrasyonu
Backend:   Python, FastAPI, SQLite, MQTT (Mosquitto)
Frontend:  HTML, CSS, JavaScript, WebSocket
DevOps:    Docker Compose, Windows batch, UDP discovery
Tarım:     Toprak nem yonetimi, sulama esigi, sera iklim izleme
```

## Firmalara Yonelik Proje Ozeti (CV / Basvuru)

> ESP32 tabanli akilli sulama prototipi gelistirdim. Sistem toprak nemi, ortam sicakligi/nemi ve su deposu seviyesini olcerek otomatik veya manuel pompa kontrolu saglar. FastAPI backend uzerinden canli dashboard ile izlenir; UDP discovery ile ag uzerinde otomatik baglanti kurulur. Ziraat muhendisligi perspektifiyle sensor kalibrasyonu, sulama esigi ve bitki su ihtiyaci mantigi uygulanmistir.

## Ogrenme Kayitlari (Otomatik Eklenir)

Her gorev sonunda asagiya yeni satir eklenir:

| Tarih | Konu | Firma Degeri |
|-------|------|--------------|
| 2026-06-26 | ESP32 tek kart gecisi, sera v3 prototip | Saha IoT cihaz gelistirme |
| 2026-06-26 | Toprak nem kalibrasyonu, pompa guvenlik mantigi | Akilli sulama urun gelistirme |
| 2026-06-26 | WiFi stabilite, brownout, guc yonetimi | Uretim oncesi prototip olgunlastirma |
| 2026-06-26 | GitHub portfolyo ve otomatik dokumantasyon sistemi | Muhendislik kulturu, dokumante calisma |

## Sonraki Hedefler (Sera v4)

- [ ] Bitki bazli sulama profilleri (domates, marul, biber)
- [ ] Sulama gecmisi ve CSV rapor
- [ ] MQTT tabanli coklu cihaz destegi
- [ ] LoRa uzak sensor dugumu
- [ ] Demo videosu + GitHub profil README
