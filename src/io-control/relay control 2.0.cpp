#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <pthread.h>
#include <gpiod.h>

#define GPIO_CHIP "gpiochip0"
#define GPIO_PIN1 27    // 开关1引脚 (BCM 27)
#define GPIO_PIN2 28    // 开关2引脚 (BCM 28)
#define INPUT_BUFFER_SIZE 32

static struct gpiod_line *switch1_line = NULL;
static struct gpiod_line *switch2_line = NULL;

// 线程参数结构体
typedef struct {
    struct gpiod_line *line;
    int duration;
} ThreadArg;

/* 信号处理回调 */
void signal_handler(int signo) {
    // 关闭所有开关并释放资源
    if (switch1_line) {
        gpiod_line_set_value(switch1_line, 1); // 高电平关断
        gpiod_line_release(switch1_line);
    }
    if (switch2_line) {
        gpiod_line_set_value(switch2_line, 1); // 高电平关断
        gpiod_line_release(switch2_line);
    }
    
    printf("\nGPIO资源已释放\n");
    exit(0);
}

/* 开关控制线程函数 */
void* switch_control_thread(void *arg) {
    ThreadArg *targ = (ThreadArg*)arg;
    
    // 导通开关（低电平）
    gpiod_line_set_value(targ->line, 0);
    printf("开关已导通\n");
    
    // 等待指定时间
    sleep(targ->duration);
    
    // 关断开关（高电平）
    gpiod_line_set_value(targ->line, 1);
    printf("开关已关断\n");
    
    free(targ);
    return NULL;
}

/* 控制回调函数 */
void control_callback(int state) {
    if (!state) return; // 只处理on命令
    
    pthread_t tid1, tid2;
    
    // 控制开关1
    ThreadArg *arg1 = malloc(sizeof(ThreadArg));
    arg1->line = switch1_line;
    arg1->duration = 2;
    pthread_create(&tid1, NULL, switch_control_thread, arg1);
    pthread_detach(tid1);

    // 等待开关1完成
    sleep(2);
    
    // 控制开关2
    ThreadArg *arg2 = malloc(sizeof(ThreadArg));
    arg2->line = switch2_line;
    arg2->duration = 1;
    pthread_create(&tid2, NULL, switch_control_thread, arg2);
    pthread_detach(tid2);
}

/* 输入处理 */
void process_user_input() {
    char input[INPUT_BUFFER_SIZE] = {0};
    
    if (!fgets(input, INPUT_BUFFER_SIZE, stdin)) {
        perror("输入读取失败");
        return;
    }

    input[strcspn(input, "\n")] = 0;

    if (strcmp(input, "on") == 0) {
        control_callback(1);
    } else if (strcmp(input, "off") == 0) {
        control_callback(0);
    } else if (strcmp(input, "exit") == 0) {
        raise(SIGINT); // 触发信号处理
    } else {
        printf("无效命令，可用命令：\n  on - 启动时序\n  off - 立即关断\n  exit - 退出\n");
    }
}

int main() {
    struct gpiod_chip *chip;

    // 初始化GPIO芯片
    chip = gpiod_chip_open_by_name(GPIO_CHIP);
    if (!chip) {
        perror("GPIO芯片打开失败");
        return EXIT_FAILURE;
    }

    // 初始化开关1
    switch1_line = gpiod_chip_get_line(chip, GPIO_PIN1);
    if (!switch1_line || gpiod_line_request_output(switch1_line, "switch1", 1) < 0) {
        perror("开关1初始化失败");
        goto cleanup;
    }

    // 初始化开关2
    switch2_line = gpiod_chip_get_line(chip, GPIO_PIN2);
    if (!switch2_line || gpiod_line_request_output(switch2_line, "switch2", 1) < 0) {
        perror("开关2初始化失败");
        goto cleanup;
    }

    // 注册信号处理
    signal(SIGINT, signal_handler);

    printf("时序控制系统已启动\n");
    printf("可用命令：\n  on - 启动时序\n  off - 紧急关断\n  exit - 退出\n");

    while (1) {
        printf("> ");
        fflush(stdout);
        process_user_input();
    }

cleanup:
    if (switch1_line) gpiod_line_release(switch1_line);
    if (switch2_line) gpiod_line_release(switch2_line);
    if (chip) gpiod_chip_close(chip);
    return EXIT_FAILURE;
}