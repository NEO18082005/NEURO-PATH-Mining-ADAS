/* * PROJECT:     AG~3 NEURO-PATH - Industrial Sensory Fusion Core
 * DESCRIPTION: Multi-sensor fusion engine managing LCD HUD telemetry, hardware failure 
 * diagnostics, and motion-gated safety indicators for heavy machinery.
 * VERSION:     13.0 (ET Hackathon Edition)
 */

#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <ESP32Servo.h>

// --- Pin Allocations ---
#define TRIG_PIN     12
#define ECHO_PIN     13
#define BUZZER_PIN    4  
#define GREEN_LED     2  
#define SERVO_PIN    14

// --- Peripheral Instances ---
LiquidCrystal_I2C lcd(0x27, 16, 2);
Servo radarServo;

// --- Global Tracking Matrices ---
long pulseDuration;
int laserDistanceCm;
int visionTargetDistance = 999; 
int currentServoAngle = 90;
bool scanningIncrement = true;

void setup() {
  Serial.begin(115200);
  
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(GREEN_LED, OUTPUT);
  
  digitalWrite(BUZZER_PIN, LOW);
  digitalWrite(GREEN_LED, LOW);

  radarServo.attach(SERVO_PIN);
  lcd.init();
  lcd.backlight();
  
  // INDUSTRIAL BOOT SPLASH
  lcd.setCursor(0, 0);
  lcd.print("AG~3 NEURO-PATH ");
  lcd.setCursor(0, 1);
  lcd.print("INDUSTRIAL ADAS ");
  
  digitalWrite(BUZZER_PIN, HIGH);
  delay(300);
  digitalWrite(BUZZER_PIN, LOW);
  delay(1000);
  
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("SYS CALIBRATING.");
  delay(800);
}

void loop() {
  // 1. Dynamic Radar Scanning Sweep
  radarServo.write(currentServoAngle);
  if (scanningIncrement) {
    currentServoAngle += 2;
    if (currentServoAngle >= 150) scanningIncrement = false;
  } else {
    currentServoAngle -= 2;
    if (currentServoAngle <= 30) scanningIncrement = true;
  }
  
  // 2. Ultrasonic ToF Ranging
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  pulseDuration = pulseIn(ECHO_PIN, HIGH, 30000); 
  
  if (pulseDuration == 0) {
    laserDistanceCm = 999;
  } else {
    laserDistanceCm = pulseDuration * 0.0343 / 2;
  }
  
  // 3. Read Inbound AI Vision Strings
  if (Serial.available() > 0) {
    String payload = Serial.readStringUntil('\n');
    if (payload.startsWith("DIST:")) {
      visionTargetDistance = payload.substring(5).toInt();
    }
  }
  
  // 4. Motion-Gated Threat Evaluator
  int evaluatedThreatZone = min(laserDistanceCm, visionTargetDistance);

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("AI:");
  lcd.print(visionTargetDistance);
  lcd.print(" HW:");
  lcd.print(laserDistanceCm);
  lcd.print(" cm");
  
  lcd.setCursor(0, 1);
  if (evaluatedThreatZone > 50) {
    lcd.print("ZONE: SECURE  ");
    digitalWrite(GREEN_LED, HIGH);
    digitalWrite(BUZZER_PIN, LOW);
  } 
  else if (evaluatedThreatZone <= 50 && evaluatedThreatZone > 20) {
    lcd.print("WORKER IN ZONE");
    digitalWrite(GREEN_LED, LOW);
    digitalWrite(BUZZER_PIN, HIGH);
    delay(50);
    digitalWrite(BUZZER_PIN, LOW);
  } 
  else {
    lcd.print("CRITICAL HAZARD!");
    digitalWrite(GREEN_LED, LOW);
    digitalWrite(BUZZER_PIN, HIGH); 
  }
  
  delay(40); 
}
