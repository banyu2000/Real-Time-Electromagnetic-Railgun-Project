#include "GpioControl.h"
#include <gpiod.h>
#include <iostream>

#define GPIO_CHIP_NAME "gpiochip0"

GpioControl::GpioControl(int pin) {
    chip = gpiod_chip_open_by_name(GPIO_CHIP_NAME);
    if (!chip) {
        std::cerr << "Failed to open GPIO chip\n";
        return;
    }

    line = gpiod_chip_get_line(chip, pin);
    if (!line) {
        std::cerr << "Failed to get GPIO line\n";
        gpiod_chip_close(chip);
        chip = nullptr;
        return;
    }

    if (gpiod_line_request_output(line, "qt_gpio", 0) < 0) {
        std::cerr << "Failed to request output mode\n";
        gpiod_line_release(line);
        gpiod_chip_close(chip);
        chip = nullptr;
        line = nullptr;
    }
}

void GpioControl::set(bool state) {
    if (line)
        gpiod_line_set_value(line, state ? 1 : 0);
}

GpioControl::~GpioControl() {
    if (line)
        gpiod_line_release(line);
    if (chip)
        gpiod_chip_close(chip);
}
