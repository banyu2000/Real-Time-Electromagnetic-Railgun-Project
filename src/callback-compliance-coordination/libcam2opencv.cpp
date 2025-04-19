// libcam2opencv.cpp
#include "libcam2opencv.h"
#include <QDebug>

Libcam2OpenCV::Libcam2OpenCV(QObject* parent) : QObject(parent), timer(new QTimer(this)) {
    connect(timer, &QTimer::timeout, this, &Libcam2OpenCV::captureFrame);
}

Libcam2OpenCV::~Libcam2OpenCV() {
    stop();
}

void Libcam2OpenCV::registerCallback(Callback* cb) {
    callback = cb;
}

void Libcam2OpenCV::start() {
    if (!cap.open(0)) {
        qWarning() << "[Camera] Failed to open camera.";
        return;
    }
    timer->start(33); // ~30 FPS
}

void Libcam2OpenCV::stop() {
    if (cap.isOpened()) {
        cap.release();
    }
    timer->stop();
}

void Libcam2OpenCV::captureFrame() {
    if (!cap.isOpened() || !callback) return;

    cv::Mat frame;
    cap >> frame;
    if (!frame.empty()) {
        callback->hasFrame(frame);
    }
}