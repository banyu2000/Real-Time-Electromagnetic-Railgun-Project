#ifndef WINDOW_H
#define WINDOW_H

#include <qwt/qwt_thermo.h>
#include <QBoxLayout>
#include <QPushButton>
#include <QLabel>
#include "libcam2opencv.h"

class Window : public QWidget
{
    Q_OBJECT

public:
    Window();
    ~Window();
    void updateImage(const cv::Mat &mat);
    void setServoDriver(PCA9685* driver) { servo = driver; }

    QwtThermo    *thermo;
    QHBoxLayout  *hLayout;
    QLabel       *image;

    struct MyCallback : Libcam2OpenCV::Callback {
        Window* window = nullptr;
        virtual void hasFrame(const cv::Mat &frame, const libcamera::ControlList &) {
            if (nullptr != window) {
                window->updateImage(frame);
            }
        }
    };

    Libcam2OpenCV camera;
    MyCallback myCallback;

private:
    void detectCans(cv::Mat &frame); // 新增方法：检测红色易拉罐
    PCA9685* servo = nullptr;
};

#endif // WINDOW_H
