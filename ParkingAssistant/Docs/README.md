# Parking Assistant Documentation

Documentation for the garage parking assistant system built on ESP32-C6.

## Project Overview

This project provides visual parking guidance for a two-car garage using:
- Distance sensing via VL53L1X Time-of-Flight sensors
- LED strip indicators (WS2812B, 180 LEDs per slot)
- 120V light towers (Blue/Yellow/Green/Red approach indicators)
- ILI9341 TFT status display

## Deployment Architecture

**This system uses a dual-deployment model:** One ESP32-C6 board per parking slot.

See [Architecture-Decision-Dual-Deployment.md](Architecture-Decision-Dual-Deployment.md) for the rationale behind this decision. This approach allows the project to use the standard C6-Standard-Breakout board rather than requiring a custom PCB.

## Documentation Index

### Architecture & Design

| Document | Description |
|----------|-------------|
| [Architecture-Decision-Dual-Deployment.md](Architecture-Decision-Dual-Deployment.md) | **Decision to use one board per parking slot** |
| [Hardware-Wiring-Guide.md](Hardware-Wiring-Guide.md) | Original dual-slot wiring reference (to be updated) |

### Implementation

*Coming soon*

### Reference

*Coming soon*

---

## Quick Reference (Per Slot)

### Key Hardware Components

| Component | Quantity | Notes |
|-----------|----------|-------|
| C6-Standard-Breakout | 1 | Standard breakout board |
| ESP32-C6-DevKitC-1-N8 | 1 | 8MB flash variant |
| VL53L1X ToF Sensor | 1 | I2C, default address 0x29 |
| ILI9341 TFT Display | 1 | 240x320 SPI display |
| WS2812B LED Strip | 1 | 180 LEDs |
| 4-Channel SSR Module | 1 | External, 120V AC rated |
| LED Strip Interface Board | 1 | Level shifter (see below) |
| Light Tower | 1 | 4 lights (B/Y/G/R) |

### I2C Address Map (Per Slot)

| Address | Device |
|---------|--------|
| - | ILI9341 uses SPI, not I2C |
| 0x29 | VL53L1X ToF Sensor |

### GPIO Assignments (Per Slot)

| GPIO | Function |
|------|----------|
| GPIO6 | I2C SDA |
| GPIO7 | I2C SCL |
| GPIO8 | SSR Blue |
| GPIO9 | SSR Yellow |
| GPIO10 | SSR Green |
| GPIO11 | SSR Red |
| GPIO18 | LED Strip Data (via level shifter) |

---

## LED Strip Interface Board

The WS2812B LED strip requires 5V logic signals, but the ESP32-C6 outputs 3.3V. A separate **LED Strip Interface Board** will be designed and fabricated to handle level shifting and power distribution.

**Board Specifications:** ~40mm x 50mm PCB

| Component | Purpose |
|-----------|---------|
| 74AHCT125 (DIP-14) | 3.3V to 5V level shifter |
| 1000µF capacitor | LED strip power filtering |
| 100Ω resistor | Data line signal integrity |
| 3-pin screw terminal (J1) | ESP32 input (3.3V, GND, Data) |
| 3-pin screw terminal (J2) | LED strip output (5V, GND, Data) |
| 2-pin screw terminal (J3) | 5V power input |
| 2-pin screw terminal (J4) | 5V power output (for injection) |
| 4x M3 mounting holes | Enclosure installation |

This board will be designed in EasyEDA and fabricated via JLCPCB. See [Architecture-Decision-Dual-Deployment.md](Architecture-Decision-Dual-Deployment.md) for full schematic and details.

---

**Project Status:** Design Phase
**Last Updated:** January 20, 2026

[DocCategory: Overview]
