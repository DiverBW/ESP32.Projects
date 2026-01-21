# Garage Door Controller - Hardware Specifications

Complete hardware component list and wiring specifications for the Garage Door Controller.

---

## Target Platform

| Item | Specification |
|------|---------------|
| MCU | ESP32-C6-DevKitC-1 |
| Framework | ESP-IDF 5.x |
| Legacy Board | LOLIN(WEMOS) D1 ESP-WROOM-02 (ESP8266) |

---

## Components

### Sensors

| Component | Model | Quantity | Interface | Notes |
|-----------|-------|----------|-----------|-------|
| Door Sensor | Magnetic Reed Switch | 2 | Digital Input | Normally closed, mounted on door frame |

**Reed Switch Specifications:**
- **Type:** Normally Closed (NC)
- **Gap Distance:** Up to 20mm typical
- **Wiring:** COM and NC terminals used
- **Input Mode:** INPUT_PULLUP (internal pullup resistor)

### Actuators

| Component | Model | Quantity | Interface | Notes |
|-----------|-------|----------|-----------|-------|
| Button Relay | 5V Relay Module | 2 | Digital Output | Simulates wall button press |

**Relay Operation:**
- Momentary pulse (500ms - 1s) to simulate button press
- Connected in parallel with existing wall button
- Active LOW triggering typical

### Display

| Component | Model | Quantity | Interface | Notes |
|-----------|-------|----------|-----------|-------|
| TFT Display | ILI9341 | 1 | SPI | 240x320 TFT, 2.4" or 2.8" |

---

## Pin Assignments

### Legacy ESP8266 Pin Mapping

| Function | ESP8266 Pin | Arduino Pin | Notes |
|----------|-------------|-------------|-------|
| Large Door Sensor | GPIO13 | D7 | INPUT_PULLUP |
| Large Door Relay | GPIO0 | D3 | Active LOW |
| Small Door Sensor | GPIO14 | D5 | INPUT_PULLUP |
| Small Door Relay | GPIO12 | D6 | Active LOW |
| SPI MOSI | GPIO4 | D2 | TFT Display |
| SPI CLK | GPIO5 | D1 | TFT Display |
| Display CS | - | - | TFT Display |
| Display DC | - | - | TFT Display |
| Display RST | - | - | TFT Display |
| Status LED | GPIO2 | D4 | Onboard LED |

### Proposed ESP32-C6 Pin Assignments

| Function | GPIO | Type | Notes |
|----------|------|------|-------|
| Large Door Sensor | GPIO4 | Input | INPUT_PULLUP |
| Large Door Relay | GPIO5 | Output | Active LOW |
| Small Door Sensor | GPIO18 | Input | INPUT_PULLUP |
| Small Door Relay | GPIO19 | Output | Active LOW |
| SPI MOSI | GPIO6 | SPI | TFT Display |
| SPI CLK | GPIO7 | SPI | TFT Display |
| Display CS | GPIO10 | Output | TFT chip select |
| Display DC | GPIO11 | Output | TFT data/command |
| Display RST | GPIO21 | Output | TFT reset |
| Status LED | GPIO15 | Output | Activity indicator |

---

## Wiring Diagram

### Reed Switch Connection (Each Door)

```
Reed Switch                  ESP32-C6
┌─────────────┐            ┌──────────────────┐
│  COM        ├────────────┤ GND              │
│  NC         ├────────────┤ GPIO4 (Large)    │
│             │            │ GPIO18 (Small)   │
└─────────────┘            └──────────────────┘

Note: Using NC (Normally Closed) contacts
      Door Closed = Switch Closed = GPIO reads LOW
      Door Open = Switch Open = GPIO reads HIGH (via pullup)
```

### Relay Module Connection (Each Door)

```
Relay Module                 ESP32-C6
┌─────────────┐            ┌──────────────────┐
│  VCC        ├────────────┤ 5V (USB)         │
│  GND        ├────────────┤ GND              │
│  IN         ├────────────┤ GPIO5 (Large)    │
│             │            │ GPIO19 (Small)   │
└─────────────┘            └──────────────────┘
```

### Relay to Wall Button Connection

```
                            Existing Wall Button
Relay Module                ┌───────────────────┐
┌─────────────┐             │  ┌─────┐          │
│  COM        ├─────────────┤──┤     ├──────────┤
│  NO         ├─────────────┤──┤     │──────────┤
└─────────────┘             │  └─────┘          │
                            │   Wall Button     │
                            └───────────────────┘

Note: Relay contacts wired in PARALLEL with existing button
      Relay closes briefly to simulate button press
```

### SPI TFT Display Connection (ILI9341)

```
TFT Display (ILI9341)        ESP32-C6
┌─────────────┐            ┌──────────┐
│  VCC        ├────────────┤ 3.3V     │
│  GND        ├────────────┤ GND      │
│  MOSI       ├────────────┤ GPIO6    │
│  SCK        ├────────────┤ GPIO7    │
│  CS         ├────────────┤ GPIO10   │
│  DC         ├────────────┤ GPIO11   │
│  RST        ├────────────┤ GPIO21   │
│  LED        ├────────────┤ 3.3V     │  (or GPIO for PWM control)
└─────────────┘            └──────────┘
```

---

## Power Requirements

| Component | Voltage | Current (typ) | Current (max) |
|-----------|---------|---------------|---------------|
| ESP32-C6 | 3.3V | 80mA | 500mA (WiFi TX) |
| Reed Switch (x2) | - | 0mA | 0mA (passive) |
| 2-Channel Relay Module | 5V | 30mA | 140mA (both active) |
| TFT Display (ILI9341) | 3.3V | 50mA | 100mA (backlight) |

**Total Power Budget:** ~5V @ 1A USB power supply recommended

---

## Safety Considerations

1. **Relay Isolation:** Use optocoupler-isolated relay modules
2. **Low Voltage:** All connections are low voltage (5V logic, 12V/24V garage door)
3. **Parallel Connection:** Relay does not replace wall button, only augments
4. **Momentary Operation:** Relay pulses briefly, not latched
5. **Obstruction Detection:** Consider adding photo-eye status input (future enhancement)

---

## Installation Notes

### Reed Switch Mounting

- Mount the magnet on the moving door
- Mount the switch on the fixed door frame
- Ensure proper alignment when door is fully closed
- Gap should be < 15mm when closed
- Use NC (normally closed) configuration for fail-safe

### Button Relay Wiring

- Locate existing wall button terminals
- Wire relay COM and NO in parallel with button
- Do not remove or replace existing button
- Test manual operation after installation

---

## Bill of Materials

| Item | Quantity | Source | Notes |
|------|----------|--------|-------|
| ESP32-C6-DevKitC-1 | 1 | Espressif/DigiKey | |
| Magnetic Reed Switch | 2 | Amazon | NC type, wired |
| 2-Channel Relay Module | 1 | Amazon | Optocoupler isolated |
| ILI9341 TFT Display | 1 | Amazon | 2.4" or 2.8" SPI, 240x320 |
| USB-C Power Supply | 1 | - | 5V 1A minimum |
| Enclosure | 1 | - | Wall mount box |
| Low Voltage Wire | 50ft | Home Depot | 22 AWG, 4-conductor |

---

[DocCategory: Architecture & Design]
