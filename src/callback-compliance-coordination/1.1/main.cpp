#include <QApplication>
#include "window.h"
#include "PCA9685.h"
#include <thread>
#include <atomic>
#include <chrono>
#include <csignal>
#include <iostream>

std::atomic<bool> running(true);
PCA9685* pca = nullptr;

// 信号处理：优雅退出
void signalHandler(int signum) {
    running = false;
    if (pca) {
        pca->setAngle(0, 90);
        pca->setAngle(1, 90);
        std::cout << "\nServos reset." << std::endl;
    }
    exit(signum);
}

// 舵机控制线程（示例：自动左右扫动）
void servoLoop(PCA9685* driver) {
    int angle = 0;
    bool forward = true;
    while (running) {
        driver->setAngle(0, angle);
        driver->setAngle(1, 180 - angle);

        if (forward) angle += 10;
        else angle -= 10;

        if (angle >= 180) forward = false;
        if (angle <= 0) forward = true;

        std::this_thread::sleep_for(std::chrono::milliseconds(300));
    }
}

int main(int argc, char *argv[]) {
    signal(SIGINT, signalHandler);

    QApplication app(argc, argv);

    PCA9685 driver;
    pca = &driver;

    // UI 和视觉窗口
    Window window;
    window.setServoDriver(&driver);
    window.show();

    // 启动舵机线程
    std::thread servoThread(servoLoop, &driver);

    int ret = app.exec();
    running = false;
    if (servoThread.joinable()) servoThread.join();
    return ret;
}
