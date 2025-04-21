# 📅 Project Development Log

This document records key milestones and development progress of the Real-Time Electromagnetic Railgun Project.

---

##  Log Format

| Date       | Task / Milestone                                 | Description / Outcome                                     | Responsible |
|------------|--------------------------------------------------|------------------------------------------------------------|-------------|

| **Feb 5**  |  Pitch Day                                      | Initial team formation and concept pitching                | All members |
| **Feb 26** |  Hardware Prepared                              | All critical components (servo, PCA9685, camera, relays) purchased | All members |
| **Mar 5**  |  Soldering Manual Switch Circuit                | Physical switch-based charging/discharging circuit completed | All members |
| **Mar 12** |  Successful Physical Firing Test                | Manual trigger circuit connected to railgun, projectile launched successfully without Raspberry Pi | All members |
| **Mar 19** |  Knowledge Allocation & Research Begins         | Team divided into respective development areas (vision, IO, GUI, servo, callback) | All members |
| **Apr 10** |  Servo Module Functional                        | Dual-axis servo control tested using PCA9685 driver         | Baichuan Shi |
| **Apr 13** |  Vision Module Working                         | Object detection module (YOLO) tested successfully           | Yu Ban |
| **Apr 15** |  GUI Operational                                | Qt interface setup with video stream and control panels      | Zhongkai Zhang |
| **Apr 16** |  GitHub Structure Optimization Started          | Repo structure, commits, and branches reviewed and re-organized | Baichuan Shi |
| **Apr 17** |  MOSFET Burnout Identified & Analyzed          | During testing, charging transistor blew due to overcurrent; cause diagnosed and relay replacement proposed | Reported by Baichuan Shi, followed up by Xiang Chen |
| **Apr 18** |  Callback Function Integration                  | Callback mechanism and multi-threaded control logic successfully integrated between vision, servo, and IO | Kunjin Chen |
| **Apr 19** |  Relay Control Circuit Integrated               | Relay-based switching tested and confirmed operational       | Xiang Chen |
| **Apr 20** |  Final Version Released                         | Code and hardware integration completed, system runs stably | All members |

---

##  Notes

- Major testing milestones were recorded with video or physical measurement evidence.
- All code components were developed and pushed under `feature/` branches, then integrated via `develop`.

---

## 📜 License & Credits

This log is part of the documentation for the Real-Time Electromagnetic Railgun Project under [MIT License](../LICENSE).
