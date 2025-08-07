#include <iostream>
#include <unistd.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <cstring>
#include <cerrno>
#include <sys/wait.h>  // 包含 wait() 函数

void client_task(int id) {
    // 创建套接字
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        std::cerr << "Client " << id << " failed to create socket." << std::endl;
        return;
    }

    // 设置套接字为非阻塞模式
    if (fcntl(sockfd, F_SETFL, O_NONBLOCK) < 0) {
        std::cerr << "Client " << id << " failed to set socket to non-blocking." << std::endl;
        close(sockfd);
        return;
    }

    // 设置服务器地址
    struct sockaddr_in serverAddr;
    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1"); // 服务器 IP
    serverAddr.sin_port = htons(8080); // 服务器端口号

    // 连接到服务器
    while (connect(sockfd, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
        if (errno == EINPROGRESS) {
            // 连接正在进行中，继续循环直到连接成功
            continue;
        }
        std::cerr << "Client " << id << " failed to connect." << std::endl;
        close(sockfd);
        return;
    }

    // 发送数据
    std::string message = "Hello from client " + std::to_string(id);
    if (send(sockfd, message.c_str(), message.size(), 0) < 0) {
        std::cerr << "Client " << id << " failed to send data." << std::endl;
    } else {
        std::cout << "Client " << id << " sent: " << message << std::endl;
    }

    char buffer[1024];
    ssize_t bytesRead = read(sockfd, buffer, sizeof(buffer) - 1);
    if (bytesRead < 0) {
        std::cerr << "Client " << id << " failed to read data." << std::endl;
    } else {
        buffer[bytesRead] = '\0'; // 确保字符串终止
        std::cout << "Client " << id << " received: " << buffer << std::endl;
    }

    // 关闭套接字
    close(sockfd);
}

int main() {
    const int num_clients = 5;

    // 创建多个客户端子进程
    for (int i = 0; i < num_clients; ++i) {
        pid_t pid = fork();
        if (pid == 0) {
            // 子进程
            client_task(i);
            return 0;
        } else if (pid < 0) {
            std::cerr << "Failed to fork process for client " << i << "." << std::endl;
            return 1;
        }
    }

    // 等待所有子进程完成
    for (int i = 0; i < num_clients; ++i) {
        wait(nullptr);
    }

    return 0;
}
