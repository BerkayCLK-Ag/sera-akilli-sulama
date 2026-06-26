import asyncio
import os
import socket
from contextlib import asynccontextmanager
from datetime import datetime, timezone
from typing import Any

from dotenv import load_dotenv
from fastapi import FastAPI, WebSocket, WebSocketDisconnect
from fastapi.middleware.cors import CORSMiddleware
from fastapi.responses import FileResponse, PlainTextResponse
from fastapi.staticfiles import StaticFiles
from pydantic import BaseModel

from mqtt_client import mqtt_client
from models.rules import DEFAULT_RULES, evaluate_rules

load_dotenv()

FRONTEND_DIR = os.path.join(os.path.dirname(__file__), "..", "frontend")
DEMO_MODE = os.getenv("DEMO_MODE", "true").lower() == "true"
DEMO_IP = os.getenv("DEMO_DEVICE_IP", "192.168.1.195")
DISCOVERY_PORT = 9999


def get_lan_ip() -> str:
    """ESP8266'nin baglanacagi yerel ag IP'si (PC yeniden acilsa bile guncel)."""
    probe = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    try:
        probe.connect(("192.168.1.1", 1))
        return probe.getsockname()[0]
    except OSError:
        return "127.0.0.1"
    finally:
        probe.close()


async def discovery_beacon_loop() -> None:
    """Her 2 sn UDP ile PC IP'sini yayinla — Arduino sketch elle guncellemez."""
    sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    sock.setsockopt(socket.SOL_SOCKET, socket.SO_BROADCAST, 1)
    while True:
        ip = get_lan_ip()
        msg = f"SERADASH|{ip}|8000".encode()
        sock.sendto(msg, ("255.255.255.255", DISCOVERY_PORT))
        parts = ip.split(".")
        if len(parts) == 4:
            subnet = f"{parts[0]}.{parts[1]}.{parts[2]}.255"
            sock.sendto(msg, (subnet, DISCOVERY_PORT))
        await asyncio.sleep(2)


async def demo_data_loop() -> None:
    while True:
        await asyncio.sleep(5)
        mqtt_client.generate_demo_sensors(DEMO_IP)
        await mqtt_client.flush_ws_messages()


@asynccontextmanager
async def lifespan(app: FastAPI):
    mqtt_client.connect()
    lan_ip = get_lan_ip()
    print(f"[sera] LAN IP: {lan_ip} — ESP UDP port {DISCOVERY_PORT} ile otomatik bulur")
    beacon_task = asyncio.create_task(discovery_beacon_loop())
    demo_task = None
    if DEMO_MODE:
        mqtt_client.generate_demo_sensors(DEMO_IP)
        if not mqtt_client.get_alarms(1):
            mqtt_client.log_alarm("Demo modu aktif — ornek sensör verileri")
        demo_task = asyncio.create_task(demo_data_loop())
    yield
    beacon_task.cancel()
    if demo_task:
        demo_task.cancel()


app = FastAPI(
    title="Akilli Sera IoT API",
    description="Ref: docs/mimari.md",
    lifespan=lifespan,
)

app.add_middleware(
    CORSMiddleware,
    allow_origins=["*"],
    allow_methods=["*"],
    allow_headers=["*"],
)


class PumpCommand(BaseModel):
    state: str = "on"
    duration: int = 5


class ServoCommand(BaseModel):
    angle: int = 90


class RulesUpdate(BaseModel):
    soil_moisture_threshold: float | None = None
    temperature_alarm_threshold: float | None = None
    water_level_min: float | None = None
    pump_duration_seconds: int | None = None
    schedule_start_hour: int | None = None
    schedule_end_hour: int | None = None
    auto_irrigation_enabled: bool | None = None


class SensorIngest(BaseModel):
    sensor: str
    value: float
    unit: str = ""


class StatusIngest(BaseModel):
    online: bool = True
    ip: str = ""
    device_id: str = "greenhouse-01"
    wifi: str = ""


class PumpAck(BaseModel):
    seq: int = 0


class BatchIngest(BaseModel):
    """Tek HTTP istegi ile tum sensorler — ESP reset riskini azaltir."""
    temperature: float | None = None
    humidity: float | None = None
    soil_moisture: float | None = None
    ip: str = ""
    device_id: str = "greenhouse-01"
    wifi: str = ""


@app.get("/api/sensors/latest")
def get_latest_sensors() -> dict[str, Any]:
    return {
        "device_id": os.getenv("DEVICE_ID", "greenhouse-01"),
        "demo_mode": DEMO_MODE,
        "sensors": mqtt_client.latest_sensors,
        "timestamp": datetime.now(timezone.utc).isoformat(),
    }


@app.post("/api/demo/refresh")
def refresh_demo_data() -> dict[str, Any]:
    mqtt_client.generate_demo_sensors(DEMO_IP)
    return {
        "status": "ok",
        "sensors": mqtt_client.latest_sensors,
    }


@app.post("/api/sensors/ingest")
def ingest_sensor(reading: SensorIngest) -> dict[str, Any]:
    """ESP8266/Arduino'dan gelen gercek sensör verisi."""
    mqtt_client.ingest_sensor(reading.sensor, reading.value, reading.unit)
    return {"status": "ok", "sensor": reading.sensor, "value": reading.value}


@app.post("/api/sensors/status")
def ingest_status(status: StatusIngest) -> dict[str, Any]:
    """Cihaz durumu (IP, online vb.)."""
    mqtt_client.ingest_status(status.model_dump())
    return {"status": "ok"}


@app.post("/api/sensors/ingest/batch")
def ingest_batch(batch: BatchIngest) -> dict[str, Any]:
    """Tek TCP baglantisi ile sicaklik + nem + toprak + durum."""
    if batch.temperature is not None:
        mqtt_client.ingest_sensor("temperature", batch.temperature, "C")
    if batch.humidity is not None:
        mqtt_client.ingest_sensor("humidity", batch.humidity, "%")
    if batch.soil_moisture is not None:
        mqtt_client.ingest_sensor("soil_moisture", batch.soil_moisture, "%")
        # Otomatik sulama ayri test — manuel pompa oncelikli
        # mqtt_client.try_auto_irrigation(batch.soil_moisture)
    mqtt_client.ingest_status({
        "online": True,
        "ip": batch.ip,
        "device_id": batch.device_id,
        "wifi": batch.wifi,
    })
    return {
        "status": "ok",
        "pump": mqtt_client.peek_pending_pump(),
    }


@app.get("/api/sensors/ingest/simple", response_class=PlainTextResponse)
def ingest_simple(
    temperature: float,
    humidity: float,
    soil_moisture: float,
    water_level: float | None = None,
    ip: str = "",
    device_id: str = "greenhouse-01",
    wifi: str = "",
) -> str:
    """Arduino/ESP32 icin duz metin — JSON parse gerekmez."""
    mqtt_client.ingest_sensor("temperature", temperature, "C")
    mqtt_client.ingest_sensor("humidity", humidity, "%")
    mqtt_client.ingest_sensor("soil_moisture", soil_moisture, "%")
    if water_level is not None:
        mqtt_client.ingest_sensor("water_level", water_level, "%")
    mqtt_client.try_auto_irrigation(soil_moisture, water_level)
    mqtt_client.ingest_status({
        "online": True,
        "ip": ip,
        "device_id": device_id,
        "wifi": wifi,
    })
    pump = mqtt_client.peek_pending_pump()
    if pump.get("state") == "on":
        return f"OK PUMP {int(pump.get('duration', 5))} {int(pump.get('seq', 0))}"
    return "OK NONE 0 0"


@app.get("/api/actuators/pump/ack-simple", response_class=PlainTextResponse)
def ack_pump_simple(seq: int = 0) -> str:
    if mqtt_client.ack_pump(seq):
        return "ACK OK"
    return "ACK NONE"


@app.get("/api/sensors/history")
def get_sensor_history(sensor: str = "humidity", hours: int = 24) -> dict:
    data = mqtt_client.query_history(sensor, hours)
    return {"sensor": sensor, "hours": hours, "data": data}


@app.post("/api/actuators/pump")
def control_pump(cmd: PumpCommand) -> dict:
    mqtt_client.publish_pump(cmd.state, cmd.duration)
    if cmd.state == "on":
        mqtt_client.log_alarm(f"Pompa manuel acildi ({cmd.duration} sn)")
    elif cmd.state == "off":
        mqtt_client.log_alarm("Pompa manuel kapatildi")
    return {"status": "ok", "state": cmd.state, "duration": cmd.duration}


@app.get("/api/actuators/pump/pending")
def get_pending_pump() -> dict:
    """Arduino komutu okur — silmez (ack gelene kadar kalir)."""
    return mqtt_client.peek_pending_pump()


@app.post("/api/actuators/pump/ack")
def ack_pump(ack: PumpAck) -> dict:
    """Arduino pompayi calistirdiktan sonra seq ile onaylar."""
    cleared = mqtt_client.ack_pump(ack.seq)
    return {"status": "ok", "cleared": cleared}


@app.post("/api/actuators/servo")
def control_servo(cmd: ServoCommand) -> dict:
    angle = max(0, min(180, cmd.angle))
    mqtt_client.publish_servo(angle)
    return {"status": "ok", "angle": angle}


@app.get("/api/rules")
def get_rules() -> dict:
    rules = mqtt_client.get_rules()
    if not rules:
        return DEFAULT_RULES.__dict__
    return rules


@app.put("/api/rules")
def update_rules(update: RulesUpdate) -> dict:
    current = mqtt_client.get_rules()
    data = {**current, **update.model_dump(exclude_none=True)}
    return mqtt_client.update_rules(data)


@app.get("/api/alarms")
def get_alarms(limit: int = 50) -> dict:
    return {"alarms": mqtt_client.get_alarms(limit)}


@app.post("/api/rules/evaluate")
def evaluate_current_rules() -> dict:
    sensors = mqtt_client.latest_sensors
    rules_data = mqtt_client.get_rules() or DEFAULT_RULES.__dict__
    from models.rules import IrrigationRules
    rules = IrrigationRules(**{k: rules_data[k] for k in IrrigationRules.__dataclass_fields__})
    hour = datetime.now().hour

    soil = sensors.get("soil_moisture", {}).get("value")
    temp = sensors.get("temperature", {}).get("value")
    water = sensors.get("water_level", {}).get("value")

    actions = evaluate_rules(rules, soil, temp, water, hour)
    for action in actions:
        if action["type"] == "alarm":
            mqtt_client.log_alarm(action["reason"])
        elif action["type"] == "pump":
            mqtt_client.publish_pump("on", action.get("duration", 5))
            mqtt_client.log_alarm(action["reason"])

    return {"actions": actions}


@app.websocket("/ws/live")
async def websocket_live(websocket: WebSocket):
    await websocket.accept()
    mqtt_client.ws_clients.add(websocket)
    try:
        await websocket.send_json({
            "type": "connected",
            "sensors": mqtt_client.latest_sensors,
        })
        while True:
            await mqtt_client.flush_ws_messages()
            try:
                await asyncio.wait_for(websocket.receive_text(), timeout=1.0)
            except TimeoutError:
                pass
    except WebSocketDisconnect:
        mqtt_client.ws_clients.discard(websocket)


frontend_path = os.path.abspath(FRONTEND_DIR)
if os.path.isdir(frontend_path):
    app.mount("/static", StaticFiles(directory=frontend_path), name="static")

    @app.get("/")
    def serve_dashboard():
        return FileResponse(os.path.join(frontend_path, "index.html"))
