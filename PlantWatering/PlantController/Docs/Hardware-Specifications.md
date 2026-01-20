# Plant Controller - Hardware Specifications

Complete hardware component list and wiring specifications for the Plant Controller.

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
| Moisture Sensor | DFRobot Waterproof Capacitive | 1 | Analog | 12-bit ADC (0-4095) |

**Moisture Sensor Specifications:**
- **Operating Voltage:** 3.3V - 5V
- **Output:** Analog voltage (inversely proportional to moisture)
- **Dry Reading:** Higher value (~2500-3000)
- **Wet Reading:** Lower value (~1000-1500)
- **Calibration Required:** Yes (dry/wet setpoints configurable via MQTT)

### Actuators

| Component | Model | Quantity | Interface | Notes |
|-----------|-------|----------|-----------|-------|
| Water Valve Relay | 5V Relay Module | 1 | Digital Output | Controls solenoid valve |

**Relay Specifications:**
- **Trigger:** Active HIGH or Active LOW (configurable)
- **Load:** 12V/24V DC Solenoid Valve
- **Isolation:** Optocoupler isolated

### Display

| Component | Model | Quantity | Interface | Notes |
|-----------|-------|----------|-----------|-------|
| LCD Display | 20x4 Character LCD | 1 | I2C (PCF8574) | Address: 0x27 |

---

## Pin Assignments

### Legacy Arduino Pin Mapping

| Function | Legacy GPIO | ESP32-C6 GPIO | Notes |
|----------|-------------|---------------|-------|
| Moisture Sensor | GPIO36 (VP) | ADC1_CH0 (GPIO0) | Analog input |
| Valve Relay | GPIO19 | GPIO19 | Digital output |
| I2C SDA | GPIO21 | GPIO6 | LCD Display |
| I2C SCL | GPIO22 | GPIO7 | LCD Display |

### Proposed ESP32-C6 Pin Assignments

| Function | GPIO | Type | Notes |
|----------|------|------|-------|
| Moisture Sensor | GPIO0 | ADC | ADC1 Channel 0 |
| Valve Relay | GPIO8 | Output | Active HIGH |
| I2C SDA | GPIO6 | I2C | Shared bus |
| I2C SCL | GPIO7 | I2C | Shared bus |
| Status LED | GPIO15 | Output | Onboard or external |

---

## Wiring Diagram

### Moisture Sensor Connection

```
Moisture Sensor              ESP32-C6
┌─────────────┐            ┌──────────┐
│  VCC (Red)  ├────────────┤ 3.3V     │
│  GND (Black)├────────────┤ GND      │
│  SIG (Yellow)├───────────┤ GPIO0    │
└─────────────┘            └──────────┘
```

### Relay Module Connection

```
Relay Module                 ESP32-C6
┌─────────────┐            ┌──────────┐
│  VCC        ├────────────┤ 5V (USB) │
│  GND        ├────────────┤ GND      │
│  IN         ├────────────┤ GPIO8    │
└─────────────┘            └──────────┘

                            Solenoid Valve
Relay Module                ┌──────────┐
┌─────────────┐             │          │
│  COM        ├─────────────┤ Terminal │
│  NO         ├──┬──────────┤ Terminal │
└─────────────┘  │          └──────────┘
                 │
              12V/24V
              Power Supply
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
| Moisture Sensor | 3.3V | 5mA | 10mA |
| Relay Module | 5V | 15mA | 70mA (coil active) |
| LCD Display | 5V | 20mA | 200mA (backlight) |

**Total Power Budget:** ~5V @ 1A USB power supply recommended

---

## Calibration Values

Stored in NVS namespace `bw_plant`:

| Key | Description | Typical Value |
|-----|-------------|---------------|
| `dry` | ADC reading for 0% moisture | 2800 |
| `wet` | ADC reading for 100% moisture | 1200 |
| `setpoint` | Target moisture % | 40 |
| `water_time` | Watering duration (seconds) | 30 |
| `soak_time` | Soak period (minutes) | 15 |

---

## Bill of Materials

| Item | Quantity | Source | Notes |
|------|----------|--------|-------|
| ESP32-C6-DevKitC-1 | 1 | Espressif/DigiKey | |
| DFRobot Moisture Sensor | 1 | DFRobot/Amazon | SKU: SEN0193 |
| 5V Relay Module | 1 | Amazon | Optocoupler isolated |
| 20x4 LCD + I2C Backpack | 1 | Amazon | PCF8574 based |
| 12V Solenoid Valve | 1 | Amazon | Normally closed |
| 12V Power Supply | 1 | Amazon | For valve |
| USB-C Power Supply | 1 | - | 5V 1A minimum |
| Enclosure | 1 | - | IP65 recommended |

---

[DocCategory: Architecture & Design]
