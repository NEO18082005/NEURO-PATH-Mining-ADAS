# AG~3 NEURO-PATH: Intelligent Blind Spot Detection System

**Neuro-Path** is a sophisticated sensor-fusion safety system designed for real-time blind spot monitoring. By combining Laser Time-of-Flight (ToF) and Ultrasonic sweeping, the system provides redundant, high-accuracy obstacle detection specifically optimized for the **ESP32-CAM** architecture.

---

## 🛠 Project Architecture

The system is engineered as a multi-modal safety suite:
1.  **Sensory Fusion Module (V12.3 Stable):** The core "Brain" handling high-speed radar sweeps, distance calculation, and diagnostic hardware checks.
2.  **Cinematic Cockpit UI:** A custom-coded 16x2 LCD interface featuring typewriter animations and progressive loading sequences.
3.  **Motion-Gated Alert Logic:** An intelligent filtering system that silences alerts when the vehicle is stationary to reduce operator fatigue and cognitive load.

---

## 📋 Component List & Specifications

| Component | Quantity | Role in Neuro-Path |
| :--- | :--- | :--- |
| **ESP32-CAM** | 1 | AI-Thinker Core Processor |
| **VL53L0X Laser ToF** | 1 | High-precision distance (I2C) |
| **HC-SR04 Ultrasonic** | 1 | Wide-angle proximity detection |
| **I2C LCD (16x2)** | 1 | Real-time Telemetry Display |
| **SG90 Servo Motor** | 1 | 180° Mechanical Radar Actuator |
| **Active Buzzer** | 1 | Auditory Warning (Motion-Gated) |
| **Red LED** | 1 | Critical Warning Indicator |
| **Green LED** | 1 | System Status Clear Indicator |
| **RC Vehicle Interface** | 1 | Digital Input for Motion Sensing |

---

## 🔌 Technical Pin Mapping (V12.3 Optimized)

> **Note:** This configuration resolves the ESP32-CAM pin shortage by consolidating the high-intensity alert system on GPIO 4 (shared with the onboard Flashlight) and the status indicator on GPIO 2.

| Component | Pin | Function |
| :--- | :--- | :--- |
| **Laser & LCD (SDA)** | GPIO 14 | I2C Data |
| **Laser & LCD (SCL)** | GPIO 15 | I2C Clock |
| **RC Forward (F)** | GPIO 13 | High = Vehicle Moving |
| **RC Backward (B)** | GPIO 12 | High = Vehicle Moving |
| **Ultrasonic Trig** | GPIO 0 | Pulse Trigger |
| **Ultrasonic Echo** | GPIO 3 | Pulse Return (U0RX) |
| **Servo Signal** | GPIO 16 | PWM Radar Sweep |
| **Red LED + Buzzer**| GPIO 4 | **DANGER/SLOW** (Onboard Flashlight) |
| **Green LED** | GPIO 2 | **SYSTEM CLEAR** (Onboard Blue LED) |

---

## 🚀 Cinematic Boot Sequence

Upon power-up, Neuro-Path executes a multi-phase initialization:
1.  **Phase 1 (Typewriter):** `NEURO-PATH` title appears character-by-character.
2.  **Phase 2 (Static):** `BLINDSPOT SYS` subtitle initialization.
3.  **Phase 3 (Loading):** Dedicated `SYSTEM BOOTING` screen with a 16-character progressive dot-loading bar traversing the entire second line.
4.  **Final Handshake:** Hardware check verifies I2C communication and Ultrasonic pulse return before entering live telemetry mode.

---

## 📊 Operational Logic

* **🟢 Status Clear:** No objects within 35cm. Green LED is **ACTIVE**.
* **🟡 Caution Slow:** Object detected (15cm - 35cm). If vehicle is moving, Red LED/Flashlight and Buzzer **PULSE** (150ms).
* **🔴 Critical Alert:** Object detected (< 15cm). If vehicle is moving, Red LED/Flashlight and Buzzer provide a **CONTINUOUS** high-frequency alert.
* **⚠️ Hardware Fail:** If sensors are disconnected, LCD displays `CHECK SENSORS` or `N/C` with a constant safety override beep.

---

## 📜 Author
* **Subhan Khan** - *Lead Developer & BCA Student* - [K.K. Modi University]
* **Mentor:** Prof. (Dr.) Mohammed Bakhtawar Ahmed

---

### **Installation Instructions**
1. Open the `.ino` file in Arduino IDE.
2. Select **AI Thinker ESP32-CAM** as your board.
3. Install required libraries: `LiquidCrystal_I2C`, `ESP32Servo`, and `Adafruit_VL53L0X`.
4. **Important:** Disconnect the wire from GPIO 0 during upload to permit flashing.
