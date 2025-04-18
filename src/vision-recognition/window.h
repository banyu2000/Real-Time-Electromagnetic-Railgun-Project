#ifndef WINDOW_H
#define WINDOW_H

#include <qwt/qwt_thermo.h>
#include <QBoxLayout>
#include <QPushButton>
#include <QLabel>
#include "libcam2opencv.h"
#include "PCA9685.h"

class Window : public QWidget
{
    Q_OBJECT

public:
    Window();
    ~Window();
    void updateImage(const cv::Mat &mat);

private:
    void detectCans(cv::Mat &frame);
    void controlServos(const cv::Rect &rect, int imgW, int imgH);

    QwtThermo    *thermo;
    QHBoxLayout  *hLayout;
    QLabel       *image;

    struct MyCallback : Libcam2OpenCV::Callback {
        Window* window = nullptr;
        virtual void hasFrame(const cv::Mat &frame, const libcamera::ControlList &) override {
            if (window) window->updateImage(frame);
        }
    } myCallback;

    Libcam2OpenCV camera;
    PCA9685 pca_;
};

#endif // WINDOW_H
