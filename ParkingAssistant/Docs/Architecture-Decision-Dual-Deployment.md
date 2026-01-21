# Architecture Decision: Dual-Deployment Model

**Decision Date:** January 20, 2026
**Status:** Approved
**Decision Maker:** Brad

---

## Context

The original ParkingAssistant design used a single ESP32-C6 board to control both parking slots in a two-car garage. This required:

- 2x VL53L1X ToF sensors with XSHUT pin sequencing for I2C address assignment
- 2x WS2812B LED strips (180 LEDs each) requiring dual SPI buses
- 1x MCP23017 I2C GPIO expander for 8-channel SSR control
- Complex power distribution (20A @ 5V for LED strips)
- Numerous GPIO pins, causing conflicts with the standard breakout board

This complexity made the project incompatible with the C6-Standard-Breakout board being designed for use across all ESP32 projects.

---

## Decision

**Deploy two separate ESP32-C6 boards, one per parking slot.**

Each board runs an independent instance of the ParkingAssistant application, configured for its specific parking slot (Large Garage or Small Garage).

---

## Rationale

### Benefits

| Aspect | Single-Board (Original) | Dual-Board (New) |
|--------|------------------------|------------------|
| **Hardware Complexity** | High - dual SPI, MCP23017 | Low - single SPI, direct GPIO |
| **Breakout Compatibility** | Incompatible | Compatible with C6-Standard-Breakout |
| **ToF Sensor Setup** | XSHUT sequencing required | Default I2C address works |
| **LED Strip Wiring** | 2 level shifters, shared buses | 1 level shifter per board |
| **SSR Control** | 8 channels via MCP23017 | 4 channels via direct GPIO |
| **Power Distribution** | Complex (20A shared) | Simpler (10A per board) |
| **Failure Isolation** | Single point of failure | Independent operation |
| **Firmware Complexity** | Higher (manage 2 slots) | Lower (single slot focus) |
| **Maintenance** | Replace entire system | Replace individual slot |

### Trade-offs

| Consideration | Impact |
|---------------|--------|
| **Cost** | Slightly higher (2 ESP32-C6 boards vs 1) - approximately $10-15 additional |
| **Power Supplies** | May need 2 USB power sources (or share one with splitter) |
| **Network** | 2 devices on network instead of 1 |
| **MQTT Topics** | Separate topic trees per slot (cleaner organization) |
| **HomeKit** | 2 separate accessories (or bridged together) |

### Key Deciding Factor

The ability to use the **C6-Standard-Breakout board** across all projects provides significant value:

1. **Reduced design effort** - One PCB design serves all projects
2. **Inventory simplification** - Stock one board type
3. **Faster deployment** - No custom board fabrication wait
4. **Lower per-project cost** - Amortized across many projects

---

## Implementation Changes

### Hardware Per Slot

| Component | Quantity | Notes |
|-----------|----------|-------|
| C6-Standard-Breakout | 1 | With appropriate solder jumper configuration |
| ESP32-C6-DevKitC-1-N8 | 1 | Mounted on breakout |
| VL53L1X ToF Sensor | 1 | Default I2C address 0x29 |
| ILI9341 TFT Display | 1 | 240x320 SPI display |
| WS2812B LED Strip | 1 | 180 LEDs |
| 4-Channel SSR Module | 1 | External, connected via screw terminals |
| LED Strip Interface Board | 1 | Level shifter + connectors (see note below) |
| Light Tower | 1 | 4 lights (Blue/Yellow/Green/Red) |
| 5V Power Supply | 1 | 10A minimum for LED strip |

### GPIO Assignments (Per Slot)

| Function | GPIO | Notes |
|----------|------|-------|
| I2C SDA | GPIO6 | VL53L1X sensor |
| I2C SCL | GPIO7 | VL53L1X sensor |
| SPI MOSI | GPIO2 | TFT Display |
| SPI CLK | GPIO3 | TFT Display |
| Display CS | GPIO4 | TFT chip select |
| Display DC | GPIO5 | TFT data/command |
| Display RST | GPIO21 | TFT reset |
| LED Strip Data | GPIO18 | Via level shifter |
| SSR Blue | GPIO8 | Light tower |
| SSR Yellow | GPIO9 | Light tower |
| SSR Green | GPIO10 | Via solder jumper |
| SSR Red | GPIO11 | Via solder jumper |

### Firmware Configuration

Each deployment will be configured via NVS or build-time defines:

```cpp
// Large Garage Slot
#define SLOT_NAME "Large"
#define MQTT_TOPIC_PREFIX "parking/large"

// Small Garage Slot
#define SLOT_NAME "Small"
#define MQTT_TOPIC_PREFIX "parking/small"
```

### MQTT Topic Structure

```
parking/
├── large/
│   ├── distance        // Current distance reading
│   ├── status          // Parked / Approaching / Empty
│   ├── led/brightness  // LED strip brightness
│   └── tower/state     // Light tower state (B/Y/G/R)
└── small/
    ├── distance
    ├── status
    ├── led/brightness
    └── tower/state
```

---

## LED Strip Interface Board

The 74AHCT125 level shifter is a 14-pin DIP IC that requires a mounting solution. Since it cannot be mounted directly on the C6-Standard-Breakout board, a separate **LED Strip Interface Board** will be designed and fabricated.

### Board Specifications

**Decision:** Design the comprehensive LED Strip Interface Board

**Dimensions:** ~40mm x 50mm

**Components:**

| Component | Value/Type | Purpose |
|-----------|------------|---------|
| U1 | 74AHCT125 (DIP-14) | 3.3V to 5V level shifter |
| C1 | 1000µF 10V electrolytic | LED strip power filtering |
| C2 | 0.1µF ceramic | IC decoupling |
| R1 | 100Ω 1/4W | Data line signal integrity |
| J1 | 3-pin screw terminal | ESP32 input (3.3V, GND, Data) |
| J2 | 3-pin screw terminal | LED strip output (5V, GND, Data) |
| J3 | 2-pin screw terminal | 5V power input (from LED PSU) |
| J4 | 2-pin screw terminal | 5V power output (for power injection) |

**Features:**

- Screw terminals for tool-free field wiring
- 5V power passthrough for LED strip power injection at multiple points
- Large filter capacitor prevents LED strip inrush from affecting ESP32
- Data line resistor reduces ringing and improves signal integrity
- M3 mounting holes (4x) for enclosure installation
- Clear silkscreen labels

**Schematic Concept:**

```
                    LED Strip Interface Board
    ┌─────────────────────────────────────────────────────────────┐
    │                                                             │
    │   J1 (ESP32 Input)              J2 (LED Strip Output)       │
    │   ┌─────────────┐               ┌─────────────┐             │
    │   │ 3.3V ───────┼───┐       ┌───┼─── 5V       │             │
    │   │ GND ────────┼───┼───────┼───┼─── GND      │             │
    │   │ DATA ───────┼───┼───┐   │   │             │             │
    │   └─────────────┘   │   │   │   └─────────────┘             │
    │                     │   │   │                               │
    │              ┌──────┴───┼───┴──────┐                        │
    │              │          │          │                        │
    │           3.3V LV    74AHCT125   HV 5V ──────┐              │
    │              │          │          │         │              │
    │              │       ┌──┴──┐       │     ┌───┴───┐          │
    │              │       │ IN  │───R1──┼─────┤ DATA  │          │
    │              │       │     │ 100Ω  │     │  OUT  │          │
    │              │       └─────┘       │     └───────┘          │
    │              │                     │                        │
    │              └─────────┬───────────┘                        │
    │                        │                                    │
    │   J3 (5V Power In)     │        J4 (5V Power Out)           │
    │   ┌─────────────┐      │        ┌─────────────┐             │
    │   │ +5V ────────┼──────┴────C1──┼─── +5V      │             │
    │   │ GND ────────┼───────────────┼─── GND      │             │
    │   └─────────────┘    1000µF     └─────────────┘             │
    │                                                             │
    │  ○                                                       ○  │
    │       M3 mounting holes (4x corners)                        │
    │  ○                                                       ○  │
    └─────────────────────────────────────────────────────────────┘
```

### Design Tool

This board will be designed in **EasyEDA** alongside the C6-Standard-Breakout board and fabricated via **JLCPCB**.

### Reusability

This LED Strip Interface Board can be used with any future project requiring WS2812B/SK6812 LED strips with the C6-Standard-Breakout board or any 3.3V microcontroller.

---

## Migration Path

If already building the original single-board design:

1. Duplicate the existing hardware for the second slot
2. Update firmware to remove dual-slot logic
3. Configure each board for its specific slot
4. Update MQTT subscriptions and HomeKit configuration

---

## Related Documents

- [C6-Standard-Breakout Project Compatibility Analysis](../../../PCB%20Boards/C6-Standard-Breakout/Docs/Project-Compatibility-Analysis.md)
- [Hardware-Wiring-Guide.md](Hardware-Wiring-Guide.md) - Original dual-slot design (to be updated)

---

**Document History:**

| Date | Change |
|------|--------|
| 2026-01-20 | Initial decision documented |

[DocCategory: Architecture & Design]
