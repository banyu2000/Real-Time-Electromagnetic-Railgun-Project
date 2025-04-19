#ifndef WINDOW_H
#define WINDOW_H

#include <qwt/qwt_thermo.h>
#include <QBoxLayout>
#include <QPushButton>
#include <QLabel>
#include "libcam2opencv.h"
#include "GpioControl.h"

class Window : public QWidget {
    ...
private:
    GpioControl* gpio;
};

class Window : public QWidget
{
    Q_OBJECT

public:
    Window();
    ~Window();
    void updateImage(const cv::Mat &mat);

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
};

#endif // WINDOW_H
