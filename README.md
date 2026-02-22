# Blind-Spot-Safety-System

# Project: ADAS Blind Spot Safety System (Prototype Phase)
**Goal:** To eliminate heavy vehicle "No-Zones" using a multi-modal sensor fusion approach on a cardboard RC truck chassis.

---

## 1. Problem & Solution Overview

* **The Problem:** Large trucks have massive blind spots (No-Zones) where cyclists and pedestrians become invisible to the driver.
* **The Solution:** A "Sensor Fusion" module providing 180° spatial awareness via Ultrasonic Radar, Precision Laser (ToF), and Live Video Streaming.

---

## 2. Component List & Functional Roles

### A. Central Processing (The Brain)
* **ESP32-CAM-MB:** Handles the Wi-Fi web server for live video and processes all logic.
* **Micro-USB Shield:** Provides stable power and a direct interface for code uploading.

### B. Detection Suite (The Eyes)
* **HC-SR04 (Ultrasonic):** Scans a 180° arc to detect the general presence of objects.
* **VL53L0X (Laser ToF):** Provides pinpoint millimetric accuracy for critical "Stop" zones.
* **SG90 Servo Motor:** Rotates the sensors to create a dynamic "Radar" sweep.

### C. Driver Interface (Alerts)
* **16x2 I2C LCD:** Displays real-time distance readouts in centimeters.
* **Active Piezo Buzzer:** Provides high-frequency audible alerts, synchronized with the Red LED.
* **Traffic Light LEDs:** * **Green (Safe):** Clear path (> 50cm).
    * **Yellow (Caution):** Approaching hazard (20cm - 50cm).
    * **Red (Danger):** Immediate collision risk (< 20cm).

---

## 3. Wiring & Connection Strategy



### Master Pin Mapping Table
| Component        | Pin Name | ESP32-CAM Pin | Notes                                   |
|------------------|----------|---------------|-----------------------------------------|
| I2C Bus (LCD/Laser)| SDA      | GPIO 14       | Shared Data line           |
| I2C Bus (LCD/Laser)| SCL      | GPIO 15       | Shared Clock line          |
| Ultrasonic Sensor| Trig     | GPIO 12       | Trigger pulse              |
| Ultrasonic Sensor| Echo     | GPIO 16       | Pulse return (U2RX)        |
| Servo Motor      | Signal   | GPIO 13       | Sweep control              |
| Green LED        | Anode(+) | GPIO 2        | Status Indicator           |
| Yellow LED       | Anode(+) | GPIO 14       | Shared with SDA (470Ω resistor)|
| Red LED/Buzzer   | Anode(+) | GPIO 15       | Shared with SCL (470Ω resistor)|

### Power & Stabilization
* **Capacitor (100µF - 220µF):** Must be placed across the 5V and GND rails to prevent "Brownout" resets during servo movement.
* **Resistors (220Ω - 470Ω):** Used with all LEDs to protect pins and prevent I2C signal interference.

---

## 4. Working Workflow
1. **Sweep:** The Servo moves 1°, triggering the HC-SR04 and VL53L0X.
2. **Process:** ESP32-CAM calculates distance and updates the LCD.
3. **Alert:** If distance < 20cm, Red LED flashes and Buzzer sounds.
4. **Stream:** Live video is sent to the dashboard via local Wi-Fi.



# 🛒 Master Component List: Blind Spot Safety System

## 1. Core Computing & Power
| Component | Quantity | Role | Notes |
| :--- | :---: | :--- | :--- |
| **ESP32-CAM-MB** | 1 | Main Controller | Includes the built-in USB/Serial shield. |
| **Micro-USB Cable** | 1 | Power & Data | Connects the ESP to your laptop. |
| **9V Battery Snap to DC Jack**| 1 | Portable Power | Connects a 9V battery to the power module. |
| **MB102 Power Supply Module**| 1 | Voltage Regulator | Safely drops 9V battery to 5V for the breadboard. |
| **9V HW Battery** | 2 | Power Source | Keep 1 extra; they drain fast during camera use. |

## 2. Sensors & Actuators (The Hardware)
| Component | Quantity | Role | Notes |
| :--- | :---: | :--- | :--- |
| **HC-SR04 Ultrasonic** | 1 | Radar Sweep | Scans 180° for general obstacles. |
| **VL53L0X ToF Sensor** | 1 | Precision Laser | For critical <20cm "Stop" zone detection. |
| **SG90 Servo Motor** | 1 | Motion | Rotates the sensors back and forth. |
| **16x2 I2C LCD Display** | 1 | Dashboard | Shows real-time distance and status text. |

## 3. Visual & Audio Indicators
| Component | Quantity | Role | Notes |
| :--- | :---: | :--- | :--- |
| **Green LED (5mm)** | 2 | "SAFE" Light | Extra included in case one fails. |
| **Yellow LED (5mm)** | 2 | "CAUTION" Light | Extra included. |
| **Red LED (5mm)** | 3 | "DANGER" Light | These fail most due to buzzer load. |
| **Active Piezo Buzzer** | 1 | Audible Alarm | Triggers with the Red LED. |

## 4. Discrete Components (The Small Parts)
| Component | Quantity | Value | Notes |
| :--- | :---: | :--- | :--- |
| **Resistors** | 5 | 220Ω | For Green and Yellow LEDs. |
| **Resistors** | 5 | 470Ω | For Red LED (protects I2C pins 14/15). |
| **Electrolytic Capacitor** | 2 | 100µF or 220µF| Stabilizes power for the Servo/Camera. |
| **Jumper Wires (M-to-M)** | 1 Pack | Connection | For breadboard-to-breadboard links. |
| **Jumper Wires (M-to-F)** | 1 Pack | Connection | For connecting sensors to the ESP pins. |
| **Full-Size Breadboard** | 1 | Base | The 830-point board is best for this project. |
