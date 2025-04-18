#include "window.h"
#include <opencv2/opencv.hpp>
#include <algorithm>

Window::Window()
{
    myCallback.window = this;
    camera.registerCallback(&myCallback);

    thermo = new QwtThermo;
    thermo->setFillBrush(QBrush(Qt::red));
    thermo->setScale(0, 255);
    thermo->show();

    image = new QLabel;
    hLayout = new QHBoxLayout();
    hLayout->addWidget(thermo);
    hLayout->addWidget(image);
    setLayout(hLayout);

    try {
        // 初始化 PCA9685 驱动
    } catch (const std::exception& e) {
        qFatal("PCA9685 初始化失败：%s", e.what());
    }

    camera.start();
}

Window::~Window() {
    camera.stop();
}

void Window::updateImage(const cv::Mat &mat) {
    cv::Mat processedFrame = mat.clone();
    detectCans(processedFrame);

    const QImage frame(processedFrame.data, processedFrame.cols, processedFrame.rows,
                       processedFrame.step, QImage::Format_RGB888);
    image->setPixmap(QPixmap::fromImage(frame));

    const int h = frame.height();
    const int w = frame.width();
    const QColor c = frame.pixelColor(w/2, h/2);
    thermo->setValue(c.lightness());
    update();
}

void Window::detectCans(cv::Mat &frame) {
    cv::flip(frame, frame, 0);

    cv::Mat hsv;
    cv::cvtColor(frame, hsv, cv::COLOR_BGR2HSV);

    cv::Scalar lower_yellow(15, 120, 80);
    cv::Scalar upper_yellow(30, 255, 255);
    cv::Mat yellow_mask;
    cv::inRange(hsv, lower_yellow, upper_yellow, yellow_mask);

    cv::Mat kernel = cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(7, 7));
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
            double aspect_match = 1.0 - fabs(aspect_ratio - 0.583) / 0.583;
            double score = area * aspect_match;

            if (score > max_score) {
                max_score = score;
                best_rect = rect;
            }
        }
    }

    if (max_score > 0) {
        cv::rectangle(frame, best_rect, cv::Scalar(0, 255, 255), 2);
        cv::Point center(best_rect.x + best_rect.width / 2,
                         best_rect.y + best_rect.height / 2);
        cv::circle(frame, center, 8, cv::Scalar(255, 0, 0), -1);

        controlServos(best_rect, frame.cols, frame.rows);
    }
}

void Window::controlServos(const cv::Rect &rect, int imgW, int imgH) {
    float cx = rect.x + rect.width * 0.5f;
    float cy = rect.y + rect.height * 0.5f;
    float normX = cx / float(imgW) - 0.5f;
    float normY = cy / float(imgH) - 0.5f;

    float hAng = std::clamp(90.0f + normX * 180.0f, 0.0f, 180.0f);
    float vAng = std::clamp(90.0f + normY * 180.0f, 0.0f, 180.0f);

    pca_.setAngle(0, hAng);
    pca_.setAngle(1, vAng);
}
