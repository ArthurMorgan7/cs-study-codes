#include <sys/epoll.h>
#include <netinet/in.h>
#include <unistd.h>
#include <fcntl.h>
#include <iostream>
#include <cstring>
#include <map>

#define MAX_EVENTS 10
#define READ_BUFFER 1024
#define PORT 12345

// 设置套接字为非阻塞模式
void setNonBlocking(int sockfd) {
    int flags = fcntl(sockfd, F_GETFL, 0);
    fcntl(sockfd, F_SETFL, flags | O_NONBLOCK);
}

int main() {
    int server_fd, client_fd, epoll_fd;
    struct sockaddr_in address;
    struct epoll_event ev, events[MAX_EVENTS];
    std::map<int, std::string> clients;

    // 创建服务器套接字
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == -1) {
        std::cerr << "Failed to create socket\n";
        return 1;
    }

    // 设置服务器地址和端口
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    memset(address.sin_zero, '\0', sizeof(address.sin_zero));

    // 绑定端口
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) == -1) {
        std::cerr << "Bind failed\n";
        close(server_fd);
        return 1;
    }

    // 开始监听
    if (listen(server_fd, SOMAXCONN) == -1) {
        std::cerr << "Listen failed\n";
        close(server_fd);
        return 1;
    }

    // 设置服务器套接字为非阻塞模式
    setNonBlocking(server_fd);

    // 创建epoll实例
    epoll_fd = epoll_create1(0);    // 底层红黑树
    if (epoll_fd == -1) {
        std::cerr << "Failed to create epoll instance\n";
        close(server_fd);
        return 1;
    }

    // 将服务器套接字添加到epoll中监听（每个文件描述符都用 epoll_event 进行封装）
    ev.events = EPOLLIN;        // 感兴趣可读事件
    ev.data.fd = server_fd;     // 将事件绑定到socket文件描述符

    // 以 epoll_event 结构体为单位存放到 epoll 的描述符中
    if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, server_fd, &ev) == -1) {
        std::cerr << "Failed to add server socket to epoll\n";
        close(server_fd);
        close(epoll_fd);
        return 1;
    }
 
    while (true) {
        // 等待事件发生
        int nfds = epoll_wait(epoll_fd, events, MAX_EVENTS, -1);
        if (nfds == -1) {
            std::cerr << "epoll_wait failed\n";
            break;
        }

        // 遍历每个发生行为的事件
        for (int i = 0; i < nfds; i++) {
            if (events[i].data.fd == server_fd) {
                // 有新的客户端连接
                socklen_t addrlen = sizeof(address);
                client_fd = accept(server_fd, (struct sockaddr *)&address, &addrlen);
                if (client_fd == -1) {
                    std::cerr << "Failed to accept connection\n";
                    continue;
                }
                std::cout << "New connection accepted\n";

                // 设置客户端套接字为非阻塞模式
                setNonBlocking(client_fd);

                // 将客户端套接字添加到epoll中监听
                ev.events = EPOLLIN | EPOLLET;
                ev.data.fd = client_fd;
                if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, client_fd, &ev) == -1) {
                    std::cerr << "Failed to add client socket to epoll\n";
                    close(client_fd);
                }

            } 
            else if (events[i].events & EPOLLIN) {
                // 数据可读
                char buffer[READ_BUFFER];
                memset(buffer, 0, sizeof(buffer));
                int n = read(events[i].data.fd, buffer, sizeof(buffer) - 1);
                if (n <= 0) {
                    // 客户端关闭连接
                    close(events[i].data.fd);
                    epoll_ctl(epoll_fd, EPOLL_CTL_DEL, events[i].data.fd, NULL);
                    std::cout << "Connection closed\n";
                } else {
                    // 处理收到的数据
                    std::cout << "Received: " << buffer;
                    clients[events[i].data.fd] = std::string(buffer);
                    ev.events = EPOLLOUT | EPOLLET;
                    ev.data.fd = events[i].data.fd;
                    epoll_ctl(epoll_fd, EPOLL_CTL_MOD, events[i].data.fd, &ev);
                }

            } else if (events[i].events & EPOLLOUT) {
                // 数据可写
                std::string response = "HTTP/1.1 200 OK\r\nContent-Length: 13\r\n\r\nHello, World!";
                write(events[i].data.fd, response.c_str(), response.size());
                ev.events = EPOLLIN | EPOLLET;
                ev.data.fd = events[i].data.fd;
                epoll_ctl(epoll_fd, EPOLL_CTL_MOD, events[i].data.fd, &ev);
            }
        }
    }

    close(server_fd);
    close(epoll_fd);
    return 0;
}
