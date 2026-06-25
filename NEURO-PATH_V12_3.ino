/*
 * PROJECT:     AG~3 NEURO-PATH: Intelligent Edge-AI Blind Spot Detection System
 * DESCRIPTION: An AI-integrated Advanced Driver Assistance System (ADAS) utilizing
 * multi-sensor fusion (Ultrasonic array & Laser ToF) to actively
 * monitor blind zones. Features a decoupled processing architecture,
 * cinematic boot animations, motion-gated alerts to prevent sensory
 * fatigue, and a sequential polling algorithm to prevent acoustic cross-talk.
 * AUTHOR:      Subhan Khan & Ayaan Khan
 * EVENT:       USAII® Global AI Hackathon 2026
 * VERSION:     13.0 (WROOM Decoupled Core)
 * DATE:        June 2026
 */

#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <ESP32Servo.h>
#include "Adafruit_VL53L0X.h"
#include <NewPing.h> // Highly recommended for multi-sonar arrays!

// --- ESP32 WROOM Decoupled Pin Definitions ---
// Motion Sensing (Safe Input Only Pins via Voltage Divider)
#define PIN_F_SENSE   34   
#define PIN_B_SENSE   35  

// Alert Matrix
#define PIN_RED_BZ    26     
#define PIN_GREEN     27     

// I2C Bus (Default ESP32 Pins)
#define I2C_SDA       21
#define I2C_SCL       22

// Rear Scanning Radar Turret
#define PIN_SERVO     25
#define REAR_TRIG     18    
#define REAR_ECHO     19
#define MAX_DISTANCE  350 // Maximum distance we want to ping for (in centimeters). Maximum sensor distance is rated at 400-500cm.

// Lateral Spatial Detection
#define LEFT_TRIG     18    // Can share Trigger pin if pointed in completely different directions
#define LEFT_ECHO     32
#define RIGHT_TRIG    18    // Can share Trigger pin
#define RIGHT_ECHO    33

// Initialize Objects
Adafruit_VL53L0X lox = Adafruit_VL53L0X();
LiquidCrystal_I2C lcd(0x27, 16, 2);
Servo radarServo;

// Setup Sonar Array using NewPing (Non-Blocking)
NewPing rearSonar(REAR_TRIG, REAR_ECHO, MAX_DISTANCE);
NewPing leftSonar(LEFT_TRIG, LEFT_ECHO, MAX_DISTANCE);
NewPing rightSonar(RIGHT_TRIG, RIGHT_ECHO, MAX_DISTANCE);


int pos = 0;
int sweepStep = 10; 
unsigned long lastBeep = 0;
bool bzState = false;

// --- CINEMATIC ANIMATION ---

void runBootAnimation() {
  lcd.clear();
  String title = "   NEURO-PATH   ";
  String bootText = "SYSTEM BOOTING";
  
  // 1. First Screen: Typewriter Title
  for (int i = 0; i < title.length(); i++) {
    lcd.setCursor(i, 0);
    lcd.print(title[i]);
    delay(100); 
  }
  
  lcd.setCursor(0, 1);
  lcd.print(" BLINDSPOT SYS");
  delay(1500);
  
  // 2. Second Screen: System Booting with Full Line Dots
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(bootText);
  
  lcd.setCursor(0, 1);
  for (int i = 0; i < 16; i++) { 
    lcd.print(".");
    delay(250); 
  }
  
  delay(500);
}

void setup() {
  Serial.begin(115200); // For debugging
  Wire.begin(I2C_SDA, I2C_SCL);
  lcd.init();
  lcd.backlight();
  
  runBootAnimation();
  
  // Motion Inputs
  pinMode(PIN_F_SENSE, INPUT); // WROOM Input only pins do not have internal pullups/pulldowns
  pinMode(PIN_B_SENSE, INPUT); 
  
  // Alert Outputs
  pinMode(PIN_RED_BZ, OUTPUT);
  pinMode(PIN_GREEN, OUTPUT);

  // Initialize Laser ToF
  if (!lox.begin()) {
    Serial.println(F("Failed to boot VL53L0X"));
    // We don't halt here, we let the UI handle the "CHECK SENSORS" logic
  } 

  radarServo.attach(PIN_SERVO, 500, 2400);
  lcd.clear();
}

void displayDistance(int dist, int colStart) {
  lcd.setCursor(colStart, 0);
  if (dist == -1) lcd.print("N/C  "); 
  else if (dist >= 350 || dist == 999) lcd.print("CLR  ");
  else if (dist >= 100) {
    float meters = dist / 100.0;
    lcd.print(meters, 1); lcd.print("m  ");
  } else {
    lcd.print(dist); lcd.print("cm "); 
  }
}

void loop() {
  
  // --- SEQUENTIAL SENSOR POLLING ---
  int rDist = rearSonar.ping_cm();
  if (rDist == 0) rDist = 999; // 0 means out of range in NewPing
  delay(30); // 30ms delay to prevent acoustic cross-talk
  
  int lDist_sonar = leftSonar.ping_cm();
  if (lDist_sonar == 0) lDist_sonar = 999;
  delay(30);
  
  int rtDist_sonar = rightSonar.ping_cm();
  if (rtDist_sonar == 0) rtDist_sonar = 999;
  
  // Calculate the closest ultrasonic threat
  int closestUltra = min(min(rDist, lDist_sonar), rtDist_sonar);


  // 2. Laser Ping (Frontal/Longitudinal)
  int lDist_laser = -1;
  Wire.beginTransmission(0x29);
  if (Wire.endTransmission() == 0) { 
    VL53L0X_RangingMeasurementData_t measure;
    lox.rangingTest(&measure, false);
    if (measure.RangeStatus != 4 && measure.RangeMilliMeter < 2000 && measure.RangeMilliMeter != 8190) {
      lDist_laser = measure.RangeMilliMeter / 10;
    } else lDist_laser = 999; 
  } else lDist_laser = -1;
  
  
  // 3. Servo Logic (Only sweep if rear sensor detects something close)
  if (rDist != 999) {
    if (!radarServo.attached()) radarServo.attach(PIN_SERVO, 500, 2400);
    radarServo.write(pos);
    pos += sweepStep;
    if(pos <= 0 || pos >= 180) sweepStep *= -1;
  } else radarServo.detach(); // Detach to save power and stop jitter when clear
  
  
  // 4. UI Update
  lcd.setCursor(0, 0);
  lcd.print("U:"); displayDistance(closestUltra, 2); 
  lcd.setCursor(7, 0); lcd.print("|");
  lcd.setCursor(9, 0);
  lcd.print("L:"); displayDistance(lDist_laser, 11);
  
  // 5. Motion Gating Logic
  bool isMoving = (digitalRead(PIN_F_SENSE) == HIGH || digitalRead(PIN_B_SENSE) == HIGH);
  
  int minDist = 999;
  if (closestUltra != -1 && closestUltra != 999) minDist = min(minDist, closestUltra);
  if (lDist_laser != -1 && lDist_laser != 999) minDist = min(minDist, lDist_laser);

  lcd.setCursor(0, 1);
  if (closestUltra == -1 && lDist_laser == -1) {
    lcd.print("CHECK SENSORS!  ");
    digitalWrite(PIN_RED_BZ, HIGH);
  } else if (closestUltra == -1) {
    lcd.print("CHECK ULTRA!    ");
    handleCombinedAlerts(minDist, isMoving);
  } else if (lDist_laser == -1) {
    lcd.print("CHECK LASER!    ");
    handleCombinedAlerts(minDist, isMoving);
  } else {
    handleCombinedAlerts(minDist, isMoving); 
  }
  
  delay(5);
}

void handleCombinedAlerts(int distance, bool moving) {
  if (distance < 15) {
    lcd.print("CAUTION: ALERT! ");
    digitalWrite(PIN_GREEN, LOW);
    if (moving) digitalWrite(PIN_RED_BZ, HIGH); 
    else digitalWrite(PIN_RED_BZ, LOW);
  } 
  else if (distance < 35) {
    lcd.print("CAUTION: SLOW   ");
    digitalWrite(PIN_GREEN, LOW);
    if (moving) {
      if (millis() - lastBeep > 150) {
        bzState = !bzState;
        digitalWrite(PIN_RED_BZ, bzState);
        lastBeep = millis();
      }
    } else {
      digitalWrite(PIN_RED_BZ, LOW);
    }
  } 
  else {
    lcd.print("STATUS: CLEAR   ");
    digitalWrite(PIN_GREEN, HIGH); 
    digitalWrite(PIN_RED_BZ, LOW);  
  }
}
