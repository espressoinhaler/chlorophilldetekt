/*
 * ============================================================
 *  LEAF CHLOROPHYLL DETECTOR
 *  Hardware: Arduino Nano · 16×2 I2C LCD · LDR + Green LED
 * ============================================================
 */

#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// ── Pin Definitions ───────────────────────────────────────────
const int LDR_PIN    = A0;   // Analog input from LDR
const int LED_PIN    = 9;    // Green LED output (PWM)

// ── LCD Setup (address 0x27, 16 cols, 2 rows) ─────────────────
LiquidCrystal_I2C lcd(0x27, 16, 2);

// ── Calibration Constants ─────────────────────────────────────
const int   SAMPLES         = 20;
const int   LED_BRIGHTNESS  = 200;   // 0–255

byte leafChar[8] = { 0b00100, 0b01110, 0b11111, 0b11111,
                      0b01110, 0b00100, 0b00100, 0b00000 };
byte barChar[8]  = { 0b11111, 0b11111, 0b11111, 0b11111,
                      0b11111, 0b11111, 0b11111, 0b11111 };

float baselineReading = 0.0;

// ─────────────────────────────────────────────────────────────
void setup() {
  Serial.begin(9600);
  lcd.init();  lcd.backlight();
  lcd.createChar(0, leafChar);
  lcd.createChar(1, barChar);
  pinMode(LED_PIN, OUTPUT);
  analogWrite(LED_PIN, 0);

  lcd.setCursor(0,0); lcd.print("  Chlorophyll  ");
  lcd.setCursor(0,1); lcd.print("   Detector  ");
  delay(2000);  lcd.clear();

  calibrateBaseline();
}

void loop() {
  lcd.clear();
  lcd.setCursor(0,0); lcd.print("Place leaf...");
  lcd.setCursor(0,1); lcd.print("Press to read");
  delay(3000);

  float spad = measureSPAD();
  String status = classifyHealth(spad);
  displayResult(spad, status);

  Serial.print("SPAD: "); Serial.print(spad, 1);
  Serial.print("  Status: "); Serial.println(status);
  delay(5000);
}

void calibrateBaseline() {
  lcd.clear();
  lcd.setCursor(0,0); lcd.print("Calibrating...");
  lcd.setCursor(0,1); lcd.print("Remove leaf");
  analogWrite(LED_PIN, LED_BRIGHTNESS);  delay(500);

  long sum = 0;
  for (int i = 0; i < SAMPLES; i++) { sum += analogRead(LDR_PIN); delay(20); }
  baselineReading = (float)sum / SAMPLES;
  analogWrite(LED_PIN, 0);
  lcd.clear(); lcd.setCursor(0,0); lcd.print("Baseline set!"); delay(2000); lcd.clear();
}

float measureSPAD() {
  analogWrite(LED_PIN, LED_BRIGHTNESS);  delay(300);

  long sum = 0;
  for (int i = 0; i < SAMPLES; i++) { sum += analogRead(LDR_PIN); delay(20); }
  float leafReading = (float)sum / SAMPLES;
  analogWrite(LED_PIN, 0);

  if (leafReading <= 0) leafReading = 1;
  float T = constrain(leafReading / baselineReading, 0.01, 1.0);
  return constrain(-log10(T) * 100.0, 0.0, 99.9);
}

String classifyHealth(float spad) {
  if (spad >= 50.0) return "Excellent";
  if (spad >= 35.0) return "Healthy";
  if (spad >= 20.0) return "Moderate";
  if (spad >= 10.0) return "Low";
  return "Deficient";
}

void displayResult(float spad, String status) {
  lcd.setCursor(0,0);
  lcd.write(byte(0)); lcd.print(" SPAD:"); lcd.print(spad, 1);
  lcd.setCursor(0,1);
  int bars = (int)map((long)spad, 0, 100, 0, 8);
  for (int i = 0; i < 8; i++) {
    if (i < bars) lcd.write(byte(1)); else lcd.print("-");
  }
  lcd.print(" "); lcd.print(status.substring(0, 7));
}