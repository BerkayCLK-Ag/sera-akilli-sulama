# Alisveris Listesi — ESP32 Gecisi + Dalgic Pompa

> **Ref:** [malzemeler.md](malzemeler.md) — setinde **olanlar** burada tekrar yazilmiyor.  
> **Hedef:** Arduino UNO + ESP8266 AT yerine **tek kart ESP32**, dalgic pompa, guvenli olcum.

**Son guncelleme:** 2025-06-20

---

## Setinde ZATEN VAR (alma)

| Parca | Not |
|-------|-----|
| Arduino UNO R3 | Ogrenme icin kalsin; sera v2'de ESP32 ana kart |
| ESP8266 modul | ESP32'de WiFi dahil — yeni modul gerekmez |
| 5V tek kanal role | Ayni kart kullanilir (IN → ESP32 GPIO) |
| DHT11, toprak nem, su seviye modulu | Ayni sensorler |
| Breadboard, jumper, direnc seti | Yeterli |
| 12V adaptör | Pompa icin (12V pompa secersen) |
| Buzzer, LCD, LDR, servo... | Ileride ayni |

---

## 1 — ZORUNLU (once bunlari al)

| # | Malzeme | Adet | Neden | Tahmini fiyat |
|---|---------|------|-------|---------------|
| 1 | **ESP32 gelistirme karti** (DevKitC veya NodeMCU-32S) | 1 | WiFi + Bluetooth tek cipte; AT modu yok, kod daha kolay | 250–400 TL |
| 2 | **USB kablo** (ESP32'ye uygun: Micro-USB veya USB-C) | 1 | UNO kablosu (USB-B) **olmaz** — programlama icin | 30–80 TL |
| 3 | **Dalgic mini pompa** (DC, 3–6V veya 5V) | 1 | Depodan saksiya aktif sulama | 40–80 TL |
| 4 | **5V adaptör** (min. 1A, pompa icin ayri hat) | 1 | 5V pompa + role COM/NO; UNO'dan **besleme** | 50–100 TL |
| 5 | **Multimetre** (dijital, DC voltaj + sureklilik) | 1 | Kablo, role, pompa, 3.3V/5V kontrol — **ogrenmenin ana araci** | 150–350 TL |

**ESP32 secim notu:** "ESP32-WROOM-32 DevKit" veya "NodeMCU-32S" yazan kartlardan biri yeterli. **ESP8266 degil**, kutuda **ESP32** yazsin.

**Pompa secim notu:** "USB mini submersible pump" / "dalgic mini pompa 5V" — hortum cikisli olsun. Ilk alimda **5V pompa + 5V adaptör** en basit (12V set adaptörunle karistirma).

---

## 2 — SU HATTI + DEMO (fiziksel sera)

| # | Malzeme | Adet | Neden | Tahmini fiyat |
|---|---------|------|-------|---------------|
| 6 | Plastik su deposu (5–10 L kap, derin) | 1 | Pompa su icinde calisir | 30–60 TL |
| 7 | **Silikon hortum** (ic cap ~4–6 mm, 1–2 m) | 1 | Depo → saksı | 20–50 TL |
| 8 | Bitki saksisi (orta boy) + torf/toprak | 1 set | Gercek nem olcumu | 40–80 TL |
| 9 | Seffaf plastik kutu / mini sera kabı | 1 | Blueprint tarzi gorunum (istege) | 50–120 TL |

---

## 3 — ONERILEN (guvenlik + saglam devre)

| # | Malzeme | Adet | Neden | Tahmini fiyat |
|---|---------|------|-------|---------------|
| 10 | **Flyback diyot** 1N4007 | 2–5 | Pompa/röle kapanirken geri voltaj koruma | 5–10 TL |
| 11 | **5V 2A USB guç modulu** veya breadboard guç hatti | 1 | ESP32 + sensorler stabil besleme (USB PC'den de olur) | 40–80 TL |
| 12 | Su seviye modulunu **kullan** (setinde var) | 0 TL | Depo bos → pompa blok (yazilim + A3) | — |
| 13 | Krokodil kablo (multimetre icin) | 1 set | Olcum kolayligi | 20–40 TL |

---

## 4 — ILERIDE (simdilik sart degil)

| Malzeme | Ne zaman |
|---------|----------|
| LiPo pil + sarj modulu | Blueprint gibi tasınabilir sera |
| 3D baski parca (ESP32 montaj) | Kutu icine gecince |
| Level shifter 3.3↔5V | Role 3.3V ile calismazsa (once dene, cogu kart calisir) |
| Ikinci toprak nem sensoru | 2 saksı icin |
| ESP32-CAM | Uzaktan gorsel (ileri faz) |

---

## Tahmini butce ozeti

| Grup | Aralik |
|------|--------|
| Zorunlu (1–5) | **520 – 1.010 TL** |
| Su hatti + demo (6–9) | **140 – 310 TL** |
| Onerilen (10–13) | **65 – 130 TL** |
| **Toplam (hepsi)** | **~725 – 1.450 TL** |

Minimum baslangic: **#1–5 + #6–7** → yaklasik **600–900 TL**

---

## ESP32 gecisinde ne degisir? (ogrenme notu)

```
ONCE (simdi):
  Arduino UNO ←SoftwareSerial→ ESP8266 (AT) ←WiFi→ PC

SONRA (hedef):
  ESP32 (tek kart) ←WiFi→ PC
  DHT, toprak, role → dogrudan ESP32 pinleri
```

**Avantaj:** AT komutu yok, `200 OK` parse hatasi yok, tek USB ile programlama.  
**Setinden:** role, sensorler, breadboard ayni kalir.

---

## Onerilen ESP32 pin plani (on siparis)

| Bilesen | ESP32 GPIO |
|---------|------------|
| DHT11 DATA | GPIO 4 |
| Toprak nem AO | GPIO 36 (VP) veya 34 |
| Su seviye AO | GPIO 39 (VN) veya 35 |
| Role IN | GPIO 5 |
| LDR (ileride) | GPIO 32 |

> ESP32'de sadece **ADC1** pinleri WiFi acikken guvenilir okur (36, 39, 32–35). Planlamayi buna gore yapariz.

---

## Alisveris kontrol listesi

```
[ ] ESP32 DevKit (ESP32 yaziyor mu?)
[ ] USB kablo (Micro veya Type-C — karta uygun)
[ ] Dalgic pompa 5V + hortum
[ ] 5V adaptör (pompa icin)
[ ] Multimetre
[ ] Su kapagi/kova
[ ] Silikon hortum
[ ] (Opsiyonel) saksı, seffaf kutu, 1N4007
```

---

## Nereden alinir?

- **Robotistan, Direnc.net, Locted, N11, Hepsiburada** — "ESP32 DevKit", "dalgic mini pompa 5V", "dijital multimetre"
- Pompa: hidroponik / akvaryum mini dalgic pompa aramasi da ayni urunu cikarir

---

*Liste alindikca `malzemeler.md` icindeki "Eksik / Onerilen" bolumu guncellenecek.*
