#include <iostream>
#include <cstring>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <poll.h>

#define MAX_CLIENTS 5
#define PORT 12345
#define BUFFER_SIZE 1024

int main() {
    // 创建监听套接字
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == -1) {
        perror("socket");
        exit(1);
    }

    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        perror("bind");
        close(server_fd);
        exit(1);
    }

    if (listen(server_fd, MAX_CLIENTS) == -1) {
        perror("listen");
        close(server_fd);
        exit(1);
    }


    // poll的文件描述符数组相比于select的位图是可以自定义长度的
    struct pollfd fds[MAX_CLIENTS + 1]; // +1 for server socket
    fds[0].fd = server_fd;
    fds[0].events = POLLIN;  // server_fd 可读，表示有新的连接请求

    // 初始化其他客户端套接字为空
    for (int i = 1; i <= MAX_CLIENTS; ++i) {
        fds[i].fd = -1;
        fds[i].events = POLLIN;
    }

    char buffer[BUFFER_SIZE];

    while (true) {
        int ret = poll(fds, MAX_CLIENTS + 1, -1); // 阻塞等待
        if (ret == -1) {
            perror("poll");
            break;
        }

        // 处理服务器套接字的连接请求
        if (fds[0].revents & POLLIN) {
            int client_fd = accept(server_fd, nullptr, nullptr);    // 一次从全连接队中取出一个连接
            if (client_fd == -1) {
                perror("accept");
                continue;
            }

            // 将新的客户端套接字添加到 fds
            bool added = false;
            for (int i = 1; i <= MAX_CLIENTS; ++i) {
                // 找到第一个空闲的位置，放置新的客户端套接字
                if (fds[i].fd == -1) {
                    fds[i].fd = client_fd;
                    std::cout << "New connection established, client fd: " << client_fd << std::endl;
                    added = true;
                    break;
                }
            }

            if (!added) {
                std::cout << "Too many clients connected, rejecting new connection." << std::endl;
                close(client_fd);
            }
        }

        // 处理 listenfd之后的客户端请求
        for (int i = 1; i <= MAX_CLIENTS; ++i) {
            if (fds[i].fd != -1 && (fds[i].revents & POLLIN)) {
                ssize_t bytes_received = recv(fds[i].fd, buffer, sizeof(buffer), 0);
                // 如果接收数据出错或客户端关闭连接
                if (bytes_received == -1) {
                    perror("recv");
                    close(fds[i].fd);
                    fds[i].fd = -1;
                } 
                // 如果客户端关闭连接
                else if (bytes_received == 0) {
                    std::cout << "Client disconnected, fd: " << fds[i].fd << std::endl;
                    close(fds[i].fd);
                    fds[i].fd = -1;
                } 
                // 如果接收到数据
                else {
                    buffer[bytes_received] = '\0';
                    std::cout << "Received from client " << fds[i].fd << ": " << buffer << std::endl;
                    // 回显客户端发送的数据
                    send(fds[i].fd, buffer, bytes_received, 0);
                }
            }
        }
    }

    close(server_fd);
    return 0;
}
