# Valve Controller Documentation

Documentation for the shared water valve controller that manages irrigation and drip system valves.

## Project Overview

The Valve Controller manages the master water valves for the plant watering system. It controls two independent water sources:
1. **Irrigation System** - Main house irrigation (Rachio controller integration)
2. **Drip System** - Dedicated drip line for potted plants

The controller provides remote valve control with status indication.

## System Architecture

```
                         ┌──────────────────┐
                         │  ValveController │
                         │    (ESP32-C6)    │
                         └────────┬─────────┘
                                  │
     ┌────────────────────────────┼────────────────────────────┐
     │                            │                            │
     ▼                            ▼                            ▼
┌─────────────────────┐  ┌─────────────────────┐  ┌───────────────┐
│  Irrigation System  │  │    Drip System      │  │  TFT Display  │
│  ┌──────┐ ┌──────┐  │  │  ┌──────┐ ┌──────┐  │  │  (ILI9341)    │
│  │Valve │ │ LED  │  │  │  │Valve │ │ LED  │  │  └───────────────┘
│  │Relay │ │Relay │  │  │  │Relay │ │Relay │  │
│  └──────┘ └──────┘  │  │  └──────┘ └──────┘  │
└─────────────────────┘  └─────────────────────┘
```

## Documentation Index

### Architecture & Design

| Document | Description |
|----------|-------------|
| [Hardware-Specifications.md](Hardware-Specifications.md) | Complete hardware components and wiring |

### Implementation

*Coming soon*

### Reference

*Coming soon*

---

## Quick Reference

### Key Hardware Components

- **MCU:** ESP32-C6
- **Valve Control:** 2x Relay modules for master valves
- **Status LEDs:** 2x LED indicators for water availability
- **Display:** ILI9341 TFT (SPI)

### Water Sources

| Source | Purpose |
|--------|---------|
| Irrigation | Rachio-controlled zones |
| Drip | Dedicated plant drip line |

### MQTT Topics

- **Up:** `ValveC/Status`, `ValveC/Settings`
- **Down:** `ValveC/Command`, `ValveC/TurnOnWater`

---

**Project Status:** Planned (Re-development from Arduino)
**Legacy Project:** PlantControl.ESP32/ValveControl
**Last Updated:** January 20, 2026

[DocCategory: Overview]
