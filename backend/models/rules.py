import os
from dataclasses import dataclass, field

from dotenv import load_dotenv

load_dotenv()


@dataclass
class IrrigationRules:
    soil_moisture_threshold: float = 30.0
    temperature_alarm_threshold: float = 35.0
    water_level_min: float = 10.0
    pump_duration_seconds: int = 5
    schedule_start_hour: int = 6
    schedule_end_hour: int = 8
    auto_irrigation_enabled: bool = True


DEFAULT_RULES = IrrigationRules()


def evaluate_rules(
    rules: IrrigationRules,
    soil_moisture: float | None,
    temperature: float | None,
    water_level: float | None,
    current_hour: int,
) -> list[dict]:
    actions: list[dict] = []

    if soil_moisture is not None and soil_moisture < rules.soil_moisture_threshold:
        if rules.auto_irrigation_enabled:
            actions.append({
                "type": "pump",
                "reason": f"Toprak nemi dusuk (%{soil_moisture:.0f} < %{rules.soil_moisture_threshold:.0f})",
                "duration": rules.pump_duration_seconds,
            })

    if temperature is not None and temperature > rules.temperature_alarm_threshold:
        actions.append({
            "type": "alarm",
            "reason": f"Sicaklik yuksek ({temperature:.1f}C > {rules.temperature_alarm_threshold}C)",
        })

    if water_level is not None and water_level < rules.water_level_min:
        actions.append({
            "type": "alarm",
            "reason": f"Su seviyesi kritik (%{water_level:.0f})",
        })

    if rules.schedule_start_hour <= current_hour < rules.schedule_end_hour:
        if soil_moisture is not None and soil_moisture < rules.soil_moisture_threshold + 10:
            actions.append({
                "type": "pump",
                "reason": "Zamanlanmis sulama penceresi",
                "duration": rules.pump_duration_seconds,
            })

    return actions
