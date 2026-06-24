# Akıllı Sera Prototipi — Demo Senaryosu

> Ref: [malzemeler.md](malzemeler.md) · [mimari.md](mimari.md)

## Fiziksel Kurulum

```
┌─────────────────────────────────────┐
│         Mini Sera Kutusu            │
│  ┌─────────┐  ┌──────────────────┐  │
│  │ Bitki   │  │ Sensör istasyonu │  │
│  │ saksısı │  │ DHT11 + LDR      │  │
│  │ + toprak│  │ Toprak nem probu │  │
│  │ nem prob│  │ Su seviye        │  │
│  └────┬────┘  └────────┬─────────┘  │
│       │               │             │
│  ┌────▼───────────────▼──────────┐  │
│  │ Sulama hattı (mini pompa)     │  │
│  └───────────────────────────────┘  │
└─────────────────────────────────────┘
         │
    Arduino UNO + ESP8266
         │
    WiFi → MQTT → FastAPI → Dashboard
```

## Donanım Bağlantı Özeti

Tam pin listesi: [malzemeler.md — Hızlı Referans](malzemeler.md#hizli-referans-sera-prototipi-baglanti-ozeti)

## Demo Adımları

### 1. Ortam Hazırlığı

```bash
# Proje klasöründe
docker-compose up -d

cd backend
python -m venv venv
venv\Scripts\activate
pip install -r requirements.txt
copy .env.example .env
uvicorn main:app --reload
```

Tarayıcı: `http://localhost:8000`

### 2. Arduino Firmware Yükleme

1. `firmware/arduino/greenhouse/greenhouse-controller.ino` açın
2. `WIFI_SSID`, `WIFI_PASS`, `MQTT_BROKER` (PC IP) değerlerini düzenleyin
3. Gerekli kütüphaneler: DHT sensor library, LiquidCrystal, Servo
4. Arduino'ya yükleyin

### 3. Canlı Veri Demo

- Dashboard'da sıcaklık, nem, toprak nemi kartlarının güncellenmesini gösterin
- 24 saatlik grafik sekmelerini değiştirin
- Cihaz durumu yeşil nokta = online

### 4. Otomatik Sulama Demo

1. Toprak nem sensörünü kuru toprağa yaklaştırın veya hava ile kurutun
2. Nem %30 altına düşünce pompa (röle tıklaması) tetiklenir
3. LCD'de "SULAMA AKTIF" görünür
4. Dashboard alarm geçmişine kayıt düşer

### 5. Manuel Kontrol Demo

- Dashboard → Pompa Aç (5 sn)
- Servo slider ile vana açısı ayarlama
- Mobil tarayıcıdan aynı dashboard'a erişim (aynı WiFi)

### 6. Kural Motoru Demo

1. Kurallar bölümünde toprak nem eşiğini %50 yapın
2. "Kuralları Değerlendir" butonuna basın
3. Eşik altındaysa pompa tetiklenir

## MQTT Test (Broker olmadan)

```bash
# Terminal 1: test mesajı gönder
mosquitto_pub -h localhost -t sera/greenhouse-01/sensor/temperature -m '{"value":24.5,"unit":"C"}'

# Terminal 2: dinle
mosquitto_sub -h localhost -t "sera/#" -v
```

## Sektöre Sunum Checklist

- [ ] GitHub repo public ve README dolu
- [ ] 2-3 dakikalık demo videosu
- [ ] Dashboard ekran görüntüleri
- [ ] malzemeler.md güncel ve test edildi işaretli
- [ ] Otomatik + manuel sulama çalışıyor
- [ ] Veri grafiği en az 1 saatlik kayıt gösteriyor

## Sorun Giderme

| Sorun | Çözüm |
|-------|-------|
| ESP8266 bağlanmıyor | Voltaj bölücü kontrol, 3.3V besleme |
| MQTT veri gelmiyor | Broker IP, firewall 1883 portu |
| InfluxDB hata | docker-compose logs influxdb |
| Dashboard boş | Backend çalışıyor mu, /api/sensors/latest test |
| DHT11 NaN | Kablo, 10K pull-up, 2 sn bekleme |

## Sonraki Adımlar (Sektör)

1. Gerçek mini pompa ekleme
2. TLS ile MQTT güvenliği
3. Bulut deploy (Railway, Render, AWS)
4. Çoklu sera / çoklu cihaz desteği
5. Bitki türüne özel nem profilleri (biyosistem uzmanlığı)
