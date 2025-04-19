// PCA9685.h
#ifndef PCA9685_H
#define PCA9685_H

#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>
#include <cmath>
#include <stdexcept>

// PCA9685 device address
#define PCA9685_ADDR 0x40
// I2C device file on Raspberry Pi 5 (GPIO2/3 corresponds to i2c-1)
#define I2C_DEV "/dev/i2c-1"

class PCA9685 {
public:
    // Constructor
    PCA9685();
    // Destructor
    ~PCA9685();

    // Set the servo angle for a specific channel
    void setAngle(uint8_t channel, float angle);
    // Set the PWM values for a specific channel
    void setPWM(uint8_t channel, uint16_t on, uint16_t off);

private:
    // File descriptor for the I2C device
    int i2c_fd;

    // Write a value to a specific register
    void writeReg(uint8_t reg, uint8_t value);
    // Set the PWM frequency
    void setPWMFreq(float freq);
    // Reset the PCA9685 device
    void reset();
};

#endif