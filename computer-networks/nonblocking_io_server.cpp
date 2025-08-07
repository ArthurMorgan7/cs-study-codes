#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <cstring>
#include <errno.h>

int main() {
    // 创建套接字
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        std::cerr << "Failed to create socket." << std::endl;
        return 1;
    }

    // 设置套接字为非阻塞模式
    if (fcntl(sockfd, F_SETFL, O_NONBLOCK) < 0) {
        std::cerr << "Failed to set socket to non-blocking." << std::endl;
        close(sockfd);
        return 1;
    }

    // 设置服务器地址
    struct sockaddr_in serverAddr;
    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(8080);

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
        // 非阻塞接受连接
        int connfd = accept(sockfd, nullptr, nullptr);
        if (connfd < 0) {
            if (errno != EWOULDBLOCK && errno != EAGAIN) {
                std::cerr << "Accept error: " << strerror(errno) << std::endl;
            }
            // 没有连接时，继续循环
            continue;
        }
        sleep(1);
        std::cout << "Client connected." << std::endl;

        // 非阻塞读取数据
        char buffer[1024];
        ssize_t n;
        while (true) {
            n = read(connfd, buffer, sizeof(buffer) - 1);
            if (n < 0) {
                if (errno != EWOULDBLOCK && errno != EAGAIN) {
                    std::cerr << "Read error: " << strerror(errno) << std::endl;
                }
                // 继续尝试读取数据
                continue;
            } else if (n == 0) {
                // 客户端关闭连接
                break;
            } else {
                sleep(2);
                buffer[n] = '\0'; // 确保字符串终止
                std::cout << "Received: " << buffer << std::endl;
                
            }
        }

        // 关闭客户端连接
        close(connfd);
    }

    // 关闭服务器套接字
    close(sockfd);
    return 0;
}
