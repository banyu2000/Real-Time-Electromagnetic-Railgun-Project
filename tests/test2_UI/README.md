# 🧪 Qt UI Integration Test – Camera + GPIO Interface

This test verifies the functionality of the user interface using Qt5 on Raspberry Pi, with real-time camera integration and GPIO control layout preparation.

---

## ✅ Purpose

This integration update:
- Displays live camera feed from Raspberry Pi (libcamera or OpenCV)
- Constructs the main UI layout using Qt5
- Adds two interactive buttons at the bottom of the interface:
  - `Auto Shoot`
  - `Track Shoot`
- These buttons are now visible and placed for future GPIO interaction

---

## 📁 Files in this directory

| File            | Description                                |
|------------------|--------------------------------------------|
| `main.cpp`       | Entry point of the Qt application          |
| `window.cpp`     | Implementation of the UI, image display, and layout |
| `window.h`       | Header for the main Window class           |
| `CMakeLists.txt` | CMake configuration for building the GUI   |

---

## 🔧 Build Instructions

### Requirements:
- Raspberry Pi with Qt5 and OpenCV installed
- Camera module (e.g., Raspberry Pi Camera v1.3) connected via CSI
- CMake, g++, and pkg-config available

### Compile with CMake:
```bash
mkdir -p build && cd build
cmake ..
make
