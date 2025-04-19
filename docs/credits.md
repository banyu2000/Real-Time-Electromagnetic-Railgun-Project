# 👥 Project Team & Responsibilities

This document outlines the role and responsibilities of each team member in the **Real-Time Electromagnetic Railgun Project**, as well as the technical scope they covered.

---

## 🧑‍💻 Team Members & Detailed Contributions

### **🧭 Xiang Chen (3052701) – I/O Control & Switching Logic**

#### 🔹 Background
Develop I/O port drivers and control logic in `feature/io-control` to ensure stable hardware signal transmission.

#### 🔹 Responsibilities
- GPIO initialization via Linux subsystem
- Designed full port mapping table and pull-up/down setup
- Developed switch state machine (standby → ready → launch → reset)
- Implemented interlock + fault detection (short-circuit, voltage anomalies)
- Integrated I/O with servo and callback modules via thread-safe queues

#### ✅ Acceptance Criteria
- Signal delay ≤ 100μs, 100% I/O read/write accuracy
- Emergency stop triggered in ≤ 50ms
- Fully documented driver code

---

### **⚙️ Baichuan Shi (2982811) – Servo Control Module**

#### 🔹 Background
Servo control is key for adjusting the firing angle in the railgun. A stable, precise control system is required.

#### 🔹 Responsibilities
- Designed PID-based angle adjustment algorithm (±0.5° tolerance)
- Wrote custom servo driver via PCA9685
- Monitored real-time angle, speed, and errors
- Integrated control with vision and UI
- Conducted performance testing

#### ✅ Acceptance Criteria
- Angle error ≤ ±0.5°
- Fast target response, stable output
- Robust driver and feedback loop

---

### **🧠 Kunjin Chen (2981978) – Callback Coordination & Threading Architecture**

#### 🔹 Background
Ensure real-time concurrent processing between vision, servo, and I/O modules via efficient callback architecture.

#### 🔹 Responsibilities
- Designed C++ virtual interface / Qt signal-slot based callbacks
- Implemented asynchronous dispatching + exception handling
- Built multithreaded architecture using C++11 / QThreads
- Optimized for performance (e.g., lock-free queues, SIMD)
- Ensured memory safety with smart pointers

#### ✅ Acceptance Criteria
- Vision callback delay ≤ 100ms, servo delay ≤ 50ms
- CPU load ≤ 80% under stress
- ≥ 95% error handling coverage, ≤ 2s recovery time

---

### **📷 Yu Ban (2931341) – Vision Recognition**

#### 🔹 Background
Enhance the accuracy and efficiency of target detection in the `feature/vision-recognition` branch.

#### 🔹 Responsibilities
- Integrated and optimized YOLOv8-light for real-time detection
- Enhanced image pre-processing (noise filtering, white balance)
- Implemented DeepSORT-based multi-target tracking
- Packaged module as `libvision.so`, created API docs
- Achieved 80%+ unit test coverage

#### ✅ Acceptance Criteria
- mAP@0.5 ≥ 85%, inference time/frame ≤ 150ms
- Modular, documented, and testable code

---

### **🖥 Zhongkai Zhang (2969533) – Qt GUI Development**

#### 🔹 Background
Integrate live video and GPIO control into a smooth, functional Qt interface.

#### 🔹 Responsibilities
- Designed UI layout (video, controls, status monitor)
- Connected live PiCamera feed via OpenCV
- Implemented resolution settings, start/stop threading
- Built GPIO status widgets (e.g., QLEDWidget, error log)
- Created reusable Qt components with Doxygen docs

#### ✅ Acceptance Criteria
- Stream delay ≤ 500ms, UI buttons functional
- GPIO control latency ≤ 200ms
- Code passed PR review and Qt guidelines

---

## 🤝 Teamwork and Integration

- Followed GitFlow model with `feature/`, `develop`, and `release` branches
- Used GitHub Issues to manage tasks and document bugs
- Integrated via callback pipelines across vision ↔ servo ↔ IO
- Each member contributed test cases and documentation

---

## 🙏 Acknowledgements

This project was developed as part of **ENG5220: Real-Time Embedded Systems** at the University of Glasgow.

Special thanks to:

- **Prof. Bernd Porr** – for guidance on embedded control systems
- **Dr. Chongfeng Wei** – for support on hardware interfacing
- **All TAs and lab staff** – for test environment and debugging help

---

## 📜 License

Distributed under the [MIT License](../LICENSE).  
We welcome forks, feedback, and future contributions.
