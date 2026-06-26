// Ref: docs/mimari.md
const API = window.location.origin;
const SENSORS = ["temperature", "humidity", "soil_moisture", "light", "water_level"];
const SENSOR_LABELS = {
  temperature: "Sıcaklık",
  humidity: "Nem",
  soil_moisture: "Toprak",
  light: "Işık",
  water_level: "Su",
};

let historyChart = null;
let activeChartSensor = "temperature";
let wsConnected = false;

const THRESHOLDS = {
  temperature: { warn: 30, danger: 35, max: 50, unit: "°C", higherIsBad: true },
  humidity: { warn: 40, danger: 30, max: 100, unit: "%", higherIsBad: false },
  soil_moisture: { warn: 40, danger: 30, max: 100, unit: "%", higherIsBad: false },
  light: { warn: 200, danger: 100, max: 1024, unit: "", higherIsBad: false },
  water_level: { warn: 30, danger: 10, max: 100, unit: "%", higherIsBad: false },
};

async function fetchJSON(url, options = {}) {
  const res = await fetch(url, {
    headers: { "Content-Type": "application/json" },
    ...options,
  });
  if (!res.ok) throw new Error(`HTTP ${res.status}`);
  return res.json();
}

function showToast(message, type = "success") {
  const container = document.getElementById("toastContainer");
  if (!container) return;
  const el = document.createElement("div");
  el.className = `toast ${type}`;
  el.textContent = message;
  container.appendChild(el);
  setTimeout(() => el.remove(), 3500);
}

function setApiStatus(online) {
  const dot = document.getElementById("apiStatusDot");
  const text = document.getElementById("apiStatusText");
  if (dot) {
    dot.classList.toggle("online", online);
    dot.classList.toggle("offline", !online);
  }
  if (text) text.textContent = online ? "API bağlı" : "API bağlantısı yok";
}

function updateLastUpdate() {
  const el = document.getElementById("lastUpdate");
  if (el) {
    el.textContent = new Date().toLocaleString("tr-TR", {
      hour: "2-digit",
      minute: "2-digit",
      second: "2-digit",
    });
  }
}

function getStatusLevel(name, val) {
  const t = THRESHOLDS[name];
  if (!t || val === null || Number.isNaN(val)) return "neutral";

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
  return { ok: "Normal", warn: "Uyarı", danger: "Kritik", neutral: "—" }[level];
}

function updateSensorCards(sensors) {
  SENSORS.forEach((name) => {
    const el = document.getElementById(`val-${name}`);
    const card = document.querySelector(`.kpi-card[data-sensor="${name}"]`);
    const badge = document.getElementById(`badge-${name}`);
    const bar = document.getElementById(`bar-${name}`);
    if (!el || !card) return;

    const data = sensors[name];
    if (!data || data.value === undefined) {
      el.textContent = "—";
      card.className = "kpi-card";
      if (badge) { badge.textContent = "—"; badge.className = "kpi-badge"; }
      if (bar) bar.style.width = "0%";
      return;
    }

    const val = Number(data.value);
    el.textContent = Number.isInteger(val) ? val : val.toFixed(1);

    const level = getStatusLevel(name, val);
    card.className = `kpi-card status-${level === "neutral" ? "ok" : level}`;

    if (badge) {
      badge.textContent = statusLabel(level);
      badge.className = `kpi-badge ${level === "neutral" ? "" : level}`;
    }

    if (bar) {
      const max = THRESHOLDS[name]?.max || 100;
      bar.style.width = `${Math.min(100, (val / max) * 100)}%`;
      bar.style.background =
        level === "danger"
          ? "linear-gradient(90deg, #dc2626, #f87171)"
          : level === "warn"
            ? "linear-gradient(90deg, #d97706, #fbbf24)"
            : "linear-gradient(90deg, #059669, #34d399)";
    }
  });

  updateDevicePanel(sensors.status || {});
  updateLastUpdate();
}

function updateDevicePanel(status) {
  const conn = document.getElementById("deviceConnection");
  const ip = document.getElementById("deviceIp");
  const deviceId = document.getElementById("deviceId");

  if (deviceId && status.device_id) {
    deviceId.textContent = status.device_id;
  }

  const online = status.online === true;
  if (conn) {
    conn.textContent = online ? "Çevrimiçi" : "Çevrimdışı";
    conn.className = `status-pill ${online ? "online" : "offline"}`;
  }

  if (ip) {
    ip.textContent = status.ip || status.ip_address || "—";
  }
}

async function refreshSensors() {
  try {
    const data = await fetchJSON(`${API}/api/sensors/latest`);
    updateSensorCards(data.sensors || {});
    if (data.device_id) {
      const el = document.getElementById("deviceId");
      if (el) el.textContent = data.device_id;
    }
    setApiStatus(true);
  } catch (err) {
    console.warn("Sensör verisi alınamadı:", err);
    setApiStatus(false);
  }
}

async function loadHistory(sensor = activeChartSensor) {
  try {
    const data = await fetchJSON(`${API}/api/sensors/history?sensor=${sensor}&hours=24`);
    renderChart(sensor, data.data || []);
  } catch (err) {
    console.warn("Geçmiş veri alınamadı:", err);
  }
}

function renderChart(sensor, points) {
  const ctx = document.getElementById("historyChart");
  if (!ctx) return;

  const labels = points.map((p) => {
    const d = new Date(p.timestamp);
    return `${d.getHours()}:${String(d.getMinutes()).padStart(2, "0")}`;
  });
  const values = points.map((p) => p.value);

  if (historyChart) historyChart.destroy();

  historyChart = new Chart(ctx, {
    type: "line",
    data: {
      labels,
      datasets: [{
        label: SENSOR_LABELS[sensor] || sensor,
        data: values,
        borderColor: "#059669",
        backgroundColor: "rgba(5, 150, 105, 0.08)",
        fill: true,
        tension: 0.35,
        pointRadius: 2,
        pointHoverRadius: 5,
        borderWidth: 2,
      }],
    },
    options: {
      responsive: true,
      maintainAspectRatio: false,
      plugins: {
        legend: { display: false },
        tooltip: {
          backgroundColor: "#0f172a",
          padding: 12,
          cornerRadius: 8,
        },
      },
      scales: {
        x: {
          ticks: { color: "#64748b", maxTicksLimit: 8, font: { size: 11 } },
          grid: { color: "#e2e8f0" },
        },
        y: {
          ticks: { color: "#64748b", font: { size: 11 } },
          grid: { color: "#e2e8f0" },
        },
      },
    },
  });
}

async function loadRules() {
  try {
    const rules = await fetchJSON(`${API}/api/rules`);
    const form = document.getElementById("rulesForm");
    if (!form) return;
    form.soil_moisture_threshold.value = rules.soil_moisture_threshold ?? 30;
    form.temperature_alarm_threshold.value = rules.temperature_alarm_threshold ?? 35;
    form.pump_duration_seconds.value = rules.pump_duration_seconds ?? 5;
    form.auto_irrigation_enabled.checked = rules.auto_irrigation_enabled !== false;
  } catch (err) {
    console.warn("Kurallar yüklenemedi:", err);
  }
}

async function loadAlarms() {
  try {
    const data = await fetchJSON(`${API}/api/alarms?limit=20`);
    const tbody = document.getElementById("alarmTableBody");
    if (!tbody) return;

    const alarms = data.alarms || [];
    if (!alarms.length) {
      tbody.innerHTML = '<tr><td colspan="2" class="empty">Henüz alarm yok</td></tr>';
      return;
    }

    tbody.innerHTML = alarms.map((a) =>
      `<tr>
        <td><time>${new Date(a.created_at).toLocaleString("tr-TR")}</time></td>
        <td>${a.message}</td>
      </tr>`
    ).join("");
  } catch (err) {
    console.warn("Alarmlar yüklenemedi:", err);
  }
}

function setupControls() {
  document.getElementById("btnPumpOn")?.addEventListener("click", async () => {
    try {
      await fetchJSON(`${API}/api/actuators/pump`, {
        method: "POST",
        body: JSON.stringify({ state: "on", duration: 5 }),
      });
      showToast("Pompa 5 saniye açıldı");
      loadAlarms();
    } catch {
      showToast("Pompa komutu gönderilemedi", "error");
    }
  });

  document.getElementById("btnPumpOff")?.addEventListener("click", async () => {
    try {
      await fetchJSON(`${API}/api/actuators/pump`, {
        method: "POST",
        body: JSON.stringify({ state: "off", duration: 0 }),
      });
      showToast("Pompa kapatıldı");
    } catch {
      showToast("Pompa komutu gönderilemedi", "error");
    }
  });

  const slider = document.getElementById("servoSlider");
  const label = document.getElementById("servoAngleLabel");
  slider?.addEventListener("input", () => {
    if (label) label.textContent = slider.value;
  });

  document.getElementById("btnServoApply")?.addEventListener("click", async () => {
    const angle = parseInt(slider?.value || "90", 10);
    try {
      await fetchJSON(`${API}/api/actuators/servo`, {
        method: "POST",
        body: JSON.stringify({ angle }),
      });
      showToast(`Servo ${angle}° konumuna ayarlandı`);
    } catch {
      showToast("Servo komutu gönderilemedi", "error");
    }
  });

  document.getElementById("btnEvaluateRules")?.addEventListener("click", async () => {
    try {
      const result = await fetchJSON(`${API}/api/rules/evaluate`, { method: "POST" });
      showToast(`${result.actions?.length || 0} kural aksiyonu değerlendirildi`);
      loadAlarms();
    } catch {
      showToast("Kural değerlendirmesi başarısız", "error");
    }
  });

  document.getElementById("rulesForm")?.addEventListener("submit", async (e) => {
    e.preventDefault();
    const form = e.target;
    try {
      await fetchJSON(`${API}/api/rules`, {
        method: "PUT",
        body: JSON.stringify({
          soil_moisture_threshold: parseFloat(form.soil_moisture_threshold.value),
          temperature_alarm_threshold: parseFloat(form.temperature_alarm_threshold.value),
          pump_duration_seconds: parseInt(form.pump_duration_seconds.value, 10),
          auto_irrigation_enabled: form.auto_irrigation_enabled.checked,
        }),
      });
      showToast("Sulama kuralları kaydedildi");
    } catch {
      showToast("Kurallar kaydedilemedi", "error");
    }
  });

  document.querySelectorAll(".tab").forEach((tab) => {
    tab.addEventListener("click", () => {
      document.querySelectorAll(".tab").forEach((t) => t.classList.remove("active"));
      tab.classList.add("active");
      activeChartSensor = tab.dataset.chart;
      loadHistory(activeChartSensor);
    });
  });
}

function connectWebSocket() {
  const wsUrl = API.replace("http", "ws") + "/ws/live";
  try {
    const ws = new WebSocket(wsUrl);
    ws.onopen = () => { wsConnected = true; };
    ws.onmessage = () => refreshSensors();
    ws.onclose = () => {
      wsConnected = false;
      setTimeout(connectWebSocket, 5000);
    };
  } catch {
    // REST polling yedek
  }
}

async function init() {
  setupControls();
  await refreshSensors();
  await loadHistory();
  await loadRules();
  await loadAlarms();
  connectWebSocket();
  setInterval(refreshSensors, 2000);
  setInterval(loadAlarms, 30000);

  try {
    const data = await fetchJSON(`${API}/api/sensors/latest`);
    if (data.demo_mode) {
      showToast("Demo modu — örnek sensör verileri gösteriliyor");
    }
  } catch {
    // refreshSensors zaten hata verdi
  }
}

init();
