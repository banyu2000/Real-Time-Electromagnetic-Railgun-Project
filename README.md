# 🕊️ Real-Time Electromagnetic Railgun - Agricultural Deterrent System (Prototype)

This project is a real-time embedded system that detects visual targets (e.g. birds), calculates servo aiming, and triggers an actuator (electromagnetic railgun in demo).  
It is designed as a **proof-of-concept for smart deterrent systems**, particularly in agricultural environments.

![System Overview](media/system_diagram.png)

---

##  Project Background & Motivation

In agriculture, birds cause significant damage to crops by eating seeds and fruits. Traditional scarecrow systems are passive, and manual patrols are time-consuming and ineffective.

Our project aims to address this problem by building an **automated deterrent platform**, capable of:
- Detecting moving visual targets using a camera
- Aiming servos to lock onto targets in real time
- Triggering an actuator (e.g., railgun / speaker / LED blaster) to deter targets

Although the current system uses a railgun for demonstration, the design is modular and can be adapted for:
-  Smart farm protection
-  Perimeter defense simulation
-  Embedded systems education

---

##  Demo Preview

>  Target detection + servo movement + firing sequence

![Demo GIF](media/demo.gif)

>  Real-time detection output

![Detection Example](media/vision_output.png)

---

##  System Overview

-  **Vision Module**: HSV-based yellow target detection (OpenCV)
-  **Servo Control**: Dual-axis aiming with PCA9685, ±0.5° accuracy
-  **Launcher**: Electromagnetic railgun with capacitor boost & relay control
-  **Callback & Multithreading**: Threads handle image capture, control logic, and GPIO actions
-  **Qt GUI**: Live video stream, system status, and manual controls

 Architecture diagram: [`docs/system_architecture.md`](docs/system_architecture.md)

---

##  How to Run

###  Build & Run Servo Test

```bash
mkdir build && cd build
cmake ..
make
sudo ./servo_test

``` 

---

###  Test Output 

- Servo sweeps automatically from 0° to 180°

- Vision module detects yellow objects and overlays bounding boxes

- Qt UI displays live feedback and control panel

Full demo video : media/demo.mp4

---

###  Documentation (`/docs`)

| File | Description |
|------|-------------|
| [`project_log.md`](docs/project_log.md)             | Project development timeline & milestones       |
| [`system_architecture.md`](docs/system_architecture.md) | Hardware/software architecture               |
| [`end_of_life.md`](docs/end_of_life.md)             | Reflection, limitations & future improvements   |
| [`credits.md`](docs/credits.md)                     | Team member responsibilities                    |

---


### 🗂 Repository Structure

- `src/` — Source code (servo, vision, callback, GUI, etc.)
- `tests/` — Functional test modules (e.g., servo_test)
- `docs/` — Project documentation
  - `project_log.md` — Development timeline
  - `system_architecture.md` — System design
  - `end_of_life.md` — Final reflection
  - `credits.md` — Team responsibilities
- `media/` — Demo images, diagrams, and videos
- `build/` — Build output folder
- `CMakeLists.txt` — Build configuration
- `README.md` — Main project overview


---

###  Contributors

| Name              | ID       | Role                               |
|-------------------|----------|------------------------------------|
| **Baichuan Shi**  | 2982811  | Servo control, GitHub management   |
| **Xiang Chen**    | 3052701  | IO logic, relay control            |
| **Yu Ban**        | 2931341  | Vision detection (OpenCV)          |
| **Kunjin Chen**   | 2981978  | Callback mechanism, threading      |
| **Zhongkai Zhang**| 2969533  | Qt GUI development                 |
---

### License

This project is licensed under the MIT License. See LICENSE for details.

---