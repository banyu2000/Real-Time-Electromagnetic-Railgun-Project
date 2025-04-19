#include <gpiod.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>

#define GPIO_CHIP "gpiochip0"
#define GPIO_PIN 27    // BCM编号27
#define INPUT_BUFFER_SIZE 32

static struct gpiod_line *led_line = NULL;

// 回调函数类型定义
typedef void (*control_callback)(int);

/* 信号处理回调 */
void signal_handler(int signo) {
    if (led_line) {
        gpiod_line_set_value(led_line, 0);
        gpiod_line_release(led_line);
    }
    printf("\nGPIO资源已释放\n");
    exit(0);
}

/* LED控制回调 */
void led_control_callback(int state) {
    if (!led_line) return;
    
    gpiod_line_set_value(led_line, state);
    printf("灯泡已%s\n", state ? "点亮" : "熄灭");
}

/* 阻塞式输入处理 */
void process_user_input(control_callback cb) {
    char input[INPUT_BUFFER_SIZE] = {0};
    
    // 阻塞等待用户输入
    if (!fgets(input, INPUT_BUFFER_SIZE, stdin)) {
        perror("输入读取失败");
        return;
    }

    // 去除换行符
    input[strcspn(input, "\n")] = 0;

    if (strcmp(input, "on") == 0) {
        cb(1);
    } else if (strcmp(input, "off") == 0) {
        cb(0);
    } else if (strcmp(input, "exit") == 0) {
        cb(0);
        printf("正在退出程序...\n");
        exit(0);
    } else {
        printf("无效命令，可用命令：\n  on - 点亮\n  off - 熄灭\n  exit - 退出\n");
    }
}

int main() {
    struct gpiod_chip *chip;

    // 初始化GPIO
    chip = gpiod_chip_open_by_name(GPIO_CHIP);
    if (!chip) {
        perror("GPIO芯片打开失败");
        return EXIT_FAILURE;
    }

    led_line = gpiod_chip_get_line(chip, GPIO_PIN);
    if (!led_line) {
        perror("GPIO线路获取失败");
        gpiod_chip_close(chip);
        return EXIT_FAILURE;
    }

    if (gpiod_line_request_output(led_line, "led_control", 0) < 0) {
        perror("输出模式设置失败");
        gpiod_line_release(led_line);
        gpiod_chip_close(chip);
        return EXIT_FAILURE;
    }

    // 注册信号回调
    signal(SIGINT, signal_handler);

    printf("灯泡控制系统已启动（GPIO%d）\n", GPIO_PIN);
    printf("输入命令：\n  on - 点亮\n  off - 熄灭\n  exit - 退出\n");

    // 主循环
    while (1) {
        printf("> ");
        fflush(stdout);
        process_user_input(led_control_callback);
    }

    // 理论上不会执行到这里
    gpiod_line_release(led_line);
    gpiod_chip_close(chip);
    return EXIT_SUCCESS;
}