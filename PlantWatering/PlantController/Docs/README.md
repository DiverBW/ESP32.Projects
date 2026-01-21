# Plant Controller Documentation

Documentation for the individual plant moisture monitoring and watering controller.

## Project Overview

The Plant Controller monitors soil moisture for a single potted plant and controls watering via a solenoid valve. Each controller operates independently but reports status to a central ValveController and PlantControlService via MQTT.

## System Architecture

```
                    ┌─────────────────┐
                    │ PlantController │
                    │    (ESP32-C6)   │
                    └────────┬────────┘
                             │
        ┌────────────────────┼────────────────────┐
        │                    │                    │
        ▼                    ▼                    ▼
┌───────────────┐   ┌───────────────┐   ┌───────────────┐
│   Moisture    │   │  Water Valve  │   │  TFT Display  │
│    Sensor     │   │    Relay      │   │  (ILI9341)    │
└───────────────┘   └───────────────┘   └───────────────┘
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
- **Moisture Sensor:** DFRobot Waterproof Capacitive Soil Moisture Sensor (Analog)
- **Valve Control:** Relay module for solenoid valve
- **Display:** ILI9341 TFT (SPI)

### Operating Modes

| Mode | Description |
|------|-------------|
| Offline | Awake, listening for MQTT, no monitoring |
| Monitor | Sampling and reporting, no watering |
| Automatic | Full automatic moisture control |

### MQTT Topics

- **Up:** `PlantControl/{id}/Status`, `PlantControl/{id}/Report`
- **Down:** `PlantControl/{id}/Config`, `PlantControl/{id}/Command`

---

**Project Status:** Planned (Re-development from Arduino)
**Legacy Project:** PlantControl.ESP32
**Last Updated:** January 20, 2026

[DocCategory: Overview]
