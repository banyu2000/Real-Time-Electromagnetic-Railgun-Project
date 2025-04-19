#include "window.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPixmap>
#include <QDebug>

Window::Window() {
    setWindowTitle("IO Control UI");

    // === 创建控件 ===
    image = new QLabel("Loading camera...");
    image->setFixedHeight(400);  // 可以根据需要调整大小
    image->setAlignment(Qt::AlignCenter);
    image->setStyleSheet("background-color: #111; color: white; font-size: 20px;");

    QPushButton *autoShootBtn = new QPushButton("Auto Shoot");
    QPushButton *trackShootBtn = new QPushButton("Track Shoot");

    autoShootBtn->setMinimumHeight(60);
    trackShootBtn->setMinimumHeight(60);
    autoShootBtn->setStyleSheet("font-size: 18px;");
    trackShootBtn->setStyleSheet("font-size: 18px;");

    // === 设置布局 ===
    QVBoxLayout *mainLayout = new QVBoxLayout(this);         // 整体上下布局
    mainLayout->addWidget(image);                            // 上方摄像头图像

    QHBoxLayout *buttonLayout = new QHBoxLayout();           // 按钮水平布局
    buttonLayout->addWidget(autoShootBtn);
    buttonLayout->addWidget(trackShootBtn);

    mainLayout->addLayout(buttonLayout);                     // 按钮加到底部
    setLayout(mainLayout);

    // === 摄像头初始化 ===
    camera.start(&myCallback);           // 启动你自己的摄像头类
    myCallback.window = this;            // 设置回调里的指针，绑定 updateImage
}

Window::~Window() {
    camera.stop();  // 停止摄像头
}

void Window::updateImage(const cv::Mat &mat) {
    if (mat.empty()) return;

    QImage img(mat.data, mat.cols, mat.rows, mat.step, QImage::Format_RGB888);
    image->setPixmap(QPixmap::fromImage(img.rgbSwapped()).scaled(image->size(), Qt::KeepAspectRatio));
}

void Window::detectCans(cv::Mat &frame) {
    // 暂时不实现，可在此添加红色罐子识别逻辑
}

g++ qt1.cpp -o qt1 `pkg-config --cflags --libs Qt5Widgets opencv4`
