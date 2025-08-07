// 单线程Reactor模式

#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <errno.h>
#include <cstring>

void setNonBlocking(int sock) {
    int flags = fcntl(sock, F_GETFL, 0);
    fcntl(sock, F_SETFL, flags | O_NONBLOCK);
}

int main() {

    int listenfd = socket(AF_INET, SOCK_STREAM, 0);
    
    // 设置 socket 为非阻塞模式
    setNonBlocking(listenfd);

    // 
    struct sockaddr_in serverAddr;
    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(8080);

    // 绑定套接字和本地地址和端口
    bind(listenfd, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
    
    // 监听状态
    listen(listenfd, SOMAXCONN);

    fd_set masterSet, readSet;
    FD_ZERO(&masterSet);
    FD_SET(listenfd, &masterSet);
    int maxfd = listenfd;

    while (true) {
        readSet = masterSet;
        int nready = select(maxfd + 1, &readSet, nullptr, nullptr, nullptr);
        
        if (nready < 0) {
            // 信号中断错误，继续循环
            if (errno == EINTR) {
                continue;
            }
            // 其他错误，输出错误信息，退出循环
            std::cerr << "select error: " << strerror(errno) << std::endl;
            break;
        }

        for (int fd = 0; fd <= maxfd; ++fd) 
        {
            if (FD_ISSET(fd, &readSet)) 
            {
                // 有新的客户端连接到达
                if (fd == listenfd) {
                    // 建立连接
                    int connfd = accept(listenfd, nullptr, nullptr);
                    // 设置为非阻塞模式
                    setNonBlocking(connfd);
                    
                    FD_SET(connfd, &masterSet);
                    if (connfd > maxfd) maxfd = connfd;
                    std::cout << "New connection accepted: " << connfd << std::endl;
                } 
                // 有新的客户端数据到达
                else {
                    // 处理客户端数据
                    char buffer[1024];
                    ssize_t n = read(fd, buffer, sizeof(buffer));
                    if (n > 0) {
                        std::cout << "Received data: " << std::string(buffer, n) << std::endl;
                        write(fd, buffer, n); // 回送数据
                    } 
                    else if (n == 0 || (n < 0 && errno != EWOULDBLOCK)) {
                        // 连接关闭或出错
                        std::cout << "Connection closed: " << fd << std::endl;
                        close(fd);
                        FD_CLR(fd, &masterSet);
                    }
                }
            }
        }
    }

    // 关闭套接字
    close(listenfd);
    return 0;
}
