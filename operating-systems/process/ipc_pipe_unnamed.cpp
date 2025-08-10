#include <iostream>
#include <unistd.h>
#include <cstring>
#include <sys/types.h>
#include <sys/wait.h>

using namespace std;

int main() {
    const char* message = "Hello from parent process!";
    char buffer[100];

    // 创建匿名管道
    int pipefd[2];  // pipefd[0] 用于读，pipefd[1] 用于写
    if (pipe(pipefd) == -1) {
        perror("pipe");
        return 1;
    }

    // 创建子进程
    pid_t pid;
    pid = fork();
    if (pid == -1) {
        perror("fork");
        return 1;
    }

    if (pid > 0) {  // 父进程
        close(pipefd[0]);  // 关闭读端
        write(pipefd[1], message, strlen(message) + 1);  // 向管道写数据
        close(pipefd[1]);  // 关闭写端
        wait(NULL);         // 等待子进程结束
    } else {  // 子进程
        close(pipefd[1]);  // 关闭写端
        read(pipefd[0], buffer, sizeof(buffer));  // 从管道读取数据
        cout << "Child process received: " << buffer <<endl;
        close(pipefd[0]);  // 关闭读端
    }

    return 0;
}
