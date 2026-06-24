# Öğrenme Yol Haritası

> Donanım referansı: [malzemeler.md](malzemeler.md)  
> Sistem mimarisi: [mimari.md](mimari.md)

Bu yol haritası biyosistem mühendisliği geçmişinden IoT'ye geçiş için tasarlanmıştır.
Her modülü sırayla tamamlayın; bir sonrakine geçmeden önce mevcut modülün çalıştığından emin olun.

---

## Genel Bakış

| Faz | Konu | Süre | Modül sayısı |
|-----|------|------|--------------|
| 1 | Arduino temelleri | 1-2 hafta | 7 |
| 2 | Sensör okuma | 1-2 hafta | 7 |
| 3 | Aktüatörler ve kontrol | 1-2 hafta | 6 |
| 4 | ESP8266 + MQTT | 1-2 hafta | 3 |
| 5 | Python backend | 1-2 hafta | — |
| 6 | Web dashboard | 1-2 hafta | — |
| 7 | Akıllı sera prototipi | 2-4 hafta | 1 |

**Toplam tahmini süre:** 4-5 ay (kendi hızınıza göre)

---

## Faz 1 — Arduino Temelleri

**Hedef:** Breadboard kullanımı, dijital/analog I/O, seri monitör.

| # | Modül | Bileşenler | Sketch | Tamamlandı |
|---|-------|-----------|--------|------------|
| 01 | Blink | UNO R3 dahili LED | `firmware/arduino/learning/01-blink/` | ☐ |
| 02 | Serial | USB kablo | `firmware/arduino/learning/02-serial/` | ☐ |
| 03 | LED | 330Ω, Kırmızı/Yeşil/Sarı LED | `firmware/arduino/learning/03-led/` | ☐ |
| 04 | RGB LED | RGB LED, dirençler | `firmware/arduino/learning/04-rgb/` | ☐ |
| 05 | Button | Push buton, 10K pull-down | `firmware/arduino/learning/05-button/` | ☐ |
| 06 | LCD | 2x16 LCD (4-bit mod) | `firmware/arduino/learning/06-lcd/` | ☐ |
| 07 | Potansiyometre | 10K pot, analogRead | `firmware/arduino/learning/07-pot/` | ☐ |

**Faz 1 çıktısı:** LCD'de "Merhaba Sera" yazan, butonla LED kontrol eden devre.

**Kontrol listesi:**
- [ ] Arduino IDE kuruldu
- [ ] CH340 sürücüsü yüklendi (gerekirse)
- [ ] Seri monitör 9600 baud ile çalışıyor
- [ ] Breadboard güç hatları (5V/GND) anlaşıldı

---

## Faz 2 — Sensör Okuma

**Hedef:** Sera için kritik çevresel verileri okumak ve yorumlamak.

| # | Modül | Bileşen | Sera kullanımı | Sketch | Tamamlandı |
|---|-------|---------|----------------|--------|------------|
| 08 | DHT11 | DHT11 | Ortam sıcaklık/nem | `08-dht11/` | ☐ |
| 09 | LM35 | LM35 | Hassas sıcaklık | `09-lm35/` | ☐ |
| 10 | LDR | LDR + 10K | Işık seviyesi | `10-ldr/` | ☐ |
| 11 | Toprak nem | Toprak nem sensörü | Sulama kararı | `11-toprak-nem/` | ☐ |
| 12 | Su seviye | Su seviye modülü | Depo takibi | `12-su-seviye/` | ☐ |
| 13 | HC-SR04 | Ultrasonik | Mesafe bazlı seviye | `13-hc-sr04/` | ☐ |
| 14 | MQ-2 | MQ-2 | Gaz güvenliği | `14-mq2/` | ☐ |
| — | Sensor Hub | Tüm sensörler | Entegre okuma | `14-sensor-hub/` | ☐ |

**Faz 2 çıktısı:** Tüm sensör değerlerini seri monitörde ve LCD'de gösteren entegre sketch.

**Biyosistem notu:** Toprak nem değerlerini bitki türüne göre yorumlayın — farklı bitkiler farklı nem aralıkları ister.

---

## Faz 3 — Aktüatörler ve Kontrol

**Hedef:** Sulama ve uyarı mekanizmalarını kontrol etmek.

| # | Modül | Bileşen | Sera kullanımı | Sketch | Tamamlandı |
|---|-------|---------|----------------|--------|------------|
| 15 | Röle | 5V röle kartı | Pompa aç/kapa | `15-relay/` | ☐ |
| 16 | Servo | SG90 | Vana/louver | `16-servo/` | ☐ |
| 17 | DC motor | DC motor + BC547 | Pompa simülasyonu | `17-dc-motor/` | ☐ |
| 18 | Buzzer | Buzzer | Alarm | `18-buzzer/` | ☐ |
| 19 | PIR | PIR sensör | Hareket algılama | `19-pir/` | ☐ |
| 20 | RTC | DS1302 | Zamanlanmış sulama | `20-rtc/` | ☐ |
| — | Otomatik sulama | Tümü | Nem < eşik → pompa | `20-auto-irrigation/` | ☐ |

**Faz 3 çıktısı:** Toprak nemi %30 altına düşünce röle ile pompayı 5 sn çalıştıran otomatik sulama.

---

## Faz 4 — ESP8266 + MQTT

**Hedef:** Sensör verilerini WiFi üzerinden bilgisayara göndermek.

| # | Modül | İçerik | Sketch | Tamamlandı |
|---|-------|--------|--------|------------|
| 21 | ESP8266 Serial | AT komutları, WiFi test | `21-esp8266-serial/` | ☐ |
| 22 | MQTT Publish | Sensör → MQTT broker | `22-mqtt-publish/` | ☐ |
| 23 | MQTT Subscribe | Broker → pompa komutu | `22-mqtt-subscribe/` | ☐ |

**Ön koşullar:**
- [ ] Mosquitto broker çalışıyor (`docker-compose up -d`)
- [ ] MQTT Explorer veya benzeri istemci kurulu
- [ ] ESP8266 voltaj bölücü devresi kuruldu

**MQTT topic'leri:** Bkz. [mimari.md](mimari.md#mqtt-topics)

---

## Faz 5 — Python Backend

**Hedef:** Sensör verilerini kaydetmek, REST API ve WebSocket sunmak.

**Adımlar:**
1. Python 3.10+ kur
2. `backend/` klasöründe venv oluştur
3. `pip install -r requirements.txt`
4. `.env` dosyasını düzenle
5. `docker-compose up -d` (Mosquitto + InfluxDB)
6. `uvicorn main:app --reload`

**Kontrol listesi:**
- [ ] `GET /api/sensors/latest` çalışıyor
- [ ] MQTT'den gelen veri InfluxDB'ye yazılıyor
- [ ] `POST /api/actuators/pump` pompa komutu gönderiyor

---

## Faz 6 — Web Dashboard

**Hedef:** Canlı sensör takibi, grafikler, pompa kontrolü.

**Adımlar:**
1. Backend çalışırken `http://localhost:8000` aç
2. Canlı kartları kontrol et
3. 24 saatlik grafikleri test et
4. Pompa manuel aç/kapa dene

---

## Faz 7 — Akıllı Sera Prototipi

**Hedef:** Sektöre sunulabilir entegre demo.

**Demo senaryosu:**
1. Dashboard'da canlı sera verileri
2. Toprak kuruduğunda otomatik sulama
3. Mobilden manuel pompa kontrolü
4. 24 saatlik veri grafiği
5. Kısa video + GitHub repo

**Firmware:** `firmware/arduino/greenhouse/greenhouse-controller.ino`

---

## Ek Modüller (Faz 7 Sonrası)

| Modül | Bileşen | Sketch |
|-------|---------|--------|
| Bluetooth | HC-06 | `23-bluetooth/` |
| RFID | RC522 | `24-rfid/` |
| Joystick | 2 eksen joystick | `25-joystick/` |
| IR kumanda | IR set | `26-ir-remote/` |
| Step motor | Step motor + sürücü | `27-stepper/` |
| Ses sensörü | Ses modülü | `28-sound/` |

---

## İlerleme Takibi

Modül tamamlandığında [malzemeler.md](malzemeler.md) dosyasında ilgili bileşenin **Test edildi** kutusunu işaretleyin (☐ → ☑).
