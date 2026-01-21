# Valve Controller - Hardware Specifications

Complete hardware component list and wiring specifications for the Valve Controller.

---

## Target Platform

| Item | Specification |
|------|---------------|
| MCU | ESP32-C6-DevKitC-1 |
| Framework | ESP-IDF 5.x |
| Legacy Board | ESP32 Node32s (Arduino) |

---

## Components

### Actuators

| Component | Model | Quantity | Interface | Notes |
|-----------|-------|----------|-----------|-------|
| Irrigation Valve Relay | 5V Relay Module | 1 | Digital Output | Master irrigation valve |
| Drip Valve Relay | 5V Relay Module | 1 | Digital Output | Drip system valve |
| Irrigation Status LED | 5V Relay Module | 1 | Digital Output | Water available indicator |
| Drip Status LED | 5V Relay Module | 1 | Digital Output | Water available indicator |

### Display

| Component | Model | Quantity | Interface | Notes |
|-----------|-------|----------|-----------|-------|
| TFT Display | ILI9341 | 1 | SPI | 240x320 TFT, 2.4" or 2.8" |

---

## Pin Assignments

### Legacy Arduino Pin Mapping

| Function | Legacy GPIO | Notes |
|----------|-------------|-------|
| Irrigation Valve Relay | GPIO27 | Digital output |
| Drip Valve Relay | GPIO15 | Digital output |
| Irrigation LED Relay | GPIO16 | Digital output |
| Drip LED Relay | GPIO32 | Digital output |
| SPI MOSI | GPIO21 | TFT Display |
| SPI CLK | GPIO22 | TFT Display |
| Display CS | GPIO10 | TFT Display |
| Display DC | GPIO11 | TFT Display |
| Display RST | GPIO12 | TFT Display |

### ESP32-C6 Pin Assignments (BW Framework Standard)

**Updated:** January 20, 2026 - Remapped to BW Framework standard GPIO assignments

| Function | GPIO | Type | Notes |
|----------|------|------|-------|
| Irrigation Valve | GPIO11 | Output | Active HIGH |
| Drip Valve | GPIO20 | Output | Active HIGH |
| Irrigation LED | GPIO21 | Output | Water available (touch pin if no touch) |
| Drip LED | GPIO22 | Output | Water available (touch pin if no touch) |
| SPI MOSI | GPIO7 | SPI | TFT Display (BW Framework) |
| SPI SCK | GPIO6 | SPI | TFT Display (BW Framework) |
| Display CS | GPIO10 | Output | TFT chip select |
| Display DC | GPIO4 | Output | TFT data/command (BW Framework) |
| Display RST | GPIO3 | Output | TFT reset (BW Framework) |
| Display Backlight | GPIO5 | Output | PWM capable (BW Framework) |
| I2C SDA | GPIO18 | I2C | For I2C peripherals |
| I2C SCL | GPIO19 | I2C | For I2C peripherals |

**Note:** Pin assignments follow the BW Framework standard for C6-Standard-Breakout board compatibility.
GPIO21-22 are shared with touch panel pins - use only if display has no touch.

---

## Wiring Diagram

### 4-Channel Relay Module Connection (BW Framework Standard)

```
4-Channel Relay Module       ESP32-C6
┌─────────────────┐        ┌──────────────┐
│  VCC            ├────────┤ 5V (USB)     │
│  GND            ├────────┤ GND          │
│  IN1 (Irrig Valve)├──────┤ GPIO11       │
│  IN2 (Drip Valve) ├──────┤ GPIO20       │
│  IN3 (Irrig LED)  ├──────┤ GPIO21       │
│  IN4 (Drip LED)   ├──────┤ GPIO22       │
└─────────────────┘        └──────────────┘
```

### Valve Wiring (Each Valve)

```
Relay Module                  Master Valve
┌─────────────┐              ┌──────────┐
│  COM        ├───── 24VAC ──┤ Common   │
│  NO         ├──────────────┤ Hot      │
└─────────────┘              └──────────┘

              └── 24VAC Transformer
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
| 4-Channel Relay Module | 5V | 60mA | 280mA (all active) |
| TFT Display (ILI9341) | 3.3V | 50mA | 100mA (backlight) |

**Total Power Budget:** ~5V @ 2A USB power supply recommended

**Valve Power:** 24VAC transformer (separate from logic power)

---

## Bill of Materials

| Item | Quantity | Source | Notes |
|------|----------|--------|-------|
| ESP32-C6-DevKitC-1 | 1 | Espressif/DigiKey | |
| 4-Channel Relay Module | 1 | Amazon | Optocoupler isolated |
| ILI9341 TFT Display | 1 | Amazon | 2.4" or 2.8" SPI, 240x320 |
| 24VAC Transformer | 1 | - | For valve solenoids |
| USB-C Power Supply | 1 | - | 5V 2A minimum |
| Enclosure | 1 | - | NEMA rated for outdoor |

---

[DocCategory: Architecture & Design]
