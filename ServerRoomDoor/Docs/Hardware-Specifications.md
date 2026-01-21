# Server Room Door Controller - Hardware Specifications

Complete hardware component list and wiring specifications for the Server Room Door Controller.

---

## Target Platform

| Item | Specification |
|------|---------------|
| MCU | ESP32-C6-DevKitC-1 |
| Framework | ESP-IDF 5.x |
| Legacy Board | ESP32 Node32s (Arduino) |

---

## Components

### Motor System

| Component | Model | Quantity | Interface | Notes |
|-----------|-------|----------|-----------|-------|
| Linear Actuator | 12V DC Linear Actuator | 1 | DC Motor | Stroke length per application |
| Motor Driver | L298N H-Bridge | 1 | 2x Digital Output | Direction control via IN1/IN2 |

**L298N Motor Driver:**
- **Logic Voltage:** 5V
- **Motor Voltage:** 12V (separate supply)
- **Control:** IN1/IN2 for direction, ENA for speed (optional)

| IN1 | IN2 | Motor Action |
|-----|-----|--------------|
| LOW | LOW | Stop (coast) |
| HIGH | LOW | Forward (open) |
| LOW | HIGH | Reverse (close) |
| HIGH | HIGH | Stop (brake) |

### Position Sensors

| Component | Model | Quantity | Interface | Notes |
|-----------|-------|----------|-----------|-------|
| Door Open Switch | Microswitch | 1 | Digital Input | Detects fully open |
| Door Closed Switch | Microswitch | 1 | Digital Input | Detects fully closed |

**Microswitch Configuration:**
- **Type:** Normally Open (NO) or Normally Closed (NC)
- **Mounting:** Actuated when door reaches limit position
- **Input Mode:** INPUT_PULLUP

### Control Inputs

| Component | Model | Quantity | Interface | Notes |
|-----------|-------|----------|-----------|-------|
| Open Button | Momentary Pushbutton | 1 | Digital Input | Manual open |
| Close Button | Momentary Pushbutton | 1 | Digital Input | Manual close |
| Mode Switch | Toggle Switch | 1 | Digital Input | Auto/Manual mode |

### Sensors

| Component | Model | Quantity | Interface | Notes |
|-----------|-------|----------|-----------|-------|
| AVR Power Sensor | Voltage Divider | 1 | Analog Input | Detects AVR 3.3V power |

**AVR Power Detection:**
- **Input:** 3.3V from AVR power indicator
- **Threshold:** ~1.5V indicates AVR is ON

### Display

| Component | Model | Quantity | Interface | Notes |
|-----------|-------|----------|-----------|-------|
| TFT Display | ILI9341 | 1 | SPI | 240x320 TFT, 2.4" or 2.8" |

---

## Pin Assignments

### Legacy ESP32 Pin Mapping

| Function | Legacy GPIO | Notes |
|----------|-------------|-------|
| Motor IN1 | GPIO33 | L298N Input 1 |
| Motor IN2 | GPIO27 | L298N Input 2 |
| Open Button | GPIO16 | INPUT_PULLUP |
| Close Button | GPIO19 | INPUT_PULLUP |
| Door Open Switch | GPIO25 | INPUT_PULLUP |
| Door Closed Switch | GPIO23 | INPUT_PULLUP |
| Mode Switch | GPIO26 | INPUT_PULLUP |
| AVR Power Sense | GPIO32 | Analog input |
| SPI MOSI | GPIO21 | TFT Display |
| SPI CLK | GPIO22 | TFT Display |
| Display CS | GPIO10 | TFT Display |
| Display DC | GPIO11 | TFT Display |
| Display RST | GPIO12 | TFT Display |

### ESP32-C6 Pin Assignments (BW Framework Standard)

**Updated:** January 20, 2026 - Remapped to BW Framework standard GPIO assignments

| Function | GPIO | Type | Notes |
|----------|------|------|-------|
| Motor IN1 | GPIO11 | Output | L298N direction |
| Motor IN2 | GPIO20 | Output | L298N direction |
| Open Button | GPIO0 | Input | INPUT_PULLUP |
| Close Button | GPIO1 | Input | INPUT_PULLUP |
| Door Open Switch | GPIO21 | Input | INPUT_PULLUP (touch pin if no touch) |
| Door Closed Switch | GPIO22 | Input | INPUT_PULLUP (touch pin if no touch) |
| Mode Switch | GPIO23 | Input | INPUT_PULLUP (touch pin if no touch) |
| AVR Power Sense | GPIO2 | ADC | Analog input |
| SPI MOSI | GPIO7 | SPI | TFT Display (BW Framework) |
| SPI SCK | GPIO6 | SPI | TFT Display (BW Framework) |
| Display CS | GPIO10 | Output | TFT chip select |
| Display DC | GPIO4 | Output | TFT data/command (BW Framework) |
| Display RST | GPIO3 | Output | TFT reset (BW Framework) |
| Display Backlight | GPIO5 | Output | PWM capable (BW Framework) |
| I2C SDA | GPIO18 | I2C | For I2C peripherals |
| I2C SCL | GPIO19 | I2C | For I2C peripherals |

**Note:** Pin assignments follow the BW Framework standard for C6-Standard-Breakout board compatibility.
GPIO21-23 are shared with touch panel pins - use only if display has no touch.

---

## Wiring Diagram

### L298N Motor Driver Connection (BW Framework Standard)

```
L298N Module                 ESP32-C6           12V Power
┌─────────────┐            ┌──────────┐      ┌──────────┐
│  +12V       ├────────────┼──────────┼──────┤ +12V     │
│  GND        ├────────────┤ GND      ├──────┤ GND      │
│  +5V        ├────────────┤ 5V (USB) │      └──────────┘
│  IN1        ├────────────┤ GPIO11   │
│  IN2        ├────────────┤ GPIO20   │
│  ENA        ├──(Jumper)──┤          │  (Keep jumper for full speed)
└─────────────┘            └──────────┘

L298N to Linear Actuator:
┌─────────────┐            ┌───────────────────┐
│  OUT1       ├────────────┤ Motor Terminal 1  │
│  OUT2       ├────────────┤ Motor Terminal 2  │
└─────────────┘            └───────────────────┘
```

### Microswitch Connection (Position Sensors) - BW Framework Standard

```
Microswitch                  ESP32-C6
┌─────────────┐            ┌────────────────────┐
│  COM        ├────────────┤ GND                │
│  NO         ├────────────┤ GPIO21 (Open SW)   │
│             │            │ GPIO22 (Closed SW) │
└─────────────┘            └────────────────────┘

Note: Using NO (Normally Open) contacts with INPUT_PULLUP
      Switch actuated = GPIO reads LOW
      Switch not actuated = GPIO reads HIGH
      GPIO21-22 are touch panel pins (available if no touch)
```

### Control Buttons and Mode Switch (BW Framework Standard)

```
Pushbuttons/Switch           ESP32-C6
┌─────────────┐            ┌──────────────────┐
│ Open BTN    ├────────────┤ GPIO0            │
│             ├────GND─────┤ GND              │
├─────────────┤            │                  │
│ Close BTN   ├────────────┤ GPIO1            │
│             ├────GND─────┤ GND              │
├─────────────┤            │                  │
│ Mode Switch ├────────────┤ GPIO23           │
│             ├────GND─────┤ GND              │
└─────────────┘            └──────────────────┘

Note: All inputs use INPUT_PULLUP
      Button pressed / Switch closed = GPIO reads LOW
      GPIO23 is touch panel pin (available if no touch)
```

### AVR Power Sensing (BW Framework Standard)

```
AVR Indicator LED            Voltage Divider      ESP32-C6
┌─────────────┐            ┌─────────────────┐   ┌──────────┐
│  3.3V Out   ├────────────┤ R1 (10K)        │   │          │
│             │            │     ├───────────┼───┤ GPIO2    │
│  GND        ├────────────┤ R2 (10K) ── GND │   │          │
└─────────────┘            └─────────────────┘   └──────────┘

Note: Voltage divider optional if AVR output is already 3.3V
      With divider: 3.3V → 1.65V (safe for ADC)
      ADC threshold ~1000 indicates AVR ON
```

### SPI TFT Display Connection (ILI9341) - BW Framework Standard

```
TFT Display (ILI9341)        ESP32-C6
┌─────────────┐            ┌──────────┐
│  VCC        ├────────────┤ 3.3V     │
│  GND        ├────────────┤ GND      │
│  MOSI       ├────────────┤ GPIO7    │
│  SCK        ├────────────┤ GPIO6    │
│  CS         ├────────────┤ GPIO10   │
│  DC         ├────────────┤ GPIO4    │
│  RST        ├────────────┤ GPIO3    │
│  LED        ├────────────┤ GPIO5    │  (PWM for brightness control)
└─────────────┘            └──────────┘
```

---

## Power Requirements

| Component | Voltage | Current (typ) | Current (max) |
|-----------|---------|---------------|---------------|
| ESP32-C6 | 3.3V | 80mA | 500mA (WiFi TX) |
| L298N Logic | 5V | 50mA | 100mA |
| Linear Actuator | 12V | 500mA | 3A (stall) |
| TFT Display (ILI9341) | 3.3V | 50mA | 100mA (backlight) |
| Buttons/Switches | - | 0mA | 0mA (passive) |

**Power Supplies Required:**
1. **5V USB:** For ESP32-C6 and TFT display (~1A recommended)
2. **12V DC:** For linear actuator via L298N (~5A for margin)

---

## Motor State Machine

```
                    ┌─────────────────┐
                    │  MOTOR_UNKNOWN  │
                    └────────┬────────┘
                             │ (on boot)
                             ▼
        ┌────────────────────┴────────────────────┐
        │                                         │
        ▼                                         ▼
┌───────────────┐                         ┌───────────────┐
│  MOTOR_OFF    │◄────────────────────────│  MOTOR_OFF    │
└───────┬───────┘                         └───────────────┘
        │                                         ▲
        │ (Open CMD)                              │ (Limit reached)
        ▼                                         │
┌───────────────┐                         ┌───────────────┐
│ MOTOR_OPENING │─────────────────────────│ MOTOR_CLOSING │
└───────────────┘  (Close CMD or Auto)    └───────────────┘
```

---

## Bill of Materials

| Item | Quantity | Source | Notes |
|------|----------|--------|-------|
| ESP32-C6-DevKitC-1 | 1 | Espressif/DigiKey | |
| 12V Linear Actuator | 1 | Amazon | Select stroke length |
| L298N Motor Driver | 1 | Amazon | Dual H-Bridge |
| Microswitch | 2 | Amazon | Limit switches |
| Momentary Pushbutton | 2 | Amazon | Panel mount |
| Toggle Switch | 1 | Amazon | SPST, panel mount |
| ILI9341 TFT Display | 1 | Amazon | 2.4" or 2.8" SPI, 240x320 |
| 12V 5A Power Supply | 1 | Amazon | For motor |
| USB-C Power Supply | 1 | - | 5V 1A for logic |
| Enclosure | 1 | - | Project box |

---

[DocCategory: Architecture & Design]
