# RGB LED — En Basit Bağlantı (Sadece Sol Taraf)

> Sinirlenme normal. Bu rehberi **satır satır** uygula. Sağ tarafa hiçbir şey takma.

## Gerekli malzeme

- 1 RGB LED (4 bacak)
- 3 adet 330Ω direnç
- 4 adet erkek-dişi jumper (3 Arduino + 1 GND)
- Breadboard + Arduino GND kablosu (siyah)

---

## Adım 0 — Her şeyi sök

RGB, dirençler, mor/yeşil/beyaz kablolar — **hepsini çıkar**.  
Sadece **Arduino GND (siyah) → sol mavi (-) hat** kalsın.

---

## Adım 1 — RGB'yi tak

4 bacak **e sütununa**, **alt alta**:

| Satır | Bacak |
|-------|--------|
| **e10** | Bacak 1 (kısa) |
| **e11** | **EN UZUN bacak** |
| **e12** | Bacak 2 (kısa) |
| **e13** | Bacak 3 (kısa) |

En uzun hangisiyse **mutlaka e11**'e gelsin.

---

## Adım 2 — GND (tek kablo)

```
d11 ── jumper ──► SOL mavi (-) hat
```

- **d11** = e11 ile **aynı satır** (uzun bacak)
- Diğer uç = Arduino siyah GND'nin olduğu **mavi hat**
- **Başka yere GND takma**

---

## Adım 3 — 3 direnç + 3 Arduino kablosu

Her renk kanalı **aynı kalıp**:

### Kanal 1 → D8
- Direnç: **e10** ↔ **e15** (10 ve 15. satır)
- Jumper: **d15** → Arduino **D8** (erkek breadboard, dişi Arduino)

### Kanal 2 → D9
- Direnç: **e12** ↔ **e16**
- Jumper: **d16** → Arduino **D9**

### Kanal 3 → D10
- Direnç: **e13** ↔ **e17**
- Jumper: **d17** → Arduino **D10**

**e11'e direnç veya D8/D9/D10 takma** — sadece GND.

---

## Şema (hepsi sol taraf)

```
SOL mavi (-) ◄── Arduino GND (siyah)
     ▲
d11──┘

D8  ── d15 ──[330Ω]── e10  (LED bacak)
D9  ── d16 ──[330Ω]── e12  (LED bacak)
D10 ── d17 ──[330Ω]── e13  (LED bacak)

e11 = EN UZUN bacak (sadece d11 → GND)
```

---

## Adım 4 — Kod

1. Arduino IDE → `04-rgb-minimal/04-rgb-minimal.ino`
2. Kart: **Uno**, Port: **COM5**
3. **Yükle**
4. Seri monitör **9600**

Sırayla: D8 (4 sn) → D9 (4 sn) → D10 (4 sn) — **bir renk yanmalı**.

---

## Yanmazsa — Ortak anot (son çare)

Bazı RGB'lerde uzun bacak **+5V** ister:

1. **d11** kablosunu mavi GND'den **sök**
2. **d11** → **kırmızı (+) 5V hat** (Arduino kırmızı kablo)
3. `04-rgb-anot-test/04-rgb-anot-test.ino` yükle

---

## Kontrol listesi

- [ ] RGB **e10–e13** (en uzun **e11**)
- [ ] **d11 → sol mavi GND**
- [ ] 3 direnç var, **e11'de direnç yok**
- [ ] D8→d15, D9→d16, D10→d17
- [ ] **Sağ tarafa (f-j) hiçbir şey yok**
- [ ] `04-rgb-minimal.ino` yüklendi

---

## Sorun değilsin

Breadboard ortadaki çizgi + sol/sağ GND herkesi çıldırtır. Bu kurulum **tek taraflı** — karışmaz.
