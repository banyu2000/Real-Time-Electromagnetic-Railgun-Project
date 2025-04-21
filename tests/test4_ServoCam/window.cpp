// window.cpp
#include "window.h"
#include <opencv2/opencv.hpp>
#include <QMessageBox>

Window::Window(QWidget *parent) : QWidget(parent) {
    setupUI();
    try {
        servoDriver = new PCA9685();
        servoThread = new ServoThread(servoDriver, &servoMutex);
        connect(servoThread, &ServoThread::servoError, this, &Window::handleServoError);
    } catch (const std::exception& e) {
        QMessageBox::critical(this, "Error", e.what());
    }

    myCallback.window = this;
    camera.registerCallback(&myCallback);
    camera.start();
}

Window::~Window() {
    camera.stop();
    servoThread->stop();
    servoThread->wait();
    delete servoThread;
    delete servoDriver;
}

void Window::setupUI() {
    thermo = new QwtThermo;
    thermo->setFillBrush(QBrush(Qt::red));
    thermo->setScale(0, 255);
    
    image = new QLabel;
    QHBoxLayout *hLayout = new QHBoxLayout;
    hLayout->addWidget(thermo);
    hLayout->addWidget(image);

    autoShootBtn = new QPushButton("Start Servo");
    trackShootBtn = new QPushButton("Track Mode");
    connect(autoShootBtn, &QPushButton::clicked, this, &Window::onAutoShoot);
    connect(trackShootBtn, &QPushButton::clicked, this, &Window::onTrackShoot);

    QHBoxLayout *btnLayout = new QHBoxLayout;
    btnLayout->addWidget(autoShootBtn);
    btnLayout->addWidget(trackShootBtn);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addLayout(hLayout);
    mainLayout->addLayout(btnLayout);
}

void Window::onAutoShoot() {
    if (!servoThread->isRunning()) {
        servoThread->start();
        autoShootBtn->setText("Stop Servo");
    } else {
        servoThread->stop();
        autoShootBtn->setText("Start Servo");
    }
}

void Window::onTrackShoot() {
    trackingEnabled = !trackingEnabled;
    trackShootBtn->setText(trackingEnabled ? "Disable Track" : "Track Mode");
    if(trackingEnabled) {
        m_currentPan = 90;
        m_currentTilt = 90;
        m_scanDirection = 1;
    }
}

void Window::calculateServoAngles(const cv::Point &target, const cv::Size &frameSize) {
    const cv::Point frameCenter(frameSize.width/2, frameSize.height/2);
    float xOffset = (target.x - frameCenter.x) / (float)frameCenter.x;
    float yOffset = (target.y - frameCenter.y) / (float)frameCenter.y;

    const float panStep = 0.3f;
    const float tiltStep = 0.2f;

    float newPan = m_currentPan + xOffset * panStep;
    float newTilt = m_currentTilt + yOffset * tiltStep;

    newPan = qBound(0.0f, newPan, 180.0f);
    newTilt = qBound(60.0f, newTilt, 120.0f);

    QMutexLocker locker(&servoMutex);
    servoDriver->setAngle(0, newPan);
    servoDriver->setAngle(1, newTilt);
    m_currentPan = newPan;
    m_currentTilt = newTilt;
}

void Window::scanningMovement() {
    QMutexLocker locker(&servoMutex);
    
    m_currentPan += 2 * m_scanDirection;
    if(m_currentPan >= 180 || m_currentPan <= 0) {
        m_scanDirection *= -1;
        m_currentTilt = qBound(60.0f, m_currentTilt + 5.0f, 120.0f);
        if(m_currentTilt >= 120) m_currentTilt = 60;
    }
    
    servoDriver->setAngle(0, m_currentPan);
    servoDriver->setAngle(1, m_currentTilt);
}

void Window::detectCans(cv::Mat &frame) {
    cv::flip(frame, frame, 0);

    cv::Mat hsv;
    cv::cvtColor(frame, hsv, cv::COLOR_BGR2HSV);

    cv::Scalar lower_yellow(15, 120, 80);
    cv::Scalar upper_yellow(30, 255, 255);
    cv::Mat yellow_mask;
    cv::inRange(hsv, lower_yellow, upper_yellow, yellow_mask);

    cv::Mat kernel = cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(7,7));
    cv::morphologyEx(yellow_mask, yellow_mask, cv::MORPH_CLOSE, kernel);
    cv::morphologyEx(yellow_mask, yellow_mask, cv::MORPH_OPEN, kernel);

    std::vector<std::vector<cv::Point>> contours;
    cv::findContours(yellow_mask, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

    cv::Rect best_rect;
    double max_score = 0.0;

    for (const auto &contour : contours) {
        cv::Rect rect = cv::boundingRect(contour);
        double aspect_ratio = (double)rect.width / rect.height;
        bool aspect_valid = (aspect_ratio > 0.5 && aspect_ratio < 0.7);

        if (aspect_valid) {
            double area = rect.area();
            double aspect_match = 1.0 - fabs(aspect_ratio - 0.583)/0.583;
            double score = area * aspect_match;

            if (score > max_score) {
                max_score = score;
                best_rect = rect;
            }
        }
    }

    if (max_score > 0) {
        cv::rectangle(frame, best_rect, cv::Scalar(0, 255, 255), 2);
        cv::Point center(
            best_rect.x + best_rect.width/2, 
            best_rect.y + best_rect.height/2
        );
        cv::circle(frame, center, 8, cv::Scalar(255, 0, 0), -1);
        m_hasTarget = true;
        m_targetCenter = center;
    } else {
        m_hasTarget = false;
    }
}

void Window::updateImage(const cv::Mat &mat) {
    cv::Mat frame = mat.clone();
    detectCans(frame);
    cv::bitwise_not(frame, frame);
    QImage qimg(frame.data, frame.cols, frame.rows, frame.step, QImage::Format_RGB888);
    image->setPixmap(QPixmap::fromImage(qimg.rgbSwapped()));

    if (trackingEnabled) {
        if(m_hasTarget) {
            calculateServoAngles(m_targetCenter, frame.size());
        } else {
            scanningMovement();
        }
    }

}

void Window::handleServoError() {
    QMessageBox::critical(this, "Servo Error", "Failed to control servo motor");
}
