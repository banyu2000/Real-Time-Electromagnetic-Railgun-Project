# System Architecture

This document describes the overall architecture of the Real-Time Electromagnetic Railgun System, including its functional modules, data/control flows, and hardware-software integration.

---

##  System Overview

The system consists of five tightly integrated subsystems:

| Subsystem        | Description |
|------------------|-------------|
| **Vision Module** | Detects and tracks targets using a camera and OpenCV-based neural network |
| **Servo Control** | Adjusts the aiming angle based on target coordinates |
| **Firing Circuit** | Charges capacitors and releases high-current pulses via relay control |
| **I/O Control** | Reads and writes GPIO for relay switching and power status feedback |
| **UI Interface** | Provides real-time video feed and system status via a Qt GUI |

---

##  Module Interaction Flow

```plaintext
[Camera Feed] ─▶ [Vision Recognition] ─▶ [Callback Dispatch] ─▶ [Servo Control]
                                                     │
                                                     ▼
                                               [IO & Relay Trigger]
                                                     │
                                        [Capacitor Discharge (Fire)]
```

---
All components are coordinated via real-time callbacks and thread-safe communication queues.
---
## Hardware Block Diagram
## 🔌 Hardware Block Diagram

- **Power Supply**: 3× 18650 Li‑ion batteries in series (~12 V nominal)  
- **Step‑up Circuit**: Boost converter with 15 µH inductor + flyback diode  
  _(or use a commercial DC–DC boost module rated for ≥20 V, ≥5 A output)_  
- **Control Switches**:  
  - Prototype: manual toggles SW1/SW2  
  - Final: 2‑channel relay module driven by Raspberry Pi GPIO  
- **Capacitor**: 1000 µF electrolytic for energy storage before discharge  
- **Discharge Switch**: N‑MOSFET or relay to route discharge current into coil  
- **Barrel (Launcher Coil)**:  
  - Copper coil wound around a non‑conductive plastic tube  
  - Steel ball projectile seated in the tube’s center  
  - When discharged, the coil’s magnetic pulse propels the ball forward  

---

## Software Threading & Signal Handling
Thread / Task | Description
Vision Thread | Processes camera frames, outputs target positions
Callback Dispatcher | Routes vision output to servo + IO threads
Servo Thread | Converts target position to angle; sends to PCA9685
I/O Thread | Reads safety sensors, controls relay pins
UI Thread | Displays video stream, status indicators, user buttons
---

## Communication Interfaces
²C: Used for servo control via PCA9685 module

GPIO: Raspberry Pi controls relay signals (charge/discharge trigger)

Internal Queues: Thread-safe std::queue/Qt signals for module coordination

USB Camera or PiCamera: Frame source for real-time processing
---

## Notes on Electrical Design

Old Design (see schematic):

Uses MOSFET (VS-70TPS12PBF) for switching

Boost inductor + diode creates charging loop

Two push-button switches (SW1/SW2) manually control flow

New Design:

Replaces manual switches and MOSFET with relay modules

GPIO triggers allow safe, programmable operation

Enhanced with software interlock logic

---

## Summary

The system ensures:

Modular and testable architecture

Real-time control loop (detection → aiming → firing)

Safe hardware operation via callback and interlock logic

Clear power and control signal paths for easy debugging and reproduction

---

