# Malzeme Envanteri — Tek Kaynak Referans

> **Bu dosya projenin merkezi donanım referansıdır.**
> Tüm sketch'ler, devre notları ve dashboard alanları bu dosyaya referans verir.
> Format: `// Ref: docs/malzemeler.md#bilesen-adi`

**Set:** Arduino Uyumlu Proje Seti  
**Son güncelleme:** 2025-06-20

---

## İçindekiler

1. [Ana Kart ve Bağlantı](#ana-kart-ve-baglanti)
2. [Prototipleme Malzemeleri](#prototipleme-malzemeleri)
3. [Dirençler](#direnc-ler)
4. [LED'ler](#led-ler)
5. [Transistör ve Potansiyometre](#transistor-ve-potansiyometre)
6. [Sensörler](#sensorler)
7. [Ekran ve Zaman](#ekran-ve-zaman)
8. [Güç Kaynağı](#guc-kaynaklari)
9. [Giriş Cihazları](#giris-cihazlari)
10. [Aktüatörler](#aktuatorler)
11. [Kablosuz Modüller](#kablosuz-moduller)
12. [Eksik / Önerilen Malzemeler](#eksik-onerilen-malzemeler)

---

## Ana Kart ve Bağlantı

### arduino-uno-r3 {#arduino-uno-r3}

| Özellik | Değer |
|---------|-------|
| Miktar | 1 adet |
| Model | Arduino UNO R3 (CH340 USB-Serial) |
| Çalışma voltajı | 5V |
| Giriş voltajı | 7-12V (barrel jack) veya USB |
| Dijital I/O | 14 pin (6 PWM) |
| Analog giriş | A0-A5 (6 kanal) |
| Maksimum akım/pin | 40 mA |
| Flash bellek | 32 KB |
| Kullanım alanı (sera) | Ana kontrol ünitesi — tüm sensör ve aktüatör yönetimi |
| İlgili sketch | Tüm firmware |
| Öğrenme fazı | Faz 1 — Temeller |
| Test edildi | ☐ |

**Pin özeti (sera projesi için ayrılmış):**

| Pin | Atama |
|-----|-------|
| D2 | DHT11 DATA |
| D3 | PWM — Servo SG90 |
| D4 | Röle IN |
| D5 | PWM — Buzzer |
| D6 | HC-SR04 TRIG |
| D7 | HC-SR04 ECHO |
| D8-D11 | LCD (RS, E, D4, D5) |
| D12-D13 | LCD (D6, D7) |
| A0 | Toprak nem sensörü |
| A1 | LDR (ışık) |
| A2 | LM35 sıcaklık |
| A3 | Su seviye sensörü |
| A4 | MQ-2 (analog) |
| D0/D1 | ESP8266 RX/TX (SoftwareSerial) |

---

### usb-kablosu {#usb-kablosu}

| Özellik | Değer |
|---------|-------|
| Miktar | 1 adet |
| Tip | USB-A → USB-B (A'dan B'ye) |
| Kullanım | UNO programlama ve seri monitör |
| Öğrenme fazı | Faz 1 |
| Test edildi | ☐ |

---

## Prototipleme Malzemeleri

### breadboard {#breadboard}

| Özellik | Değer |
|---------|-------|
| Miktar | 1 adet |
| Tip | Büyük boy breadboard (830 tie-point) |
| Kullanım | Devre prototipleme, sensör/actuator bağlantıları |
| Öğrenme fazı | Faz 1 |
| Test edildi | ☐ |

---

### jumper-erkek-erkek {#jumper-erkek-erkek}

| Özellik | Değer |
|---------|-------|
| Miktar | 40 pin (ayrılabilir) |
| Tip | Erkek-Erkek jumper kablo |
| Kullanım | Breadboard içi bağlantılar |
| Öğrenme fazı | Faz 1 |
| Test edildi | ☐ |

---

### jumper-erkek-dis {#jumper-erkek-dis}

| Özellik | Değer |
|---------|-------|
| Miktar | 40 pin (ayrılabilir) |
| Tip | Erkek-Dişi jumper kablo |
| Kullanım | Arduino pin → breadboard/sensör bağlantısı |
| Öğrenme fazı | Faz 1 |
| Test edildi | ☐ |

---

## Dirençler

### direnc-330ohm {#direnc-330ohm}

| Özellik | Değer |
|---------|-------|
| Miktar | 10 adet |
| Değer | 330 Ω |
| Renk kodu | Turuncu-Turuncu-Kahverengi |
| Kullanım | LED akım sınırlama (~10 mA @ 5V) |
| Öğrenme fazı | Faz 1 |
| Test edildi | ☐ |

---

### direnc-1kohm {#direnc-1kohm}

| Özellik | Değer |
|---------|-------|
| Miktar | 5 adet |
| Değer | 1 KΩ |
| Renk kodu | Kahverengi-Siyah-Kırmızı |
| Kullanım | Voltaj bölücü (ESP8266 level shifter), pull-up |
| Öğrenme fazı | Faz 1, Faz 4 |
| Test edildi | ☐ |

---

### direnc-10kohm {#direnc-10kohm}

| Özellik | Değer |
|---------|-------|
| Miktar | 5 adet |
| Değer | 10 KΩ |
| Renk kodu | Kahverengi-Siyah-Turuncu |
| Kullanım | Buton pull-down, LDR voltaj bölücü, DHT11 pull-up |
| Öğrenme fazı | Faz 1, Faz 2 |
| Test edildi | ☐ |

---

## LED'ler

### led-kirmizi {#led-kirmizi}

| Özellik | Değer |
|---------|-------|
| Miktar | 5 adet |
| Tip | 5 mm Kırmızı LED |
| Forward voltaj | ~2.0V |
| Direnç | 330 Ω seri |
| Kullanım (sera) | Alarm / hata göstergesi |
| İlgili sketch | firmware/arduino/learning/03-led/ |
| Öğrenme fazı | Faz 1 |
| Test edildi | ☐ |

---

### led-yesil {#led-yesil}

| Özellik | Değer |
|---------|-------|
| Miktar | 5 adet |
| Tip | 5 mm Yeşil LED |
| Forward voltaj | ~2.2V |
| Kullanım (sera) | Normal durum / pompa aktif göstergesi |
| İlgili sketch | firmware/arduino/learning/03-led/ |
| Öğrenme fazı | Faz 1 |
| Test edildi | ☐ |

---

### led-sari {#led-sari}

| Özellik | Değer |
|---------|-------|
| Miktar | 5 adet |
| Tip | 5 mm Sarı LED |
| Forward voltaj | ~2.1V |
| Kullanım (sera) | Uyarı / nem düşük göstergesi |
| İlgili sketch | firmware/arduino/learning/03-led/ |
| Öğrenme fazı | Faz 1 |
| Test edildi | ☐ |

---

### rgb-led {#rgb-led}

| Özellik | Değer |
|---------|-------|
| Miktar | 1 adet |
| Tip | Ortak katot veya ortak anot RGB LED (4 pin) |
| Pinler | R, G, B, COM |
| Direnç | 330 Ω × 3 (her kanal) |
| Kullanım (sera) | Çok seviyeli durum göstergesi (nem/sıcaklık renk kodu) |
| İlgili sketch | firmware/arduino/learning/04-rgb/ |
| Öğrenme fazı | Faz 1 |
| Test edildi | ☐ |

---

## Transistör ve Potansiyometre

### bc547 {#bc547}

| Özellik | Değer |
|---------|-------|
| Miktar | 1 adet |
| Tip | NPN Transistör (BC547) |
| Pinler (düz yüz) | Sol: Kollektör, Orta: Baz, Sağ: Emiter |
| Maks. kollektör akımı | 100 mA |
| Kullanım (sera) | DC motor sürme, toprak nem sensörü güç anahtarı |
| Baz direnci | 1 KΩ önerilir |
| İlgili sketch | firmware/arduino/learning/17-dc-motor/ |
| Öğrenme fazı | Faz 3 |
| Test edildi | ☐ |

---

### potansiyometre-10k {#potansiyometre-10k}

| Özellik | Değer |
|---------|-------|
| Miktar | 1 adet |
| Değer | 10 KΩ |
| Pinler | GND, SIG (ortanca), VCC |
| Kullanım (sera) | Manuel eşik ayarı (sulama nem limiti simülasyonu) |
| Arduino pin | A0 (test), dashboard'dan ayarlanır (prototip) |
| İlgili sketch | firmware/arduino/learning/07-pot/ |
| Öğrenme fazı | Faz 1 |
| Test edildi | ☐ |

---

## Sensörler

### ldr {#ldr}

| Özellik | Değer |
|---------|-------|
| Miktar | 1 adet |
| Tip | 5 mm LDR (Işık Dependent Resistor) |
| Çalışma | Karanlıkta yüksek direnç, aydınlıkta düşük |
| Bağlantı | Voltaj bölücü: LDR + 10K direnç |
| Arduino pin | A1 |
| Kullanım (sera) | Işık seviyesi takibi, bitki büyüme ışığı analizi |
| İlgili sketch | firmware/arduino/learning/10-ldr/ |
| Öğrenme fazı | Faz 2 |
| Test edildi | ☐ |

---

### hc-sr04 {#hc-sr04}

| Özellik | Değer |
|---------|-------|
| Miktar | 1 adet |
| Tip | HC-SR04 Ultrasonik Mesafe Sensörü |
| Çalışma voltajı | 5V |
| Ölçüm aralığı | 2 cm – 400 cm |
| Doğruluk | ±3 mm |
| Pinler | VCC, TRIG, ECHO, GND |
| Arduino pin | D6 (TRIG), D7 (ECHO) |
| Kullanım (sera) | Su deposu seviye ölçümü (alternatif yöntem) |
| İlgili sketch | firmware/arduino/learning/13-hc-sr04/ |
| Öğrenme fazı | Faz 2 |
| Test edildi | ☐ |

---

### lm35 {#lm35}

| Özellik | Değer |
|---------|-------|
| Miktar | 1 adet |
| Tip | LM35 Sıcaklık Sensörü |
| Çalışma voltajı | 4V – 30V |
| Çıkış | 10 mV/°C (lineer analog) |
| Arduino pin | A2 |
| Kullanım (sera) | Hassas ortam sıcaklığı (DHT11 yedek/doğrulama) |
| İlgili sketch | firmware/arduino/learning/09-lm35/ |
| Öğrenme fazı | Faz 2 |
| Test edildi | ☐ |

---

### dht11 {#dht11}

| Özellik | Değer |
|---------|-------|
| Miktar | 1 adet |
| Tip | DHT11 Sıcaklık ve Nem Sensörü |
| Çalışma voltajı | 3.3V – 5V |
| Sıcaklık aralığı | 0–50°C (±2°C) |
| Nem aralığı | 20–90% RH (±5%) |
| Pinler | VCC, DATA, NC, GND |
| Arduino pin | D2 |
| Pull-up | 10K (modül üzerinde genelde mevcut) |
| Kütüphane | DHT sensor library (Adafruit) |
| Kullanım (sera) | Ortam sıcaklığı ve nem takibi — **kritik sensör** |
| İlgili sketch | firmware/arduino/learning/08-dht11/ |
| Öğrenme fazı | Faz 2 |
| Test edildi | ☐ |

---

### mq2 {#mq2}

| Özellik | Değer |
|---------|-------|
| Miktar | 1 adet |
| Tip | MQ-2 Gaz Sensör Modülü |
| Algılanan gazlar | LPG, duman, propan, metan, hidrojen |
| Çalışma voltajı | 5V |
| Pinler | VCC, GND, DO (dijital), AO (analog) |
| Arduino pin | A4 (analog) veya D? (dijital) |
| Isınma süresi | ~20 saniye (preheat gerekli) |
| Kullanım (sera) | Yangın/duman güvenliği (opsiyonel) |
| İlgili sketch | firmware/arduino/learning/14-mq2/ |
| Öğrenme fazı | Faz 2 |
| Test edildi | ☐ |

---

### toprak-nem {#toprak-nem}

| Özellik | Değer |
|---------|-------|
| Miktar | 1 adet |
| Tip | Toprak Nem Algılama Sensörü |
| Çalışma voltajı | 3.3V – 5V |
| Pinler | VCC, GND, AO (analog), DO (dijital) |
| Arduino pin | A0 |
| Çıkış | Kuru: ~1023, Islak: ~300 (yaklaşık) |
| **Önemli** | Korozyonu azaltmak için sadece ölçüm anında güç verin (BC547 ile) |
| Kullanım (sera) | Sulama kararı — **kritik sensör** |
| İlgili sketch | firmware/arduino/learning/11-toprak-nem/ |
| Öğrenme fazı | Faz 2 |
| Test edildi | ☐ |

---

### su-seviye {#su-seviye}

| Özellik | Değer |
|---------|-------|
| Miktar | 1 adet |
| Tip | Su Seviye Sensör Modülü |
| Çalışma voltajı | 3.3V – 5V |
| Pinler | VCC, GND, AO, DO |
| Arduino pin | A3 |
| Kullanım (sera) | Su deposu / sulama tankı seviye takibi |
| İlgili sketch | firmware/arduino/learning/12-su-seviye/ |
| Öğrenme fazı | Faz 2 |
| Test edildi | ☐ |

---

### pir {#pir}

| Özellik | Değer |
|---------|-------|
| Miktar | 1 adet |
| Tip | PIR Hareket Sensörü (HC-SR501 benzeri) |
| Çalışma voltajı | 5V – 20V |
| Pinler | VCC, OUT, GND |
| Çıkış | HIGH = hareket algılandı |
| Isınma süresi | ~30 saniye |
| Kullanım (sera) | Sera girişi güvenlik / ziyaretçi algılama |
| İlgili sketch | firmware/arduino/learning/19-pir/ |
| Öğrenme fazı | Faz 3 |
| Test edildi | ☐ |

---

### ses-sensoru {#ses-sensoru}

| Özellik | Değer |
|---------|-------|
| Miktar | 1 adet |
| Tip | Ses Sensör Kartı (mikrofon modülü) |
| Çalışma voltajı | 3.3V – 5V |
| Pinler | VCC, GND, AO, DO |
| Kullanım (sera) | Anormal ses algılama (opsiyonel, Faz 7 sonrası) |
| İlgili sketch | firmware/arduino/learning/28-sound/ |
| Öğrenme fazı | Ek modül |
| Test edildi | ☐ |

---

## Ekran ve Zaman

### lcd-2x16 {#lcd-2x16}

| Özellik | Değer |
|---------|-------|
| Miktar | 1 adet |
| Tip | 2×16 Karakter LCD (Işıksız, Yeşil üzerine siyah) |
| Kontrol modu | 4-bit paralel (I2C adaptör yok) |
| Pinler | VSS, VDD, V0, RS, RW, E, D0-D7, A, K |
| Kontrast | V0 potansiyometre veya 10K trimpot |
| Arduino pin | D8(RS), D9(E), D10-D13(D4-D7) |
| Kütüphane | LiquidCrystal (built-in) |
| Kullanım (sera) | Yerel durum gösterimi (WiFi yokken) |
| İlgili sketch | firmware/arduino/learning/06-lcd/ |
| Öğrenme fazı | Faz 1 |
| Test edildi | ☐ |

---

### ds1302 {#ds1302}

| Özellik | Değer |
|---------|-------|
| Miktar | 1 adet |
| Tip | DS1302 RTC (Gerçek Zaman Saati) Modülü |
| Çalışma voltajı | 3.3V – 5V |
| Pinler | VCC, GND, CLK, DAT, RST |
| Yedek pil | CR2032 (modül üzerinde) |
| Kütüphane | DS1302RTC veya Rtc_By_Makuna |
| Kullanım (sera) | Zamanlanmış sulama (06:00-08:00) |
| İlgili sketch | firmware/arduino/learning/20-rtc/ |
| Öğrenme fazı | Faz 3 |
| Test edildi | ☐ |

---

## Güç Kaynakları

### pil-9v {#pil-9v}

| Özellik | Değer |
|---------|-------|
| Miktar | 1 adet |
| Tip | 9V Alkalin pil |
| Kullanım | Taşınabilir test (kısa süreli) |
| Öğrenme fazı | Faz 1 |
| Test edildi | ☐ |

---

### barrel-donusturucu {#barrel-donusturucu}

| Özellik | Değer |
|---------|-------|
| Miktar | 1 adet |
| Tip | 9V pil → DC Barrel (5.5×2.1 mm) dönüştürücü kablo |
| Kullanım | UNO harici güç beslemesi |
| Öğrenme fazı | Faz 1 |
| Test edildi | ☐ |

---

### adaptor-12v {#adaptor-12v}

| Özellik | Değer |
|---------|-------|
| Miktar | 1 adet |
| Tip | 12V 500mA Adaptör |
| Kullanım | Sürekli çalışma, röle + pompa beslemesi |
| **Not** | UNO'ya doğrudan 12V verilebilir (regülatör ısınır); uzun süreli kullanımda 7-9V tercih edin |
| Öğrenme fazı | Faz 3, Faz 7 |
| Test edildi | ☐ |

---

## Giriş Cihazları

### push-buton {#push-buton}

| Özellik | Değer |
|---------|-------|
| Miktar | 2 adet |
| Tip | 4 Pin Push Buton (tactile switch) |
| Bağlantı | 10K pull-down direnç, dijital giriş |
| Kullanım (sera) | Manuel pompa tetikleme, mod değiştirme |
| İlgili sketch | firmware/arduino/learning/05-button/ |
| Öğrenme fazı | Faz 1 |
| Test edildi | ☐ |

---

### ir-kumanda {#ir-kumanda}

| Özellik | Değer |
|---------|-------|
| Miktar | 1 set |
| Tip | IR Alıcı + Verici Kumanda Seti |
| Alıcı pin | VCC, GND, OUT |
| Kütüphane | IRremote |
| Kullanım (sera) | Uzaktan pompa kontrolü (ek modül) |
| İlgili sketch | firmware/arduino/learning/26-ir-remote/ |
| Öğrenme fazı | Ek modül |
| Test edildi | ☐ |

---

### joystick {#joystick}

| Özellik | Değer |
|---------|-------|
| Miktar | 1 adet |
| Tip | 2 Eksen Joystick Modülü (PS2 tipi) |
| Pinler | VRx, VRy, SW, VCC, GND |
| Kullanım (sera) | Manuel servo/vana kontrolü (ek modül) |
| İlgili sketch | firmware/arduino/learning/25-joystick/ |
| Öğrenme fazı | Ek modül |
| Test edildi | ☐ |

---

### rfid-rc522 {#rfid-rc522}

| Özellik | Değer |
|---------|-------|
| Miktar | 1 kit |
| Tip | RC522 RFID/NFC Okuyucu + Kart/Etiket |
| İletişim | SPI |
| Çalışma voltajı | 3.3V (**5V doğrudan bağlamayın**) |
| Kütüphane | MFRC522 |
| Kullanım (sera) | Kullanıcı yetkilendirme — kim sulama yaptı? |
| İlgili sketch | firmware/arduino/learning/24-rfid/ |
| Öğrenme fazı | Ek modül |
| Test edildi | ☐ |

---

## Aktüatörler

### buzzer {#buzzer}

| Özellik | Değer |
|---------|-------|
| Miktar | 1 adet |
| Tip | Aktif veya pasif Buzzer (5V) |
| Pin | D5 (PWM ile ton kontrolü) |
| Kullanım (sera) | Kritik alarm (nem düşük, su bitti, sıcaklık yüksek) |
| İlgili sketch | firmware/arduino/learning/18-buzzer/ |
| Öğrenme fazı | Faz 3 |
| Test edildi | ☐ |

---

### dc-motor {#dc-motor}

| Özellik | Değer |
|---------|-------|
| Miktar | 1 adet |
| Tip | DC Motor (3-6V) |
| Sürücü | BC547 transistör + diyot (flyback) |
| Kullanım (sera) | Pompa simülasyonu (gerçek pompa için röle kullanın) |
| İlgili sketch | firmware/arduino/learning/17-dc-motor/ |
| Öğrenme fazı | Faz 3 |
| Test edildi | ☐ |

---

### servo-sg90 {#servo-sg90}

| Özellik | Değer |
|---------|-------|
| Miktar | 1 adet |
| Tip | SG90 Mikro Servo Motor |
| Çalışma voltajı | 4.8V – 6V |
| Açı | 0° – 180° |
| Pin | D3 (PWM) |
| Kütüphane | Servo (built-in) |
| Kullanım (sera) | Vana açısı, havalandırma louver kontrolü |
| İlgili sketch | firmware/arduino/learning/16-servo/ |
| Öğrenme fazı | Faz 3 |
| Test edildi | ☐ |

---

### role-5v {#role-5v}

| Özellik | Değer |
|---------|-------|
| Miktar | 1 adet |
| Tip | 5V Tek Kanal Röle Kartı (optokuplörlü) |
| Pinler | VCC, GND, IN |
| Arduino pin | D4 |
| Kontrol | LOW veya HIGH tetik (kartına göre; genelde LOW = aktif) |
| **Uyarı** | Pompa/motor ayrı güç kaynağından beslenmeli; röle sadece anahtarlama yapar |
| Kullanım (sera) | Sulama pompası aç/kapa — **kritik aktüatör** |
| İlgili sketch | firmware/arduino/learning/15-relay/ |
| Öğrenme fazı | Faz 3 |
| Test edildi | ☐ |

---

### step-motor {#step-motor}

| Özellik | Değer |
|---------|-------|
| Miktar | 1 set |
| Tip | Redüktörlü Step Motor + Sürücü Kartı (ULN2003) |
| Pinler | IN1, IN2, IN3, IN4 |
| Kütüphane | Stepper (built-in) |
| Kullanım (sera) | Perde/havalandırma açma (ek modül) |
| İlgili sketch | firmware/arduino/learning/27-stepper/ |
| Öğrenme fazı | Ek modül |
| Test edildi | ☐ |

---

## Kablosuz Modüller

### esp8266 {#esp8266}

| Özellik | Değer |
|---------|-------|
| Miktar | 1 adet |
| Tip | ESP8266 WiFi Modülü (ESP-01 veya NodeMCU benzeri) |
| Çalışma voltajı | **3.3V** (5V toleranslı değil!) |
| Akım | ~80 mA (WiFi transmit sırasında 170 mA+) |
| İletişim | UART (AT komutları veya Arduino SoftwareSerial) |
| Arduino bağlantı | D0 (RX) ← ESP TX, D1 (TX) → ESP RX (voltaj bölücü!) |
| **KRİTİK** | ESP8266 RX pinine 5V DOĞRUDAN bağlamayın! 1K+2K voltaj bölücü veya level shifter kullanın |
| Kütüphane | SoftwareSerial + AT firmware veya PubSubClient |
| Kullanım (sera) | WiFi → MQTT → Backend veri iletimi — **kritik modül** |
| İlgili sketch | firmware/arduino/learning/21-esp8266-serial/, firmware/arduino/greenhouse/ |
| Öğrenme fazı | Faz 4 |
| Test edildi | ☐ |

**Voltaj bölücü devresi (Arduino D1 → ESP8266 RX):**

```
Arduino D1 (TX, 5V) ── 1KΩ ──┬── ESP8266 RX (3.3V)
                              │
                             2KΩ
                              │
                             GND
```

---

### hc06-bluetooth {#hc06-bluetooth}

| Özellik | Değer |
|---------|-------|
| Miktar | 1 adet |
| Tip | HC-06 Bluetooth Modülü (Slave) |
| Çalışma voltajı | 3.3V – 5V (logic 3.3V) |
| Baud rate | Varsayılan 9600 |
| Kullanım (sera) | WiFi olmadığında telefondan yerel kontrol |
| İlgili sketch | firmware/arduino/learning/23-bluetooth/ |
| Öğrenme fazı | Ek modül |
| Test edildi | ☐ |

---

## Eksik / Önerilen Malzemeler

> **Tam liste:** [alisveris-listesi-esp32.md](alisveris-listesi-esp32.md) — ESP32 geçişi + dalgıç pompa + multimetre

| Malzeme | Neden gerekli | Tahmini fiyat |
|---------|---------------|---------------|
| ESP32 DevKit (WROOM-32) | UNO+ESP8266 yerine tek kart WiFi | 250-400 TL |
| USB Micro veya Type-C kablo | ESP32 programlama (UNO kablosu olmaz) | 30-80 TL |
| Mini dalgiç pompa (5V) | Aktif sulama | 40-80 TL |
| 5V adaptör (pompa hattı) | Pompa gücü (Arduino'dan ayrı) | 50-100 TL |
| Dijital multimetre | Ölçüm, arıza bulma | 150-350 TL |
| Silikon hortum + su deposu | Depo → saksı hattı | 50-110 TL |
| Flyback diyot (1N4007) | DC motor/röle koruma | 1-10 TL |
| Mini sera kutusu / şeffaf kap | Fiziksel prototip | 50-100 TL |
| Bitki saksısı + toprak | Demo için | 40-80 TL |

---

## Malzeme Kutusu ve E-Kitap

### malzeme-kutusu {#malzeme-kutusu}

Set ile birlikte gelen organizasyon kutusu — tüm bileşenleri kategorilere ayırarak saklayın.

### e-kitap {#e-kitap}

Set ile birlikte gelen Uygulama E-Kitabı (PDF) — temel devre örnekleri için referans.

---

## Hızlı Referans: Sera Prototipi Bağlantı Özeti

```
Arduino UNO R3
├── D2  → DHT11 DATA
├── D3  → Servo SG90 signal
├── D4  → Röle IN (pompa)
├── D5  → Buzzer
├── D6  → HC-SR04 TRIG
├── D7  → HC-SR04 ECHO
├── D8-D13 → LCD 4-bit
├── D0/D1 → ESP8266 RX/TX
├── A0  → Toprak nem AO
├── A1  → LDR (voltaj bölücü)
├── A2  → LM35 OUT
└── A3  → Su seviye AO

Güç: 12V adaptör → UNO barrel + Röle VCC (pompa ayrı hat)
```

---

*Bu dosyayı güncellerken ilgili sketch'teki `// Ref:` satırını da kontrol edin.*
