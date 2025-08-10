#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>

// 自定义信号处理函数
void sigint_handler(int signum) {
    printf("\n[处理函数] 收到信号 %d (%s)\n", signum, strsignal(signum));
}

int main() {
    // 注册自定义信号处理函数
    struct sigaction sa;
    sa.sa_handler = sigint_handler;
    sigemptyset(&sa.sa_mask);  // 处理信号时不额外屏蔽其他信号
    sa.sa_flags = 0;

    if (sigaction(SIGINT, &sa, NULL) == -1) {
        perror("sigaction");
        return 1;
    }

    // 设置一个信号集，用于屏蔽 SIGINT
    sigset_t block_set, old_set;
    sigemptyset(&block_set);
    sigaddset(&block_set, SIGINT);

    printf("=== 阶段 1: 屏蔽 SIGINT，按 Ctrl+C 试试 ===\n");
    if (sigprocmask(SIG_BLOCK, &block_set, &old_set) == -1) {
        perror("sigprocmask");
        return 1;
    }

    sleep(5); // 这 5 秒内 SIGINT 会被阻塞

    printf("=== 阶段 2: 解除屏蔽 SIGINT ===\n");
    if (sigprocmask(SIG_SETMASK, &old_set, NULL) == -1) {
        perror("sigprocmask");
        return 1;
    }

    // 此时如果之前在阶段 1 按了 Ctrl+C，会立即触发处理函数
    sleep(5);

    printf("=== 程序结束 ===\n");
    return 0;
}
