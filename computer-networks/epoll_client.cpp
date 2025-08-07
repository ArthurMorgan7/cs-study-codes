#include <iostream>
#include <sys/epoll.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <cstring>
#include <fcntl.h>

#define MAX_EVENTS 10
#define BUFFER_SIZE 1024
#define SERVER_PORT 12345
#define SERVER_IP "127.0.0.1"

void set_nonblocking(int sock) {
    // 获取文件描述符的标志 fcntl + F_GETFL
    int opts = fcntl(sock, F_GETFL);
    if (opts < 0) {
        std::cerr << "fcntl(F_GETFL) failed\n";
        exit(1);
    }
    
    opts = (opts | O_NONBLOCK);

    // 设置文件描述符的标志 fcntl + F_SETFL
    if (fcntl(sock, F_SETFL, opts) < 0) {
        std::cerr << "fcntl(F_SETFL) failed\n";
        exit(1);
    }
}

int main() {
    int sockfd, epollfd;
    struct sockaddr_in server_addr;             // 目标地址和端口号
    struct epoll_event ev, events[MAX_EVENTS];  // 事件的集合，每个事件 = fd + 事件类型
    char buffer[BUFFER_SIZE];

    // 创建套接字
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        std::cerr << "Socket creation failed\n";
        return -1;
    }

    // 设置非阻塞模式
    set_nonblocking(sockfd);

    // 设置服务器地址
    memset(&server_addr, 0, sizeof(server_addr));   // 对结构体进行零初始化
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr);

    // 连接服务器
    if (connect(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        if (errno != EINPROGRESS) {
            std::cerr << "Connection failed\n";
            return -1;
        }
    }

    // 创建epoll实例
    epollfd = epoll_create1(0);
    if (epollfd == -1) {
        std::cerr << "Failed to create epoll file descriptor\n";
        return -1;
    }

    // ① 将套接字加入epoll监控
    ev.events = EPOLLOUT | EPOLLET;  // 初始监控写事件，非阻塞
    ev.data.fd = sockfd;
    if (epoll_ctl(epollfd, EPOLL_CTL_ADD, sockfd, &ev) == -1) {
        std::cerr << "Failed to add socket to epoll\n";
        return -1;
    }

    // ② 将标准输入（键盘）也加入epoll监控
    ev.events = EPOLLIN;  // 监控读事件
    ev.data.fd = STDIN_FILENO;
    if (epoll_ctl(epollfd, EPOLL_CTL_ADD, STDIN_FILENO, &ev) == -1) {
        std::cerr << "Failed to add stdin to epoll\n";
        return -1;
    }

    // 主循环：通过epoll等待事件发生
    while (true) {
        int nfds = epoll_wait(epollfd, events, MAX_EVENTS, -1);  // 阻塞等待事件
        if (nfds == -1) {
            std::cerr << "epoll_wait failed\n";
            break;
        }

        for (int i = 0; i < nfds; ++i) {
            if (events[i].data.fd == sockfd) {
                // 可写事件：向服务器发送数据
                if (events[i].events & EPOLLOUT) {
                    const char* message = "Hello from client!";
                    ssize_t n = send(sockfd, message, strlen(message), 0);
                    if (n == -1) {
                        std::cerr << "Failed to send data\n";
                    } else {
                        std::cout << "Sent to server: " << message << std::endl;
                    }

                    // 修改事件类型为读取服务器响应
                    ev.events = EPOLLIN | EPOLLET;
                    ev.data.fd = sockfd;
                    epoll_ctl(epollfd, EPOLL_CTL_MOD, sockfd, &ev);
                } 
                // 可读事件：从服务器接收数据
                else if (events[i].events & EPOLLIN) {
                    ssize_t n = recv(sockfd, buffer, BUFFER_SIZE - 1, 0);
                    if (n > 0) {
                        buffer[n] = '\0';  // 确保字符串结尾
                        std::cout << "Received from server: " << buffer << std::endl;
                    } else if (n == 0) {
                        std::cout << "Server closed connection\n";
                        close(sockfd);
                        return 0;
                    } else {
                        std::cerr << "recv failed\n";
                    }
                }
            } 
            // 标准输入可读事件：从键盘读取输入并发送给服务器
            else if (events[i].data.fd == STDIN_FILENO) { 
                ssize_t n = read(STDIN_FILENO, buffer, BUFFER_SIZE - 1);
                if (n > 0) {
                    buffer[n] = '\0';
                    send(sockfd, buffer, n, 0);
                }
            }
        }
    }

    close(sockfd);
    close(epollfd);
    return 0;
}
