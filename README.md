# Enterprise IoT & Embedded Automation Platform

An ESP32-based IoT monitoring and automation system, simulated in Wokwi. Sensor data is published over MQTT to a cloud broker and shown on a Node-RED dashboard.

## Features
- Multi-sensor monitoring: temperature, humidity, soil moisture, gas and vibration
- Real-time cloud connectivity using MQTT (HiveMQ public broker)
- Automation: relay (pump/fan) turns ON when soil moisture < 30% or temperature > 35°C
- Predictive maintenance: vibration anomaly detection using a moving average
- Automated alerts: buzzer and alert flag when gas > 70% or a vibration anomaly is detected
- Live dashboard with gauges, trend chart and status indicators

## Architecture
Sensors -> ESP32 -> Wi-Fi -> MQTT broker (HiveMQ) -> Node-RED -> Dashboard

## Hardware (simulated in Wokwi)
| Component | Role | ESP32 pin |
|---|---|---|
| DHT22 | Temperature and humidity | GPIO 15 |
| Potentiometer 1 | Soil moisture | GPIO 34 |
| Potentiometer 2 | Gas sensor | GPIO 35 |
| Potentiometer 3 | Vibration | GPIO 32 |
| Relay module | Pump / fan | GPIO 26 |
| Buzzer | Alert | GPIO 27 |

## Files
- `sketch.ino` - ESP32 firmware
- `diagram.json` - Wokwi circuit
- `libraries.txt` - required libraries (DHT sensor library, PubSubClient)
- `nodered-flow.json` - dashboard flow (import into Node-RED)

## How to run
1. Open the Wokwi project: **PASTE YOUR WOKWI LINK HERE**
2. Press Play. Data is published to the topic `uppara_sreenath/iot/level3/data`.
3. In Node-RED, install `node-red-dashboard`, import `nodered-flow.json`, and click Deploy.
4. Open `http://localhost:1880/ui` to see the dashboard.

## Test results
| Test | Input | Result |
|---|---|---|
| Dry soil | Soil < 30% | Relay ON |
| High temperature | Temp > 35°C | Relay ON |
| Gas leak | Gas > 70% | Buzzer ON, alert = 1 |
| Vibration spike | Sudden jump | Alert = 1 |
| Normal | All values normal | Relay OFF, alert = 0 |

## Power optimization
Use deep sleep (`esp_deep_sleep_start()`) and a longer publish interval to reduce energy use.

## Screenshots
Add the circuit, Serial Monitor, HiveMQ data and dashboard screenshots here.

## Author
Uppara Sreenath
