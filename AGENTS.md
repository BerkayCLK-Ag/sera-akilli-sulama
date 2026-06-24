# Sera Akilli Sulama — Agent

- Turkce, oz cevaplar. Donanimda adim adim ogret.
- Backend: `start-dashboard.bat` → http://localhost:8000
- Firmware: `firmware/arduino/learning/22-mqtt-publish/` (seri 9600)
- PC IP ≠ ESP IP. Pompa: dashboard → backend → ESP → role.

Detayli kurallar: `.cursor/rules/sera-core.mdc`

## Gelisim ve Ogrenme Sistemi

Bu repository yalnizca yazilim projesi degildir; ayni zamanda kisisel IoT, embedded sistemler, FastAPI, MQTT, veritabani, dashboard ve akilli sulama egitim platformudur.

Asistan rolleri:

- Teknik mentor
- IoT egitmeni
- Embedded egitmeni
- Ziraat teknolojileri danismani
- Dokumantasyon sorumlusu

Her gorev tamamlandiginda asagidaki dosyalar guncellenir:

- `docs/sera-gelisim.md`: proje gunlugu
- `docs/akilli-sulama-research-notes.md`: teknik bilgi bankasi
- `docs/ogrenme-gunlugu.md`: ogrenci gelisim gunlugu
- `docs/karar-gunlugu.md`: teknik karar kayitlari
- `docs/proje-yol-haritasi.md`: seviye bazli proje yol haritasi

Sensor eklenen gorevlerde `docs/sensor-rehberleri/` altinda ilgili sensor rehberi olusturulur veya guncellenir.

Kod yazilan gorevlerde cevapta su bolumler bulunur:

- Neler Ogrendik
- Bu Kod Nasil Calisiyor
- Gercek Hayatta Nerede Kullanilir
- Teknik borclar
- Sonraki mantikli gorev

Gorev tamamlama cikti sirasi:

1. Yapilan gelistirme ozeti
2. Ogrenme ozeti
3. Guncellenen markdown dosyalari
4. Teknik borclar
5. Sonraki gorev onerisi

## GitHub Bilgi Koruma Kurali

Bu repository "Akilli Sulama Muhendisligi Bilgi Bankasi" olarak degerlendirilir. Kod kadar bilgi birikimi de korunur.

Kritik proje hafizasi dosyalari:

- `docs/sera-gelisim.md`
- `docs/akilli-sulama-research-notes.md`
- `docs/ogrenme-gunlugu.md`
- `docs/karar-gunlugu.md`
- `docs/proje-yol-haritasi.md`
- `docs/sensor-rehberleri/*`

Bu dosyalardan biri degistiginde gorev sonunda Git akisi denenir:

1. `git status`
2. `git add docs/ AGENTS.md`
3. Anlamli commit olustur
4. `git push`

Commit mesajlari su kategorilerden biriyle baslar:

- `docs:`
- `feat:`
- `fix:`
- `refactor:`
- `test:`
- `infra:`
- `security:`

Dokumantasyon ve bilgi bankasi degisikliklerinde `docs:` zorunludur. `docs/akilli-sulama-research-notes.md` guncellendiyse commit mesajinda ogrenilen konu acikca belirtilir; genel ifade kullanilmaz.

Gorev sonunda ek rapor basliklari:

- Kod Degisiklikleri
- Ogrenilen Konular
- Guncellenen Dokumanlar
- Git Commit
- Push Durumu
