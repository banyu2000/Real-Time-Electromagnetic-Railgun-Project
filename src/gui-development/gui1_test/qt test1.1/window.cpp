#include "window.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QDebug>
#include <QImage>
#include <QPixmap>
#include <QColor>
#include <QPushButton>

Window::Window() {
    myCallback.window = this;
    camera.registerCallback(&myCallback);
    camera.start();

    // 创建温度计控件
    thermo = new QwtThermo;
    thermo->setFillBrush(Qt::red);
    thermo->setRange(0, 255);

    // 创建图像显示控件
    image = new QLabel("Loading camera...");
    image->setAlignment(Qt::AlignCenter);
    image->setStyleSheet("background-color: #111; color: white;");

    // 创建按钮
    QPushButton *autoShootBtn = new QPushButton("Auto Shoot");
    QPushButton *trackShootBtn = new QPushButton("Track Shoot");
    autoShootBtn->setMinimumHeight(50);
    trackShootBtn->setMinimumHeight(50);
    autoShootBtn->setStyleSheet("font-size: 18px;");
    trackShootBtn->setStyleSheet("font-size: 18px;");

    // 布局：顶部是 thermo + 图像
    QHBoxLayout *topLayout = new QHBoxLayout;
    topLayout->addWidget(thermo);
    topLayout->addWidget(image);

    // 底部按钮布局
    QHBoxLayout *buttonLayout = new QHBoxLayout;
    buttonLayout->addWidget(autoShootBtn);
    buttonLayout->addWidget(trackShootBtn);

    // 主布局：垂直排列
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addLayout(topLayout);
    mainLayout->addLayout(buttonLayout);

    setLayout(mainLayout);

    // 按钮槽函数（暂不连接实际逻辑，仅测试）
    connect(autoShootBtn, &QPushButton::clicked, this, []() {
        qDebug() << "[Auto Shoot] Button clicked";
    });
    connect(trackShootBtn, &QPushButton::clicked, this, []() {
        qDebug() << "[Track Shoot] Button clicked";
    });
}

Window::~Window() {
    camera.stop();
}

void Window::updateImage(const cv::Mat &mat) {
    cv::Mat frame = mat.clone();
    detectCans(frame);  // 可选：红色易拉罐识别

    QImage qImage(frame.data, frame.cols, frame.rows, frame.step, QImage::Format_RGB888);
    image->setPixmap(QPixmap::fromImage(qImage.rgbSwapped()).scaled(image->size(), Qt::KeepAspectRatio));

    // 取图像中心像素亮度显示在 Thermo 上
    cv::Vec3b centerPixel = mat.at<cv::Vec3b>(mat.rows / 2, mat.cols / 2);
    QColor color(centerPixel[2], centerPixel[1], centerPixel[0]);
    thermo->setValue(color.lightness());
}

void Window::detectCans(cv::Mat &frame) {
    cv::flip(frame, frame, 1);  // 水平翻转
    cv::Mat hsv, mask;
    cv::cvtColor(frame, hsv, cv::COLOR_BGR2HSV);

    // 黄色 HSV 区间（可调）
    cv::Scalar lower_yellow(20, 100, 100);
    cv::Scalar upper_yellow(40, 255, 255);
    cv::inRange(hsv, lower_yellow, upper_yellow, mask);

    // 形态学去噪
    cv::erode(mask, mask, cv::Mat(), cv::Point(-1, -1), 2);
    cv::dilate(mask, mask, cv::Mat(), cv::Point(-1, -1), 2);

    std::vector<std::vector<cv::Point>> contours;
    cv::findContours(mask, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

    for (const auto &c : contours) {
        cv::Rect r = cv::boundingRect(c);
        float ratio = (float)r.width / r.height;
        if (ratio > 0.4 && ratio < 2.5 && r.area() > 1000) {
            cv::rectangle(frame, r, cv::Scalar(0, 255, 255), 2);
            cv::Point center(r.x + r.width / 2, r.y + r.height / 2);
            cv::circle(frame, center, 5, cv::Scalar(255, 0, 0), -1);
        }
    }
}
