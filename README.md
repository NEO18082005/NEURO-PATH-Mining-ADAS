# 🚀 AG~3 NEURO-PATH: Industrial Safety Intelligence System

**ET AI Hackathon 2026 Submission | Problem Statement 1: AI-Powered Industrial Safety Intelligence**

AG~3 NEURO-PATH is a decoupled, Edge-AI Advanced Driver Assistance System (ADAS) engineered specifically for heavy industrial environments (e.g., mining dump trucks, steel plant loaders). It solves the critical issue of operator sensory fatigue by using AI to gate emergency alerts, ensuring that operators only receive warnings when a genuine compound risk is detected.

## 🧠 The Architecture (Decoupled Intelligence)
Traditional industrial safety systems suffer from isolated data streams. NEURO-PATH solves this via a decoupled processing pipeline:
1. **The Vision Edge (Python/YOLOv8):** A high-speed server running YOLOv8 Nano via OpenCV. It captures raw IP streams and classifies objects (vehicles, workers) in real-time.
2. **The Sensory Core (ESP32-CAM/WROOM):** The deterministic hardware layer. It manages the ultrasonic/laser Time-of-Flight sensors and handles the motion-gated UI (LCD/Buzzer).

## 🎯 Scope, Assumptions & Non-Goals
To ensure strict operational safety and clear deployment boundaries, this system is governed by the following parameters:
* **Scope:** Real-time object classification and multi-sensor distance ranging to augment human situational awareness.
* **Assumptions:** Deployed as a retrofit unit on 12V/24V heavy industrial vehicles with an onboard Edge compute node (e.g., Raspberry Pi/Jetson).
* **Non-Goals:** This system **does not** interface with the vehicle's CAN bus and **does not** take autonomous control of the braking systems. It is strictly an intelligence and alerting layer.

## 🛡️ Responsible AI & Model Drift Mitigation
Relying solely on a camera's pinhole geometry model for distance estimation in real-world industrial settings is dangerous due to varying vehicle sizes and angles. 
**Our Solution:** The AI acts as the *classification* engine, while the hardware ultrasonic/laser matrix acts as the *ground-truth distance ranging* engine. If the AI model suffers from drift (e.g., unrecognised vehicle designs), the deterministic hardware sensors act as an absolute fail-safe.

## 🛠️ Tech Stack
* **Hardware:** ESP32-CAM (OV2640), Ultrasonic Sensor Matrix, Laser ToF.
* **Software:** Python 3.10+, Ultralytics (YOLOv8 Nano), OpenCV, C++ (Arduino IDE).

## 🚀 Quick Start
1. Flash the ESP32 code located in `/ESP_32_WROOM_CODE` via the Arduino IDE (Ensure board is set to ESP32 Dev Module).
2. Install Python dependencies: `pip install -r requirements.txt`
3. Launch the Vision Edge server: `python neuro_path_server.py`
