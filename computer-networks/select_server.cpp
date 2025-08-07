#include <iostream>
#include <cstring>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/select.h>

#define MAX_CLIENTS 5
#define PORT 12345
#define BUFFER_SIZE 1024

// 创建并绑定套接字
int create_server_socket(int port) {
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);    // TCP协议
    if (server_fd == -1) {
        perror("socket");
        exit(1);
    }

    // 设置IPv4形式的网络地址
    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(port);             // 监听本地的12345端口

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

    return server_fd;
}

int main() {
    // 用于监听的文件描述符
    int server_fd = create_server_socket(PORT);
    
    fd_set master_set, read_fds;
    FD_ZERO(&master_set);           // 初始化
    FD_SET(server_fd, &master_set); // 将服务器套接字加入到文件描述符集合中

    while (true) {
        read_fds = master_set; // 复制一份 master_set

        // 使用 select 等待文件描述符的变化
        int ret = select(FD_SETSIZE, &read_fds, nullptr, nullptr, nullptr);
        if (ret == -1) {
            perror("select");
            break;
        }

        // 处理服务器套接字上的连接请求
        if (FD_ISSET(server_fd, &read_fds)) {
            struct sockaddr_in client_addr;
            socklen_t client_len = sizeof(client_addr);
            int client_fd = accept(server_fd, (struct sockaddr*)&client_addr, &client_len);
            if (client_fd == -1) {
                perror("accept");
                continue;
            }

            FD_SET(client_fd, &master_set); // 将新的客户端套接字加入到文件描述符集合中
            std::cout << "New connection established, client fd: " << client_fd << std::endl;
        }

        // 处理客户端套接字的读操作
        for (int i = 0; i < FD_SETSIZE; ++i) {
            int client_fd = i;

            // 如果该文件描述符在集合中并且有数据可读
            if (FD_ISSET(client_fd, &read_fds)) {
                if (client_fd != server_fd) {
                    char buffer[BUFFER_SIZE];
                    ssize_t bytes_received = recv(client_fd, buffer, sizeof(buffer), 0);
                    if (bytes_received == -1) {
                        perror("recv");
                        close(client_fd);
                        FD_CLR(client_fd, &master_set);
                    } else if (bytes_received == 0) {
                        std::cout << "Client disconnected, fd: " << client_fd << std::endl;
                        close(client_fd);
                        FD_CLR(client_fd, &master_set); // 客户端断开连接，移除文件描述符
                    } else {
                        buffer[bytes_received] = '\0';
                        std::cout << "Received from client " << client_fd << ": " << buffer << std::endl;

                        // 回显客户端发送的数据
                        send(client_fd, buffer, bytes_received, 0);
                    }
                }
            }
        }
    }

    close(server_fd);
    return 0;
}
