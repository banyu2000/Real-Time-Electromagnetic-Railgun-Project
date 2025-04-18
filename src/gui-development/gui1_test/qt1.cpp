#include <QApplication>
#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QTimer>
#include <QImage>
#include <QPixmap>
#include <QDebug>
#include <opencv2/opencv.hpp>

// 将 OpenCV 的 cv::Mat 转换为 Qt 可显示的 QImage
QImage MatToQImage(const cv::Mat &mat) {
    return QImage(mat.data, mat.cols, mat.rows, mat.step, QImage::Format_RGB888).rgbSwapped();
}

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    QWidget window;
    window.setWindowTitle("IO Control UI");

    // === 主布局（垂直） ===
    QVBoxLayout *mainLayout = new QVBoxLayout(&window);

    // === 顶部摄像头显示区域 ===
    QLabel *cameraLabel = new QLabel("Camera Loading...");
    cameraLabel->setFixedHeight(400);  // 可根据实际屏幕设置
    cameraLabel->setAlignment(Qt::AlignCenter);
    cameraLabel->setStyleSheet("background-color: #111; color: white; font-size: 20px;");
    mainLayout->addWidget(cameraLabel);

    // === 底部按钮区域（水平） ===
    QHBoxLayout *buttonLayout = new QHBoxLayout();

    QPushButton *autoShootButton = new QPushButton("Auto Shoot");
    QPushButton *trackShootButton = new QPushButton("Track Shoot");

    autoShootButton->setMinimumHeight(60);
    trackShootButton->setMinimumHeight(60);
    autoShootButton->setStyleSheet("font-size: 18px;");
    trackShootButton->setStyleSheet("font-size: 18px;");

    buttonLayout->addWidget(autoShootButton);
    buttonLayout->addWidget(trackShootButton);
    mainLayout->addLayout(buttonLayout);

    window.setLayout(mainLayout);
    window.resize(800, 600);
    window.show();

    // === 打开摄像头 ===
    cv::VideoCapture cap(0);
    if (!cap.isOpened()) {
        qDebug() << "⚠️ Failed to open camera.";
        cameraLabel->setText("Camera Error");
    } else {
        // 定时器：30ms 刷新一次摄像头画面
        QTimer *timer = new QTimer(&window);
        QObject::connect(timer, &QTimer::timeout, [&]() {
            cv::Mat frame;
            cap >> frame;
            if (!frame.empty()) {
                cv::flip(frame, frame, 1);  // 水平翻转（可选）
                QImage img = MatToQImage(frame);
                cameraLabel->setPixmap(QPixmap::fromImage(img).scaled(cameraLabel->size(), Qt::KeepAspectRatio));
            }
        });
        timer->start(30);
    }

    // === 按钮槽函数（添加你自己的控制逻辑） ===
    QObject::connect(autoShootButton, &QPushButton::clicked, [&]() {
        qDebug() << "[Auto Shoot] button pressed.";
        // TODO: 调用舵机、发射控制
    });

    QObject::connect(trackShootButton, &QPushButton::clicked, [&]() {
        qDebug() << "[Track Shoot] button pressed.";
        // TODO: 调用图像识别、目标定位、发射
    });

    return app.exec();
}
g++ qt1.cpp -o qt1 `pkg-config --cflags --libs Qt5Widgets opencv4`
