#include <iostream>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/select.h>
#include <cstring>

int main() {
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in serverAddr;
    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(8080);

    bind(sockfd, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
    listen(sockfd, 5);

    fd_set readfds;
    FD_ZERO(&readfds);
    FD_SET(sockfd, &readfds);
    int maxfd = sockfd;

    while (true) {
        fd_set tempfds = readfds;
        int nready = select(maxfd + 1, &tempfds, nullptr, nullptr, nullptr);
        if (nready < 0) {
            std::cerr << "Select error: " << strerror(errno) << std::endl;
            break;
        }

        for (int fd = 0; fd <= maxfd; ++fd) {
            if (FD_ISSET(fd, &tempfds)) {
                if (fd == sockfd) {
                    int connfd = accept(sockfd, nullptr, nullptr);
                    FD_SET(connfd, &readfds);
                    if (connfd > maxfd) maxfd = connfd;
                } else {
                    char buffer[1024];
                    ssize_t n = read(fd, buffer, sizeof(buffer));
                    if (n > 0) {
                        std::cout << "Received: " << buffer << std::endl;
                        write(fd, buffer, n); // 回显数据
                    } else {
                        close(fd);
                        FD_CLR(fd, &readfds);
                    }
                }
            }
        }
    }

    close(sockfd);
    return 0;
}
