# Server Room Door Controller Documentation

Documentation for the automated server room door controller with AVR-triggered operation.

## Project Overview

The Server Room Door Controller automatically manages a door using a linear actuator motor. When the AVR receiver is powered on (generating heat), the door closes for sound isolation. When the AVR is off, the door opens for improved air ventilation. The system also supports manual override via pushbuttons.

## System Architecture

```
                           ┌───────────────────┐
                           │ ServerRoomDoor    │
                           │    (ESP32-C6)     │
                           └─────────┬─────────┘
                                     │
     ┌──────────────────────────┬────┴────┬────────────────────────────┐
     │                          │         │                            │
     ▼                          ▼         ▼                            ▼
┌───────────────┐     ┌─────────────┐ ┌────────────────┐     ┌───────────────┐
│ Linear Motor  │     │ Microswitches│ │Manual Buttons │     │  TFT Display  │
│   (L298N)     │     │  (Open/Close)│ │ (Open/Close)  │     │  (ILI9341)    │
└───────────────┘     └─────────────┘ └────────────────┘     └───────────────┘
        │                    │                │                      │
        │                    │                │                      │
        ▼                    ▼                ▼                      │
┌───────────────┐     ┌─────────────┐ ┌────────────────┐            │
│  Door/Panel   │     │  Position   │ │  Mode Switch   │            │
│               │     │  Detection  │ │ (Auto/Manual)  │            │
└───────────────┘     └─────────────┘ └────────────────┘            │
                                                                     │
                           ┌─────────────────────────────────────────┘
                           │
                           ▼
                    ┌─────────────┐
                    │ AVR Power   │
                    │  Sensing    │
                    └─────────────┘
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
- **Motor:** 12V Linear Actuator
- **Motor Driver:** L298N H-Bridge
- **Position Sensors:** 2x Microswitches (fully open, fully closed)
- **Control:** 2x Pushbuttons (open, close)
- **Mode Select:** Toggle switch (Auto/Manual)
- **AVR Detection:** Analog voltage sensing (3V when on)
- **Display:** ILI9341 TFT (SPI)

### Operating Modes

| Mode | Description |
|------|-------------|
| Automatic | Door position controlled by AVR power state |
| Manual | Door controlled by open/close pushbuttons |

### HomeKit Services

| Feature | HomeKit Service | Characteristics |
|---------|-----------------|-----------------|
| Door | ContactSensor | ContactSensorState (Open/Closed) |
| AVR Power | ContactSensor | ContactSensorState (On/Off) |

---

**Project Status:** Planned (Re-development from Arduino)
**Legacy Project:** ServerRoomDoor
**Last Updated:** January 20, 2026

[DocCategory: Overview]
