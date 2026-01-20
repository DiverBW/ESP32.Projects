# HomeKit Reference Application Documentation

Documentation for the reference/test implementation of BWHomeKit component.

## Project Overview

The HomeKit Reference Application is a hardware-free test implementation used to validate the BWHomeKit component and HomeKit integration patterns. It exposes virtual accessories for testing HomeKit pairing, communication, and the Home app interface without requiring physical sensors or actuators.

## Purpose

1. **Component Validation:** Test BWHomeKit wrapper functionality
2. **HAP Protocol Testing:** Verify HomeKit Accessory Protocol implementation
3. **Development Tool:** Rapid iteration without hardware dependencies
4. **Integration Testing:** Validate MQTT-HomeKit bridging patterns

## System Architecture

```
                         ┌───────────────────────┐
                         │ HomeKitReference App  │
                         │      (ESP32-C6)       │
                         └───────────┬───────────┘
                                     │
              ┌──────────────────────┼──────────────────────┐
              │                      │                      │
              ▼                      ▼                      ▼
       ┌─────────────┐      ┌───────────────┐      ┌───────────────┐
       │  BWHomeKit  │      │    BWMQTT     │      │  BWWebServer  │
       │  Component  │      │   Component   │      │   Component   │
       └─────────────┘      └───────────────┘      └───────────────┘
              │                      │                      │
              ▼                      ▼                      ▼
       ┌─────────────┐      ┌───────────────┐      ┌───────────────┐
       │   HomeKit   │      │ MQTT Broker   │      │  Config Web   │
       │   Home App  │      │  (HomeWeb)    │      │     Page      │
       └─────────────┘      └───────────────┘      └───────────────┘
```

## Documentation Index

### Architecture & Design

*No hardware specifications - this is a software-only reference implementation*

### Implementation

*Coming soon*

### Reference

*Coming soon*

---

## Quick Reference

### Virtual Accessories

The reference app exposes these virtual HomeKit accessories for testing:

| Accessory | Service Type | Characteristics | Notes |
|-----------|--------------|-----------------|-------|
| Virtual Switch | Switch | On/Off | Basic toggle test |
| Virtual Light | Lightbulb | On/Off, Brightness | Dimmer test |
| Virtual Sensor | TemperatureSensor | CurrentTemperature | Read-only value test |
| Virtual Contact | ContactSensor | ContactSensorState | Binary sensor test |

### Test Scenarios

| Scenario | Description |
|----------|-------------|
| Pairing | Test QR code and setup code pairing flow |
| State Changes | Toggle accessories from Home app |
| State Sync | Verify state reflects in Home app after local change |
| MQTT Bridge | Test HomeKit ↔ MQTT state synchronization |
| Provisioning | Test BWProvisioning with HomeKit setup code field |

### BW Framework Components Used

| Component | Purpose |
|-----------|---------|
| BWCore | App identity (AppID, DeviceID, Version) |
| BWWifi | Network connectivity |
| BWProvisioning | Initial setup and HomeKit code configuration |
| BWWebServer | Configuration web interface |
| BWMQTT | HomeWeb status publishing |
| BWHomeKit | HomeKit accessory protocol |

---

## Hardware Requirements

**None** - This is a reference implementation designed to run without physical hardware.

The ESP32-C6 development board is the only required hardware. All accessories are virtual/simulated.

---

## Configuration

### HomeKit Setup Code

- Stored in NVS via BWProvisioning
- Can be set via captive portal or web interface
- Format: XXX-XX-XXX (e.g., 100-00-001)
- Category: Bridge (0x02)

### NVS Keys

| Namespace | Key | Description |
|-----------|-----|-------------|
| `bw_hk` | `setup_code` | HomeKit setup code |
| `bw_hk` | `paired` | Pairing status |
| `bw_app` | `app_name` | Application name |
| `bw_app` | `device_id` | Device identifier |

---

## Development Workflow

1. **Build and Flash:** Standard ESP-IDF build
2. **Provision:** Connect to AP, configure WiFi and HomeKit code
3. **Pair:** Add to Home app using setup code
4. **Test:** Interact with virtual accessories
5. **Validate:** Check MQTT messages and web status page

---

**Project Status:** Planned
**Dependencies:** BWHomeKit component (not yet implemented)
**Last Updated:** January 20, 2026

[DocCategory: Overview]
