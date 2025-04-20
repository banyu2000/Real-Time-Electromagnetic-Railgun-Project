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
        // 初始化高电平（关断状态）
        line_.request({
            "switch_ctrl",
            gpiod::line_request::DIRECTION_OUTPUT,
            0
        }, 1); // 初始值设为1（高电平）
    }

    // 设置电平（true=高电平/关断，false=低电平/导通）
    void set(bool state) {
        line_.set_value(state ? 1 : 0); // 逻辑反转
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
            // 开关1导通（低电平）
            control_switch(*switch1_, 2000);
            
            // 间隔0.5秒
            std::this_thread::sleep_for(500ms);
            
            // 开关2导通（低电平）
            control_switch(*switch2_, 1000);
            
            running_ = false;
        }).detach();
    }

    void stop() {
        // 关断时设为高电平
        switch1_->set(true);
        switch2_->set(true);
    }

private:
    void control_switch(GPIOWrapper& sw, int duration_ms) {
        sw.set(false); // 低电平导通
        std::cout << "开关导通（低电平）" << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(duration_ms));
        sw.set(true);  // 高电平关断
        std::cout << "开关关断（高电平）" << std::endl;
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
            std::cout << "有效命令：\n  on - 启动时序\n  exit - 退出\n";
        }
    }
}

int main() {
    // 注册信号处理
    std::signal(SIGINT, signal_handler);
    std::signal(SIGTERM, signal_handler);

    try {
        controller = std::make_unique<SequenceController>();
        std::cout << "系统已就绪（低电平触发）\nGPIO27-开关1，GPIO22-开关2" << std::endl;
        
        std::thread input_thread(user_input_handler);
        input_thread.detach();

        while (!exit_flag) {
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }

    } catch (const std::exception& e) {
        std::cerr << "错误: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    std::cout << "\n系统关闭" << std::endl;
    return EXIT_SUCCESS;
}