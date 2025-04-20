#include <iostream>
#include <string>
#include <memory>
#include <thread>
#include <atomic>
#include <csignal>
#include <gpiod.hpp>

class GPIOWrapper {
public:
    GPIOWrapper(const std::string& chip_name, unsigned int pin) 
        : chip_(gpiod::chip(chip_name)), 
          line_(chip_.get_line(pin)) {
        line_.request({
            "switch_ctrl",
            gpiod::line_request::DIRECTION_OUTPUT,
            0
        }, 0); // 初始低电平
    }

    void set(bool state) {
        line_.set_value(state ? 1 : 0);
    }

    ~GPIOWrapper() {
        line_.release();
    }

private:
    gpiod::chip chip_;
    gpiod::line line_;
};

class SequenceController {
public:
    SequenceController()
        : switch1_(std::make_unique<GPIOWrapper>("gpiochip0", 27)),
          switch2_(std::make_unique<GPIOWrapper>("gpiochip0", 22)),
          running_(false) {}

    void start_sequence() {
        if (running_) return;
        running_ = true;
        
        std::thread([this]() {
            control_switch(*switch1_, 2000);
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
            control_switch(*switch2_, 1000);
            running_ = false;
        }).detach();
    }

    void stop() {
        switch1_->set(false);
        switch2_->set(false);
    }

private:
    void control_switch(GPIOWrapper& sw, int duration_ms) {
        sw.set(true);
        std::cout << "Switch ON" << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(duration_ms));
        sw.set(false);
        std::cout << "Switch OFF" << std::endl;
    }

    std::unique_ptr<GPIOWrapper> switch1_;
    std::unique_ptr<GPIOWrapper> switch2_;
    std::atomic<bool> running_;
};

// 全局控制器实例和原子标志
std::unique_ptr<SequenceController> controller;
std::atomic<bool> exit_flag(false);

void signal_handler(int) {
    exit_flag = true;
    if (controller) {
        controller->stop();
    }
}

void user_input_handler() {
    std::string cmd;
    while (!exit_flag) {
        std::cout << "> ";
        std::getline(std::cin, cmd);

        if (cmd == "on") {
            if (controller) controller->start_sequence();
        } else if (cmd == "exit") {
            exit_flag = true;
        } else {
            std::cout << "Valid commands:\n  on - Start sequence\n  exit - Quit\n";
        }
    }
}

int main() {
    // 注册信号处理
    std::signal(SIGINT, signal_handler);
    std::signal(SIGTERM, signal_handler);

    try {
        controller = std::make_unique<SequenceController>();
        std::cout << "System ready. GPIO27(switch1), GPIO22(switch2)" << std::endl;
        
        std::thread input_thread(user_input_handler);
        input_thread.detach();

        while (!exit_flag) {
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    std::cout << "\nSystem shutdown" << std::endl;
    return EXIT_SUCCESS;
}