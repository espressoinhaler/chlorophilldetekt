# 🌿 Leaf Chlorophyll Detector

A portable, low-cost sensor that estimates leaf chlorophyll content using transmitted green light — the same principle as a laboratory SPAD meter, built from components costing under **₹300**.

---

## Overview

This Arduino Nano project shines a green LED through a leaf and uses an LDR (light-dependent resistor) to measure how much light is absorbed. Chlorophyll selectively absorbs green wavelengths, so the more chlorophyll present, the less light reaches the sensor. The firmware applies the Beer–Lambert law to derive a SPAD-style index (0–99.9) and displays the result on a 16×2 I2C LCD with a bar graph and health classification.

---

## Features

- 📊 Real-time SPAD index displayed on 16×2 I2C LCD
- 🌱 5-level health classification: Excellent → Deficient
- 📉 Visual bar graph on LCD row 2
- 🔌 Serial output for data logging
- ⚡ Baseline calibration on startup
- 💸 BOM cost under ₹300

---

## Hardware

### Bill of Materials

| Component | Quantity |
|---|---|
| Arduino Nano | 1 |
| 16×2 I2C LCD (address `0x27`) | 1 |
| LDR (photoresistor) | 1 |
| Green LED | 1 |
| 10kΩ resistor (LDR pull-down) | 1 |
| 220Ω resistor (LED current limiting) | 1 |
| Breadboard + jumper wires | — |

### Wiring

| Component | Arduino Pin |
|---|---|
| LDR (with 10kΩ to GND) | A0 |
| Green LED anode (220Ω series) | D9 (PWM) |
| Green LED cathode | GND |
| LCD SDA | A4 |
| LCD SCL | A5 |
| LCD VCC | 5V |
| LCD GND | GND |

---

## How It Works

**1. Baseline calibration**
With no leaf present, the green LED illuminates the LDR directly. This ADC value becomes the reference transmittance I₀ — the maximum possible light reaching the sensor.

**2. Leaf insertion**
A leaf is placed between the LED and LDR. Chlorophyll molecules selectively absorb green wavelengths, reducing transmitted intensity I. The ADC records this lower value.

**3. Absorbance calculation**
The Beer–Lambert law is applied:

```
Absorbance = −log₁₀(I / I₀)
SPAD Index = Absorbance × 100   →   range: 0 – 99.9
```

**4. Health classification**
The SPAD value is mapped to a health category and shown on the LCD.

---

## SPAD Classification

| SPAD Index | Status | Interpretation |
|---|---|---|
| 50 – 99.9 | 🟢 Excellent | Peak photosynthetic capacity |
| 35 – 49.9 | 🟩 Healthy | Adequate chlorophyll levels |
| 20 – 34.9 | 🟡 Moderate | Mild nitrogen deficiency likely |
| 10 – 19.9 | 🟠 Low | Chlorosis, fertilisation advised |
| 0 – 9.9 | 🔴 Deficient | Severe deficiency or dead leaf |

---

## Software

### Dependencies

Install via Arduino IDE Library Manager:

- [`LiquidCrystal_I2C`](https://github.com/johnrickman/LiquidCrystal_I2C) by John Rickman
- `Wire.h` — built into Arduino IDE

### Setup & Upload

1. Clone this repository
   ```bash
   git clone https://github.com/your-username/leaf-chlorophyll-detector.git
   ```
2. Open `chlorophyll_detector.ino` in the Arduino IDE
3. Install the `LiquidCrystal_I2C` library
4. Select **Board:** Arduino Nano and the correct **Port**
5. Upload the sketch

### Calibration Constants

You can tune these at the top of the sketch:

```cpp
const int SAMPLES        = 20;   // ADC readings averaged per measurement
const int LED_BRIGHTNESS = 200;  // Green LED PWM (0–255)
```

---

## Usage

1. Power on the Arduino — the LCD shows "Chlorophyll Detector" briefly
2. **Calibrate:** remove any leaf and wait; the device averages 20 ADC readings to set the baseline
3. **Measure:** place a leaf between the LED and LDR when prompted; hold steady for ~3 seconds
4. The LCD displays the SPAD index and health status; Serial Monitor (9600 baud) logs both values

---

## Serial Output

```
SPAD: 42.7  Status: Healthy
SPAD: 18.3  Status: Low
```

---

## Project Structure

```
leaf-chlorophyll-detector/
├── chlorophyll_detector.ino   # Main Arduino sketch
├── README.md
└── LICENSE
```

---

## License

MIT License — free to use, modify, and distribute.
