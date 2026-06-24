# Ogrenme Gunlugu

Bu dosya ogrencinin IoT, embedded sistemler ve akilli sulama alanindaki gelisimini takip eder.

## 2026-06-25

### Bugun Ne Ogrendim

- Projenin yalnizca kod degil, ayni zamanda sistemli bir ogrenme ortami olarak tasarlanmasi gerektigini ogrendim.
- Akilli sulama sistemlerinde firmware, backend, dashboard, veritabani, elektronik ve ziraat bilgisinin birlikte dusunulmesi gerektigini gordum.

### Bilmedigim Konular

- ESP32 ile production-grade MQTT firmware mimarisi
- Sensor kalibrasyonunun veritabaninda nasil saklanacagi
- Pompa koruma mantiklari
- AquaCrop entegrasyonu icin gerekli veri modeli

### Arastirilmasi Gereken Konular

- MQTT QoS, retained message ve Last Will and Testament
- Toprak nem sensoru kalibrasyonu
- Role, pompa, flyback diyot ve ayri guc beslemesi
- ET tabanli sulama mantigi

### Bir Sonraki Pratik Calisma

ESP8266/ESP32 saha cihazi icin guvenli pompa kontrolu, sensor okuma ve backend'e veri gonderme akisini kucuk bir MVP olarak tasarlamak.

## 2026-06-25

### Bugun Ne Ogrendim

- Proje hafizasi dosyalarinin GitHub'a commit/push edilerek korunmasi gerektigini ogrendim.
- Commit mesajinin sadece "update docs" gibi genel olmamasi, ogrenilen konuyu acikca anlatmasi gerektigini gordum.

### Bilmedigim Konular

- Git PATH ayari bu bilgisayarda neden komut satirinda gorunmuyor?
- GitHub remote ayari ve kimlik dogrulama durumu nedir?

### Arastirilmasi Gereken Konular

- Windows'ta Git kurulumu ve PATH kontrolu
- `git status`, `git add`, `git commit`, `git push` temel akisi
- GitHub'da dokumantasyon odakli commit mesaj standartlari

### Bir Sonraki Pratik Calisma

Git komutlarini terminalde calisir hale getirip ilk dokumantasyon commitini GitHub'a gondermek.

## 2026-06-25

### Bugun Ne Ogrendim

- `git config --global user.name` ve `git config --global user.email` komutlari commit yazar bilgisini ayarlar.
- Bu ayarlar GitHub hesabina giris yapmak anlamina gelmez.
- GitHub'a baglanmak icin remote repo URL'si ve kimlik dogrulama gerekir.

### Bilmedigim Konular

- `BerkayCLK-Ag` hesabi icin GitHub authentication bu bilgisayarda nasil yapilacak?
- GitHub repository daha once olusturuldu mu, yoksa yeni mi olusturulacak?

### Arastirilmasi Gereken Konular

- Git remote mantigi: `origin`, `main`, upstream
- GitHub HTTPS authentication ve token/credential manager
- GitHub CLI ile `gh auth login` kullanimi

### Bir Sonraki Pratik Calisma

`BerkayCLK-Ag` hesabi altinda repo URL'sini netlestirip remote eklemek ve ilk push'u yapmak.
