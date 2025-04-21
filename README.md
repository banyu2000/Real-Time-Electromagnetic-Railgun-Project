# 🕊️ Real-Time Electromagnetic Railgun - Agricultural Deterrent System (Prototype)

This project is a real-time embedded system that detects visual targets (e.g. birds), calculates servo aiming, and triggers an actuator (electromagnetic railgun in demo).  
It is designed as a **proof-of-concept for smart deterrent systems**, particularly in agricultural environments.

## 🧩 System Overview

This project is organized into five tightly integrated subsystems, each responsible for a key piece of the “detect → aim → fire” feedback loop:

1. **Vision Module**  
   - Captures live video frames via libcamera  
   - Converts frames to OpenCV `cv::Mat` for processing  
   - Applies HSV color filtering + morphological operations to detect blue cylindrical targets  
   - Outputs the target’s centroid coordinates and contour bounding box

2. **Servo Control Module**  
   - Uses a PCA9685 I²C PWM driver to command two servos (horizontal & vertical)  
   - Converts detected error offsets (px) into angular adjustments (±0.5° precision)  
   - Supports two modes:  
     - **Manual Sweep**: continuous back‑and‑forth scan  
     - **Tracking**: lock onto detected target and hold position

3. **Launcher (Electromagnetic Barrel)**  
   - **Step‑up Circuit**: 3× 18650 cells → boost converter (15 µH coil + diode) → 1000 µF storage capacitor  
   - **Discharge Path**: relay (or N‑MOSFET) closes to dump capacitor into copper coil wound around a plastic barrel  
   - **Projectile**: small steel ball seated in the barrel’s center is launched by the magnetic pulse

4. **Callback & Multithreading Framework**  
   - **LibCam2OpenCV** runs camera capture in its own thread, invokes `hasFrame()` slot in the GUI thread  
   - **ServoThread** (inside `Window`) processes angle updates in a worker thread to keep UI responsive  
   - **SequenceController** runs charge–pause–discharge relay sequence on a detached thread when “lock” conditions are met  
   - Thread‐safe coordination via `std::atomic<bool>` and `std::function` callbacks

5. **Qt GUI Interface**  
   - **Live View**: displays current camera frame with overlaid detection box & center point  
   - **Controls**:  
     - **Start Sweep** button toggles manual scanning mode  
     - **Track Mode** button enables automatic target‐lock behavior  
   - **Indicators**: shows real‐time servo angles, relay status, and system health  
   - Built with Qt Widgets and optionally uses Qwt for custom gauges (e.g. QwtThermo)

---

All five subsystems are orchestrated by the main application, which:  
- Initializes hardware (I²C, GPIO, camera)  
- Launches the Qt event loop and camera capture  
- Monitors for SIGINT to gracefully shut down threads and reset actuators  
- Provides a modular codebase in `src/`, easy to extend with new sensors, actuators, or network interfaces  


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

![Demo mp4](media/demo.mp4)

>  Real-time detection output

![Detection Example](media/vision_output.mp4)

---

##  Tests Preview

Below are snapshots from our various unit- and integration‑tests, all located under the `tests/` directory:

| Test Module         | Description                       | Preview                  |
|---------------------|-----------------------------------|--------------------------|
| **Servo Test**      | `tests/test1_servo/` sweep demo   | ![Servo Test](media/test1_servo.mp4) |
| **GUI Test**        | `tests/test2_UI/` UI functionality | ![GUI Test](media/vision_output.mp4)     |
| **IO Control Test** | `tests/test3_io_control/` relay logic | ![IO Test](media/test3_io.mp4)      |
| **ServoCam Test**   | `tests/test4_ServoCam/` full pipeline | ![ServoCam](media/test4_servocam.mp4) |
| **Camera Test**     | `tests/test5_camera/` framerate check | ![Camera](media/test5_camera.mp4)   |

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

- Vision module detects blue objects and overlays bounding boxes

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


###  Repository Structure

- `build/` — Build output directory (excluded by `.gitignore`)
- `docs/` — Project documentation
  - `project_log.md` — Development timeline and milestones
  - `system_architecture.md` — Architecture and design overview
  - `end_of_life.md` — Final reflection and improvement analysis
  - `credits.md` — Team member responsibilities
- `media/` — Demo images, GIFs, and video assets
  - `demo.gif`, `vision_output.png`, `system_diagram.png`
- `src/` — Main source code (servo, vision, callback, GUI, etc.)
- `tests/` — Unit tests and module demos (e.g., `servo_test`)
- `.gitignore` — Git exclusion rules (build folders, binaries, temp files)
- `CMakeLists.txt` — CMake build configuration
- `LICENSE` — MIT open-source license
- `README.md` — Project overview, usage, and documentation links



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

## 🎥 Social Media

Check out our project demo on TikTok:  
[Real‑Time Electromagnetic Railgun Demo](https://www.tiktok.com/@user99861255807300?_t=ZN-8ve2aKE0Rx0&_r=1)

---
### License

This project is licensed under the MIT License. See LICENSE for details.

---