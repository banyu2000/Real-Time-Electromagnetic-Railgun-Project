# ServoCam Test

A real-time object detection and tracking system that integrates **libcamera**, **OpenCV**, and a **PCA9685** PWM servo driver, featuring a Qt-based GUI with auto-scan and tracking modes.

---

## Features

- **Camera Capture**: High-performance image acquisition using **libcamera** and **OpenCV**
- **Object Detection**: Yellow cylindrical can detection via HSV color filtering and contour analysis
- **Servo Control**: Pan-tilt control of two servos through PCA9685 over I²C
- **Tracking Mode**: Real-time target centering by adjusting servo angles based on object position
- **Scan Mode**: Automatic sweeping movement when no target is detected
- **Multithreading**: Dedicated `ServoThread` for servo operations; main thread handles image processing and UI
- **Callback Mechanism**: `Libcam2OpenCV` invokes a callback for each captured frame
- **GUI Interface**: Built with Qt5 Widgets and Qwt thermometer widget to display video feed and system status

---

## Repository Structure

```
├── CMakeLists.txt       # Build configuration
├── main.cpp             # Application entry point, initializes Qt application and main window
├── window.h/.cpp        # Qt window: UI setup, image updates, detection logic, and servo control
├── libcam2opencv.h/.cpp # Wrapper for libcamera to deliver OpenCV Mat frames via callback
├── PCA9685.h/.cpp       # PCA9685 I²C driver implementation and angle-to-PWM mapping
└── README.md            # Project documentation (this file)
```

---

## Hardware Requirements

- A libcamera-supported camera module (e.g., Raspberry Pi Camera)
- PCA9685 PWM driver connected via I²C
- Two standard analog servos connected to PCA9685 channels 0 (pan) and 1 (tilt)
- Linux-based embedded platform (e.g., Raspberry Pi) with I²C enabled (`/dev/i2c-1`)

---

## Software Dependencies

- **C++17**
- **libcamera**
- **OpenCV**
- **Qt5 Widgets**
- **Qwt**
- **pthreads**
- **pkg-config**

---

## Build and Run Instructions

1. **Install Dependencies** (Debian/Ubuntu):
   ```bash
   sudo apt update
   sudo apt install -y libopencv-dev libqt5widgets5 libqt5core5 pkg-config libqwt-qt5-dev libcamera-dev
   ```

2. **Build the Project**:
   ```bash
   mkdir -p build && cd build
   cmake ..
   make -j$(nproc)
   ```

3. **Run the Application**:
   ```bash
   sudo ./ServoCam
   ```
   (*sudo* may be required for I²C access)

---

## Usage

- **Start Servo**: Click to begin or stop automatic scanning.
- **Track Mode**: Toggle to enable or disable object tracking. In tracking mode, the servos will follow the detected can; if no can is found, the system reverts to scan mode.

---

## Module Overview

### libcam2opencv

- Initializes and configures the camera via **libcamera**.
- Maps camera buffers into **OpenCV** `cv::Mat` frames.
- Emits each frame through a virtual callback `hasFrame(const cv::Mat&, const libcamera::ControlList&)`.
- Allows setting capture parameters: resolution, framerate, brightness, contrast.

### PCA9685

- Opens `/dev/i2c-1` and sets up the PWM driver at 50 Hz.
- Provides `setAngle(uint8_t channel, float angle)` to convert angles (0–180°) into PWM pulse widths.
- Handles raw register writes and frequency adjustment.

### Window (Qt GUI)

- Builds the user interface with Qt5 Widgets and a Qwt thermometer for status display.
- Receives each frame in `updateImage()`, applies detection, draws overlays, and updates the video display.
- Contains `ServoThread` for continuous scanning movements and invokes servo adjustments in tracking mode.

---

## Customization

- Modify HSV threshold values in `detectCans()` to detect objects of different colors.
- Adjust pan/tilt step sizes for faster or smoother tracking responses.
- Replace the HSV-based detector with machine learning models (e.g., YOLO, TensorFlow) for advanced object recognition.

---

