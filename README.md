# AG~3 NEURO-PATH: Intelligent Edge-AI Blind Spot Detection System

**Neuro-Path** is an advanced, human-centric Advanced Driver Assistance System (ADAS) engineered for the **USAII® Global AI Hackathon 2026**. Designed to combat "sensory fatigue" and high cognitive switch costs during driving emergencies, this project utilizes a decoupled architecture to provide deterministic, zero-latency blind spot monitoring. 

By combining highly optimized Edge AI vision processing with a localized multi-sensor fusion hardware matrix, Neuro-Path protects vulnerable road users without overloading the driver's cognitive workload.

---

## 🏆 USAII® Global AI Hackathon 2026 Submission
* **Team:** AG~3 NEURO-PATH 
* **Members:** Subhan Khan & Ayaan Khan
* **Core Technologies:** YOLOv8 Nano, Python OpenCV, Embedded C++, Sensor Fusion (Ultrasonic/Laser ToF), ESP32 Architecture.

---

## 🛠 Project Architecture (Decoupled Compute Strategy)

The system is engineered as a two-tier safety suite to guarantee performance and hardware stability:

1. **The "Vision Edge" (Python Server + ESP32-CAM):** A highly optimized Python pipeline running **YOLOv8 Nano** to process real-time serial video streams. It actively filters environmental noise to classify only valid targets (Cars, Motorcycles, Buses, and Trucks).
2. **The "Sensory Core" (ESP32 WROOM V12.3 Stable):** The local hardware fail-safe layer. It handles high-speed radar sweeps, time-of-flight distance ranging, and diagnostic hardware checks to guarantee deterministic, crash-free performance independent of the AI vision stream.
3. **Cognitive-First UI (Motion-Gated Logic):** A custom I2C LCD HUD and parallel-wired warning matrix that delivers zero-latency alerts *only* when a genuine threat breaches the safety envelope and the vehicle is in motion.

---

## 📋 Hardware Matrix & Component Specifications

| Component | Quantity | Role in Neuro-Path |
| :--- | :--- | :--- |
| **ESP32 WROOM Dev Board**| 1 | "Sensory Core" Processor (Local Failsafes) |
| **ESP32-CAM (OV2640)** | 1 | "Vision Edge" Serial Streamer |
| **VL53L0X Laser ToF** | 1 | High-precision longitudinal ranging (I2C) |
| **HC-SR04 Ultrasonic** | 3 | Tri-directional lateral spatial detection |
| **I2C LCD (16x2)** | 1 | Real-time Telemetry HUD |
| **MG90S Servo Motor** | 1 | 180° Rear-Scanning Radar Turret Actuator |
| **Active Buzzer** | 1 | Auditory Warning (Motion-Gated) |
| **Red LED** | 1 | Critical Threat Visual Indicator |
| **Green LED** | 1 | Safety Envelope Clear Indicator |
| **Optocoupler/Divider** | 1 | Logic Level Shifter for Safe Motion Sensing |

---

## 🔌 Technical Pin Mapping (Sensory Core - ESP32 WROOM)

> **Engineering Note:** To ensure hardware stability and prevent strapping pin boot failures, all high-bandwidth vision tasks are offloaded to the ESP32-CAM, while all localized telemetry and motion sensing are handled safely on the ESP32 WROOM's input-only and standard GPIO pins.

| Component | ESP32 WROOM Pin | Function |
| :--- | :--- | :--- |
| **Laser & LCD (SDA)** | GPIO 21 | I2C Data Bus |
| **Laser & LCD (SCL)** | GPIO 22 | I2C Clock Bus |
| **RC Forward (F)** | GPIO 34 | Motion Input (Voltage Divided/Safe) |
| **RC Backward (B)** | GPIO 35 | Motion Input (Voltage Divided/Safe) |
| **Rear Radar Trig** | GPIO 18 | Pulse Trigger |
| **Rear Radar Echo** | GPIO 19 | Pulse Return |
| **Left Sonar Echo** | GPIO 32 | Dedicated Return (Prevents Contention) |
| **Right Sonar Echo**| GPIO 33 | Dedicated Return (Prevents Contention) |
| **Radar Servo PWM** | GPIO 25 | 180° Sweep Actuator |
| **Red LED + Buzzer**| GPIO 26 | **DANGER/SLOW** Matrix (Parallel) |
| **Green LED** | GPIO 27 | **SYSTEM CLEAR** Indicator |

---

## 🚀 Cognitive-First Boot Sequence

Upon power-up, the Sensory Core executes a multi-phase cinematic initialization to confirm system readiness without overwhelming the operator:
1. **Phase 1 (Typewriter):** `NEURO-PATH` title appears character-by-character.
2. **Phase 2 (Static):** `BLINDSPOT SYS` subtitle initialization.
3. **Phase 3 (Loading):** Dedicated `SYSTEM BOOTING` screen with a 16-character progressive dot-loading bar.
4. **Final Handshake:** Hardware check verifies I2C communication and Ultrasonic pulse returns before synchronizing with the Python Vision Server.

---

## 📊 Operational Alert Logic (Motion-Gated)

To eliminate sensory fatigue, auditory alerts are strictly gated by vehicle motion state:

* **🟢 Status Clear:** No objects within threat envelope. Green LED is **ACTIVE**.
* **🟡 Caution Slow:** Object detected in secondary zone. If the vehicle is in motion, Red LED and Buzzer **PULSE** (150ms).
* **🔴 Critical Alert:** Object breaches primary safety envelope. If the vehicle is in motion, Red LED and Buzzer provide a **CONTINUOUS** high-frequency alert.
* **⚠️ Hardware Fail:** If sensors disconnect, the LCD displays a diagnostic code with a constant safety override beep.

---

### **Installation & Deployment**

**1. The Vision Server (Python)**
* Install requirements: `pip install ultralytics opencv-python pyserial`
* Ensure your ESP32-CAM is running a standard serial camera streamer sketch.
* Update the COM port in `vision_server.py` and run the script to initialize YOLOv8 tracking.

**2. The Sensory Core (Arduino IDE)**
* Open the `.ino` file in Arduino IDE.
* Select **DOIT ESP32 DEVKIT V1** as your board.
* Install required libraries via Library Manager: 
  * `LiquidCrystal_I2C`
  * `ESP32Servo`
  * `Adafruit_VL53L0X`
  * `NewPing` (Critical for non-blocking sonar polling)
* Compile and upload to the ESP32 WROOM board.
