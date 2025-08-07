#include <iostream>
#include <cstring>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h> // 包含 inet_pton 函数
#include <sys/wait.h>  // 包含 wait() 函数
#include <cerrno>      // 包含 errno
#include <string>      // 包含 std::string

void client_task(int id) {
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        std::cerr << "Client " << id << " failed to create socket: " << strerror(errno) << std::endl;
        return;
    }

    struct sockaddr_in serverAddr;
    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(8080);

    // 使用 inet_pton 将字符串 IP 地址转换为网络字节序的二进制形式
    if (inet_pton(AF_INET, "127.0.0.1", &serverAddr.sin_addr) <= 0) {
        std::cerr << "Client " << id << ": Invalid address or Address not supported." << std::endl;
        close(sockfd);
        return;
    }

    // 连接到服务器
    if (connect(sockfd, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
        std::cerr << "Client " << id << " failed to connect: " << strerror(errno) << std::endl;
        close(sockfd);
        return;
    }
    // sleep(2);

    // 发送数据
    std::string message = "Hello from client " + std::to_string(id);
    ssize_t bytesSent = send(sockfd, message.c_str(), message.size(), 0);
    if (bytesSent < 0) {
        std::cerr << "Client " << id << " failed to send data: " << strerror(errno) << std::endl;
    } else {
        std::cout << "Client " << id << " sent: " << message << std::endl;
    }
    

    // 等待服务器响应（读取数据）
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
            std::cerr << "Failed to fork process for client " << i << ": " << strerror(errno) << std::endl;
            return 1;
        }
    }

    // 等待所有子进程完成
    for (int i = 0; i < num_clients; ++i) {
        wait(nullptr);
    }

    return 0;
}
