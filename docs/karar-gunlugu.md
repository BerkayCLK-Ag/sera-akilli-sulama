# Karar Gunlugu

Bu dosya projede alinan teknik kararlarin nedenlerini, alternatiflerini, avantajlarini ve dezavantajlarini kaydeder.

## Dokumantasyon Odakli Ogrenme Sistemi Secildi

### Sebep

Projenin hedefi yalnizca calisan kod yazmak degil; IoT, embedded sistemler, MQTT, FastAPI, veritabani, web dashboard ve akilli sulama alaninda kalici ogrenme saglamaktir.

### Alternatifler

- Sadece kod odakli ilerlemek
- Dokumantasyonu sadece README icinde tutmak
- Ayri ayri gunluk dosyalari yerine tek buyuk not dosyasi kullanmak

### Avantajlar

- Her gelistirme sonrasi ne ogrenildigi gorunur hale gelir.
- Teknik kararlar unutulmaz.
- Sensor, elektronik ve ziraat bilgisi zamanla kisisel bilgi bankasina donusur.
- Gercek saha projeleri icin daha profesyonel calisma aliskanligi kazandirir.

### Dezavantajlar

- Her gorev sonunda ek dokumantasyon zamani gerekir.
- Dosyalar duzenli tutulmazsa tekrar veya daginiklik olusabilir.

### Sonuc

Bes ana dokumantasyon dosyasi ve sensor rehberleri yaklasimi kabul edildi.

## GitHub Bilgi Koruma Akisi Secildi

### Sebep

Bu repository akilli sulama muhendisligi bilgi bankasi olarak kullanilacak. Bu nedenle research notes, ogrenme gunlugu, karar gunlugu, proje yol haritasi ve sensor rehberleri kaybolmamali.

### Alternatifler

- Dokumanlari sadece lokal bilgisayarda tutmak
- Sadece kod degisikliklerini commit etmek
- Dokumantasyon commitlerini ara sira manuel yapmak

### Avantajlar

- Yeni ogrenilen bilgiler GitHub'da kalici hale gelir.
- Proje gecmisi, kararlar ve teknik notlar izlenebilir olur.
- Farkli cihazlarda calisirken bilgi kaybi azalir.
- Gercek saha uygulamalarinda ariza ve iyilestirme gecmisi korunur.

### Dezavantajlar

- Her gorev sonunda Git kontrolu ek zaman alir.
- Git kurulumu veya remote problemi varsa push basarisiz olabilir.
- Gizli bilgilerin yanlislikla commit edilmemesi icin dikkat gerekir.

### Sonuc

Kritik proje hafizasi dosyalari degistiginde `docs:` ile baslayan anlamli commit mesaji kullanilacak ve push denenecek.

## Git Kimligi ve GitHub Authentication Ayrimi Kaydedildi

### Sebep

Yerel Git kullanici adi/e-posta ayarlari ile GitHub hesabina push yetkisi ayni sey degildir. Projede bilgi kaybi olmamasi icin bu ayrimin net bilinmesi gerekir.

### Alternatifler

- Sadece Git user.name ve user.email ayarlayip push'un calismasini beklemek
- GitHub CLI ile hesap girisi yapmak
- HTTPS remote ve Git Credential Manager ile giris yapmak

### Avantajlar

- Commitlerde dogru isim ve e-posta gorunur.
- Push yetkisi icin ayrica remote ve authentication gerektigi netlesir.
- Yanlis GitHub hesabina push yapma riski azalir.

### Dezavantajlar

- Ilk kurulumda birden fazla adim gerekir.
- Codex/GitHub Connector hesabi ile kullanicinin hedef GitHub hesabi farkliysa dikkat gerekir.

### Sonuc

Yerel Git kimligi `BerkayCLK-Ag` olarak ayarlandi. GitHub push icin `BerkayCLK-Ag` hesabi altinda repo URL'si ve authentication ayrica tamamlanacak.

## GitHub Remote Origin Secildi

### Sebep

Proje hafizasinin GitHub'da korunmasi icin yerel repository'nin uzak repository ile eslenmesi gerekiyordu.

### Alternatifler

- Remote eklemeden sadece lokal commit tutmak
- Farkli bir GitHub hesabi altinda repo kullanmak
- ZIP yedekleriyle ilerlemek

### Avantajlar

- Bilgi bankasi GitHub'da korunur.
- Yerel bilgisayar arizalansa bile dokumantasyon kaybolmaz.
- Sonraki commit ve push islemleri daha kolay hale gelir.

### Dezavantajlar

- Yanlis dosyalar commit edilirse gizli bilgi GitHub'a gidebilir.
- Kod dosyalarini push etmeden once `.env`, veritabani ve cache dosyalari kontrol edilmelidir.

### Sonuc

Remote adresi `https://github.com/BerkayCLK-Ag/sera-akilli-sulama.git` olarak secildi ve ilk dokumantasyon push'u basariyla yapildi.
