// libcam2opencv.h
#ifndef LIBCAM2OPENCV_H
#define LIBCAM2OPENCV_H

#include <opencv2/opencv.hpp>
#include <QTimer>
#include <QObject>

class Libcam2OpenCV : public QObject {
    Q_OBJECT

public:
    class Callback {
    public:
        virtual void hasFrame(const cv::Mat& frame) = 0;
        virtual ~Callback() {}
    };

    Libcam2OpenCV(QObject* parent = nullptr);
    ~Libcam2OpenCV();

    void start();
    void stop();
    void registerCallback(Callback* cb);

private slots:
    void captureFrame();

private:
    cv::VideoCapture cap;
    QTimer* timer;
    Callback* callback = nullptr;
};

#endif // LIBCAM2OPENCV_H