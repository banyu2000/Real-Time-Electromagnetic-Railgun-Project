#include <QApplication>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QWidget>
#include <QLabel>
#include <QTimer>
#include <QSlider>
#include <QImage>
#include <QPixmap>
#include <opencv2/opencv.hpp>
#include <wiringPi.h>
#include "PCA9685Driver.h"

#define FIRE_PIN 6  // BCM GPIO6, wiringPi number 22

// Convert OpenCV Mat to QImage
QImage MatToQImage(const cv::Mat &mat) {
    return QImage(mat.data, mat.cols, mat.rows, mat.step, QImage::Format_RGB888).rgbSwapped();
}

class MainWindow : public QWidget {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr)
        : QWidget(parent),
        servo1(0),  // Servo on Channel 0
        servo2(1)   // Servo on Channel 1
    {
        setWindowTitle("Railgun Control Panel");

        // Camera display
        cameraLabel = new QLabel("Camera Feed");
        cameraLabel->setFixedSize(400, 300);

        // Servo angle labels
        label1 = new QLabel("Servo 1 Angle: 0°");
        label2 = new QLabel("Servo 2 Angle: 0°");

        // Servo control sliders
        slider1 = new QSlider(Qt::Horizontal);
        slider1->setRange(0, 180);
        slider2 = new QSlider(Qt::Horizontal);
        slider2->setRange(0, 180);

        // Fire button
        fireButton = new QPushButton("FIRE!");

        // Layout
        QVBoxLayout *mainLayout = new QVBoxLayout();
        mainLayout->addWidget(cameraLabel);

        QHBoxLayout *servo1Layout = new QHBoxLayout();
        servo1Layout->addWidget(label1);
        servo1Layout->addWidget(slider1);

        QHBoxLayout *servo2Layout = new QHBoxLayout();
        servo2Layout->addWidget(label2);
        servo2Layout->addWidget(slider2);

        mainLayout->addLayout(servo1Layout);
        mainLayout->addLayout(servo2Layout);
        mainLayout->addWidget(fireButton);

        setLayout(mainLayout);

        // Camera
        cap.open(0);
        if (!cap.isOpened()) {
            cameraLabel->setText("Failed to open camera!");
        }

        // Timer
        QTimer *timer = new QTimer(this);
        connect(timer, &QTimer::timeout, this, &MainWindow::updateFrame);
        timer->start(50);  // 20 FPS

        // Connect signals
        connect(slider1, &QSlider::valueChanged, this, &MainWindow::updateServo1);
        connect(slider2, &QSlider::valueChanged, this, &MainWindow::updateServo2);
        connect(fireButton, &QPushButton::clicked, this, &MainWindow::fire);

        // Initialize GPIO
        if (wiringPiSetup() == -1) {
            qDebug("WiringPi setup failed!");
        } else {
            pinMode(FIRE_PIN, OUTPUT);
            digitalWrite(FIRE_PIN, LOW);  // Default to LOW
        }
    }

    ~MainWindow() {
        cap.release();
        digitalWrite(FIRE_PIN, LOW); // Turn off firing pin on exit
    }

private slots:
    void updateFrame() {
        cv::Mat frame;
        cap >> frame;
        if (!frame.empty()) {
            cameraLabel->setPixmap(QPixmap::fromImage(MatToQImage(frame)).scaled(cameraLabel->size(), Qt::KeepAspectRatio));
        }
    }

    void updateServo1(int angle) {
        servo1.setAngle(angle);
        label1->setText(QString("Servo 1 Angle: %1°").arg(angle));
    }

    void updateServo2(int angle) {
        servo2.setAngle(angle);
        label2->setText(QString("Servo 2 Angle: %1°").arg(angle));
    }

    void fire() {
        qDebug("Fire button pressed!");
        digitalWrite(FIRE_PIN, HIGH);  // Trigger fire
        QTimer::singleShot(500, this, []() {
            digitalWrite(FIRE_PIN, LOW);  // Auto turn off after 500ms
        });
    }

private:
    QLabel *cameraLabel;
    QLabel *label1;
    QLabel *label2;
    QSlider *slider1;
    QSlider *slider2;
    QPushButton *fireButton;

    cv::VideoCapture cap;
    PCA9685Driver servo1;
    PCA9685Driver servo2;
};

#include "main.moc"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    MainWindow window;
    window.resize(420, 600);
    window.show();
    return app.exec();
}
