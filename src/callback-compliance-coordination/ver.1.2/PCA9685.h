// PCA9685.h
#ifndef PCA9685_H
#define PCA9685_H

#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>
#include <cmath>
#include <stdexcept>

#define PCA9685_ADDR 0x40
#define I2C_DEV "/dev/i2c-1"

class PCA9685 {
public:
    PCA9685();
    ~PCA9685();
    void setAngle(uint8_t channel, float angle);
    void setPWM(uint8_t channel, uint16_t on, uint16_t off);

private:
    int i2c_fd;
    void writeReg(uint8_t reg, uint8_t value);
    void setPWMFreq(float freq);
    void reset();
};

#endif
