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
