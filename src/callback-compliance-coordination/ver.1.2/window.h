// window.h
#ifndef WINDOW_H
#define WINDOW_H

// 先包含Qt头文件
#include <QObject>
#include <QThread>
#include <QMutex>
#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QBoxLayout>
#include <qwt/qwt_thermo.h>

// 显式包含OpenCV头文件
#include <opencv2/opencv.hpp>

// 最后包含其他头文件
#include "libcam2opencv.h"
#include "PCA9685.h"


class ServoThread : public QThread {
    Q_OBJECT
public:
    ServoThread(PCA9685* driver, QMutex* mutex)
        : m_driver(driver), m_mutex(mutex), m_running(true) {}
    
    void run() override {
        try {
            while(m_running) {
                QMutexLocker locker(m_mutex);
                for(int angle = 0; angle <= 180 && m_running; angle += 10) {
                    m_driver->setAngle(0, angle);
                    m_driver->setAngle(1, 180 - angle);
                    QThread::sleep(1);
                }
            }
        } catch (...) {
            emit servoError(); // 正确使用emit
        }
    }

    void stop() { m_running = false; }

signals:  // 信号声明
    void servoError();

private:
    PCA9685* m_driver;
    QMutex* m_mutex;
    bool m_running;
};

class Window : public QWidget {
    Q_OBJECT
public:
    Window(QWidget *parent = nullptr);
    ~Window();

    void updateImage(const cv::Mat &mat);

public slots:  // 正确定义槽
    void onAutoShoot();
    void onTrackShoot();
    void handleServoError();

private:
    void detectCans(cv::Mat &frame);
    void setupUI();
    void calculateServoAngles(const cv::Point &target, const cv::Size &frameSize);
    void scanningMovement();

    QwtThermo *thermo;
    QLabel *image;
    QPushButton *autoShootBtn;
    QPushButton *trackShootBtn;
    
    PCA9685 *servoDriver;
    QMutex servoMutex;
    ServoThread *servoThread;
    bool trackingEnabled = false;

    cv::Point m_targetCenter;
    bool m_hasTarget = false;
    int m_scanDirection = 1;
    float m_currentPan = 90;
    float m_currentTilt = 90;

    Libcam2OpenCV camera;
    struct MyCallback : Libcam2OpenCV::Callback {
        Window* window = nullptr;
        virtual void hasFrame(const cv::Mat &frame, const libcamera::ControlList &) {
            if (window) window->updateImage(frame);
        }
    } myCallback;
};

#endif // WINDOW_H
