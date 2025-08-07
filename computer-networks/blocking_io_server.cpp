#include <iostream>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <cstring>


int main() {
    // 创建套接字
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        std::cerr << "Failed to create socket." << std::endl;
        return 1;
    }

    // 设置服务器地址
    struct sockaddr_in serverAddr;
    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;        // IPv4 协议族
    serverAddr.sin_addr.s_addr = INADDR_ANY;    // 不限制客户端的IP
    serverAddr.sin_port = htons(8080);      // 端口号 8080

    // 绑定套接字
    if (bind(sockfd, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
        std::cerr << "Failed to bind socket." << std::endl;
        close(sockfd);
        return 1;
    }

    // 监听
    if (listen(sockfd, 5) < 0) {
        std::cerr << "Failed to listen on socket." << std::endl;
        close(sockfd);
        return 1;
    }

    std::cout << "Server is listening on port 8080..." << std::endl;

    while (true) {
        // 接收连接（阻塞）
        int connfd = accept(sockfd, nullptr, nullptr);
        if (connfd < 0) {
            std::cerr << "Failed to accept connection." << std::endl;
            continue;
        }

        sleep(1);   // 模拟连接延迟
        std::cout << "Client connected." << std::endl;

        // 读取数据（阻塞：直到数据读取完成）
        char buffer[1024];
        ssize_t bytesRead = read(connfd, buffer, sizeof(buffer) - 1); // 阻塞等待数据
        if (bytesRead < 0) {
            std::cerr << "Failed to read data." << std::endl;
        } else {
            sleep(2);   // 模拟处理时间
            buffer[bytesRead] = '\0'; // 确保字符串终止
            std::cout << "Received: " << buffer << std::endl;
            
        }
        std::string response = "Message received: " + std::string(buffer);
        send(connfd, std::string(buffer).c_str(), response.size(), 0);
        // 关闭客户端连接
        close(connfd);
    }

    // 关闭服务器套接字
    close(sockfd);
    return 0;
}
