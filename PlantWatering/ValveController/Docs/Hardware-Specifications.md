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

### Sensors

| Component | Model | Quantity | Interface | Notes |
|-----------|-------|----------|-----------|-------|
| Pressure Sensor | 5V Analog Pressure Transducer | 2 | Analog | 0-100 PSI range |

**Pressure Sensor Specifications:**
- **Operating Voltage:** 5V
- **Output:** 0.5V - 4.5V (linear with pressure)
- **Range:** 0-100 PSI typical
- **Thread:** 1/4" NPT

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
| LCD Display | 20x4 Character LCD | 1 | I2C (PCF8574) | Address: 0x27 |

---

## Pin Assignments

### Legacy Arduino Pin Mapping

| Function | Legacy GPIO | Notes |
|----------|-------------|-------|
| Irrigation Pressure Sensor | GPIO33 | Analog input |
| Drip Pressure Sensor | GPIO34 | Analog input |
| Irrigation Valve Relay | GPIO27 | Digital output |
| Drip Valve Relay | GPIO15 | Digital output |
| Irrigation LED Relay | GPIO16 | Digital output |
| Drip LED Relay | GPIO32 | Digital output |
| I2C SDA | GPIO21 | LCD Display |
| I2C SCL | GPIO22 | LCD Display |

### Proposed ESP32-C6 Pin Assignments

| Function | GPIO | Type | Notes |
|----------|------|------|-------|
| Irrigation Pressure | GPIO0 | ADC | ADC1 Channel 0 |
| Drip Pressure | GPIO1 | ADC | ADC1 Channel 1 |
| Irrigation Valve | GPIO8 | Output | Active HIGH |
| Drip Valve | GPIO9 | Output | Active HIGH |
| Irrigation LED | GPIO10 | Output | Water available |
| Drip LED | GPIO11 | Output | Water available |
| I2C SDA | GPIO6 | I2C | Shared bus |
| I2C SCL | GPIO7 | I2C | Shared bus |
| Status LED | GPIO15 | Output | Onboard or external |

---

## Wiring Diagram

### Pressure Sensor Connection

```
Pressure Sensor (x2)         ESP32-C6
┌─────────────┐            ┌──────────────┐
│  VCC (Red)  ├────────────┤ 5V (USB)     │
│  GND (Black)├────────────┤ GND          │
│  SIG (Yellow)├───────────┤ GPIO0 (Irrig)│
│             │            │ GPIO1 (Drip) │
└─────────────┘            └──────────────┘

Note: Pressure sensors output 0.5-4.5V, requires voltage divider
      for 3.3V ADC input or use with ADC attenuation.
```

### 4-Channel Relay Module Connection

```
4-Channel Relay Module       ESP32-C6
┌─────────────────┐        ┌──────────────┐
│  VCC            ├────────┤ 5V (USB)     │
│  GND            ├────────┤ GND          │
│  IN1 (Irrig Valve)├──────┤ GPIO8        │
│  IN2 (Drip Valve) ├──────┤ GPIO9        │
│  IN3 (Irrig LED)  ├──────┤ GPIO10       │
│  IN4 (Drip LED)   ├──────┤ GPIO11       │
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

### I2C LCD Connection

```
LCD Module (PCF8574)         ESP32-C6
┌─────────────┐            ┌──────────┐
│  VCC        ├────────────┤ 5V (USB) │
│  GND        ├────────────┤ GND      │
│  SDA        ├────────────┤ GPIO6    │
│  SCL        ├────────────┤ GPIO7    │
└─────────────┘            └──────────┘
```

---

## Power Requirements

| Component | Voltage | Current (typ) | Current (max) |
|-----------|---------|---------------|---------------|
| ESP32-C6 | 3.3V | 80mA | 500mA (WiFi TX) |
| Pressure Sensor (x2) | 5V | 10mA each | 20mA each |
| 4-Channel Relay Module | 5V | 60mA | 280mA (all active) |
| LCD Display | 5V | 20mA | 200mA (backlight) |

**Total Power Budget:** ~5V @ 2A USB power supply recommended

**Valve Power:** 24VAC transformer (separate from logic power)

---

## Pressure Sensor Calibration

Stored in NVS namespace `bw_valve`:

| Key | Prefix | Description |
|-----|--------|-------------|
| `SLV` | i/d | Sensor Low Value (ADC at 0 PSI) |
| `SHV` | i/d | Sensor High Value (ADC at max PSI) |
| `PLV` | i/d | Pressure Low Value (PSI at low ADC) |
| `PHV` | i/d | Pressure High Value (PSI at high ADC) |
| `PT` | i/d | Pressure Threshold (water available) |

Prefix: `i` = Irrigation, `d` = Drip

---

## Bill of Materials

| Item | Quantity | Source | Notes |
|------|----------|--------|-------|
| ESP32-C6-DevKitC-1 | 1 | Espressif/DigiKey | |
| Pressure Transducer 0-100PSI | 2 | Amazon | 1/4" NPT, 0.5-4.5V |
| 4-Channel Relay Module | 1 | Amazon | Optocoupler isolated |
| 20x4 LCD + I2C Backpack | 1 | Amazon | PCF8574 based |
| 24VAC Transformer | 1 | - | For valve solenoids |
| USB-C Power Supply | 1 | - | 5V 2A minimum |
| Enclosure | 1 | - | NEMA rated for outdoor |
| 1/4" NPT Tee Fittings | 2 | Home Depot | For sensor installation |

---

[DocCategory: Architecture & Design]
