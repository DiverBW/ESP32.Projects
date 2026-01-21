# Hardware Wiring Guide

**Project:** Parking Assistant
**Target MCU:** ESP32-C6
**Created:** January 13, 2026
**Status:** SUPERSEDED

---

## IMPORTANT NOTICE

**This document describes the original single-board dual-slot architecture, which has been superseded.**

See [Architecture-Decision-Dual-Deployment.md](Architecture-Decision-Dual-Deployment.md) for the current design using:
- Two separate ESP32-C6 boards (one per parking slot)
- BW Framework standard GPIO assignments
- C6-Standard-Breakout board compatibility

The information below is retained for reference only.

---

**Original Status:** Design

---

## Overview

This document defines the complete hardware architecture for a dual-garage parking assistant system. The system guides drivers into correct parking positions using LED strip indicators and 120V light towers, with distance sensing provided by Time-of-Flight sensors.

### System Capabilities

- Two independent garage door stations (large and small garage)
- Visual guidance via 180-LED WS2812B strips per door
- 4-light tower per door (Blue/Yellow/Green/Red) for approach indication
- Distance sensing via I2C ToF sensors
- LCD color display for status/configuration
- Optional button for UI navigation

---

## Component Summary

| Component | Quantity | Purpose |
|-----------|----------|---------|
| ESP32-C6 DevKit | 1 | Main controller |
| VL53L1X ToF Sensor | 2 | Distance measurement (one per door) |
| WS2812B LED Strip (180 LEDs) | 2 | Visual parking guidance (one per door) |
| MCP23017 I2C GPIO Expander | 1 | Relay control expansion |
| 8-Channel SSR Module | 1 | 120V light tower control |
| LCD Display (ST7789 or ILI9341) | 1 | Status display |
| Tactile Button | 1 | UI navigation (optional) |
| Logic Level Shifter (3.3V→5V) | 2 | LED strip signal integrity |
| 5V Power Supply | 1 | LED strip power (sized appropriately) |

---

## GPIO Pin Allocation

### ESP32-C6 Pin Assignment

| GPIO | Function | Component | Notes |
|------|----------|-----------|-------|
| GPIO0 | I2C SDA | Shared Bus | ToF sensors + MCP23017 |
| GPIO1 | I2C SCL | Shared Bus | ToF sensors + MCP23017 |
| GPIO2 | XSHUT_1 | VL53L1X #1 | Large garage ToF shutdown |
| GPIO3 | XSHUT_2 | VL53L1X #2 | Small garage ToF shutdown |
| GPIO6 | SPI2 MOSI | LED Strip #1 | Large garage LEDs (via level shifter) |
| GPIO7 | SPI2 CLK | LCD Display | Shared with LCD |
| GPIO8 | SPI2 CS | LCD Display | LCD chip select |
| GPIO9 | LCD DC | LCD Display | Data/Command select |
| GPIO10 | LCD RST | LCD Display | LCD reset |
| GPIO11 | LCD BL | LCD Display | Backlight control (optional) |
| GPIO18 | SPI3 MOSI | LED Strip #2 | Small garage LEDs (via level shifter) |
| GPIO21 | Button | UI Input | Pull-up enabled (optional) |

**Reserved/Unavailable Pins:**
- GPIO12-17: Used for internal flash (not available)
- GPIO4-5: USB-JTAG (keep available for debugging)

### I2C Address Allocation

| Address | Device | Notes |
|---------|--------|-------|
| 0x29 | VL53L1X #1 | Default address (assigned first at boot) |
| 0x30 | VL53L1X #2 | Changed via XSHUT sequencing |
| 0x20 | MCP23017 | A0=A1=A2=GND |

---

## I2C Bus Architecture

### Overview

All I2C devices share a single bus (SDA/SCL) with individual addressing. The VL53L1X sensors require XSHUT pin sequencing at boot to assign unique addresses.

### ToF Sensor Address Assignment Sequence

1. Power on with both XSHUT pins LOW (sensors in shutdown)
2. Set XSHUT_1 HIGH → VL53L1X #1 wakes at default 0x29
3. Change VL53L1X #1 address to 0x30 via I2C command
4. Set XSHUT_2 HIGH → VL53L1X #2 wakes at default 0x29
5. Both sensors now accessible at unique addresses

### I2C Wiring Diagram

```
ESP32-C6                    VL53L1X #1 (Large Garage)
┌─────────┐                 ┌─────────┐
│    GPIO0├────────┬────────┤ SDA     │
│    GPIO1├────────┼────┬───┤ SCL     │
│    GPIO2├────────┼────┼───┤ XSHUT   │
│         │        │    │   │ VIN─────┼──── 3.3V
│         │        │    │   │ GND─────┼──── GND
└─────────┘        │    │   └─────────┘
                   │    │
                   │    │   VL53L1X #2 (Small Garage)
                   │    │   ┌─────────┐
                   ├────┼───┤ SDA     │
                   │    ├───┤ SCL     │
           GPIO3───┼────┼───┤ XSHUT   │
                   │    │   │ VIN─────┼──── 3.3V
                   │    │   │ GND─────┼──── GND
                   │    │   └─────────┘
                   │    │
                   │    │   MCP23017
                   │    │   ┌─────────┐
                   └────┼───┤ SDA     │
                        └───┤ SCL     │
                            │ A0──────┼──── GND
                            │ A1──────┼──── GND
                            │ A2──────┼──── GND
                            │ VDD─────┼──── 3.3V
                            │ VSS─────┼──── GND
                            │ RESET───┼──── 3.3V (pull-up)
                            └─────────┘
```

### I2C Pull-up Resistors

Use 4.7kΩ pull-up resistors on SDA and SCL to 3.3V. Most breakout boards include these, so check before adding external resistors to avoid over-pulling the bus.

---

## LED Strip Wiring

### Signal Integrity Requirements

The ESP32-C6 operates at 3.3V logic, but WS2812B LEDs expect 5V logic signals. While they often work at 3.3V, signal integrity degrades over long strips. A logic level shifter is **strongly recommended** for reliable operation.

### Level Shifter Wiring

```
ESP32-C6          Level Shifter          WS2812B Strip
┌─────────┐       ┌───────────┐          ┌─────────────┐
│    GPIO6├───────┤ LV1   HV1 ├──────────┤ DIN         │
│     3.3V├───────┤ LV    HV  ├───── 5V  │             │
│      GND├───────┤ GND   GND ├──────────┤ GND         │
└─────────┘       └───────────┘          │             │
                                     5V ─┤ VCC         │
                                         └─────────────┘
```

**Recommended Level Shifter:** 74AHCT125 or similar unidirectional buffer

### Power Considerations

Each WS2812B LED draws up to 60mA at full white brightness. For 180 LEDs:
- **Maximum draw:** 180 × 60mA = 10.8A at full white
- **Typical draw:** 2-4A with color patterns and brightness limiting
- **Recommended PSU:** 5V 10A minimum per strip (20A total for both strips)

**Power Injection:** For 180-LED strips, inject power at both ends and middle to prevent voltage drop and color shift.

### LED Strip Wiring Diagram

```
5V Power Supply (10A)
┌─────────────────────┐
│  +5V ──────┬────────┼─────────────────┬─────────────────┐
│            │        │                 │                 │
│            │        │   WS2812B Strip (180 LEDs)        │
│            │        │   ┌───┬───┬───┬───┬───┬───┐       │
│            │        └───┤VCC│   │   │VCC│   │VCC├───────┘
│            │            │   │   │...│   │...│   │
│  Level     │        ┌───┤DIN│   │   │   │   │   │
│  Shifter   │        │   │   │   │   │   │   │   │
│  ┌─────┐   │        │   │GND│   │   │GND│   │GND│
│  │ HV1 ├───┼────────┘   └─┬─┴───┴───┴─┬─┴───┴─┬─┘
│  │     │   │              │           │       │
│  └─────┘   │              │           │       │
│  GND ──────┴──────────────┴───────────┴───────┘
└─────────────────────┘

LED #0 = Garage door end (first LED near door)
LED #179 = Rear wall end (last LED at parking stop)
```

---

## MCP23017 GPIO Expander

### Overview

The MCP23017 provides 16 additional GPIO pins over I2C. We use 8 pins (one port) to control the SSR relay module for the light towers.

### Port Assignment

| Port | Pin | Relay | Light | Garage |
|------|-----|-------|-------|--------|
| GPA0 | 21 | SSR1 | Blue | Large |
| GPA1 | 22 | SSR2 | Yellow | Large |
| GPA2 | 23 | SSR3 | Green | Large |
| GPA3 | 24 | SSR4 | Red | Large |
| GPA4 | 25 | SSR5 | Blue | Small |
| GPA5 | 26 | SSR6 | Yellow | Small |
| GPA6 | 27 | SSR7 | Green | Small |
| GPA7 | 28 | SSR8 | Red | Small |
| GPB0-7 | - | Reserved | Future Use | - |

### MCP23017 Wiring

```
MCP23017                  8-Channel SSR Module
┌─────────┐               ┌─────────────────────┐
│    GPA0 ├───────────────┤ IN1 (Blue Large)    │
│    GPA1 ├───────────────┤ IN2 (Yellow Large)  │
│    GPA2 ├───────────────┤ IN3 (Green Large)   │
│    GPA3 ├───────────────┤ IN4 (Red Large)     │
│    GPA4 ├───────────────┤ IN5 (Blue Small)    │
│    GPA5 ├───────────────┤ IN6 (Yellow Small)  │
│    GPA6 ├───────────────┤ IN7 (Green Small)   │
│    GPA7 ├───────────────┤ IN8 (Red Small)     │
│         │               │                     │
│     VDD ├──── 3.3V      │ VCC ────────────────┼─── 3.3V or 5V*
│     VSS ├──── GND       │ GND ────────────────┼─── GND
└─────────┘               └─────────────────────┘

* Check SSR module input voltage requirements
```

---

## 120V Light Tower Wiring

### Safety Warning

**⚠️ HIGH VOLTAGE - 120V AC**

This section involves line voltage wiring. All 120V connections must be:
- Made in proper junction boxes
- Performed by qualified personnel
- In compliance with local electrical codes

### Light Tower Connection

Each light tower has 4 lights (Blue, Yellow, Green, Red) controlled by individual SSRs.

```
120V AC ──┬──────────────────────────────────────────┐
          │                                          │
          │    SSR Module                            │
          │    ┌─────────────────────────────────┐   │
          ├────┤ AC-IN (Common)                  │   │
          │    │                                 │   │
          │    │ OUT1 ──────────── Blue Light ──┼───┤
          │    │ OUT2 ──────────── Yellow Light ┼───┤
          │    │ OUT3 ──────────── Green Light ─┼───┤
          │    │ OUT4 ──────────── Red Light ───┼───┤
          │    │                                 │   │
          │    │ OUT5 ──────────── Blue Light ──┼───┤ (Small Garage)
          │    │ OUT6 ──────────── Yellow Light ┼───┤
          │    │ OUT7 ──────────── Green Light ─┼───┤
          │    │ OUT8 ──────────── Red Light ───┼───┘
          │    └─────────────────────────────────┘
          │
       Neutral ─────────────────────────────────────── All Light Neutrals
```

### SSR Module Specifications

**Recommended SSR:** Solid State Relay module rated for:
- Input: 3.3V or 5V DC (logic level compatible)
- Output: 120V AC, 2A minimum per channel
- Isolation: Optically isolated input
- Zero-crossing: Preferred for lamp loads

---

## LCD Display Wiring

### Display Selection

Recommended displays (SPI interface):
- **ST7789** (240×240 or 240×320) - Common, well-supported
- **ILI9341** (240×320) - Larger, more pins

### SPI Connection

```
ESP32-C6              LCD Display (ST7789)
┌─────────┐           ┌─────────┐
│    GPIO7├───────────┤ SCK     │
│    GPIO6├───────────┤ MOSI    │  (Note: Shared with LED Strip #1)
│    GPIO8├───────────┤ CS      │
│    GPIO9├───────────┤ DC      │
│   GPIO10├───────────┤ RST     │
│   GPIO11├───────────┤ BL      │  (Backlight, optional PWM)
│     3.3V├───────────┤ VCC     │
│      GND├───────────┤ GND     │
└─────────┘           └─────────┘
```

**Note:** The LCD shares SPI2 MOSI with LED Strip #1. This works because:
- WS2812B only uses MOSI (no CLK or CS)
- LCD transactions complete before LED updates
- Both are output-only devices

---

## Complete System Wiring Diagram

```
                            ┌─────────────────────────────────────────────────────┐
                            │                    ESP32-C6                         │
                            │                                                     │
                            │  GPIO0 (SDA) ────┐                                  │
                            │  GPIO1 (SCL) ────┼───── I2C Bus                     │
                            │  GPIO2 ──────────┼───── XSHUT_1 (ToF #1)            │
                            │  GPIO3 ──────────┼───── XSHUT_2 (ToF #2)            │
                            │                  │                                  │
                            │  GPIO6 ──────────┼───── SPI2 MOSI ──┬── LCD MOSI    │
                            │  GPIO7 ──────────┼───── SPI2 CLK ───┼── LCD SCK     │
                            │  GPIO8 ──────────┼───── LCD CS      │               │
                            │  GPIO9 ──────────┼───── LCD DC      │  Level        │
                            │  GPIO10 ─────────┼───── LCD RST     │  Shifter #1   │
                            │  GPIO11 ─────────┼───── LCD BL      └───┤ HV1 ──────┼─── LED Strip #1 DIN
                            │                  │                                  │
                            │  GPIO18 ─────────┼───── SPI3 MOSI ─── Level ────────┼─── LED Strip #2 DIN
                            │                  │                    Shifter #2    │
                            │  GPIO21 ─────────┼───── Button (pull-up)            │
                            │                  │                                  │
                            │  3.3V ───────────┼───── VCC (all 3.3V devices)      │
                            │  GND ────────────┼───── GND (common ground)         │
                            └──────────────────┼──────────────────────────────────┘
                                               │
        ┌──────────────────────────────────────┴──────────────────────────────────┐
        │                              I2C Bus                                    │
        │                                                                         │
   ┌────┴────┐          ┌─────────┐          ┌─────────┐                          │
   │ VL53L1X │          │ VL53L1X │          │ MCP23017│                          │
   │   #1    │          │   #2    │          │ (0x20)  │                          │
   │ (0x30)  │          │ (0x29)  │          │         │                          │
   │ Large   │          │ Small   │          │ GPA0-7 ─┼───── 8-Ch SSR Module     │
   │ Garage  │          │ Garage  │          │         │          │               │
   └─────────┘          └─────────┘          └─────────┘          │               │
                                                                  │               │
                                              ┌───────────────────┘               │
                                              │                                   │
                                    ┌─────────┴─────────┐                         │
                                    │  Light Towers     │                         │
                                    │  (120V AC)        │                         │
                                    │                   │                         │
                                    │  Large: B/Y/G/R   │                         │
                                    │  Small: B/Y/G/R   │                         │
                                    └───────────────────┘                         │
        │                                                                         │
        │                    5V Power Supply (20A)                                │
        │                    ┌────────────────────────────────────────────────────┤
        │                    │                                                    │
        │              ┌─────┴─────┐                    ┌──────────┐              │
        │              │ LED Strip │                    │ LED Strip│              │
        │              │    #1     │                    │    #2    │              │
        │              │ (180 LED) │                    │ (180 LED)│              │
        │              │  Large    │                    │  Small   │              │
        │              └───────────┘                    └──────────┘              │
        └─────────────────────────────────────────────────────────────────────────┘
```

---

## Power Distribution

### Power Rails Summary

| Rail | Voltage | Source | Devices |
|------|---------|--------|---------|
| 3.3V | 3.3V | ESP32-C6 onboard regulator | ToF sensors, MCP23017, LCD, level shifters (LV side) |
| 5V | 5V | External PSU | LED strips, level shifters (HV side) |
| 120V AC | 120V | Mains | Light tower bulbs (via SSR) |

### Grounding Requirements

**Critical:** All ground references must be connected:
- ESP32-C6 GND
- 5V PSU GND
- Level shifter GND (both sides)
- MCP23017 VSS
- SSR module GND
- LED strip GND

### Power Supply Sizing

| Component | Max Current | Notes |
|-----------|-------------|-------|
| ESP32-C6 | 500mA | Peak during WiFi TX |
| 2× VL53L1X | 40mA | 20mA each |
| MCP23017 | 25mA | Driving SSR inputs |
| LCD Display | 100mA | With backlight |
| 2× LED Strips | 20A | 10A each at full white |

**Recommended Power Supplies:**
- 5V USB-C (for ESP32-C6): 2A minimum
- 5V DC (for LED strips): 20A minimum, regulated

---

## Bill of Materials

### Core Components

| Item | Qty | Description | Notes |
|------|-----|-------------|-------|
| ESP32-C6-DevKitC-1 | 1 | ESP32-C6 development board | N8 variant preferred |
| VL53L1X Breakout | 2 | ToF distance sensor | Pololu or Adafruit |
| MCP23017 Breakout | 1 | I2C GPIO expander | Adafruit #732 or equivalent |
| 8-Channel SSR Module | 1 | Solid state relay board | 3.3V input compatible |
| ST7789 LCD | 1 | 240×240 or 240×320 SPI display | ILI9341 also works |
| 74AHCT125 | 2 | Quad level shifter | One per LED strip |

### LED System

| Item | Qty | Description | Notes |
|------|-----|-------------|-------|
| WS2812B Strip | 2 | 180 LEDs each, 60 LED/m | IP30 or IP65 |
| 5V 20A PSU | 1 | Switching power supply | Mean Well or equivalent |
| JST Connectors | 6 | 3-pin for LED strip connections | Power injection points |

### Passive Components

| Item | Qty | Description | Notes |
|------|-----|-------------|-------|
| 4.7kΩ Resistor | 2 | I2C pull-ups | May not need if on breakouts |
| 100µF Capacitor | 2 | LED strip power filtering | At strip input |
| 0.1µF Capacitor | 4 | Decoupling | Near ICs |

### Mechanical

| Item | Qty | Description | Notes |
|------|-----|-------------|-------|
| Project Enclosure | 1 | For ESP32 and electronics | |
| Junction Box | 1 | For 120V connections | NEC compliant |
| Mounting Hardware | - | Standoffs, screws, etc. | |

---

## Assembly Notes

### Pre-Assembly Checklist

- [ ] Verify all I2C devices have unique addresses
- [ ] Test each VL53L1X sensor individually before combining
- [ ] Verify SSR module input voltage requirements (3.3V vs 5V)
- [ ] Confirm level shifter pinout matches your board

### Assembly Sequence

1. Mount ESP32-C6 in enclosure
2. Wire I2C bus with pull-ups
3. Connect and test VL53L1X sensors (address assignment)
4. Wire MCP23017 and verify I2C communication
5. Connect SSR module and test relay switching
6. Wire level shifters for LED strips
7. Connect single LED strip and test
8. Connect second LED strip and test
9. Wire LCD display and test
10. Connect button and test
11. Wire 120V light towers (qualified electrician)
12. Full system integration test

### Testing Each Subsystem

**I2C Bus:**
```
i2c_scanner should show:
- 0x20: MCP23017
- 0x29: VL53L1X #2
- 0x30: VL53L1X #1
```

**LED Strips:**
- Test with simple color sweep
- Verify brightness limiting works
- Check for flickering (signal integrity issue)

**Light Towers:**
- Test each relay individually
- Verify no cross-activation
- Check SSR heat under load

---

## Future Expansion Considerations

### Available Resources After This Design

| Resource | Used | Available |
|----------|------|-----------|
| GPIO Pins | ~12 | ~6-8 |
| I2C Addresses | 3 | 124 |
| SPI Buses | 2 | 0 |
| MCP23017 Pins | 8 | 8 (GPB port) |

### Potential Additions

- Additional sensors (temperature, humidity)
- More status LEDs
- Buzzer/speaker for audio feedback
- Second button for menu navigation
- WiFi-based configuration via web interface

---

## Related Documents

- [BWLed Component Documentation](../../../BW%20Framework/BWLed/Docs/README.md)
- VL53L1X Datasheet (ST Microelectronics)
- MCP23017 Datasheet (Microchip)
- ESP32-C6 Technical Reference Manual (Espressif)

---

**Document Status:** Design  
**Last Updated:** January 13, 2026

[DocCategory: Architecture & Design]
