// PCA9685.cpp
#include "PCA9685.h"

PCA9685::PCA9685() {
    if((i2c_fd = open(I2C_DEV, O_RDWR)) < 0) {
        throw std::runtime_error("Failed to open I2C device");
    }
    if(ioctl(i2c_fd, I2C_SLAVE, PCA9685_ADDR) < 0) {
        close(i2c_fd);
        throw std::runtime_error("Failed to set I2C address");
    }
    reset();
    setPWMFreq(50);
}

PCA9685::~PCA9685() {
    if(i2c_fd >= 0) close(i2c_fd);
}

void PCA9685::writeReg(uint8_t reg, uint8_t value) {
    uint8_t buffer[2] = {reg, value};
    if(write(i2c_fd, buffer, 2) != 2) {
        throw std::runtime_error("I2C write failed");
    }
}

void PCA9685::reset() {
    writeReg(0x00, 0x00);
    usleep(10000);
}

void PCA9685::setPWMFreq(float freq) {
    freq = std::min(1526.0f, std::max(24.0f, freq));
    float prescale_val = 25000000.0 / 4096.0 / freq - 1.0;
    uint8_t prescale = static_cast<uint8_t>(prescale_val + 0.5);

    uint8_t oldmode = 0;
    if(read(i2c_fd, &oldmode, 1) != 1) {
        throw std::runtime_error("I2C read failed");
    }

    writeReg(0x00, (oldmode & 0x7F) | 0x10);
    writeReg(0xFE, prescale);
    writeReg(0x00, oldmode);
    usleep(5000);
    writeReg(0x00, oldmode | 0xA1);
}

void PCA9685::setPWM(uint8_t channel, uint16_t on, uint16_t off) {
    uint8_t reg = 0x06 + 4 * channel;
    uint8_t data[5] = {
        reg,
        static_cast<uint8_t>(on & 0xFF),
        static_cast<uint8_t>(on >> 8),
        static_cast<uint8_t>(off & 0xFF),
        static_cast<uint8_t>(off >> 8)
    };
    if(write(i2c_fd, data, 5) != 5) {
        throw std::runtime_error("PWM set failed");
    }
}

void PCA9685::setAngle(uint8_t channel, float angle) {
    angle = std::min(180.0f, std::max(0.0f, angle));
    const float pulse_us = 500.0f + (angle * 2000.0f) / 180.0f;
    const uint16_t tick = static_cast<uint16_t>((pulse_us * 4096.0f) / 20000.0f);
    setPWM(channel, 0, tick);
}
