# HomeKit Reference Application Documentation

Documentation for the reference/test implementation of BWHomeKit component.

## Project Overview

The HomeKit Reference Application is a hardware-free test implementation used to validate the BWHomeKit component and HomeKit integration patterns. It exposes virtual accessories for testing HomeKit pairing, communication, and the Home app interface without requiring physical sensors or actuators.

## Purpose

1. **Component Validation:** Test BWHomeKit wrapper functionality
2. **HAP Protocol Testing:** Verify HomeKit Accessory Protocol implementation
3. **Development Tool:** Rapid iteration without hardware dependencies
4. **Integration Testing:** Validate WiFi + HomeKit startup sequence

## Virtual Accessories

| Accessory | Service Type | Behavior |
|-----------|--------------|----------|
| Virtual Door | ContactSensor | Toggle with BOOT button |
| Virtual Motion | OccupancySensor | Auto-toggles every 30 seconds |
| Virtual Temp | TemperatureSensor | Drifts between 18-28°C |
| Virtual Switch | Switch | Controllable from Home app |

## Building

### Prerequisites

1. **ESP-IDF:** v5.x installed and configured
2. **esp-homekit-sdk:** Clone to `C:\BradSoftware\ESP32\esp-homekit-sdk`
   ```bash
   cd C:\BradSoftware\ESP32
   git clone --recursive https://github.com/espressif/esp-homekit-sdk.git
   ```

### Build Commands

```bash
cd C:\BradSoftware\ESP32\Projects\HomeKitReference
idf.py set-target esp32c6
idf.py build
idf.py flash monitor
```

## First Run

1. **Flash the device** with the built firmware
2. **No stored WiFi?** Device starts in AP mode
   - Connect to the AP (name: `HomeKitRef-XXYYZZ`)
   - Configure WiFi via captive portal
3. **Has stored WiFi?** Device connects automatically
4. **Once connected:** HomeKit server starts
5. **Add to Home app:**
   - Open Home app → "+" → "Add Accessory"
   - Scan QR code or enter setup code: `111-22-333`

## Usage

### Button Controls

| Button | Action |
|--------|--------|
| BOOT (GPIO 9) | Toggle contact sensor state |

### Automatic Behaviors

- Occupancy sensor toggles every 30 seconds
- Temperature drifts slowly between 18°C and 28°C

### Serial Monitor Output

```
I (xxx) HomeKitRef: WiFi state: Connected
I (xxx) HomeKitRef: HomeKit started successfully
I (xxx) HomeKitRef: Setup code: 111-22-333
I (xxx) HomeKitRef: Contact sensor: OPEN
I (xxx) HomeKitRef: Occupancy sensor: OCCUPIED
```

## Configuration

### Default Setup Code

`111-22-333` (stored in NVS, can be changed via API)

### Partition Table

| Name | Type | Size | Purpose |
|------|------|------|---------|
| nvs | NVS | 24KB | Application NVS |
| factory_nvs | NVS | 24KB | HomeKit factory data |
| factory | App | ~1.9MB | Application firmware |

## Files

| File | Description |
|------|-------------|
| `CMakeLists.txt` | Project configuration |
| `main/main.cpp` | Application entry point |
| `sdkconfig.defaults` | ESP32-C6 configuration |
| `partitions.csv` | Custom partition table |

## BW Framework Components Used

| Component | Purpose |
|-----------|---------|
| BWCore | App identity (name, ID, version) |
| BWWifi | Network connectivity |
| BWHomeKit | HomeKit accessory protocol |

---

**Project Status:** Implementation Complete (testing pending)
**Target Hardware:** ESP32-C6
**Last Updated:** January 21, 2026

[DocCategory: Overview]
