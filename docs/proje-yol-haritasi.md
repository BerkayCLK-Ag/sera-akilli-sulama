# Proje Yol Haritasi

Bu yol haritasi akilli sulama sistemini ogrenme seviyelerine ayirir. Tamamlanan maddeler gorevler ilerledikce isaretlenir.

## Seviye 1 - Temel Arduino

- [x] Arduino proje yapisi olusturuldu
- [x] Blink, LED, buton, LCD gibi temel ders klasorleri hazirlandi
- [ ] Temel devrelerin tamamini fiziksel olarak test et
- [ ] Multimetre ile 5V, GND ve direncleri olcmeyi pratik et

## Seviye 2 - Temel Sensorler

- [x] DHT11, LM35, LDR, toprak nem, su seviye ve HC-SR04 icin ders dosyalari hazirlandi
- [ ] Toprak nem sensoru kuru/islak kalibrasyonu yap
- [ ] Su seviye sensorunu gercek depo senaryosunda test et
- [ ] Sensor rehberlerini `docs/sensor-rehberleri/` altinda olustur

## Seviye 3 - ESP8266

- [x] ESP8266 seri haberlesme ve HTTP veri gonderme denemeleri baslatildi
- [x] UDP ile PC IP bulma yaklasimi eklendi
- [ ] WiFi bilgilerini koddan ayir
- [ ] ESP8266 AT komutlarindaki hata senaryolarini dokumante et

## Seviye 4 - MQTT

- [x] MQTT mimarisi dokumante edildi
- [x] Mosquitto Docker servisi eklendi
- [ ] Gercek MQTT publish/subscribe firmware yaz
- [ ] MQTT QoS, retained ve LWT pratikleri yap

## Seviye 5 - Dashboard

- [x] FastAPI ile dashboard servis edilmeye baslandi
- [x] Sensor kartlari, grafik, kural formu ve pompa kontrolu eklendi
- [x] Dokumantasyon ve bilgi bankasi dosyalari icin GitHub koruma kurali eklendi
- [ ] Mobil kullanim icin pompa onay modali ekle
- [ ] Sulama gecmisi grafikte goster

## Seviye 6 - Otomatik Sulama

- [x] Basit esik tabanli otomatik sulama mantigi kuruldu
- [ ] Histerezis ekle
- [ ] Tank seviyesi dusukse pompayi kilitle
- [ ] Maksimum pompa suresi ve cooldown ekle
- [ ] Sulama olaylarini veritabanina kaydet

## Seviye 7 - Coklu Sulama Bolgesi

- [ ] Device registry modeli olustur
- [ ] Farm, greenhouse, zone kavramlarini ekle
- [ ] Coklu sensor ve coklu pompa mimarisi tasarla

## Seviye 8 - Hava Durumu Entegrasyonu

- [ ] Hava durumu API sec
- [ ] Sicaklik, nem, yagis ve ET verilerini kaydet
- [ ] Hava durumuna gore sulama oneri sistemi tasarla

## Seviye 9 - AquaCrop Entegrasyonu

- [ ] AquaCrop icin gerekli girdileri arastir
- [ ] Bitki, toprak, iklim ve sulama veri modelini hazirla
- [ ] Gunluk veri export formatini tasarla

## Seviye 10 - Gercek Tarla Uygulamasi

- [ ] IP65 kutu, kablo, konnektor ve guc koruma planla
- [ ] Solar/batarya secenegini arastir
- [ ] Pilot sera veya tarla kurulum kontrol listesi hazirla
- [ ] En az 7 gun kesintisiz saha testi yap
