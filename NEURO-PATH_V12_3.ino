/* * PROJECT:     NEURO-PATH: Intelligent Blind Spot Detection System
 * DESCRIPTION: An AI-integrated vehicle safety solution utilizing multi-sensor fusion 
 * (Ultrasonic & Laser) to monitor blind zones. Features cinematic 
 * boot animations, motion-gated alerts, and resource-optimized 
 * pin mapping for ESP32-CAM.
 * AUTHOR:      Subhan Khan
 * VERSION:     12.3 (Stable)
 * DATE:        May 2026
 */

#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <ESP32Servo.h>
#include "Adafruit_VL53L0X.h"
#include "soc/soc.h"           
#include "soc/rtc_cntl_reg.h"  

// --- Pin Definitions (ESP32-CAM Optimized) ---
#define PIN_F_SENSE   13    // Remote Input: Forward Motion
#define PIN_B_SENSE   12    // Remote Input: Backward Motion
#define PIN_TRIG      0     // Ultrasonic Trigger
#define PIN_ECHO      3     // Ultrasonic Echo (U0RX)
#define PIN_SERVO     16    // Radar Sweep Servo
#define PIN_RED_BZ    4     // Red Warning LED, Buzzer, and Onboard Flashlight
#define PIN_GREEN     2     // Onboard Blue LED (System Status Clear)

#define I2C_SDA       14    // LCD/Laser Data
#define I2C_SCL       15    // LCD/Laser Clock

// Global Objects
Adafruit_VL53L0X lox = Adafruit_VL53L0X();
LiquidCrystal_I2C lcd(0x27, 16, 2);
Servo radarServo;

// Global Variables
int pos = 0;                // Current Servo Position
int sweepStep = 10;         // Radar Sweep Speed
unsigned long lastBeep = 0; // Pulsing logic timer
bool bzState = false;       // Buzzer toggle state

/**
 * Executes the Cinematic Startup Sequence
 * Phase 1: Typewriter Title
 * Phase 2: Static Subtitle
 * Phase 3: Full-line Progressive Loading
 */
void runBootAnimation() {
  lcd.clear();
  String title = "   NEURO-PATH   ";
  String subTitle = " BLINDSPOT SYS";
  String bootText = "SYSTEM BOOTING";
  
  // Typewriter Title Effect
  for (int i = 0; i < title.length(); i++) {
    lcd.setCursor(i, 0);
    lcd.print(title[i]);
    delay(100); 
  }
  
  lcd.setCursor(0, 1);
  lcd.print(subTitle);
  delay(1500);
  
  // Transition to System Loading
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(bootText);
  
  lcd.setCursor(0, 1);
  for (int i = 0; i < 16; i++) { 
    lcd.print(".");
    delay(200); 
  }
  delay(500);
}

void setup() {
  // Disable Brownout detection for ESP32 stability during sensor/servo startup
  WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0); 
  
  Wire.begin(I2C_SDA, I2C_SCL);
  lcd.init();
  lcd.backlight();
  
  runBootAnimation();
  
  // Pin Configuration
  pinMode(PIN_F_SENSE, INPUT_PULLDOWN);
  pinMode(PIN_B_SENSE, INPUT_PULLDOWN);
  pinMode(PIN_TRIG, OUTPUT);
  pinMode(PIN_ECHO, INPUT_PULLUP); // PULLUP ensures N/C detection
  pinMode(PIN_RED_BZ, OUTPUT);
  pinMode(PIN_GREEN, OUTPUT);

  lox.begin(); 
  radarServo.attach(PIN_SERVO, 500, 2400);
  lcd.clear();
}

/**
 * Handles distance display with dynamic unit conversion
 * @param dist Distance in cm
 * @param colStart LCD column index
 */
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
  int uDist = -1; // Ultrasonic distance
  int lDist = -1; // Laser distance

  // --- SENSOR ACQUISITION: ULTRASONIC ---
  digitalWrite(PIN_TRIG, LOW); delayMicroseconds(2);
  digitalWrite(PIN_TRIG, HIGH); delayMicroseconds(10);
  digitalWrite(PIN_TRIG, LOW);
  
  long uDur = pulseIn(PIN_ECHO, HIGH, 20000); 
  if (uDur == 0) {
    if (digitalRead(PIN_ECHO) == HIGH) uDist = -1; // Physical Disconnection
    else uDist = 999; // No return (Sky/Clear)
  } else {
    uDist = uDur * 0.034 / 2;
    if (uDist > 350) uDist = 999;
  }

  // --- SENSOR ACQUISITION: LASER ---
  Wire.beginTransmission(0x29);
  if (Wire.endTransmission() == 0) { 
    VL53L0X_RangingMeasurementData_t measure;
    lox.rangingTest(&measure, false);
    if (measure.RangeStatus != 4 && measure.RangeMilliMeter < 2000 && measure.RangeMilliMeter != 8190) {
      lDist = measure.RangeMilliMeter / 10;
    } else lDist = 999; 
  } else lDist = -1; 

  // --- ACTUATOR CONTROL: SERVO RADAR ---
  if (uDist != -1) {
    if (!radarServo.attached()) radarServo.attach(PIN_SERVO, 500, 2400);
    radarServo.write(pos);
    pos += sweepStep;
    if(pos <= 0 || pos >= 180) sweepStep *= -1;
  } else radarServo.detach(); 

  // --- UI UPDATE: TELEMETRY ---
  lcd.setCursor(0, 0);
  lcd.print("U:"); displayDistance(uDist, 2); 
  lcd.setCursor(7, 0); lcd.print("|");
  lcd.setCursor(9, 0);
  lcd.print("L:"); displayDistance(lDist, 11);

  // --- DECISION LOGIC: MOTION & FUSION ---
  bool isMoving = (digitalRead(PIN_F_SENSE) == HIGH || digitalRead(PIN_B_SENSE) == HIGH);
  int minDist = 999;
  if (uDist != -1 && uDist != 999) minDist = min(minDist, uDist);
  if (lDist != -1 && lDist != 999) minDist = min(minDist, lDist);

  // --- OUTPUT DISPATCHER ---
  lcd.setCursor(0, 1);
  if (uDist == -1 && lDist == -1) {
    lcd.print("CHECK SENSORS!  ");
    digitalWrite(PIN_RED_BZ, HIGH); // System failure override
  } else if (uDist == -1) {
    lcd.print("CHECK ULTRA!    ");
    handleCombinedAlerts(minDist, isMoving);
  } else if (lDist == -1) {
    lcd.print("CHECK LASER!    ");
    handleCombinedAlerts(minDist, isMoving);
  } else {
    handleCombinedAlerts(minDist, isMoving); 
  }
  
  delay(5); // Minimum loop delay for performance
}

/**
 * Combined visual and auditory alert logic
 * @param distance Minimum sensed distance
 * @param moving Vehicle motion status
 */
void handleCombinedAlerts(int distance, bool moving) {
  if (distance < 15) {
    lcd.print("CAUTION: ALERT! ");
    digitalWrite(PIN_GREEN, LOW); 
    if (moving) digitalWrite(PIN_RED_BZ, HIGH); // Continuous Danger Alert
    else digitalWrite(PIN_RED_BZ, LOW);
  } 
  else if (distance < 35) {
    lcd.print("CAUTION: SLOW   ");
    digitalWrite(PIN_GREEN, LOW);
    if (moving) {
      if (millis() - lastBeep > 150) { // Pulsed Caution Alert
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
