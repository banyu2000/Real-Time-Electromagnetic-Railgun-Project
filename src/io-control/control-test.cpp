#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <pthread.h>
#include <gpiod.h>

#define GPIO_CHIP "gpiochip0"
#define SWITCH1_PIN 27   // BCM编号27
#define SWITCH2_PIN 22   // BCM编号22
#define INPUT_BUFFER_SIZE 32

static struct gpiod_line *switch1 = NULL;
static struct gpiod_line *switch2 = NULL;

// 线程参数结构体
typedef struct {
    struct gpiod_line *line;
    int duration;
} ThreadArg;

/* 信号处理回调 */
void signal_handler(int signo) {
    // 关闭所有开关并释放资源
    if (switch1) {
        gpiod_line_set_value(switch1, 0);
        gpiod_line_release(switch1);
    }
    if (switch2) {
        gpiod_line_set_value(switch2, 0);
        gpiod_line_release(switch2);
    }
    printf("\nGPIO资源已释放\n");
    exit(0);
}

/* 开关控制线程 */
void* switch_control(void *arg) {
    ThreadArg *targ = (ThreadArg*)arg;
    
    // 导通开关（高电平）
    gpiod_line_set_value(targ->line, 1);
    printf("开关已导通\n");
    
    sleep(targ->duration);  // 保持导通
    
    // 关断开关（低电平）
    gpiod_line_set_value(targ->line, 0);
    printf("开关已关断\n");
    
    free(targ);
    return NULL;
}

/* 时序控制线程 */
void* sequence_control(void *arg) {
    // 控制开关1
    ThreadArg *arg1 = malloc(sizeof(ThreadArg));
    arg1->line = switch1;
    arg1->duration = 2;
    pthread_t tid1;
    pthread_create(&tid1, NULL, switch_control, arg1);
    pthread_join(tid1, NULL);  // 等待开关1完成

    sleep(0.5);  // 间隔0.5秒

    // 控制开关2
    ThreadArg *arg2 = malloc(sizeof(ThreadArg));
    arg2->line = switch2;
    arg2->duration = 1;
    pthread_t tid2;
    pthread_create(&tid2, NULL, switch_control, arg2);
    pthread_join(tid2, NULL);  // 等待开关2完成

    return NULL;
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
        pthread_t seq_tid;
        pthread_create(&seq_tid, NULL, sequence_control, NULL);
        pthread_detach(seq_tid);
    } else if (strcmp(input, "exit") == 0) {
        raise(SIGINT);
    } else {
        printf("无效命令，可用命令：\n  on - 启动时序\n  exit - 退出\n");
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

    // 初始化开关1（GPIO27）
    switch1 = gpiod_chip_get_line(chip, SWITCH1_PIN);
    if (!switch1 || gpiod_line_request_output(switch1, "switch1", 0) < 0) {
        perror("开关1初始化失败");
        goto cleanup;
    }

    // 初始化开关2（GPIO22）
    switch2 = gpiod_chip_get_line(chip, SWITCH2_PIN);
    if (!switch2 || gpiod_line_request_output(switch2, "switch2", 0) < 0) {
        perror("开关2初始化失败");
        goto cleanup;
    }

    // 注册信号处理
    signal(SIGINT, signal_handler);

    printf("时序控制系统已启动\n");
    printf("可用命令：\n  on - 启动时序\n  exit - 退出\n");

    while (1) {
        printf("> ");
        fflush(stdout);
        process_user_input();
    }

cleanup:
    if (switch1) gpiod_line_release(switch1);
    if (switch2) gpiod_line_release(switch2);
    if (chip) gpiod_chip_close(chip);
    return EXIT_FAILURE;
}