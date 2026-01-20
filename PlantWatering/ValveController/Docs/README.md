# Valve Controller Documentation

Documentation for the shared water valve controller that manages irrigation and drip system valves.

## Project Overview

The Valve Controller manages the master water valves for the plant watering system. It controls two independent water sources:
1. **Irrigation System** - Main house irrigation (Rachio controller integration)
2. **Drip System** - Dedicated drip line for potted plants

The controller monitors water pressure to confirm valve operation and water availability.

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
│  Irrigation System  │  │    Drip System      │  │  LCD Display  │
│  ┌───────┐ ┌──────┐ │  │  ┌───────┐ ┌──────┐ │  │    (I2C)      │
│  │Pressure│ │Valve │ │  │  │Pressure│ │Valve │ │  └───────────────┘
│  │Sensor │ │Relay │ │  │  │Sensor │ │Relay │ │
│  └───────┘ └──────┘ │  │  └───────┘ └──────┘ │
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
- **Pressure Sensors:** 2x Analog water pressure transducers
- **Valve Control:** 2x Relay modules for master valves
- **Status LEDs:** 2x LED indicators for water availability
- **Display:** 20x4 LCD (I2C)

### Water Sources

| Source | Purpose | Pressure Range |
|--------|---------|----------------|
| Irrigation | Rachio-controlled zones | 40-80 PSI |
| Drip | Dedicated plant drip line | 20-40 PSI |

### MQTT Topics

- **Up:** `ValveC/Status`, `ValveC/Settings`
- **Down:** `ValveC/Command`, `ValveC/TurnOnWater`

---

**Project Status:** Planned (Re-development from Arduino)
**Legacy Project:** PlantControl.ESP32/ValveControl
**Last Updated:** January 20, 2026

[DocCategory: Overview]
