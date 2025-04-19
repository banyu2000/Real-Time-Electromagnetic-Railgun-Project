#ifndef WINDOW_H
#define WINDOW_H

#include <QWidget>
#include <QBoxLayout>
#include <QLabel>
#include <qwt/qwt_thermo.h>
#include "libcam2opencv.h"
#include "PCA9685.h"

class Window : public QWidget {
    Q_OBJECT

public:
    Window();
    ~Window();
    void updateImage(const cv::Mat &mat);
    void setServoDriver(PCA9685* driver) { servo = driver; }

    QwtThermo    *thermo;
    QHBoxLayout  *hLayout;
    QLabel       *image;

    // 修正回调类：删除多余的参数
    struct MyCallback : Libcam2OpenCV::Callback {
        Window* window = nullptr;
        virtual void hasFrame(const cv::Mat &frame) override {
            if (window) window->updateImage(frame);
        }
    };

    Libcam2OpenCV camera;
    MyCallback myCallback;

private:
    void detectCans(cv::Mat &frame);
    PCA9685* servo = nullptr;
};

#endif // WINDOW_H