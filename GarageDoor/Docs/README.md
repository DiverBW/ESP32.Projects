# Garage Door Controller Documentation

Documentation for the dual garage door monitoring and control system.

## Project Overview

The Garage Door Controller monitors the open/closed status of two garage doors using magnetic reed switches and provides remote control capability via relay-driven button pushers. Status is displayed locally on a TFT display and published via MQTT for HomeKit integration.

## System Architecture

```
                         ┌────────────────────┐
                         │  GarageDoorController │
                         │      (ESP32-C6)       │
                         └──────────┬───────────┘
                                    │
     ┌──────────────────────────────┼──────────────────────────────┐
     │                              │                              │
     ▼                              ▼                              ▼
┌─────────────────────┐  ┌─────────────────────┐  ┌───────────────┐
│    Large Door       │  │    Small Door       │  │  TFT Display  │
│  ┌───────┐ ┌──────┐ │  │  ┌───────┐ ┌──────┐ │  │    (I2C)      │
│  │ Reed  │ │Button│ │  │  │ Reed  │ │Button│ │  └───────────────┘
│  │Switch │ │Relay │ │  │  │Switch │ │Relay │ │
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

- **MCU:** ESP32-C6 (upgraded from ESP8266)
- **Door Sensors:** 2x Magnetic reed switches (normally closed)
- **Button Control:** 2x Relay modules for wall button simulation
- **Display:** ILI9341 TFT (SPI)

### Door States

| State | Reed Switch | Description |
|-------|-------------|-------------|
| Closed | Closed (LOW) | Magnet aligned with sensor |
| Open | Open (HIGH) | Magnet separated from sensor |

### MQTT Topics

- **Status:** `GarageDoor2/LargeDoor`, `GarageDoor2/SmallDoor`
- **Control:** `GarageDoor2/LargeDoor/Control`, `GarageDoor2/SmallDoor/Control`
- **Commands:** `Open Door`, `Close Door`, `Close Both`

### HomeKit Services

| Door | HomeKit Service | Characteristics |
|------|-----------------|-----------------|
| Large | GarageDoorOpener | CurrentDoorState, TargetDoorState, ObstructionDetected |
| Small | GarageDoorOpener | CurrentDoorState, TargetDoorState, ObstructionDetected |

---

**Project Status:** Planned (Re-development from Arduino)
**Legacy Project:** GarageDoor.Control.ESP8266
**Last Updated:** January 20, 2026

[DocCategory: Overview]
