import asyncio
import json
import os
import random
import sqlite3
from datetime import datetime, timezone
from typing import Any

import paho.mqtt.client as mqtt
from dotenv import load_dotenv
from influxdb_client import InfluxDBClient, Point
from influxdb_client.client.write_api import SYNCHRONOUS

load_dotenv()

MQTT_BROKER = os.getenv("MQTT_BROKER", "localhost")
MQTT_PORT = int(os.getenv("MQTT_PORT", "1883"))
DEVICE_ID = os.getenv("DEVICE_ID", "greenhouse-01")

INFLUXDB_URL = os.getenv("INFLUXDB_URL", "http://localhost:8086")
INFLUXDB_TOKEN = os.getenv("INFLUXDB_TOKEN", "sera-influx-token-change-me")
INFLUXDB_ORG = os.getenv("INFLUXDB_ORG", "sera")
INFLUXDB_BUCKET = os.getenv("INFLUXDB_BUCKET", "greenhouse")
SQLITE_PATH = os.getenv("SQLITE_PATH", "sera.db")


class GreenhouseMQTTClient:
    def __init__(self) -> None:
        self.latest_sensors: dict[str, dict[str, Any]] = {}
        self.ws_clients: set[Any] = set()
        self._pending_ws_messages: list[dict] = []
        self._pending_pump: dict[str, Any] | None = None
        self._pump_seq: int = 0
        self._last_auto_pump_at: datetime | None = None
        self._influx: InfluxDBClient | None = None
        self._write_api = None
        self._mqtt: mqtt.Client | None = None
        self._init_sqlite()

    def _init_sqlite(self) -> None:
        conn = sqlite3.connect(SQLITE_PATH)
        conn.execute("""
            CREATE TABLE IF NOT EXISTS alarms (
                id INTEGER PRIMARY KEY AUTOINCREMENT,
                message TEXT NOT NULL,
                created_at TEXT NOT NULL
            )
        """)
        conn.execute("""
            CREATE TABLE IF NOT EXISTS rules (
                id INTEGER PRIMARY KEY CHECK (id = 1),
                soil_moisture_threshold REAL DEFAULT 30,
                temperature_alarm_threshold REAL DEFAULT 35,
                water_level_min REAL DEFAULT 10,
                pump_duration_seconds INTEGER DEFAULT 5,
                schedule_start_hour INTEGER DEFAULT 6,
                schedule_end_hour INTEGER DEFAULT 8,
                auto_irrigation_enabled INTEGER DEFAULT 1
            )
        """)
        conn.execute("""
            INSERT OR IGNORE INTO rules (id) VALUES (1)
        """)
        conn.commit()
        conn.close()

    def generate_demo_sensors(self, ip: str = "192.168.1.195") -> None:
        """Gelistirme icin ornek sera verisi — dashboard onizleme."""
        now = datetime.now(timezone.utc).isoformat()
        self.latest_sensors = {
            "temperature": {
                "value": round(random.uniform(21.0, 27.5), 1),
                "unit": "C",
                "timestamp": now,
            },
            "humidity": {
                "value": round(random.uniform(52, 78), 0),
                "unit": "%",
                "timestamp": now,
            },
            "soil_moisture": {
                "value": round(random.uniform(28, 58), 0),
                "unit": "%",
                "timestamp": now,
            },
            "light": {
                "value": round(random.uniform(350, 920), 0),
                "unit": "raw",
                "timestamp": now,
            },
            "water_level": {
                "value": round(random.uniform(55, 92), 0),
                "unit": "%",
                "timestamp": now,
            },
            "status": {
                "online": True,
                "ip": ip,
                "device_id": DEVICE_ID,
                "wifi": "WIFI_ADINIZ",
                "timestamp": now,
            },
        }
        self._pending_ws_messages.append({"type": "sensor", "demo": True})

    def ingest_sensor(self, name: str, value: float, unit: str = "") -> None:
        now = datetime.now(timezone.utc).isoformat()
        entry = {"value": value, "unit": unit, "timestamp": now}
        self.latest_sensors[name] = entry
        self._write_to_influx(name, value, unit)
        self._pending_ws_messages.append({"type": "sensor", "sensor": name, **entry})

    def ingest_status(self, payload: dict[str, Any]) -> None:
        now = datetime.now(timezone.utc).isoformat()
        self.latest_sensors["status"] = {**payload, "timestamp": now}
        self._pending_ws_messages.append({"type": "sensor", "sensor": "status"})

    def connect(self) -> None:
        try:
            self._influx = InfluxDBClient(
                url=INFLUXDB_URL,
                token=INFLUXDB_TOKEN,
                org=INFLUXDB_ORG,
            )
            self._write_api = self._influx.write_api(write_options=SYNCHRONOUS)
        except Exception as exc:
            print(f"InfluxDB baglantisi basarisiz (devam ediliyor): {exc}")
            self._influx = None
            self._write_api = None

        self._mqtt = mqtt.Client(mqtt.CallbackAPIVersion.VERSION2)
        self._mqtt.on_connect = self._on_connect
        self._mqtt.on_message = self._on_message
        try:
            self._mqtt.connect(MQTT_BROKER, MQTT_PORT, 60)
            self._mqtt.loop_start()
            print(f"MQTT broker'a baglandi: {MQTT_BROKER}:{MQTT_PORT}")
        except Exception as exc:
            print(f"MQTT baglantisi basarisiz: {exc}")

    def _on_connect(self, client, userdata, flags, reason_code, properties) -> None:
        topic = f"sera/{DEVICE_ID}/#"
        client.subscribe(topic)
        print(f"MQTT subscribe: {topic}")

    def _on_message(self, client, userdata, msg) -> None:
        try:
            payload = json.loads(msg.payload.decode())
        except json.JSONDecodeError:
            return

        parts = msg.topic.split("/")
        if len(parts) < 4:
            return

        category = parts[2]
        name = parts[3]

        if category == "sensor":
            value = payload.get("value")
            unit = payload.get("unit", "")
            entry = {
                "value": value,
                "unit": unit,
                "timestamp": datetime.now(timezone.utc).isoformat(),
            }
            self.latest_sensors[name] = entry
            self._write_to_influx(name, value, unit)
            self._pending_ws_messages.append({"type": "sensor", "sensor": name, **entry})

        elif category == "status":
            self.latest_sensors["status"] = {
                **payload,
                "timestamp": datetime.now(timezone.utc).isoformat(),
            }

    def _write_to_influx(self, sensor: str, value: Any, unit: str) -> None:
        if self._write_api is None or value is None:
            return
        try:
            point = (
                Point("sensor")
                .tag("device_id", DEVICE_ID)
                .tag("sensor_type", sensor)
                .field("value", float(value))
                .field("unit", unit)
            )
            self._write_api.write(bucket=INFLUXDB_BUCKET, org=INFLUXDB_ORG, record=point)
        except Exception as exc:
            print(f"InfluxDB yazma hatasi: {exc}")

    async def flush_ws_messages(self) -> None:
        if not self._pending_ws_messages:
            return
        messages = self._pending_ws_messages[:]
        self._pending_ws_messages.clear()
        dead = set()
        for ws in self.ws_clients:
            for data in messages:
                try:
                    await ws.send_json(data)
                except Exception:
                    dead.add(ws)
                    break
        self.ws_clients -= dead

    def publish_pump(self, state: str, duration: int = 5) -> None:
        now = datetime.now(timezone.utc).isoformat()
        self._pump_seq += 1
        self._pending_pump = {
            "state": state,
            "duration": duration,
            "seq": self._pump_seq,
        }
        self.latest_sensors["pump"] = {
            "value": 1 if state == "on" else 0,
            "unit": "state",
            "state": state,
            "duration": duration,
            "timestamp": now,
        }
        self._pending_ws_messages.append({"type": "actuator", "actuator": "pump", **self._pending_pump})

        if self._mqtt is None:
            return
        topic = f"sera/{DEVICE_ID}/actuator/pump"
        payload = json.dumps({"state": state, "duration": duration})
        self._mqtt.publish(topic, payload)

    def consume_pending_pump(self) -> dict[str, Any]:
        if not self._pending_pump:
            return {"state": "none", "duration": 0}
        cmd = self._pending_pump
        self._pending_pump = None
        return cmd

    def peek_pending_pump(self) -> dict[str, Any]:
        if not self._pending_pump:
            return {"state": "none", "duration": 0}
        return dict(self._pending_pump)

    def clear_pending_pump(self) -> None:
        self._pending_pump = None

    def ack_pump(self, seq: int) -> bool:
        if not self._pending_pump:
            return False
        if int(self._pending_pump.get("seq", 0)) != seq:
            return False
        self._pending_pump = None
        return True

    def try_auto_irrigation(
        self, soil_moisture: float, water_level: float | None = None
    ) -> bool:
        """Toprak nemi esik altindaysa pompa komutunu kuyruga ekler (2 dk bekleme)."""
        from models.rules import DEFAULT_RULES, IrrigationRules

        rules_data = self.get_rules()
        if not rules_data:
            rules_data = DEFAULT_RULES.__dict__
        rules = IrrigationRules(
            **{k: rules_data[k] for k in IrrigationRules.__dataclass_fields__}
        )

        if not rules.auto_irrigation_enabled:
            return False
        if soil_moisture >= rules.soil_moisture_threshold:
            return False
        if water_level is not None and water_level < rules.water_level_min:
            self.log_alarm(
                f"Pompa engellendi: su seviyesi dusuk (%{water_level:.0f})"
            )
            return False
        if self._pending_pump:
            return False

        now = datetime.now(timezone.utc)
        if self._last_auto_pump_at:
            elapsed = (now - self._last_auto_pump_at).total_seconds()
            if elapsed < 120:
                return False

        self.publish_pump("on", rules.pump_duration_seconds)
        self.log_alarm(
            f"Otomatik sulama: toprak %{soil_moisture:.0f} "
            f"< esik %{rules.soil_moisture_threshold:.0f}"
        )
        self._last_auto_pump_at = now
        return True

    def publish_servo(self, angle: int) -> None:
        if self._mqtt is None:
            return
        topic = f"sera/{DEVICE_ID}/actuator/servo"
        payload = json.dumps({"angle": angle})
        self._mqtt.publish(topic, payload)

    def log_alarm(self, message: str) -> None:
        conn = sqlite3.connect(SQLITE_PATH)
        conn.execute(
            "INSERT INTO alarms (message, created_at) VALUES (?, ?)",
            (message, datetime.now(timezone.utc).isoformat()),
        )
        conn.commit()
        conn.close()

    def get_alarms(self, limit: int = 50) -> list[dict]:
        conn = sqlite3.connect(SQLITE_PATH)
        rows = conn.execute(
            "SELECT id, message, created_at FROM alarms ORDER BY id DESC LIMIT ?",
            (limit,),
        ).fetchall()
        conn.close()
        return [{"id": r[0], "message": r[1], "created_at": r[2]} for r in rows]

    def get_rules(self) -> dict:
        conn = sqlite3.connect(SQLITE_PATH)
        row = conn.execute("SELECT * FROM rules WHERE id = 1").fetchone()
        conn.close()
        if not row:
            return {}
        return {
            "soil_moisture_threshold": row[1],
            "temperature_alarm_threshold": row[2],
            "water_level_min": row[3],
            "pump_duration_seconds": row[4],
            "schedule_start_hour": row[5],
            "schedule_end_hour": row[6],
            "auto_irrigation_enabled": bool(row[7]),
        }

    def update_rules(self, data: dict) -> dict:
        conn = sqlite3.connect(SQLITE_PATH)
        conn.execute("""
            UPDATE rules SET
                soil_moisture_threshold = ?,
                temperature_alarm_threshold = ?,
                water_level_min = ?,
                pump_duration_seconds = ?,
                schedule_start_hour = ?,
                schedule_end_hour = ?,
                auto_irrigation_enabled = ?
            WHERE id = 1
        """, (
            data.get("soil_moisture_threshold", 30),
            data.get("temperature_alarm_threshold", 35),
            data.get("water_level_min", 10),
            data.get("pump_duration_seconds", 5),
            data.get("schedule_start_hour", 6),
            data.get("schedule_end_hour", 8),
            int(data.get("auto_irrigation_enabled", True)),
        ))
        conn.commit()
        conn.close()
        return self.get_rules()

    def query_history(self, sensor: str, hours: int = 24) -> list[dict]:
        if self._influx is None:
            return self._mock_history(sensor, hours)
        try:
            query_api = self._influx.query_api()
            flux = f'''
            from(bucket: "{INFLUXDB_BUCKET}")
              |> range(start: -{hours}h)
              |> filter(fn: (r) => r["_measurement"] == "sensor")
              |> filter(fn: (r) => r["sensor_type"] == "{sensor}")
              |> filter(fn: (r) => r["_field"] == "value")
            '''
            tables = query_api.query(flux, org=INFLUXDB_ORG)
            results = []
            for table in tables:
                for record in table.records:
                    results.append({
                        "timestamp": record.get_time().isoformat(),
                        "value": record.get_value(),
                    })
            return results
        except Exception as exc:
            print(f"InfluxDB sorgu hatasi: {exc}")
            return self._mock_history(sensor, hours)

    def _mock_history(self, sensor: str, hours: int) -> list[dict]:
        import random
        from datetime import timedelta
        now = datetime.now(timezone.utc)
        base = {
            "temperature": 24.0,
            "humidity": 65.0,
            "soil_moisture": 45.0,
            "light": 500.0,
            "water_level": 80.0,
        }.get(sensor, 50.0)
        return [
            {
                "timestamp": (now - timedelta(hours=h)).isoformat(),
                "value": round(base + random.uniform(-5, 5), 1),
            }
            for h in range(hours, 0, -1)
        ]


mqtt_client = GreenhouseMQTTClient()
