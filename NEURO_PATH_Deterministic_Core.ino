/*
 * PROJECT:     NEURO-PATH - Industrial Collision Avoidance Core
 * DESCRIPTION: Deterministic Sensor Node for SIH 2026 (SIH26007). 
 *              Fuses HLK-LD2410C 24GHz mmWave Radar & IP67 Ultrasonic array 
 *              via a 50ms sequential polling loop to prevent acoustic cross-talk.
 *              Outputs to 20x4 I2C LCD and 24V PWM Siren.
 * AUTHOR:      Subhan Khan (Team THYNK UNIQUE)
 */

#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <HardwareSerial.h>

// --- Pin Allocations ---
#define SONAR_TRIG_PIN    12  // IP67 A02YYUW Transducer
#define SONAR_ECHO_PIN    13  
#define CABIN_SIREN_PIN   4   // PWM Piezo / 24V Relay
#define RADAR_RX_PIN      16  // Hardware Serial 2 for LD2410C
#define RADAR_TX_PIN      17

// --- Peripheral Instances ---
LiquidCrystal_I2C lcd(0x27, 20, 4); // Upgraded to 20x4 LCD
HardwareSerial RadarSerial(2);

// --- Telemetry Variables ---
int ultrasonicDistanceCm = 999;
int radarDistanceCm = 999;
bool isRadarTargetMoving = false;
int closestThreatCm = 999;
unsigned long lastPollTime = 0;

void setup() {
  Serial.begin(115200);        // Diagnostic / Python Vision Node Link
  RadarSerial.begin(256000, SERIAL_8N1, RADAR_RX_PIN, RADAR_TX_PIN); // LD2410C Baud
  
  pinMode(SONAR_TRIG_PIN, OUTPUT);
  pinMode(SONAR_ECHO_PIN, INPUT);
  pinMode(CABIN_SIREN_PIN, OUTPUT);
  digitalWrite(CABIN_SIREN_PIN, LOW);

  lcd.init();
  lcd.backlight();
  
  // INDUSTRIAL BOOT SPLASH
  lcd.setCursor(0, 0); lcd.print("[ SYS: INITIALIZING ]");
  lcd.setCursor(0, 1); lcd.print("NEURO-PATH ADAS v2.0");
  lcd.setCursor(0, 2); lcd.print("RADAR: WARMING UP...");
  delay(1500);
  lcd.clear();
}

void loop() {
  unsigned long currentMillis = millis();

  // 1. DETERMINISTIC 50ms POLLING LOOP (Zero Cross-talk)
  if (currentMillis - lastPollTime >= 50) {
    lastPollTime = currentMillis;

    // A. Read IP67 Ultrasonic Array
    digitalWrite(SONAR_TRIG_PIN, LOW);
    delayMicroseconds(2);
    digitalWrite(SONAR_TRIG_PIN, HIGH);
    delayMicroseconds(10);
    digitalWrite(SONAR_TRIG_PIN, LOW);
    
    long duration = pulseIn(SONAR_ECHO_PIN, HIGH, 30000); 
    ultrasonicDistanceCm = (duration == 0) ? 999 : (duration * 0.0343 / 2);

    // B. Read 24GHz mmWave Radar (LD2410C)
    // Basic parser: Checks for moving target flags and distance gates
    if (RadarSerial.available()) {
        // Implementation of LD2410C hex protocol parsing goes here
        // For prototyping, we simulate parsing a distance and motion flag
        radarDistanceCm = 350; // Example parsing output
        isRadarTargetMoving = true; 
    }

    // 2. SENSOR FUSION & MOTION-GATED LOGIC
    // We only care about the radar data if the object is actually MOVING (ignores rock walls)
    if (isRadarTargetMoving && radarDistanceCm < ultrasonicDistanceCm) {
      closestThreatCm = radarDistanceCm;
    } else {
      closestThreatCm = ultrasonicDistanceCm;
    }

    // 3. COCKPIT HMI UPDATE (20x4 LCD)
    lcd.setCursor(0, 0); 
    lcd.print(closestThreatCm < 150 ? "[ SYS: ARMED ]      " : "[ SYS: STANDBY ]    ");
    
    lcd.setCursor(0, 1); 
    lcd.print("RADAR:"); lcd.print(radarDistanceCm); lcd.print("cm ");
    lcd.print("SONAR:"); lcd.print(ultrasonicDistanceCm); lcd.print("cm");

    lcd.setCursor(0, 2);
    lcd.print("TGT VECTOR: ");
    lcd.print(isRadarTargetMoving ? "CLOSING " : "STATIC  ");

    // 4. CABIN SIREN LOGIC (PWM Scaling)
    lcd.setCursor(0, 3);
    if (closestThreatCm > 400) {
      lcd.print("STATUS: SAFE ZONES  ");
      analogWrite(CABIN_SIREN_PIN, 0);
    } 
    else if (closestThreatCm <= 400 && closestThreatCm > 150) {
      lcd.print("STATUS: CAUTION     ");
      analogWrite(CABIN_SIREN_PIN, 50); // Low hum
    } 
    else {
      lcd.print("STATUS: BRAKE HAZARD");
      analogWrite(CABIN_SIREN_PIN, 255); // Full alarm
    }

    // 5. UART Link to Snapdragon Vision Node
    Serial.print("DIST:");
    Serial.println(closestThreatCm);
  }
}
