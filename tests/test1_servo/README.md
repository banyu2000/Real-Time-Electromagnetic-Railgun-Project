# 🧪 Servo Unit Test – PCA9685 PWM Driver

This test verifies the functionality of the servo control system using a PCA9685 PWM driver over I2C on Raspberry Pi 5.

---

##  Purpose

This unit test:
- Initializes the PCA9685 device via I2C
- Continuously sweeps two servo motors across their full angular range (0° to 180°)
- Uses graceful termination (SIGINT) to reset both servos to 90° on exit

---

##  Files in this directory

| File          | Description                          |
|---------------|--------------------------------------|
| `main.cpp`    | Test program for dual-axis servo sweep |
| `PCA9685.cpp` | Implementation of the I2C PWM driver |
| `PCA9685.h`   | Header file for PCA9685 class        |
| `CMakeLists.txt` | CMake configuration for building the test |

---

##  Build Instructions

### Requirements:
- Raspberry Pi with I2C enabled (`/dev/i2c-1`)
- 2 servo motors connected to PCA9685 (channels 0 and 1)
- CMake & C++17 compiler (e.g., `g++`)

### Compile with CMake:
```bash
mkdir -p build && cd build
cmake ..
make

The output executable will be: servo_test

 ### Run the Test

sudo ./servo_test
The horizontal servo (channel 0) will sweep from 0° to 180°

The vertical servo (channel 1) will sweep from 180° to 0°

Press Ctrl+C to stop – the servos will return to 90° automatically

### Example console output:
Horizontal: 0°   Vertical: 180°
Horizontal: 10°  Vertical: 170°
...
Program terminated. Servos reset.

### Test Outcome Criteria

Metric	Condition
I2C connection	No runtime exception
Angle movement	Smooth servo sweep observed
Exit handling	Servos return to neutral

### Notes：

PCA9685 default I2C address: 0x40

If you're not using /dev/i2c-1, change I2C_DEV in PCA9685.h

Use a dedicated power supply for servos if they draw significant current