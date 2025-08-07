#include <iostream>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <cstring>

#define PIPE_NAME "/tmp/my_named_pipe"

int main() {
    pid_t pid;

    // 创建命名管道
    if (mkfifo(PIPE_NAME, 0666) == -1) {
        perror("mkfifo");
        return 1;
    }

    pid = fork();
    if (pid == -1) {
        perror("fork");
        return 1;
    }

    if (pid > 0) {  // 父进程
        const char* message = "Hello from parent process!";
        // 打开命名管道写入数据
        int fd = open(PIPE_NAME, O_WRONLY);
        if (fd == -1) {
            perror("open");
            return 1;
        }
        write(fd, message, strlen(message) + 1);
        close(fd);
    } else {  // 子进程
        char buffer[100];
        // 打开命名管道读取数据
        int fd = open(PIPE_NAME, O_RDONLY);
        if (fd == -1) {
            perror("open");
            return 1;
        }
        read(fd, buffer, sizeof(buffer));
        std::cout << "Child process received: " << buffer << std::endl;
        close(fd);
    }

    // 删除命名管道
    unlink(PIPE_NAME);

    return 0;
}
