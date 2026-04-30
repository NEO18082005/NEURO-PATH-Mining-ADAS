# AG~3 NEURO-PATH: Intelligent Blindspot & Trajectory System

**Neuro-Path** is a sophisticated sensor-fusion safety system designed for blindspot monitoring. This repository contains the core firmware for the **Sensory Fusion Module** and an optional **Standalone Surveillance Module**.

---

## 🛠 Project Architecture

The system is now split into two distinct functional blocks:
1. **Primary Sensor Fusion (V8.2):** Handles Laser ToF, Ultrasonic data, and real-time cockpit feedback.
2. **Optional Surveillance Module:** A dedicated ESP32-CAM board running a high-bandwidth video server.

---

## 📋 Component List & Specifications

| Component | Quantity | Role in Neuro-Path |
| :--- | :--- | :--- |
| **ESP32-CAM** | 1 (or 2) | Core Processor for Logic & Vision |
| **VL53L0X Laser ToF** | 1 | High-precision distance measurement |
| **HC-SR04 Ultrasonic** | 1 | Wide-angle proximity detection |
| **I2C LCD (16x2)** | 1 | Real-time Cockpit Telemetry |
| **SG90 Servo Motor** | 1 | 180° Radar Sweep Actuator |
| **Active Buzzer** | 1 | Audio Alert (Movement Gated) |
| **LED Array (R, Y, G)** | 3 | Visual Status Indicators |
| **10kΩ Resistors** | 2 | Pull-down resistors for Signal Stability |
| **RC Vehicle Interface** | 1 | Forward/Backward Signal Input |

---

## 🔌 Technical Pin Mapping (Sensory Fusion Board)

> **Note:** Red LED and Buzzer are strictly gated by the vehicle's motion state. They will remain silent when the vehicle is stationary to prevent operator fatigue.

| Component | Pin | Notes |
| :--- | :--- | :--- |
| **Laser & LCD (SDA)** | GPIO 14 | Shared I2C Bus |
| **Laser & LCD (SCL)** | GPIO 15 | Shared I2C Bus |
| **RC Forward (F)** | GPIO 13 | High = Moving |
| **RC Backward (B)** | GPIO 12 | High = Moving |
| **Ultrasonic Trig** | GPIO 0 | Proximity Trigger |
| **Ultrasonic Echo** | GPIO 3 | Proximity Echo |
| **Servo Signal** | GPIO 16 | PWM Radar Sweep |
| **Red LED + Buzzer**| GPIO 1 | Alert (Active only when Moving) |
| **Yellow LED** | GPIO 4 | Caution/Slow Zone |
| **Green LED** | GPIO 2 | System Clear |

---

## 🚀 Installation & Boot Sequence

### **1. Firmware Upload**
Flash the `NEURO-PATH_V8.2.ino` to your primary ESP32-CAM. 
* **Critical:** Unplug the wire from GPIO 0 during upload to avoid bootloader conflicts.

### **2. System Initialization**
Upon power-up, the system executes the following:
* **Screen 1 (3s):** `NEURO-PATH BLINDSPOT SYS`
* **Screen 2 (1s):** `SYSTEM BOOT...` (Sensor Calibration)

### **3. Operational Logic**
* **🟢 Status Clear:** No objects detected. Green LED Active.
* **🟡 Caution Slow:** Object in medium range (15cm - 35cm). Yellow LED Active. If moving, pulsed audio warning.
* **🔴 Critical Alert:** Object < 15cm. Red LED Active. If moving, continuous high-frequency audio alert.

---

## 🎥 Optional: Standalone Camera Server
For users adding the second ESP32-CAM for live recording:
1. Flash the standard `CameraWebServer` example.
2. Mount the second board at a high vantage point on the vehicle.
3. Access the live feed via the local IP displayed in the Serial Monitor.

---

## 📜 Authors
* **Subhan Khan** - *Lead Developer & BCA Student* - [K.K. Modi University]
