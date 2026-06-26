// DERS 04 — Gerçek API'den veri çekme
// Ders 03: rastgele sayı ürettik (JavaScript içinde)
// Ders 04: sayılar PC'deki backend'den gelir (Arduino/DHT → backend → burası)

const API = "http://localhost:8000";

const sicaklikEl = document.getElementById("sicaklik");
const nemEl = document.getElementById("nem");
const toprakEl = document.getElementById("toprak");
const suEl = document.getElementById("su");
const saatEl = document.getElementById("son-guncelleme");
const apiDurumEl = document.getElementById("api-durum");

// --- Backend'den JSON al ---
async function verileriCek() {
  try {
    // fetch = internet/PC'den veri iste
    const cevap = await fetch(`${API}/api/sensors/latest`);

    if (!cevap.ok) {
      throw new Error("API hata: " + cevap.status);
    }

    const data = await cevap.json();
    console.log("API cevabı:", data);

    apiDurumEl.textContent = "API bağlı ✓";
    apiDurumEl.className = "api-durum bagli";

    ekraniGuncelle(data.sensors || {});
  } catch (hata) {
    console.error("Bağlantı hatası:", hata);
    apiDurumEl.textContent = "API kapalı — start-dashboard.bat çalıştır";
    apiDurumEl.className = "api-durum kapali";
  }
}

// --- Gelen JSON'u ekrana yaz ---
function ekraniGuncelle(sensors) {
  yaz(sicaklikEl, sensors.temperature, 1);
  yaz(nemEl, sensors.humidity, 0);
  yaz(toprakEl, sensors.soil_moisture, 0);
  yaz(suEl, sensors.water_level, 0);

  saatEl.textContent = new Date().toLocaleTimeString("tr-TR");
}

function yaz(element, sensorData, ondalik) {
  if (!element) return;

  if (!sensorData || sensorData.value === undefined) {
    element.textContent = "--";
    return;
  }

  const val = Number(sensorData.value);
  element.textContent = ondalik > 0 ? val.toFixed(ondalik) : Math.round(val);
}

// --- İlk yükleme + 5 sn'de bir tekrar ---
verileriCek();
setInterval(verileriCek, 5000);
