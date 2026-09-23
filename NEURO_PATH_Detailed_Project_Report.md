# PROJECT REPORT  
## NEURO-PATH  
### A Zero-Cloud, Weather-Immune Proximity Detection and Driver-Assistance System for Open-Cast Mine Vehicles

---

## Smart India Hackathon 2026

**Problem Statement ID:** SIH26007  
**Problem Statement:** Safe and Efficient Operation of Mine Vehicles in Low-Visibility Conditions in Open-Cast Iron Ore Mines  
**Theme:** Smart Automation / Hardware  
**Category:** Hardware  
**Team ID:** 122928  
**Team Name:** THYNK UNIQUE  
**Team Lead:** Subhan Khan  
**Target Deployment Site:** NMDC Bailadila Iron Ore Mines  

---

## Document Status

| Field | Description |
|---|---|
| Document Type | Technical Project Report |
| Project Classification | Industrial Safety Hardware / Edge-AI ADAS |
| Intended Audience | Ministry of Steel, NMDC, DGMS stakeholders, SIH evaluation panel, mining safety officers and fleet managers |
| Prototype Maturity | Functional research prototype with industrialization roadmap |
| Deployment Positioning | Retrofit-ready proximity detection and driver-assistance subsystem |
| Data Connectivity | 100% offline during safety-critical operation |
| Primary Design Objective | Detect dynamic blind-spot threats under fog, dust and degraded optical visibility |

> **Important qualification:** NEURO-PATH is presented as a prototype and safety-support system. Final certification, mine approval, functional-safety assessment, electromagnetic compatibility testing and field validation must be completed before production deployment or use as a sole safety control.

---

# Executive Summary

Open-cast iron ore mines operate large trackless transportation machinery in environments characterized by heavy dust, monsoon fog, vibration, poor illumination, complex terrain and restricted driver visibility. During severe conditions, effective optical visibility can fall to approximately **3–5 m**, substantially reducing the operator’s ability to identify pedestrians, light vehicles, berms, equipment and other hazards around a dumper.

Conventional camera-based systems and optical LiDAR are vulnerable because they depend on photons travelling through the scene and returning with sufficient contrast. Water droplets, suspended iron-ore dust, mud contamination and low illumination can significantly reduce image quality and ranging reliability. At the same time, many conventional proximity systems produce repeated warnings against static quarry walls, berms and fixed infrastructure. The resulting nuisance alarms create sensory fatigue and encourage operators to mute, ignore or bypass the system.

**NEURO-PATH**, developed by **THYNK UNIQUE**, addresses this problem through a four-zone, multi-sensor architecture:

1. **Zone 1 — Physical Sensing:**  
   A 24 GHz mmWave radar provides motion and distance-related information, while three IP67 ultrasonic sensors map close-range blind-spot regions.

2. **Zone 2 — Deterministic Edge Core:**  
   An ESP32-WROOM-32 executes bare-metal C++ and FreeRTOS-based sensor scheduling, fusion, alert generation and fail-safe logging.

3. **Zone 3 — Edge-AI Vision:**  
   A Poco F1 platform using the Qualcomm Snapdragon 845 executes YOLOv8-Nano through NCNN for optical corroboration when visual information remains usable.

4. **Zone 4 — Cockpit Human-Machine Interface:**  
   A 20×4 I²C LCD, WS2812B directional LED ring and PWM-controlled cabin siren provide immediate, location-specific driver feedback.

The system’s central innovation is **motion-gated logic**. Instead of treating every detected object as an imminent hazard, NEURO-PATH evaluates whether the sensed return exhibits a dynamic signature. Stationary quarry walls and berms can therefore be suppressed, while moving pedestrians, vehicles or equipment are escalated for driver warning. The design target is a **greater than 85% reduction in nuisance alarms**, subject to validation under representative mine conditions.

A second innovation is the **50 ms sequential polling architecture**. The radar and three ultrasonic channels are activated in separate time slots over a **200 ms complete sensing cycle**. Only one acoustic transducer is triggered at a time, allowing echoes to decay before the next measurement and preventing cross-talk between adjacent ultrasonic sensors.

The proposed industrial retrofit cost is estimated at approximately **₹26,500 per machine**, compared with **₹1.5 lakh or more** for many OEM camera-based or multi-camera ADAS packages. The architecture operates without cellular coverage, cloud subscriptions or remote inference. This makes it suitable for deep-pit mining environments where connectivity is intermittent, unavailable or operationally undesirable.

---

# 1. Project Identity and Vision

## 1.1 Project Name

# NEURO-PATH

### Suggested tagline

> **Turning Monsoon Fog Into Zero-Downtime Operations**

### Safety proposition

> **A 100% offline, weather-immune deterministic ADAS that uses multi-sensor fusion to isolate dynamic threats with millimetric-scale ranging resolution, while eliminating cloud latency and bandwidth dependence.**

The phrase “millimetric-scale” refers to the intended resolution of individual sensor measurements and not to a guarantee of millimetre-level absolute positioning of pedestrians or vehicles in all field conditions. Absolute performance must be established through mine-specific calibration and validation.

---

## 1.2 Project Objective

The objective of NEURO-PATH is to design and validate a retrofit-compatible proximity detection and driver-assistance system for open-cast mine vehicles that:

- Maintains hazard awareness during **3–5 m optical visibility conditions**.
- Detects dynamic threats in vehicle blind spots.
- Suppresses static environmental geometry such as quarry walls and berms.
- Provides directional warnings without requiring cellular connectivity.
- Operates with predictable, deterministic timing.
- Reduces nuisance alarms and operator sensory fatigue.
- Uses commercially available components for supply-chain readiness.
- Supports low-cost deployment across legacy mine fleets.
- Creates a black-box event log for post-incident review and system auditing.

---

## 1.3 Target Machine

The first deployment target is a heavy mine dumper or trackless transportation machine operating in:

- Open-cast iron ore mining areas.
- Haul roads and loading zones.
- Dumping and reversing areas.
- Stockyards and crushing plant approaches.
- Narrow corridors and restricted manoeuvring areas.
- Monsoon fog and high-particulate dust conditions.

The system is designed as an **assistive safety layer**. It is not intended to replace:

- Competent operator control.
- Mine traffic-management plans.
- Trained banksmen or spotters.
- Physical exclusion zones.
- Machine interlocks.
- Emergency stop systems.
- Standard operating procedures.
- Legally mandated proximity detection controls.

---

# 2. Problem Definition

## 2.1 Operational Context

Open-cast mining vehicles are large, heavy and difficult to manoeuvre. The driver’s direct field of view can be obstructed by:

- Engine compartments.
- Dump bodies.
- Counterweights.
- Chassis structures.
- Wheel arches.
- Cabin pillars.
- Payload geometry.
- Dust and water accumulation on mirrors or cameras.
- Terrain discontinuities and berms.

The risk becomes more severe when a machine reverses, turns in a loading area or approaches a pedestrian, service vehicle or light vehicle. The driver may have insufficient time to perceive and react to a person located in a blind spot.

---

## 2.2 Low-Visibility Failure Mode

The project materials identify a practical optical degradation threshold of approximately **3–5 m** in dense monsoon fog and iron-ore dust.

Under such conditions:

- Camera contrast is reduced.
- Backscatter produces a bright veil across the image.
- Lens contamination reduces sharpness.
- Dust particles scatter visible and near-infrared light.
- LiDAR returns become less reliable due to particulate reflections and attenuation.
- The operator’s natural visual scan becomes ineffective beyond a short range.
- Static and dynamic objects become difficult to distinguish visually.

This results in a dangerous combination:

1. The driver cannot reliably see the blind-spot region.
2. Camera-based systems may generate degraded or ambiguous detections.
3. Existing proximity systems may warn continuously against static walls.
4. The operator becomes desensitized to the warning.
5. The system is muted or ignored.
6. A real dynamic hazard may not receive the required attention.

---

## 2.3 Nuisance Alarm and Sensory Fatigue

A proximity detection system is only effective if the operator trusts and responds to its alerts.

Repeated warnings caused by static objects can lead to:

- Alarm habituation.
- Reduced perceived credibility.
- Delayed reaction.
- Warning muting.
- Operator override.
- Increased cognitive workload.
- Failure to distinguish a genuine threat from environmental clutter.

NEURO-PATH therefore treats **alarm quality** as a safety requirement, not merely a user-interface feature.

The system is designed to distinguish between:

| Object Type | Expected Behaviour | NEURO-PATH Response |
|---|---|---|
| Quarry wall | Stationary radar return | Suppress or display as static boundary |
| Berm | Stationary or slowly varying geometry | Suppress unless movement signature appears |
| Pedestrian | Dynamic, irregular movement | Escalate |
| Light vehicle | Dynamic movement | Escalate |
| Moving dumper | Strong dynamic return | Escalate |
| Excavator arm | Moving equipment component | Escalate according to zone logic |
| Dust plume | Weak, unstable or non-coherent return | Filter using persistence and fusion logic |

---

# 3. Proposed Solution

## 3.1 System Concept

NEURO-PATH is a locally processed, multi-sensor blind-spot detection system. It combines:

- **Radio-frequency sensing** for motion and environmental penetration.
- **Acoustic ranging** for short-range zone measurement.
- **Optical AI** for object-class corroboration when useful.
- **Deterministic rule logic** for safety-critical warning decisions.
- **Directional cockpit cueing** for immediate human response.
- **Local event logging** for traceability without cellular networks.

The system does not rely on a central cloud server. Critical threat decisions are made directly on the vehicle.

---

## 3.2 Design Principles

### 3.2.1 Local-first safety

Safety-critical data must not depend on an external network, remote server or cloud inference pipeline.

### 3.2.2 Deterministic response

Sensor acquisition, filtering and alert generation must follow a known timing schedule.

### 3.2.3 Sensor complementarity

No single sensor is assumed to be reliable in all conditions. Radar, ultrasound and vision are used for different physical strengths.

### 3.2.4 Human-centred warning

The cockpit should provide a clear direction and severity indication rather than an undifferentiated alarm.

### 3.2.5 Fail-safe degradation

If a sensor fails, becomes disconnected or produces implausible data, the system must indicate degraded operation rather than silently presenting a false “safe” state.

### 3.2.6 Retrofit practicality

The solution must be installable on legacy machines without extensive modification to the vehicle’s hydraulic, braking or engine-control systems.

---

# 4. NEURO-PATH Four-Zone Architecture

## 4.1 System-Level Data Flow

```text
Physical Environment
        │
        ▼
Zone 1: Radar + Ultrasonic Sensors
        │
        ▼
Zone 2: ESP32 Deterministic Sensor Fusion
        │
        ├── Threat flags and telemetry ───► Zone 3: Snapdragon 845 / YOLOv8-Nano
        │
        ▼
Zone 4: LCD + Directional LED Ring + Cabin Siren
        │
        ▼
Driver Decision and Corrective Manoeuvre
```

[INSERT COMPLETE NEURO-PATH SYSTEM ARCHITECTURE DIAGRAM HERE]

---

## 4.2 Zone 1 — Physical Sensors

### 4.2.1 HLK-LD2410C 24 GHz mmWave Radar

The HLK-LD2410C is used as the primary motion-sensitive RF channel.

#### Intended functions

- Detect movement in the monitored region.
- Provide presence and motion-related information.
- Support Doppler-based dynamic/static discrimination.
- Operate during fog, dust and poor illumination.
- Provide a sensor channel independent of optical contrast.

#### Why 24 GHz radar is suitable

A 24 GHz signal has a wavelength of approximately:

\[
\lambda = \frac{c}{f}
\]

where:

- \(c\) is the speed of light, approximately \(3 \times 10^8\) m/s.
- \(f = 24 \times 10^9\) Hz.

Therefore:

\[
\lambda \approx 12.5 \text{ mm}
\]

This wavelength is substantially larger than visible-light wavelengths and is less strongly affected by many suspended dust particles and water droplets found in fog. Individual particulate particles may still create scattering, but the overall propagation behaviour is more robust than a photon-dependent camera or optical LiDAR system in dense particulate environments.

#### Doppler relationship

For a moving target, the Doppler frequency shift can be approximated as:

\[
f_d = \frac{2v}{\lambda}
\]

where:

- \(f_d\) is the Doppler shift.
- \(v\) is the radial velocity.
- \(\lambda\) is the radar wavelength.

For a 24 GHz radar:

\[
f_d \approx \frac{2v}{0.0125}
\]

Thus, a target moving radially relative to the sensor produces a measurable frequency displacement. Static quarry walls have approximately:

\[
v \approx 0 \Rightarrow f_d \approx 0
\]

This physical distinction is the basis of the motion-gated alarm filter.

#### Engineering limitation

The radar does not inherently provide complete semantic understanding of every object. It may detect:

- Moving people.
- Moving vehicles.
- Rotating or oscillating machinery.
- Moving dust or vegetation.
- Vehicle vibration artefacts.

Consequently, radar output is fused with temporal filtering, ultrasonic distance information and optional optical AI confirmation.

---

### 4.2.2 A02YYUW IP67 Ultrasonic Arrays

Three A02YYUW IP67 ultrasonic sensors are used to support close-range blind-spot mapping.

#### Proposed placement

| Sensor | Indicative Region |
|---|---|
| Ultrasonic Left | Left-side blind spot |
| Ultrasonic Right | Right-side blind spot |
| Ultrasonic Rear | Rear/reversing blind spot |

The final mounting geometry should be established using the dumper’s dimensions, turning envelope, wheel sweep, body articulation and minimum pedestrian exclusion distance.

#### Functions

- Measure short-range distance to nearby objects.
- Provide zone-specific proximity information.
- Support detection in areas where radar returns may be ambiguous.
- Provide close-range confirmation of an object near the vehicle body.
- Provide redundancy for near-field threat detection.

#### Acoustic ranging principle

For an ultrasonic pulse:

\[
d = \frac{v_s \cdot t}{2}
\]

where:

- \(d\) is the distance to the reflecting object.
- \(v_s\) is the speed of sound in air.
- \(t\) is the round-trip time.
- The factor of 2 accounts for outbound and return propagation.

The speed of sound varies with temperature, humidity and atmospheric conditions. The production system should therefore include temperature compensation or conservative detection margins.

#### Environmental limitations

Ultrasonic performance may degrade due to:

- Heavy rain.
- Strong wind.
- Uneven or absorbent surfaces.
- Mud contamination.
- Large angular surfaces.
- Acoustic noise.
- Temperature variation.
- Cross-talk between adjacent transducers.

NEURO-PATH addresses the cross-talk risk through sequential triggering.

---

## 4.3 Zone 2 — Deterministic Edge Core

### 4.3.1 ESP32-WROOM-32

The ESP32-WROOM-32 acts as the central real-time controller.

#### Responsibilities

- Schedule the radar and ultrasonic sensors.
- Read UART, trigger/echo and digital sensor outputs.
- Apply range validation and plausibility checks.
- Execute motion-gated logic.
- Fuse sensor status and threat flags.
- Control the LCD, LED ring and siren.
- Transmit telemetry to the Snapdragon 845 node.
- Store black-box event records in local flash.
- Detect sensor faults and communication failures.
- Maintain deterministic timing.

### 4.3.2 Software execution model

The ESP32 software is divided into functional tasks:

| Task | Function |
|---|---|
| Sensor Scheduler | Controls acquisition timing |
| Radar Parser | Extracts radar presence and motion information |
| Ultrasonic Driver | Controls one transducer at a time |
| Signal Validator | Rejects invalid, stale or impossible readings |
| Motion Gate | Determines dynamic versus static return |
| Fusion Engine | Combines radar, acoustic and visual flags |
| HMI Controller | Produces directional warnings |
| Logger | Stores event and diagnostic records |
| Watchdog Supervisor | Detects task lock-up or timing failure |

The safety-critical warning path should remain independent of non-essential functions such as extended telemetry or diagnostic display.

---

## 4.4 Zone 3 — Edge-AI Vision

### 4.4.1 Snapdragon 845 / Poco F1 platform

The Poco F1 provides a practical prototype platform based on the Qualcomm Snapdragon 845 system-on-chip.

YOLOv8-Nano is executed natively through NCNN, an optimized neural-network inference framework suitable for embedded deployment.

#### Intended vision functions

- Detect pedestrians.
- Detect vehicles.
- Detect selected machinery classes.
- Provide optical corroboration for radar events.
- Improve semantic interpretation during acceptable visibility.
- Support data collection during pilot validation.

#### Vision is corroborative, not exclusive

The optical channel is deliberately not the only detection source. During dense fog or dust:

- YOLO confidence may decline.
- Bounding boxes may become unstable.
- Frame quality may be insufficient.
- Object classification may fail.

The radar and ultrasonic layers therefore continue to provide the primary weather-resilient proximity function.

---

## 4.5 Zone 4 — Cockpit HMI

### 4.5.1 20×4 I²C LCD

The LCD provides textual status and diagnostic information.

Indicative display states:

```text
NEURO-PATH ACTIVE
RADAR: OK  SONAR: OK
ZONE: LEFT  2.4 m
THREAT: MOVING
```

During normal operation:

```text
SYSTEM STATUS: SAFE
RADAR / SONAR READY
VISION: CORROBORATION
```

During degraded operation:

```text
SENSOR FAULT
RIGHT ULTRASONIC OFF
SERVICE REQUIRED
```

### 4.5.2 WS2812B directional LED ring

The LED ring provides spatially intuitive warning cues.

| Colour | Meaning |
|---|---|
| Green | No active threat |
| Blue | System active / monitoring |
| Yellow | Caution or near-field static object |
| Orange | Dynamic object detected |
| Red | Critical proximity threat |
| Flashing red segment | Directional emergency hazard |

The ring should be physically oriented so that the illuminated quadrant corresponds to the actual left, right or rear vehicle zone.

### 4.5.3 PWM cabin siren

The PWM siren provides audible warning priority.

Suggested alert strategy:

- Slow intermittent tone: caution.
- Faster intermittent tone: dynamic object detected.
- Continuous or high-duty-cycle tone: critical proximity.
- Distinct fault tone: sensor or communication failure.

The acoustic warning should be calibrated to remain audible inside the cabin while avoiding excessive continuous exposure that could itself contribute to alarm fatigue.

---

# 5. Sensor Physics and Environmental Performance

## 5.1 Why Cameras and LiDAR Degrade in Mining Conditions

Optical systems operate using visible or near-infrared photons. In dust and fog, the received signal is affected by:

### Absorption

Some energy is absorbed by water droplets or particulate matter.

### Scattering

Particles redirect photons away from the receiver. This reduces contrast and range.

### Backscatter

Light is reflected by particles between the sensor and target, producing a bright haze. This is particularly damaging to cameras and LiDAR because the sensor receives a strong return from the intervening medium.

### Lens contamination

Water droplets and iron-ore dust on the lens create:

- Blurring.
- Reduced contrast.
- False edges.
- Partial occlusion.
- Incorrect object segmentation.

### Low illumination

Fog and dust often occur with low-light conditions, further reducing camera performance.

---

## 5.2 Why 24 GHz mmWave Radar is More Robust

Radar uses electromagnetic waves at a much longer wavelength than visible light. The interaction with small particles depends strongly on the ratio between particle size and wavelength.

When particles are much smaller than the wavelength, scattering is generally less severe than in the optical regime. A 24 GHz wavelength is approximately **12.5 mm**, while many airborne dust particles and fog droplets are considerably smaller.

This does not mean radar is completely unaffected. Heavy rain, large wet particles, multipath, machine vibration and antenna contamination may still impact performance. However, radar does not require:

- Visible contrast.
- Texture.
- Illumination.
- A clean optical lens.
- A high-quality image.

Radar can therefore maintain a useful motion channel when camera and LiDAR performance is degraded.

---

## 5.3 Complementary Sensor Behaviour

| Condition | Camera / YOLO | Radar | Ultrasonic |
|---|---:|---:|---:|
| Clear daylight | Strong | Strong | Strong near-field |
| Dense fog | Degraded | Stronger relative performance | Moderate |
| Iron-ore dust | Degraded | Stronger relative performance | Moderate |
| Complete darkness | Weak without IR | Strong | Strong near-field |
| Static wall | Detectable but nuisance-prone | Static return | Detectable |
| Moving pedestrian | Potentially strong if visible | Dynamic return | Near-field confirmation |
| Mud-covered lens | Severe degradation | Less affected if radome clean | Sensor-face dependent |
| Very close obstacle | Variable | Useful | Strong near-field |

The architecture intentionally avoids depending on a single sensing modality.

---

# 6. Proprietary Logic Innovation I: Motion-Gated Logic

## 6.1 Design Problem

A conventional proximity alarm may trigger whenever an object enters a distance threshold:

\[
d < d_{\text{alarm}}
\]

This rule is insufficient in a quarry because static objects are common. A machine may operate close to:

- Quarry walls.
- Stockpiles.
- Berms.
- Haul-road edges.
- Fixed plant structures.
- Loading faces.
- Dumping points.

If every stationary object produces a full alarm, the operator will receive frequent warnings even when the object presents no immediate dynamic collision risk.

---

## 6.2 Motion-Gated Decision Principle

NEURO-PATH extends distance-based logic with a motion criterion:

\[
A = D \land M \land P
\]

where:

- \(A\) = active critical alarm.
- \(D\) = object is inside the configured danger zone.
- \(M\) = object exhibits a dynamic or closing-motion signature.
- \(P\) = signal passes persistence and plausibility checks.

A more complete score may be represented as:

\[
S = w_d D_s + w_v V_s + w_p P_s + w_c C_s
\]

where:

- \(D_s\) = normalized distance risk.
- \(V_s\) = velocity or Doppler risk.
- \(P_s\) = persistence score.
- \(C_s\) = cross-sensor corroboration score.
- \(w_d,w_v,w_p,w_c\) = configurable weights.

An alarm is generated only when:

\[
S \geq S_{\text{alarm}}
\]

---

## 6.3 Static-Object Suppression

A static object is not necessarily ignored completely. Instead, it is classified as a lower-priority environmental condition.

Example:

```text
Radar presence detected
        │
        ▼
Doppler / motion check
        │
   ┌────┴────┐
   │         │
Static     Moving
   │         │
Low-level   Check distance,
status      persistence and fusion
   │         │
No siren    Directional warning
```

This avoids the unsafe design choice of blindly suppressing all radar returns. A slowly moving or approaching object must still be escalated.

---

## 6.4 Proposed Motion-Gated States

| State | Description | Driver Output |
|---|---|---|
| M0 | No valid target | Green ring |
| M1 | Static object outside warning zone | No alarm |
| M2 | Static object inside caution zone | Yellow directional indication |
| M3 | Ambiguous movement | Orange indication and intermittent tone |
| M4 | Confirmed moving object | Red directional warning |
| M5 | Critical closing threat | Flashing red ring and high-priority siren |
| MF | Sensor fault or invalid data | Fault display and maintenance alert |

---

## 6.5 False-Alarm Reduction Target

The project targets a **greater than 85% reduction in nuisance alarms** relative to an unfiltered proximity threshold system.

This value should be treated as a design target until supported by:

- Controlled static-wall tests.
- Berm and stockpile tests.
- Moving-person tests.
- Moving-vehicle tests.
- Dust and fog trials.
- Repeated operator-in-the-loop evaluations.
- Long-duration fleet logging.

Recommended evaluation metric:

\[
\text{False Alarm Reduction} =
\left(
1-\frac{F_{\text{motion-gated}}}{F_{\text{baseline}}}
\right)\times100
\]

where:

- \(F_{\text{baseline}}\) is the number of nuisance alarms using distance-only logic.
- \(F_{\text{motion-gated}}\) is the number after Doppler and persistence filtering.

[INSERT MOTION-GATED LOGIC FLOWCHART HERE]

---

# 7. Proprietary Logic Innovation II: 50 ms Sequential Polling

## 7.1 Cross-Talk Problem

Three ultrasonic sensors mounted on the same vehicle can interfere with one another if they transmit simultaneously or too closely in time.

Potential outcomes include:

- One sensor receiving another sensor’s echo.
- False short-range readings.
- False long-range readings.
- Inconsistent distance measurements.
- Oscillating alarm states.
- Incorrect threat-zone assignment.

This is particularly important around walls and narrow corridors where reflections may persist.

---

## 7.2 Sequential Polling Schedule

The system assigns one 50 ms slot to each sensing action.

| Slot | Time Window | Sensor Action |
|---|---:|---|
| T0 | 0–50 ms | 24 GHz radar acquisition / update |
| T1 | 50–100 ms | Left ultrasonic trigger and echo capture |
| T2 | 100–150 ms | Right ultrasonic trigger and echo capture |
| T3 | 150–200 ms | Rear ultrasonic trigger and echo capture |
| Cycle End | 200 ms | Fusion, HMI update and next cycle |

The full cycle is:

\[
T_{\text{cycle}} = 4 \times 50\text{ ms}=200\text{ ms}
\]

The effective complete update rate is therefore:

\[
f_{\text{cycle}}=\frac{1}{0.2}=5\text{ Hz}
\]

The acoustic channels are never commanded to transmit simultaneously.

---

## 7.3 Echo Decay and Maximum Range

For an ultrasonic sensor, the time available for echo reception must be consistent with the maximum configured range.

\[
t_{\text{echo}} = \frac{2d_{\max}}{v_s}
\]

For example, at a nominal \(d_{\max}\) of 4 m and \(v_s \approx 343\text{ m/s}\):

\[
t_{\text{echo}}\approx \frac{8}{343}\approx23.3\text{ ms}
\]

A 50 ms slot provides a conservative timing envelope for:

- Pulse emission.
- Echo reception.
- Timeout handling.
- Sensor reset.
- Data validation.
- Scheduling overhead.

The exact timing must be confirmed against the final sensor datasheet, enclosure acoustics, mounting geometry and expected environmental conditions.

---

## 7.4 Zero Cross-Talk Design Intent

The design target is **zero cross-talk under validated operating conditions**.

This means:

- One ultrasonic transducer emits at a time.
- The controller waits for the configured reception window.
- Echo data is associated with the active sensor only.
- Timeout and invalid-echo states are recorded.
- The next transducer is activated only after the prior slot is complete.

[INSERT 50 ms SEQUENTIAL POLLING TIMING DIAGRAM HERE]

---

## 7.5 Polling Pseudocode

```cpp
void sensing_cycle()
{
    radar_update();
    validate_radar();

    ultrasonic_read(LEFT);
    validate_ultrasonic(LEFT);

    ultrasonic_read(RIGHT);
    validate_ultrasonic(RIGHT);

    ultrasonic_read(REAR);
    validate_ultrasonic(REAR);

    motion_gate();
    sensor_fusion();
    update_hmi();
    log_event_if_required();

    watchdog_feed();
}
```

A production implementation should use hardware timers, non-blocking state machines and bounded execution windows wherever possible.

---

# 8. Sensor Fusion and Threat Classification

## 8.1 Fusion Philosophy

NEURO-PATH does not require all sensors to agree perfectly. Instead, it assigns each channel a role:

- Radar: dynamic presence and velocity indication.
- Ultrasonic: near-field distance and quadrant confirmation.
- Vision: semantic corroboration.
- ESP32: deterministic decision authority.
- HMI: clear warning communication.

---

## 8.2 Example Fusion Cases

### Case A: Static quarry wall

```text
Radar: Presence detected
Doppler: Near zero
Ultrasonic: Distance stable
YOLO: No person or vehicle
Result: Static environmental object; no critical siren
```

### Case B: Moving pedestrian in left blind spot

```text
Radar: Dynamic return
Doppler: Non-zero
Left ultrasonic: Near-field object detected
YOLO: Pedestrian confidence positive or unavailable due to fog
Result: Left red LED segment + high-priority siren
```

### Case C: Vehicle approaching from rear

```text
Radar: Dynamic return
Rear ultrasonic: Distance decreasing
YOLO: Vehicle corroboration where visible
Result: Rear directional warning
```

### Case D: Vision-only detection in moderate visibility

```text
Radar: Weak or unavailable
Ultrasonic: Outside near-field range
YOLO: Pedestrian detected
Result: Caution or warning according to confidence and persistence
```

### Case E: Sensor fault

```text
Radar UART timeout
or
Ultrasonic echo invalid
or
Snapdragon link unavailable
Result: Fault status shown; safety logic enters degraded mode
```

---

## 8.3 Threat Priority Model

| Priority | Condition | Action |
|---|---|---|
| P0 | No threat | Normal operation |
| P1 | Static obstacle / boundary | Directional caution only |
| P2 | Ambiguous object within caution distance | Intermittent tone |
| P3 | Moving object within warning zone | Directional siren |
| P4 | Closing object at critical distance | Immediate high-priority alarm |
| PF | Sensor or controller fault | Fault alert and maintenance flag |

---

# 9. Zero-Cloud Architecture

## 9.1 Architecture Description

NEURO-PATH processes safety-critical spatial data locally on:

- Snapdragon 845 edge-AI hardware.
- ESP32 deterministic control hardware.
- Local flash storage for event records.

No cloud service is required for:

- Sensor acquisition.
- Motion classification.
- Local threat decisions.
- Driver warning.
- Basic event logging.

This design is particularly valuable in deep-pit mine areas where cellular networks may be intermittent or unavailable.

---

## 9.2 Latency Elimination

A cloud-dependent architecture may require:

1. Sensor acquisition.
2. Network packet formation.
3. Uplink transmission.
4. Cloud processing.
5. Downlink transmission.
6. Local alert actuation.

This introduces variable latency and possible packet loss.

NEURO-PATH removes these network-dependent stages from the emergency path:

```text
Sensor → ESP32 → Local Decision → HMI
```

The local path is bounded by sensor acquisition, computation and actuator timing rather than network conditions.

The system does not claim that every complete decision occurs in sub-millisecond time. The correct engineering objective is **bounded, locally controlled latency**, with sensor-specific update timing explicitly characterized.

---

## 9.3 Academic Basis: Algorithmic Ecology

The architecture is scientifically supported by the project team’s cited paper:

> **“Algorithmic Ecology: A Framework for Achieving Carbon-Neutrality in Global Data Infrastructure”**  
> Authors cited in the project material: **Subhan Khan and Mohammed Bakhtawar Ahmed**  
> Publisher cited in the project material: **STM Journals, 2026**

The paper is used as the conceptual basis for decentralizing computation away from remote data centres and placing processing near the point of data generation.

The relevance to NEURO-PATH is threefold:

### 9.3.1 Reduced data movement

Spatial sensor data is not continuously uploaded to a remote server. This reduces:

- Network traffic.
- Wireless infrastructure dependence.
- Repeated data transmission.
- Energy spent moving high-frequency sensor streams.

### 9.3.2 Lower operational latency

Local inference eliminates the round-trip communication delay associated with remote processing.

### 9.3.3 Reduced infrastructure dependency

A vehicle can continue operating its proximity-warning function even when:

- Cellular coverage is absent.
- Backhaul infrastructure fails.
- Mine networks are congested.
- Cloud services are unavailable.
- Subscription services are interrupted.

The environmental claim should be presented as an **architectural energy-efficiency rationale**, not as a quantified carbon-offset certification. A complete carbon assessment would require measurements of:

- Device power consumption.
- Cloud server power consumption.
- Network energy per transmitted gigabyte.
- Operational duty cycle.
- Hardware manufacturing impacts.
- Maintenance and replacement cycles.

---

## 9.4 Data Governance and Cybersecurity

The zero-cloud architecture also reduces exposure to:

- Unauthorised remote access.
- External service outages.
- Cloud data retention risk.
- Connectivity-based attack surfaces.
- Vendor lock-in.

The system should still implement:

- Signed firmware.
- Secure boot where supported.
- Protected diagnostic ports.
- Role-based access to logs.
- Local log integrity checks.
- Controlled USB extraction.
- Tamper-evident enclosure seals.

---

# 10. Hardware Design and Industrialization

## 10.1 Prototype Hardware Stack

| Subsystem | Component |
|---|---|
| Radar | HLK-LD2410C 24 GHz mmWave radar |
| Ultrasonic sensing | Three A02YYUW IP67 ultrasonic sensors |
| Real-time controller | ESP32-WROOM-32 |
| Edge-AI processor | Poco F1 / Snapdragon 845 |
| Vision inference | YOLOv8-Nano via NCNN |
| Driver display | 20×4 I²C LCD |
| Directional warning | WS2812B LED ring |
| Audible warning | PWM cabin siren |
| Storage | Local flash / onboard storage |
| Communications | UART, I²C, GPIO, PWM |
| Enclosure | Industrial sealed housing |
| Vehicle interface | Protected DC power input |

---

## 10.2 Power Architecture

A production implementation should include:

- Automotive-grade DC-DC conversion.
- Reverse-polarity protection.
- Overvoltage protection.
- Undervoltage lockout.
- Surge suppression.
- Fuse protection.
- EMI filtering.
- Ignition-controlled startup.
- Controlled shutdown for the Snapdragon node.

The actual vehicle supply voltage must be verified for the target dumper. The prototype should not be connected directly to an unregulated heavy-vehicle electrical bus without suitable protection.

---

## 10.3 Mechanical Packaging

Recommended industrialization features include:

- IP67-rated sensor housings.
- Replaceable protective radar radome.
- Hydrophobic acoustic sensor membranes where compatible.
- M12 industrial connectors.
- Strain-relieved cable glands.
- Anti-vibration mounting brackets.
- Rubber or elastomeric isolation.
- Impact-resistant polycarbonate or aluminium enclosure.
- Drainage paths for water ingress prevention.
- Easy access for maintenance and sensor replacement.

---

## 10.4 Environmental Qualification Plan

Before pilot deployment, the system should be evaluated for:

| Test | Purpose |
|---|---|
| Vibration test | Validate mounting and connectors |
| Shock test | Evaluate impact resilience |
| Water ingress test | Confirm enclosure protection |
| Dust ingress test | Validate sealing and sensor surfaces |
| Thermal cycling | Check operation across temperature range |
| EMC/EMI test | Verify immunity and emissions |
| Power transient test | Validate vehicle supply protection |
| Mud contamination test | Evaluate sensor degradation |
| Fog chamber test | Establish optical performance threshold |
| Iron-ore dust test | Measure radar, acoustic and optical degradation |

The project deck refers to AEC-Q100 components and IP67 housings. These should be treated as procurement and industrialization goals unless each selected component is verified for the relevant qualification.

---

# 11. Compliance and Regulatory Alignment

## 11.1 DGMS Circular No. 03 of 2024

The project materials identify **DGMS (Tech) (SOMA) Circular No. 03 of 2024** as a relevant government source concerning accident prevention involving Trackless Transportation Machinery in opencast mines.

NEURO-PATH aligns with the stated safety intent by providing:

- Proximity detection.
- Blind-spot monitoring.
- Directional warning.
- Offline availability.
- Local event logging.
- Support for safer movement of trackless machinery.
- A retrofit pathway for legacy vehicles.

However, the presence of a prototype system must not be interpreted as automatic legal compliance. Final compliance should be evaluated by the mine operator and competent authorities against:

- The exact vehicle class.
- Applicable DGMS directions.
- Installation location.
- Detection zones.
- Warning and intervention requirements.
- Maintenance procedures.
- Inspection and certification obligations.
- Operator training.
- Failure response.

---

## 11.2 ISO 16001:2020

The project materials identify **ISO 16001:2020** as a relevant standard for earth-moving machinery, object detection systems and visibility aids.

NEURO-PATH is designed to support an engineering assessment against the standard’s relevant principles, including:

- Definition of hazard zones.
- Detection coverage.
- Object-detection performance.
- Warning-zone configuration.
- Testing methodology.
- Visibility-aid integration.
- Human-machine interface considerations.

The final product should be subjected to a formal compliance gap analysis covering:

1. Vehicle geometry.
2. Sensor field of view.
3. Detection probability.
4. False-positive and false-negative rates.
5. Environmental operating conditions.
6. Warning timing.
7. Driver response expectations.
8. Sensor failure behaviour.
9. Installation and calibration procedure.
10. Validation documentation.

---

## 11.3 Safety Integrity and Functional Safety

NEURO-PATH should not be described as a certified autonomous driving or functional-safety system at the prototype stage.

The industrialization roadmap should consider:

- Hazard and risk analysis.
- Safety requirements specification.
- Failure Mode and Effects Analysis.
- Fault-tree analysis.
- Diagnostic coverage.
- Watchdog supervision.
- Safe-state definition.
- Independent validation.
- Software configuration control.
- Verification and validation evidence.

Potential safety-related failure modes include:

- Radar stuck-at-presence.
- Radar silent failure.
- Ultrasonic echo stuck high or low.
- Sensor cable disconnection.
- False Doppler caused by machine vibration.
- Camera inference freeze.
- ESP32 task deadlock.
- HMI failure.
- Siren failure.
- Power supply brownout.
- Incorrect sensor orientation.
- Blocked or contaminated transducer.

---

# 12. Prototype Validation

## 12.1 Validation Objectives

The prototype validation programme should establish:

- Detection range.
- Directional coverage.
- Dynamic-object detection probability.
- Static-object suppression performance.
- Nuisance alarm rate.
- Ultrasonic cross-talk performance.
- End-to-end alert latency.
- Fault-detection behaviour.
- Fog and dust degradation characteristics.
- Operator usability.

---

## 12.2 Test Matrix

| Test Category | Test Scenario | Key Metric |
|---|---|---|
| Static geometry | Quarry wall, berm, stockpile | Nuisance alarms per hour |
| Dynamic pedestrian | Person crossing blind spot | Detection probability |
| Dynamic vehicle | Light vehicle approach | Detection probability and warning time |
| Rear reversing | Person behind dumper | Near-field detection |
| Fog | Visibility reduced to 3–5 m | Radar and ultrasonic stability |
| Dust | Iron-ore dust exposure | Signal degradation |
| Cross-talk | Multiple acoustic channels | Invalid echo rate |
| Vibration | Vehicle engine and movement | False motion rate |
| Power | Vehicle supply variation | Reset and recovery behaviour |
| Fault injection | Sensor disconnect | Fault indication time |
| HMI | Driver-in-the-loop test | Reaction time and comprehension |

---

## 12.3 Performance Metrics

### Detection probability

\[
P_D = \frac{N_{\text{detected threats}}}{N_{\text{actual threats}}}
\]

### False alarm rate

\[
FAR = \frac{N_{\text{nuisance alarms}}}{T_{\text{operating time}}}
\]

### Warning latency

\[
T_{\text{warning}} =
T_{\text{HMI actuation}} -
T_{\text{threat entering detection zone}}
\]

### Sensor availability

\[
A = \frac{T_{\text{valid operation}}}{T_{\text{scheduled operation}}}
\]

### Cross-talk rate

\[
C_r =
\frac{N_{\text{cross-talk-corrupted readings}}}
{N_{\text{total ultrasonic readings}}}
\]

The target for the sequential polling design is:

\[
C_r \rightarrow 0
\]

under validated installation and environmental conditions.

---

# 13. Feasibility

## 13.1 Technical Feasibility

NEURO-PATH uses established and accessible technologies:

- ESP32 embedded control.
- UART and I²C interfaces.
- 24 GHz radar.
- IP67 ultrasonic sensors.
- Snapdragon-based mobile edge computing.
- YOLOv8-Nano.
- NCNN inference.
- LED and LCD driver hardware.
- Local flash logging.

The architecture is technically feasible because the computational responsibilities are separated:

- The ESP32 executes deterministic safety logic.
- The Snapdragon platform executes computationally heavier visual inference.
- The HMI is simple and directly controlled.
- Critical operation is not dependent on a remote server.

---

## 13.2 Operational Feasibility

The system is intended for:

- Legacy vehicle retrofits.
- Vehicles with no cellular coverage.
- Mine operators seeking affordable proximity detection.
- Fleets requiring independent system operation.
- Pilot testing on a single dumper before fleet expansion.

The proposed pilot window is **3–6 months**, subject to:

- Vehicle access.
- Mine safety approvals.
- Sensor mounting approval.
- Controlled testing locations.
- Operator participation.
- Environmental test availability.
- Data collection and review.

---

## 13.3 Supply-Chain Feasibility

The prototype uses commercial off-the-shelf components such as:

- ESP32-WROOM-32.
- HLK-LD2410C.
- A02YYUW sensors.
- Standard LCD, LED and siren components.
- An available Snapdragon 845 platform.

For production deployment, selected components should be replaced or supplemented with industrially qualified equivalents where necessary.

---

# 14. Financial Viability

## 14.1 Prototype Bill of Materials

The project presentation identifies a prototype BOM estimate of approximately:

# **₹5,271**

This represents a proof-of-concept configuration and does not necessarily include:

- Industrial enclosure redesign.
- Installation labour.
- Vehicle-specific brackets.
- EMC testing.
- Environmental qualification.
- Certification.
- Production tooling.
- Spare units.
- Calibration equipment.
- Maintenance infrastructure.

---

## 14.2 Industrial Retrofit Estimate

The projected industrial retrofit cost is:

# **₹26,500 per machine**

This estimate may include:

- Industrial enclosure.
- Protected DC power conversion.
- Mounting hardware.
- Cabling and connectors.
- Sensor brackets.
- HMI installation.
- Labour.
- Calibration.
- Basic commissioning.
- Service margin.

---

## 14.3 Comparative Cost Position

| Solution | Indicative Cost Position |
|---|---:|
| NEURO-PATH industrial retrofit | **₹26,500 per machine** |
| Prototype configuration | **₹5,271** |
| OEM camera-based ADAS | **₹1.5 lakh or more** |
| Multi-camera / premium OEM ADAS | Multi-lakh range |

The proposed cost advantage enables:

- Deployment across ageing fleets.
- Retrofitting without replacing the vehicle.
- Gradual fleet-wide adoption.
- Lower capital expenditure.
- Reduced vendor lock-in.
- No recurring cloud subscription fee.

A formal total-cost-of-ownership analysis should additionally include:

- Calibration intervals.
- Sensor replacement.
- Maintenance labour.
- Battery and power-system impact.
- Software update costs.
- Training.
- Downtime during installation.

---

# 15. Impact and Benefits

## 15.1 Safety Impact

NEURO-PATH is intended to reduce the likelihood of blind-spot incidents involving:

- Ground workers.
- Maintenance staff.
- Light vehicles.
- Other dumpers.
- Service equipment.
- Personnel near reversing zones.

The system contributes to safety by:

- Extending hazard awareness beyond direct visibility.
- Distinguishing dynamic threats from static geometry.
- Providing directional rather than generic warnings.
- Maintaining operation in low-connectivity areas.
- Recording events for investigation and improvement.

The system cannot guarantee “zero fatalities” by itself. The appropriate engineering claim is that it is designed to reduce exposure to blind-spot collision risk as one layer within a broader mine safety system.

---

## 15.2 Operator Impact

The most important human-factor benefit is improved trust.

Reducing nuisance alarms by the design target of **greater than 85%** can help:

- Prevent continuous beeping.
- Reduce warning fatigue.
- Improve attention to genuine alarms.
- Reduce the motivation to mute the system.
- Improve directional understanding.
- Lower cognitive workload during reversing and turning.

---

## 15.3 Fleet Manager Impact

Fleet and mine managers receive:

- A low-cost retrofit pathway.
- Offline operation.
- No cloud subscription dependency.
- Local diagnostic logs.
- Scalable architecture.
- Sensor-level fault visibility.
- Potential reduction in weather-related operational stoppages.

---

## 15.4 Productivity Impact

The project’s stated objective is to reduce avoidable downtime during poor visibility.

The system is not intended to encourage unsafe operation during conditions where the mine’s risk assessment requires stoppage. Instead, it can provide additional hazard awareness during approved operating conditions, subject to mine procedures.

Potential benefits include:

- Better continuity of haul cycles.
- Reduced unnecessary stoppages.
- Faster recovery after visibility degradation.
- More predictable vehicle utilisation.
- Improved confidence during controlled low-visibility operations.

---

## 15.5 Environmental Impact

The zero-cloud architecture avoids continuous transmission of raw spatial data to remote infrastructure.

Potential environmental benefits include:

- Reduced network traffic.
- Reduced cloud processing dependency.
- Lower operational energy associated with data movement.
- Reduced demand for remote computing infrastructure.
- Longer availability of the system in areas with poor connectivity.

These benefits should be quantified during future lifecycle assessment rather than presented as a certified carbon reduction.

---

# 16. Risks and Mitigation

| Risk | Impact | Mitigation |
|---|---|---|
| Heavy dust affects optical camera | Reduced AI confidence | Radar and ultrasonic primary sensing |
| Ultrasonic cross-talk | Incorrect range | 50 ms sequential polling |
| Static walls create warnings | Operator fatigue | Doppler-based motion gating |
| Radar vibration artefacts | False motion | Temporal filtering, mounting isolation |
| Sensor face contamination | Reduced range | Protective housings and cleaning procedure |
| Cellular outage | Loss of cloud system | 100% offline local processing |
| Power transients | System reset | Automotive-grade power protection |
| HMI failure | Driver may miss warning | Redundant visual and audible cues |
| AI model uncertainty | Misclassification | Radar and acoustic corroboration |
| Operator over-reliance | Unsafe behaviour | Training and operating procedures |
| Environmental conditions exceed design | Reduced performance | Defined operating envelope and fail-safe indication |
| Unverified compliance assumptions | Regulatory risk | Formal DGMS and ISO gap analysis |

---

# 17. Pilot Deployment Plan at NMDC Bailadila

## Phase 1 — Bench Validation

**Duration:** 2–4 weeks

Activities:

- Validate radar UART communication.
- Validate ultrasonic triggers and echoes.
- Verify 50 ms scheduling.
- Test LCD, LED ring and siren.
- Verify local logging.
- Confirm Snapdragon telemetry.
- Test power protection.

Deliverables:

- Electrical test report.
- Firmware revision baseline.
- Sensor timing logs.
- Initial failure-mode register.

---

## Phase 2 — Controlled Yard Testing

**Duration:** 4–6 weeks

Activities:

- Install system on a stationary dumper.
- Validate sensor fields of view.
- Place static and moving targets.
- Test left, right and rear zones.
- Calibrate warning thresholds.
- Validate operator display.

Deliverables:

- Zone coverage map.
- Detection and false-alarm data.
- HMI usability report.
- Mounting and wiring drawings.

---

## Phase 3 — Low-Risk Operational Pilot

**Duration:** 6–10 weeks

Activities:

- Operate on a selected machine in controlled routes.
- Record sensor and warning events.
- Include day, night, dust and light fog conditions.
- Compare alarms against operator and spotter observations.
- Review all fault events.

Deliverables:

- Field performance report.
- Mean time between faults.
- Nuisance-alarm rate.
- Operator feedback.
- Maintenance recommendations.

---

## Phase 4 — Harsh-Condition Validation

**Duration:** Subject to seasonal conditions

Activities:

- Test in monsoon fog.
- Test in elevated dust conditions.
- Assess sensor contamination.
- Measure detection performance at 3–5 m optical visibility.
- Validate warning timing during reversing and turning.

Deliverables:

- Environmental degradation curves.
- Final calibration envelope.
- Safety case update.
- Recommendation for expanded deployment.

---

## Phase 5 — Fleet Deployment Readiness

Activities:

- Freeze hardware revision.
- Finalize enclosure and wiring kit.
- Establish installation manual.
- Develop calibration procedure.
- Train maintenance and safety teams.
- Define spare-parts inventory.
- Complete compliance gap analysis.

---

# 18. Maintenance and Serviceability

## 18.1 Daily Operator Checks

The driver should verify:

- System power-on indication.
- Radar status.
- Ultrasonic status.
- LCD operation.
- LED ring operation.
- Siren test.
- No active sensor fault.
- Sensor surfaces are not obstructed.

---

## 18.2 Scheduled Maintenance

Maintenance personnel should inspect:

- Sensor alignment.
- Bracket integrity.
- Connector locking.
- Cable abrasion.
- Enclosure seals.
- Water or mud ingress.
- Radar radome condition.
- Ultrasonic transducer faces.
- Flash-log health.
- Firmware version.
- HMI functionality.

---

## 18.3 Calibration

Calibration should be performed after:

- Initial installation.
- Sensor replacement.
- Bracket impact.
- Major vehicle repair.
- Repositioning.
- Repeated false alarms.
- Changes to vehicle body geometry.

---

# 19. Novelty and Differentiation

NEURO-PATH differentiates itself through the combination of:

1. **Weather-resilient sensing** using radar and ultrasound.
2. **Doppler-based static geometry suppression.**
3. **Strict sequential acoustic scheduling.**
4. **Deterministic ESP32 decision logic.**
5. **Optional edge-AI optical corroboration.**
6. **Directional driver cueing.**
7. **100% offline operation.**
8. **Local black-box event logging.**
9. **Legacy fleet retrofit compatibility.**
10. **Estimated industrial cost of ₹26,500 per machine.**

The key innovation is not any single sensor. It is the architectural integration of heterogeneous sensing, deterministic timing and human-centred warning design for the specific conditions of open-cast iron ore mining.

---

# 20. Limitations and Engineering Honesty

A credible industrial safety proposal must clearly state its limitations.

NEURO-PATH currently requires further validation for:

- Absolute detection range in dense fog.
- Radar performance around large metal machinery.
- Ultrasonic performance in heavy rain and wind.
- Object classification accuracy under severe dust.
- Vehicle-specific sensor placement.
- Electromagnetic compatibility.
- Long-duration vibration exposure.
- Mine-specific DGMS acceptance.
- Formal ISO compliance.
- Production-grade cybersecurity.
- Functional-safety assessment.
- False-negative performance in complex multipath environments.

The system should never be marketed as “weather immune” in the absolute sense. A technically responsible formulation is:

> **NEURO-PATH is designed to maintain useful hazard awareness when optical systems are degraded by fog, dust and low illumination. Its operational limits must be established through field validation.**

---

# 21. Future Development Roadmap

## 21.1 Hardware Enhancements

- Industrial 24 GHz or 77 GHz radar module with validated range and velocity output.
- Multiple radar nodes for full azimuth coverage.
- Industrial microcontroller with extended temperature qualification.
- Ruggedized edge-AI computer.
- Dedicated camera with heated and hydrophobic lens.
- Temperature and humidity compensation.
- Automotive-grade connectors and harnessing.
- Redundant power supply path.
- Protected local storage.

---

## 21.2 Software Enhancements

- Adaptive warning thresholds based on vehicle speed.
- Time-to-collision estimation.
- Sensor health scoring.
- Self-calibration assistance.
- Signed firmware updates.
- Improved event compression.
- Data replay tools.
- Automated test framework.
- Machine-specific configuration profiles.
- More robust object tracking.

---

## 21.3 Safety Enhancements

- Independent emergency-warning channel.
- Redundant siren driver.
- Secondary warning beacon.
- External personnel alert module.
- Optional wearable tags for workers.
- Fleet-level offline data collection.
- Integration with mine traffic-management systems.
- Formal safety case and assurance argument.

---

# 22. Conclusion

NEURO-PATH is a practical, retrofit-oriented response to one of the most difficult safety problems in open-cast mining: operating heavy vehicles when visibility is reduced to **3–5 m** by monsoon fog and iron-ore dust.

Its central engineering contribution is the combination of:

- **24 GHz mmWave radar** for weather-resilient motion sensing.
- **Three IP67 ultrasonic channels** for close-range blind-spot mapping.
- **ESP32 deterministic control** for bounded, local decision-making.
- **Snapdragon 845 edge-AI** for optical corroboration.
- **Motion-gated logic** to suppress stationary quarry geometry.
- **50 ms sequential polling** to prevent ultrasonic cross-talk.
- **Directional HMI cueing** to improve driver response.
- **Zero-cloud architecture** for remote mine operation.
- **₹26,500 estimated industrial retrofit cost** for legacy fleet deployment.

The system is designed not merely to detect objects, but to improve the reliability of the warning itself. By reducing unnecessary alerts, preserving operator trust and maintaining operation without cellular networks, NEURO-PATH addresses both the technical and human factors that cause conventional proximity systems to fail in practice.

The proposed pilot at NMDC Bailadila should focus on measured validation rather than unsupported performance claims. With controlled testing, environmental qualification, formal compliance review and mine-specific calibration, NEURO-PATH can evolve from a functional prototype into an industry-ready safety-support platform for India’s heavy mining fleet.

> **NEURO-PATH: securing the lives that secure the nation’s resources.**

---

# Appendix A — Indicative System Block Diagram

```text
                  ┌──────────────────────────────┐
                  │       MINE ENVIRONMENT       │
                  │ Fog | Dust | Berms | People  │
                  │ Vehicles | Quarry Walls      │
                  └──────────────┬───────────────┘
                                 │
          ┌──────────────────────┼──────────────────────┐
          │                      │                      │
          ▼                      ▼                      ▼
┌─────────────────┐    ┌─────────────────┐    ┌─────────────────┐
│ 24 GHz Radar    │    │ Ultrasonic Left │    │ Ultrasonic Right│
│ Motion / Doppler│    │ Close Range     │    │ Close Range     │
└────────┬────────┘    └────────┬────────┘    └────────┬────────┘
         │                      │                      │
         └──────────────────────┼──────────────────────┘
                                │
                     ┌──────────▼──────────┐
                     │   ESP32-WROOM-32    │
                     │ Deterministic Core  │
                     │ Motion Gate         │
                     │ Sensor Fusion       │
                     │ Fault Detection     │
                     └───────┬───────┬─────┘
                             │       │
                ┌────────────▼─┐   ┌─▼──────────────┐
                │ Snapdragon    │   │ Cockpit HMI   │
                │ 845 / NCNN    │   │ LCD           │
                │ YOLOv8-Nano   │   │ LED Ring      │
                └───────────────┘   │ PWM Siren     │
                                    └───────────────┘
```

---

# Appendix B — Indicative Threat-State Logic

```text
START
  │
  ▼
Initialize sensors and HMI
  │
  ▼
Run 50 ms scheduled acquisition
  │
  ├── Radar update
  ├── Left ultrasonic
  ├── Right ultrasonic
  └── Rear ultrasonic
  │
  ▼
Validate readings
  │
  ├── Invalid? → Mark sensor degraded
  └── Valid? → Continue
  │
  ▼
Estimate target distance and motion
  │
  ├── Static object?
  │       └── Caution / suppress critical alarm
  │
  └── Moving object?
          │
          ▼
      Check persistence
          │
          ▼
      Check zone and distance
          │
          ├── Safe → Green
          ├── Caution → Yellow
          ├── Warning → Orange + intermittent tone
          └── Critical → Red + high-priority siren
```

---

# Appendix C — Indicative Acceptance Criteria

| Parameter | Initial Design Target |
|---|---:|
| Optical visibility challenge | **3–5 m** |
| Complete sensor cycle | **200 ms** |
| Individual scheduled slot | **50 ms** |
| Acoustic transducers active simultaneously | **One** |
| Nuisance-alarm reduction target | **>85%** |
| Critical processing dependency | **No cloud** |
| Prototype BOM | **₹5,271** |
| Industrial retrofit estimate | **₹26,500 per machine** |
| Pilot readiness target | **3–6 months**, subject to approvals |
| Safety-critical data processing | **100% local** |

---

# Appendix D — References

1. Carr, J., DuCarme, J., and Jobes, C. C.  
   **Evaluation of an Advanced Proximity Detection System for Continuous Mining Machines.**  
   National Institute for Occupational Safety and Health, 2012.  
   https://stacks.cdc.gov/view/cdc/227508

2. Sensors, MDPI.  
   **Sensors journal article, Volume 22, Issue 16, Article 5989.**  
   https://www.mdpi.com/1424-8220/22/16/5989

3. International Organization for Standardization.  
   **ISO 16001:2017 — Earth-moving machinery — Object detection systems and visibility aids — Performance requirements and tests.**  
   https://www.iso.org/standard/63688.html

4. DFRobot.  
   **A02YYUW Waterproof Ultrasonic Sensor Wiki, SEN0311.**  
   https://wiki.dfrobot.com/sen0311/

5. Espressif Systems.  
   **ESP32-WROOM-32 Datasheet.**  
   https://documentation.espressif.com/esp32-wroom-32_datasheet_en.html

6. **DGMS (Tech) (SOMA) Circular No. 03 of 2024**, concerning accident prevention involving Trackless Transportation Machinery in opencast mines and the importance of proximity detection controls. https://onlineminingexam.graphy.com/blog/dgms-tech-soma-circular-no-03-of-2024?utm_source=gemini

7. **Millimeter-Wave Radar for Mining Applications**, cited IEEE/ResearchGate material concerning the use of mmWave radar in dust and water-vapour environments. https://www.researchgate.net/publication/227871149_Seeing_through_dust_and_water_vapor_Millimeter_wave_radar_sensors_for_mining_applications?utm_source=gemini

8. **Algorithmic Ecology: A Framework for Achieving Carbon-Neutrality in Global Data Infrastructure**, cited as a peer-reviewed work authored by **Subhan Khan and Mohammed Bakhtawar Ahmed**, supporting decentralized edge processing, reduced data movement and lower latency.
   https://doi.org/10.37591/IJSU.v03i01.243250

9. Project team field and applied research:
   - Sensor degradation analysis in simulated iron-ore dust and monsoon fog.
   - ESP32-based validation of the **50 ms sequential polling loop**.
   - Prototype validation of multi-sensor fusion and local warning logic.
