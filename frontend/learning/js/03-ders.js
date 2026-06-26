// DERS 03 — JavaScript: Canlı sensör verisi
// HTML = iskelet | CSS = görünüm | JS = hareket + veri

// --- 1) HTML'deki elementleri bul (id ile) ---
const sicaklikEl = document.getElementById("sicaklik");
const nemEl = document.getElementById("nem");
const toprakEl = document.getElementById("toprak");
const suEl = document.getElementById("su");
const saatEl = document.getElementById("son-guncelleme");

// --- 2) Rastgele değer üret (DHT11 simülasyonu) ---
function rastgele(min, max) {
  return min + Math.random() * (max - min);
}

// --- 3) Ekranı güncelle ---
function guncelle() {
  const sicaklik = rastgele(20, 25);
  const nem = rastgele(40, 70);
  const toprak = rastgele(30, 55);
  const su = rastgele(60, 90);

  // textContent = elementin içindeki yazıyı değiştir
  sicaklikEl.textContent = sicaklik.toFixed(1);
  nemEl.textContent = Math.round(nem);
  toprakEl.textContent = Math.round(toprak);
  suEl.textContent = Math.round(su);

  // Saati göster
  const simdi = new Date();
  saatEl.textContent = simdi.toLocaleTimeString("tr-TR");

  // Konsol (F12 → Console sekmesinde görürsün)
  console.log("Veri güncellendi:", sicaklik.toFixed(1), "°C");
}

// --- 4) Sayfa açılınca 1 kez çalıştır ---
guncelle();

// --- 5) Her 3 saniyede bir tekrarla (3000 ms) ---
setInterval(guncelle,5000);
