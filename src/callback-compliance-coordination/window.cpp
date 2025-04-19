#include "window.h"
#include <opencv2/opencv.hpp>

Window::Window() : QWidget() {
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
    camera.start();
}

Window::~Window() {
    camera.stop();
}

void Window::detectCans(cv::Mat &frame) {
    cv::flip(frame, frame, 0); // 保持上下翻转

    // 1. 转换到HSV颜色空间
    cv::Mat hsv;
    cv::cvtColor(frame, hsv, cv::COLOR_BGR2HSV);

    // 2. 黄色HSV范围
    cv::Scalar lower_yellow(15, 120, 80);
    cv::Scalar upper_yellow(30, 255, 255);
    cv::Mat yellow_mask;
    cv::inRange(hsv, lower_yellow, upper_yellow, yellow_mask);

    // 3. 形态学强化处理
    cv::Mat kernel = cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(7,7));
    cv::morphologyEx(yellow_mask, yellow_mask, cv::MORPH_CLOSE, kernel);
    cv::morphologyEx(yellow_mask, yellow_mask, cv::MORPH_OPEN, kernel);

    // 4. 轮廓检测与过滤
    std::vector<std::vector<cv::Point>> contours;
    cv::findContours(yellow_mask, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

    cv::Rect best_rect;
    double max_score = 0.0;

    for (const auto &contour : contours) {
        cv::Rect rect = cv::boundingRect(contour);
        
        // 直接使用像素宽高比筛选 (7:12≈0.583)
        double aspect_ratio = (double)rect.width / rect.height;
        bool aspect_valid = (aspect_ratio > 0.5 && aspect_ratio < 0.7); // ±20%容差

        if (aspect_valid) {
            // 综合评分（面积 + 宽高比匹配度）
            double area = rect.area();
            double aspect_match = 1.0 - fabs(aspect_ratio - 0.583)/0.583; // 0~1
            double score = area * aspect_match;

            if (score > max_score) {
                max_score = score;
                best_rect = rect;
            }
        }
    }

    // 5. 绘制结果
    if (max_score > 0) {
        cv::rectangle(frame, best_rect, cv::Scalar(0, 255, 255), 2); // 黄色框
        cv::Point center(
            best_rect.x + best_rect.width/2, 
            best_rect.y + best_rect.height/2
        );
        cv::circle(frame, center, 8, cv::Scalar(255, 0, 0), -1); // 蓝色中心点
    }
}

void Window::updateImage(const cv::Mat &mat) {
    cv::Mat processedFrame = mat.clone();
    detectCans(processedFrame); // 调用优化后的检测方法

    // 显示处理后的图像
    QImage frame(processedFrame.data, processedFrame.cols, processedFrame.rows, 
                 processedFrame.step, QImage::Format_RGB888);
    image->setPixmap(QPixmap::fromImage(frame.rgbSwapped()).scaled(image->size(), Qt::KeepAspectRatio));

    // 更新温度计（可选）
    const int h = frame.height();
    const int w = frame.width();
    const QColor c = frame.pixelColor(w/2, h/2);
    thermo->setValue(c.lightness());
    update();
}