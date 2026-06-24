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
