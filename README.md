# 🦾 Smart Servo Vision System

基于 **Raspberry Pi 5** 实现的一个 **实时视觉识别 + 舵机控制** 系统，使用 **Qt 图形界面** 展示图像识别结果，并驱动 **舵机自动跟踪**目标。

---

## 🧩 功能组成

| 模块         | 描述                                                                 |
|--------------|----------------------------------------------------------------------|
| `window.*`   | 图像采集 + Qt GUI + 图像识别逻辑（包括红色物体检测等）             |
| `PCA9685.*`  | I2C 控制舵机转动模块，基于 PWM 驱动芯片 PCA9685                    |
| `main.cpp`   | 应用入口，负责启动窗口 + 舵机线程 + 信号退出处理                     |
| `qt1.cpp`    | 简化版 UI 演示代码（仅用于测试）                                     |
| `qt2.cpp`    | 基于 `window.h` 的 GUI 示例（可选测试）                             |
| `libcam2opencv.*` | 摄像头回调封装类，用于与 libcamera 接口连接（⚠️ 自定义或外部） |

---

## 📷 功能演示

- 实时摄像头画面采集（OpenCV）
- 检测画面中的红色易拉罐 / 目标物体
- 舵机驱动左右扫动，自动控制角度
- Qt 图形界面实时显示图像 + 可视化温度计组件（`QwtThermo`）

---

## 🚀 编译步骤（Raspberry Pi）

### 1. 安装依赖

```bash
sudo apt update
sudo apt install qtbase5-dev libopencv-dev libcamera-dev libqwt-qt5-dev cmake build-essential
```

### 2. 编译项目

```bash
git clone https://yourrepo.git  # ← 替换为你自己的仓库链接
cd yourrepo
mkdir build && cd build
cmake ..
make -j4
```

### 3. 运行

```bash
./robot_app
```

---

## 🛠️ 项目结构

```
.
├── main.cpp                 # 主程序入口，启动 UI 和舵机线程
├── window.h / window.cpp    # Qt 窗口 + 图像识别 + 摄像头 + 舵机接口
├── PCA9685.h / .cpp         # 舵机控制模块（通过 I2C 接口驱动）
├── qt1.cpp / qt2.cpp        # GUI 简易测试程序（可选）
├── CMakeLists.txt           # 构建配置文件
├── libcam2opencv.h          # 摄像头回调（如为外部库请注明来源）
└── build/                   # 编译输出目录（推荐创建）
```

---

## 🧠 设计特点

- 基于类封装的设计，安全使用回调处理图像帧
- 舵机驱动为独立线程运行，实时响应
- 所有组件均模块化易于拓展（如图像识别加入追踪、联动动作等）
- 完全使用 C++17，适配 Raspberry Pi 5 实时系统需求

---

## 🧪 TODO / 待拓展功能

- [ ] 增加目标颜色自定义识别（GUI 控制）
- [ ] 结合深度学习进行更智能的图像判断
- [ ] 支持舵机反馈回读（通过额外硬件）
- [ ] 将自动发射逻辑与识别结果联动

---

## 📸 示例截图（建议添加）

> 可以上传运行界面截图放在这里说明程序效果，例如图像识别目标 + 舵机控制状态

---

## 📜 License

MIT License / GPLv3（请按你的需求填写）