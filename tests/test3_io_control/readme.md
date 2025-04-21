#  Relay Switch Unit Test – GPIO Control with libgpiod

This unit test verifies GPIO-based relay switching using the `libgpiod` C++ library on Raspberry Pi 5.  
It simulates a charge–discharge sequence using two GPIO pins controlling physical switches.

---

##  Purpose

This test program:
- Initializes two GPIO output lines (GPIO27 and GPIO22)
- Responds to user input (`on`) to activate a switch sequence
- Gracefully shuts down and resets the switches on `Ctrl+C` or `exit` command

---

##  Files in This Directory

| File           | Description                                   |
|----------------|-----------------------------------------------|
| `main.cpp`     | Main test program with GPIO sequence control  |
| `CMakeLists.txt` | Add if using CMake to build the program |

---

##  Build Instructions

### Prerequisites:
- Raspberry Pi 5 with GPIO support
- Relay or LED circuit connected to GPIO27 and GPIO22
- `libgpiod` installed:
  ```bash
  sudo apt install libgpiod-dev
  ```
  
---

## Compile Manually:
```bash
mkdir build && cd build
cmake ..
make
```
---

## Run the Test
```bash
 ./relay_test
```
---

### You will see a prompt like:
```scss
System ready. GPIO27(switch1), GPIO22(switch2)
> 
```
Type on to trigger the switch sequence:

switch1 turns ON for 2s → OFF

0.5s pause

switch2 turns ON for 1s → OFF

Type exit or press Ctrl+C to quit. All GPIOs will be turned off cleanly.

---

## Test Outcome Criteria

Metric | Condition
GPIO Initialization | No runtime exception or access error
Sequence Logic | Relays trigger in correct timing
Signal Handling | Exits cleanly on Ctrl+C, resets pins

---

### Notes

-Uses GPIO27 and GPIO22 by default. Change in SequenceController if needed.

-Requires libgpiod and root privileges (sudo) to access GPIO.

-You can connect relays or LEDs for visual confirmation.

---

## Example Output
```YAML
System ready. GPIO27(switch1), GPIO22(switch2)
> on
Switch ON
Switch OFF
Switch ON
Switch OFF
> exit
System shutdown
```
---