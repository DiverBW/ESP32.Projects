# Parking Assistant Documentation

Documentation for the dual-garage parking assistant system built on ESP32-C6.

## Project Overview

This project provides visual parking guidance for a two-car garage using:
- Distance sensing via VL53L1X Time-of-Flight sensors
- LED strip indicators (WS2812B, 180 LEDs per door)
- 120V light towers (Blue/Yellow/Green/Red approach indicators)
- LCD status display

## Documentation Index

### Architecture & Design

| Document | Description |
|----------|-------------|
| [Hardware-Wiring-Guide.md](Hardware-Wiring-Guide.md) | Complete hardware architecture, pin assignments, and wiring diagrams |

### Implementation

*Coming soon*

### Reference

*Coming soon*

---

## Quick Reference

### Key Hardware Components

- **MCU:** ESP32-C6
- **Distance Sensors:** 2× VL53L1X (I2C)
- **LED Strips:** 2× WS2812B (180 LEDs each, SPI2/SPI3)
- **Relay Control:** MCP23017 I2C GPIO expander + 8-channel SSR
- **Display:** ST7789 or ILI9341 LCD (SPI)

### I2C Address Map

| Address | Device |
|---------|--------|
| 0x20 | MCP23017 GPIO Expander |
| 0x29 | VL53L1X #2 (Small Garage) |
| 0x30 | VL53L1X #1 (Large Garage) |

---

**Project Status:** Design Phase  
**Last Updated:** January 13, 2026

[DocCategory: Overview]
