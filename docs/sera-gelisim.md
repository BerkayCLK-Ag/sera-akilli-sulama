# Sera Gelisim Gunlugu

Bu dosya akilli sulama projesinde yapilan gelistirmeleri, nedenlerini, degisen dosyalari ve ogrenilen konulari kaydeder.

## 2026-06-25

### Yapilan Is

Repository icin gelisim ve ogrenme sistemi kurallari tanimlandi. Projenin sadece kod gelistirme degil, ayni zamanda IoT, embedded sistemler, MQTT, FastAPI, veritabani, dashboard ve akilli sulama egitim platformu olarak ilerlemesi kararlastirildi.

### Degisen Dosyalar

- `AGENTS.md`
- `docs/sera-gelisim.md`
- `docs/akilli-sulama-research-notes.md`
- `docs/ogrenme-gunlugu.md`
- `docs/karar-gunlugu.md`
- `docs/proje-yol-haritasi.md`

### Ogrendiklerim

- Bir IoT projesinde kod kadar dokumantasyon, karar kaydi ve ogrenme takibi de onemlidir.
- Akilli sulama sistemi; firmware, backend, veritabani, dashboard, elektronik ve ziraat bilgisinin birlikte calistigi cok katmanli bir sistemdir.

### Sonraki Adim

Ilk teknik milestone olarak ESP8266/ESP32, MQTT, pompa guvenligi ve sensor kalibrasyonunu kapsayan kucuk ama sahaya yakin bir MVP plani hazirlanmali.

## 2026-06-25

### Yapilan Is

GitHub bilgi koruma kurali eklendi. Kritik dokumantasyon dosyalarinin sadece belge degil, proje hafizasi oldugu kabul edildi. Bu dosyalarda degisiklik oldugunda Git commit ve push akisi gorev sonunda denenmelidir.

### Degisen Dosyalar

- `AGENTS.md`
- `docs/sera-gelisim.md`
- `docs/akilli-sulama-research-notes.md`
- `docs/ogrenme-gunlugu.md`
- `docs/karar-gunlugu.md`
- `docs/proje-yol-haritasi.md`

### Ogrendiklerim

- Ogrenme projelerinde bilgi kaybi, kod kaybi kadar kritik olabilir.
- Research notes, karar gunlugu ve ogrenme gunlugu Git ile korunursa proje zamanla kisisel bir muhendislik bilgi bankasina donusur.
- Commit mesajlari ogrenilen konuyu acik soylemelidir; boylece gecmise donup bilgi izlemek kolaylasir.

### Sonraki Adim

Git komutlari ortamda calisacak sekilde PATH veya Git kurulumu kontrol edilmeli. Ardindan kritik dokuman degisiklikleri GitHub'a duzenli push edilmelidir.

## 2026-06-25

### Yapilan Is

Yerel Git kullanici kimligi `BerkayCLK-Ag` ve `brkyclk16@gmail.com` olarak ayarlandi. GitHub Connector tarafinda gorunen hesabinin `BerkayCLK-Ag` degil `berlay19-ops` oldugu tespit edildi. Repository icinde henuz GitHub remote adresi bulunmadigi goruldu.

### Degisen Dosyalar

- `docs/sera-gelisim.md`
- `docs/ogrenme-gunlugu.md`
- `docs/karar-gunlugu.md`

### Ogrendiklerim

- Git `user.name` ve `user.email` ayarlari commit kimligini belirler; GitHub'a push yetkisi vermez.
- GitHub'a push icin repository remote adresi ve dogru hesaba ait kimlik dogrulama gerekir.
- Codex/GitHub uygulamasinda gorunen hesap ile yerel Git kimligi farkli olabilir.

### Sonraki Adim

GitHub'da `BerkayCLK-Ag` hesabi altinda repository olusturulmali veya mevcut repo URL'si eklenmeli. Sonra dogru hesapla authentication yapilip `git remote add origin ...` ve `git push` denenmeli.

## 2026-06-25

### Yapilan Is

GitHub remote adresi `https://github.com/BerkayCLK-Ag/sera-akilli-sulama.git` olarak eklendi. Yerel `main` branch'i `origin/main` ile eslendi ve ilk dokumantasyon commit'i GitHub'a basariyla push edildi.

### Degisen Dosyalar

- `docs/sera-gelisim.md`
- `docs/ogrenme-gunlugu.md`
- `docs/karar-gunlugu.md`

### Ogrendiklerim

- `git remote add origin ...` komutu yerel repository ile GitHub repository arasinda bag kurar.
- `git push -u origin main` ilk push sirasinda yerel branch'i uzak branch ile esler.
- Upstream eslestikten sonra sonraki push islemleri genellikle sadece `git push` ile yapilabilir.

### Sonraki Adim

Kod, firmware, backend ve frontend dosyalari hassas bilgi kontrolu yapildiktan sonra ayri bir commit ile GitHub'a eklenmeli.

## 2026-06-25

### Yapilan Is

ESP32 D1 R32 (LOLIN, ESP32-WROOM-32, UNO form factor) ulaşti. Arduino UNO + ESP8266 AT modu mimarisinden ESP32 tek kart mimarisine gecis plani olusturuldu. 5 faz boyunca ilerlenmesi planlandi: blink test, sensörler, röle/sulama, WiFi+backend, siparis gelince (2. role + pompa + OLED). Renk kodlu kablo standardi belirlendi ve tum breadboard baglantilari dokumine edildi. Faz 1-4 sketch'leri (23-29 numarali) yazildi.

### Degisen/Olusturulan Dosyalar

- `docs/esp32-d1r32-calisma-plani.md` (yeni)
- `docs/devre-semalari/29-esp32-d1r32-renk-kodlu-baglanti.md` (yeni)
- `firmware/arduino/learning/23-esp32-blink/` (yeni)
- `firmware/arduino/learning/24-esp32-dht/` (yeni)
- `firmware/arduino/learning/25-esp32-toprak/` (yeni)
- `firmware/arduino/learning/26-esp32-su-seviye/` (yeni)
- `firmware/arduino/learning/27-esp32-sensor-hub/` (yeni)
- `firmware/arduino/learning/28-esp32-role-sulama/` (yeni)
- `firmware/arduino/learning/29-esp32-wifi-backend/` (yeni)

### Ogrendiklerim

- ESP32'de GPIO logic seviyesi 3.3V; Arduino UNO'da 5V. Sensör VCC'si 3.3V olmalidur, aksi halde ADC girisine 5V gidip hasar verebilir.
- ADC1 pinleri (GPIO32-39) WiFi acikken güvenilir. ADC2 pinleri WiFi ile cakisir — sensor analog cikislari ADC1'e baglanmalidir.
- GPIO36 (VP) ve GPIO39 (VN) input-only pinlerdir; sensör okuma icin yeterli, diger islemler icin kullanilamaz.
- Röle modülü VCC=5V ister (bobin gerilimi), ancak IN pini transistorlü modüllerde 3.3V ile tetiklenebilir.
- ESP32'de Arduino IDE board ayari "WEMOS D1 MINI ESP32" veya "ESP32 Dev Module"; baud rate 115200.

### Sonraki Adim

1. Arduino IDE'de ESP32 board package kur (Espressif)
2. 23-esp32-blink.ino yukle — kart tanindigini dogrula
3. Renk kodlu kablo standardi ile breadboard bagla
4. 27-esp32-sensor-hub.ino ile tum sensorleri test et
5. 28-esp32-role-sulama.ino ile WiFi'siz otomatik sulamayı test et

## 2026-06-25

### Yapilan Is

Tum malzemeler (OLED, dalgic pompa, saksı, ESP32) tamamlandi. Sera v3 tam prototip planlandi: fiziksel depo-saksı-hortum hatti, OLED lokal ekran, pompa guvenligi (depo + flyback diyot), entegre firmware (`sera-v3-prototip.ino`), OLED test sketch (30), baglanti semasi ve kurulum rehberi. Backend ingest_simple water_level destegi ve pompa icin su seviye kilidi eklendi.

### Degisen/Olusturulan Dosyalar

- `docs/sera-v3-prototip-kurulum.md`
- `docs/devre-semalari/30-sera-v3-pompa-oled-baglanti.md`
- `firmware/arduino/greenhouse/sera-v3-prototip.ino`
- `firmware/arduino/learning/30-esp32-oled-test/`
- `backend/main.py`, `backend/mqtt_client.py`

### Sonraki Adim

1. OLED test (30-esp32-oled-test)
2. Pompa fiziksel montaj (depo + hortum + saksı)
3. sera-v3-prototip.ino WiFi ayari + dashboard

## 2026-06-26

### Yapilan Is

Temel akilli sulama prototipi tamamlandi sayildi. GitHub portfolyo sistemi kuruldu: portfolio-yetkinlikler.md, profil README sablonu, otomatik guncelleme kurali, push-github.bat. README profesyonel portfolyo formatina cevrildi.

### Degisen/Olusturulan Dosyalar

- `docs/portfolio-yetkinlikler.md`
- `docs/github-profil-README.md`
- `.cursor/rules/github-portfolio.mdc`
- `push-github.bat`
- `README.md`, `AGENTS.md`

### Sonraki Adim

GitHub profil reposu (BerkayCLK-Ag/BerkayCLK-Ag) olusturulup README yuklenmeli. Sera v4 plani baslatilabilir.
