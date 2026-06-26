// Demo dashboard — Python/backend gerekmez, dosyayi tarayicida acin
const SENSORS = ["temperature", "humidity", "soil_moisture", "light", "water_level"];
const SENSOR_LABELS = {
  temperature: "Sıcaklık",
  humidity: "Nem",
  soil_moisture: "Toprak",
  light: "Işık",
  water_level: "Su",
};

const BASE = {
  temperature: 24.0,
  humidity: 65.0,
  soil_moisture: 42.0,
  light: 620.0,
  water_level: 78.0,
};

const THRESHOLDS = {
  temperature: { warn: 30, danger: 35, max: 50, higherIsBad: true },
  humidity: { warn: 40, danger: 30, max: 100, higherIsBad: false },
  soil_moisture: { warn: 40, danger: 30, max: 100, higherIsBad: false },
  light: { warn: 200, danger: 100, max: 1024, higherIsBad: false },
  water_level: { warn: 30, danger: 10, max: 100, higherIsBad: false },
};

let historyChart = null;
let activeChartSensor = "temperature";
const demoAlarms = [
  { created_at: new Date().toISOString(), message: "Demo modu başlatıldı — ESP IP 192.168.1.195" },
  { created_at: new Date(Date.now() - 120000).toISOString(), message: "WiFi bağlantısı başarılı (2.4 GHz)" },
];

function rand(min, max) {
  return min + Math.random() * (max - min);
}

function generateSensors() {
  return {
    temperature: { value: +(BASE.temperature + rand(-1.5, 1.5)).toFixed(1) },
    humidity: { value: Math.round(BASE.humidity + rand(-4, 4)) },
    soil_moisture: { value: Math.round(BASE.soil_moisture + rand(-3, 3)) },
    light: { value: Math.round(BASE.light + rand(-80, 80)) },
    water_level: { value: Math.round(BASE.water_level + rand(-2, 2)) },
    status: {
      online: true,
      ip: "192.168.1.195",
      device_id: "greenhouse-01",
      wifi: "WIFI_ADINIZ",
    },
  };
}

function generateHistory(sensor, hours = 24) {
  const base = BASE[sensor] || 50;
  const now = Date.now();
  return Array.from({ length: hours }, (_, i) => ({
    timestamp: new Date(now - (hours - i) * 3600000).toISOString(),
    value: +(base + rand(-5, 5)).toFixed(1),
  }));
}

function showToast(msg, type = "success") {
  const c = document.getElementById("toastContainer");
  const el = document.createElement("div");
  el.className = `toast ${type}`;
  el.textContent = msg;
  c.appendChild(el);
  setTimeout(() => el.remove(), 3000);
}

function getStatusLevel(name, val) {
  const t = THRESHOLDS[name];
  if (!t) return "ok";
  if (t.higherIsBad) {
    if (val >= t.danger) return "danger";
    if (val >= t.warn) return "warn";
    return "ok";
  }
  if (val <= t.danger) return "danger";
  if (val <= t.warn) return "warn";
  return "ok";
}

function statusLabel(level) {
  return { ok: "Normal", warn: "Uyarı", danger: "Kritik" }[level];
}

function updateCards(sensors) {
  SENSORS.forEach((name) => {
    const el = document.getElementById(`val-${name}`);
    const card = document.querySelector(`.kpi-card[data-sensor="${name}"]`);
    const badge = document.getElementById(`badge-${name}`);
    const bar = document.getElementById(`bar-${name}`);
    const data = sensors[name];
    if (!el || !data) return;

    const val = Number(data.value);
    el.textContent = Number.isInteger(val) ? val : val.toFixed(1);
    const level = getStatusLevel(name, val);
    card.className = `kpi-card status-${level}`;
    if (badge) {
      badge.textContent = statusLabel(level);
      badge.className = `kpi-badge ${level}`;
    }
    if (bar) {
      const max = THRESHOLDS[name].max;
      bar.style.width = `${Math.min(100, (val / max) * 100)}%`;
    }
  });

  document.getElementById("lastUpdate").textContent = new Date().toLocaleTimeString("tr-TR");
}

function renderChart(sensor, points) {
  const ctx = document.getElementById("historyChart");
  if (!ctx) return;
  if (historyChart) historyChart.destroy();

  historyChart = new Chart(ctx, {
    type: "line",
    data: {
      labels: points.map((p) => {
        const d = new Date(p.timestamp);
        return `${d.getHours()}:00`;
      }),
      datasets: [{
        label: SENSOR_LABELS[sensor],
        data: points.map((p) => p.value),
        borderColor: "#059669",
        backgroundColor: "rgba(5, 150, 105, 0.08)",
        fill: true,
        tension: 0.35,
        borderWidth: 2,
        pointRadius: 2,
      }],
    },
    options: {
      responsive: true,
      maintainAspectRatio: false,
      plugins: { legend: { display: false } },
      scales: {
        x: { ticks: { color: "#64748b", maxTicksLimit: 8 }, grid: { color: "#e2e8f0" } },
        y: { ticks: { color: "#64748b" }, grid: { color: "#e2e8f0" } },
      },
    },
  });
}

function renderAlarms() {
  const tbody = document.getElementById("alarmTableBody");
  tbody.innerHTML = demoAlarms.map((a) =>
    `<tr><td><time>${new Date(a.created_at).toLocaleString("tr-TR")}</time></td><td>${a.message}</td></tr>`
  ).join("");
}

function setupControls() {
  document.getElementById("btnPumpOn")?.addEventListener("click", () => {
    showToast("Demo: Pompa 5 sn açıldı (simülasyon)");
    demoAlarms.unshift({
      created_at: new Date().toISOString(),
      message: "Pompa manuel açıldı (5 sn) — demo",
    });
    renderAlarms();
  });
  document.getElementById("btnPumpOff")?.addEventListener("click", () => {
    showToast("Demo: Pompa kapatıldı");
  });
  const slider = document.getElementById("servoSlider");
  const label = document.getElementById("servoAngleLabel");
  slider?.addEventListener("input", () => { label.textContent = slider.value; });
  document.getElementById("btnServoApply")?.addEventListener("click", () => {
    showToast(`Demo: Servo ${slider.value}° — simülasyon`);
  });
  document.querySelectorAll(".tab").forEach((tab) => {
    tab.addEventListener("click", () => {
      document.querySelectorAll(".tab").forEach((t) => t.classList.remove("active"));
      tab.classList.add("active");
      activeChartSensor = tab.dataset.chart;
      renderChart(activeChartSensor, generateHistory(activeChartSensor));
    });
  });
}

function tick() {
  updateCards(generateSensors());
}

setupControls();
tick();
renderChart("temperature", generateHistory("temperature"));
renderAlarms();
setInterval(tick, 3000);
showToast("Demo dashboard yüklendi — veriler 3 sn'de bir güncelleniyor");
