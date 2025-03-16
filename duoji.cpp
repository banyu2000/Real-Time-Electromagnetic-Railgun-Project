#include <iostream>
#include <functional>
#include <wiringPi.h>

#define SERVO_PIN 1  // PWM 引脚

class ServoController {
public:
    using Callback = std::function<void(int)>;

    ServoController(int pin) : pwmPin(pin), angle(90), callback(nullptr) {
        wiringPiSetup();
        pinMode(pwmPin, PWM_OUTPUT);
        pwmSetMode(PWM_MODE_MS);
        pwmSetRange(2000); // PWM 范围
        pwmSetClock(192);   // PWM 频率
        setAngle(angle);
    }

    void setCallback(Callback cb) {
        callback = cb;
    }

    void setAngle(int newAngle) {
        if (newAngle < 0) newAngle = 0;
        if (newAngle > 180) newAngle = 180;

        angle = newAngle;
        int pwmValue = map(angle, 0, 180, 50, 250); // 映射到 PWM 范围
        pwmWrite(pwmPin, pwmValue);

        if (callback) {
            callback(angle);
        }
    }

    int getAngle() const {
        return angle;
    }

private:
    int pwmPin;
    int angle;
    Callback callback;

    int map(int value, int inMin, int inMax, int outMin, int outMax) {
        return (value - inMin) * (outMax - outMin) / (inMax - inMin) + outMin;
    }
};

// 测试回调函数
void angleChangedCallback(int newAngle) {
    std::cout << "舵机角度改变为: " << newAngle << " 度" << std::endl;
}

int main() {
    ServoController servo(SERVO_PIN);
    servo.setCallback(angleChangedCallback);

    // 测试舵机角度变化
    for (int i = 0; i <= 180; i += 30) {
        servo.setAngle(i);
        delay(500); // 500ms 延迟
    }

    return 0;
}
