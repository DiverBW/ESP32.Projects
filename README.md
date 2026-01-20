# ESP32 Projects

ESP-IDF based IoT projects for home automation, built on the [BW Framework](../BW%20Framework/).

## Overview

This repository contains ESP32 application projects that leverage reusable components from the BW Framework. All projects target the **ESP32-C6** platform and integrate with HomeKit via the BWHomeKit component.

## Projects

| Project | Description | Status |
|---------|-------------|--------|
| [ParkingAssistant](ParkingAssistant/) | Dual-car garage parking guidance with distance sensors and LED indicators | Design |
| [PlantWatering/PlantController](PlantWatering/PlantController/) | Individual plant moisture monitoring and watering control | Planned |
| [PlantWatering/ValveController](PlantWatering/ValveController/) | Shared water valve controller for irrigation and drip systems | Planned |
| [GarageDoor](GarageDoor/) | Dual garage door monitoring and control with reed switches | Planned |
| [ServerRoomDoor](ServerRoomDoor/) | Automated door with linear actuator, AVR-triggered operation | Planned |
| [HomeKitReference](HomeKitReference/) | Reference implementation for testing BWHomeKit component | Planned |

## Architecture

```
                           ┌─────────────────┐
                           │   Home App      │
                           │   (HomeKit)     │
                           └────────┬────────┘
                                    │
                           ┌────────▼────────┐
                           │   ESP32-C6      │
                           │   Application   │
                           └────────┬────────┘
                                    │
        ┌───────────────────────────┼───────────────────────────┐
        │                           │                           │
        ▼                           ▼                           ▼
┌───────────────┐          ┌───────────────┐          ┌───────────────┐
│  BWHomeKit    │          │    BWMQTT     │          │   BWWifi      │
│  (HomeKit)    │          │   (HomeWeb)   │          │  (Network)    │
└───────────────┘          └───────────────┘          └───────────────┘
        │                           │                           │
        └───────────────────────────┴───────────────────────────┘
                                    │
                           ┌────────▼────────┐
                           │    BWCore       │
                           │  (App Identity) │
                           └─────────────────┘
```

## BW Framework Components

These projects use shared components from the BW Framework:

| Component | Purpose |
|-----------|---------|
| BWCore | Centralized app identity (AppName, DeviceID, Version) |
| BWWifi | WiFi management (STA/AP modes) |
| BWProvisioning | Captive portal setup with HomeKit code |
| BWWebServer | HTTP configuration interface |
| BWMQTT | MQTT client for HomeWeb integration |
| BWHomeKit | Apple HomeKit accessory protocol |
| BWDisplay | LCD/TFT display abstraction |
| BWLed | WS2812B LED strip control |
| BWOTA | Over-the-air firmware updates |

## Development Setup

### Prerequisites

- ESP-IDF 5.x
- ESP32-C6-DevKitC-1 or compatible board
- VS Code with ESP-IDF extension (recommended)

### Building a Project

```bash
cd <project-folder>
idf.py set-target esp32c6
idf.py build
idf.py flash monitor
```

### Including BW Framework Components

Add to your project's `CMakeLists.txt`:

```cmake
set(EXTRA_COMPONENT_DIRS
    "C:/BradSoftware/ESP32/BW Framework/BWCore"
    "C:/BradSoftware/ESP32/BW Framework/BWWifi"
    "C:/BradSoftware/ESP32/BW Framework/BWMQTT"
    # Add other components as needed
)
```

## HomeKit Integration

All projects support Apple HomeKit integration:

| Project | HomeKit Services |
|---------|------------------|
| ParkingAssistant | OccupancySensor (x2), ContactSensor (x2) |
| PlantController | HumiditySensor, Valve |
| ValveController | Valve (x2) |
| GarageDoor | GarageDoorOpener (x2) |
| ServerRoomDoor | ContactSensor (x2: door + AVR) |

### Setup Code Management

- Codes stored in NVS (not hardcoded)
- Configured via BWProvisioning captive portal
- Can be updated via device web page
- Registry: See [BW Framework/Docs/HomeKit-Setup-Codes.md](../BW%20Framework/Docs/HomeKit-Setup-Codes.md)

## MQTT Topics

All projects publish status to HomeWeb via MQTT:

| Topic Pattern | Purpose |
|---------------|---------|
| `{AppName}/Status` | Device online/offline, current state |
| `{AppName}/Heartbeat` | Periodic health check |
| `{AppName}/HomeKit/Reset` | Request HomeKit re-pairing |

## Project Structure

```
Projects/
├── README.md                 # This file
├── .gitignore               # Git ignore rules
├── GarageDoor/
│   └── Docs/
│       ├── README.md
│       └── Hardware-Specifications.md
├── HomeKitReference/
│   └── Docs/
│       └── README.md
├── ParkingAssistant/
│   └── Docs/
│       ├── README.md
│       └── Hardware-Wiring-Guide.md
├── PlantWatering/
│   ├── PlantController/
│   │   └── Docs/
│   │       ├── README.md
│   │       └── Hardware-Specifications.md
│   └── ValveController/
│       └── Docs/
│           ├── README.md
│           └── Hardware-Specifications.md
└── ServerRoomDoor/
    └── Docs/
        ├── README.md
        └── Hardware-Specifications.md
```

## Legacy Projects

These projects are re-implementations of existing Arduino-based systems:

| New Project | Legacy Project | Original Platform |
|-------------|----------------|-------------------|
| PlantController | PlantControl.ESP32 | ESP32 (Arduino) |
| ValveController | ValveControl | ESP32 (Arduino) |
| ParkingAssistant | CarParkingAssistant.ESP32 | ESP32 (Arduino) |
| GarageDoor | GarageDoor.Control.ESP8266 | ESP8266 (Arduino) |
| ServerRoomDoor | ServerRoomDoor | ESP32 (Arduino) |

Legacy code location: `C:\BradSoftware\Arduino\Projects\`

## Related Repositories

- **BW Framework:** `C:\BradSoftware\ESP32\BW Framework\` - Reusable ESP-IDF components
- **Arduino Projects:** `C:\BradSoftware\Arduino\Projects\` - Legacy implementations

---

**Author:** Brad Ward
**Last Updated:** January 20, 2026

[DocCategory: Overview]
