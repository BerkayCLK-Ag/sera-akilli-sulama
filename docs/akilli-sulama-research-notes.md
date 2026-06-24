# Akilli Sulama Research Notes

Bu dosya zamanla kisisel IoT ve akilli sulama bilgi bankasi olarak buyuyecek.

# Akilli Sulama Platformu

## Ne Ise Yarar

Toprak nemi, sicaklik, hava nemi, su deposu seviyesi ve pompa durumunu izleyerek sulama kararlarini daha kontrollu hale getirir.

## Nasil Calisir

Saha cihazlari sensorlerden veri okur. Bu veriler WiFi, MQTT veya HTTP ile backend'e aktarilir. Backend verileri kaydeder, kurallari uygular ve dashboard'a gosterir. Gerekirse pompa veya vana komutu cihaza geri gonderilir.

## Avantajlar

- Gereksiz sulamayi azaltir.
- Bitki stresini erken fark etmeye yardimci olur.
- Sera ve tarla verilerini kayit altina alir.
- Manuel kontrol ve otomatik kontrol birlikte kullanilabilir.

## Dezavantajlar

- Sensor kalibrasyonu yapilmazsa yanlis sulama karari verilebilir.
- Elektrik, su ve elektronik ayni sistemde oldugu icin guvenlik onemlidir.
- WiFi veya enerji kesintileri sistem davranisini etkileyebilir.

## Gercek Tarla Kullanimi

Gercek uygulamada sistem; bolge bazli sulama, depo korumasi, pompa korumasi, su akisi dogrulama, hava durumu verisi ve bitki turune gore esiklerle birlikte tasarlanmalidir.

## Dikkat Edilecek Noktalar

- Pompa Arduino pininden beslenmemelidir.
- Role sadece anahtarlama yapar; pompa ayri guc kaynagindan beslenir.
- Toprak nem sensoru mutlaka kalibre edilmelidir.
- Kritik komutlar icin onay ve olay kaydi tutulmalidir.

# Proje Dokumantasyon Sistemi

## Ne Ise Yarar

Yapilan her teknik adimi, alinan kararlari ve ogrenilen konulari kaydederek projeyi egitim platformuna donusturur.

## Gercek Proje Faydasi

Sahada calisan sistemlerde dokumantasyon; ariza bulma, bakim, yeni cihaz ekleme ve kullanici egitimi icin kritik hale gelir.

# GitHub Bilgi Koruma Is Akisi

## Ne Ise Yarar

Proje icinde uretilen teknik bilginin kaybolmamasini saglar. Akilli sulama gibi cok disiplinli bir alanda sensor notlari, kalibrasyon bilgileri, kararlar ve saha tecrubeleri zamanla en degerli varlik haline gelir.

## Nasil Calisir

Kritik dokumantasyon dosyalari degistiginde once Git durumu kontrol edilir. Sonra ilgili dosyalar stage edilir, anlamli bir commit mesaji ile commit olusturulur ve GitHub'a push edilir.

## Avantajlar

- Ogrenilen bilgiler kalici hale gelir.
- Gecmiste hangi karar neden alindi gorulebilir.
- Sensor, pompa, MQTT, FastAPI ve saha uygulama notlari zamanla aranabilir bilgi bankasina donusur.
- Farkli bilgisayarda calisirken bilgi kaybi azalir.

## Dezavantajlar

- Her dokumantasyon degisikliginden sonra commit disiplini gerekir.
- Commit mesajlari ozensiz yazilirsa bilgi izleme degeri azalir.
- Git kurulumu veya GitHub erisimi yoksa push adimi basarisiz olabilir.

## Gercek Tarla Kullanimi

Saha projelerinde her ariza, kalibrasyon, sensor degisimi, pompa problemi ve sulama karari kaydedilmelidir. Bu kayitlar sonraki sera veya tarla kurulumlarinda hata tekrarini azaltir.

## Dikkat Edilecek Noktalar

- Research notes guncellendiyse commit mesaji genel olmamali; ogrenilen konu yazilmalidir.
- Ornek: `docs: document MQTT QoS and retained messages`
- Gizli bilgiler, WiFi sifreleri, tokenlar ve musteri/saha ozel bilgileri commit edilmemelidir.
