// 使用 socket 实现 HTTP 服务器和客户端
#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <fstream>
#include <sstream>
#include <string.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUFFER_SIZE 4096

/* ----------------------------------- 客户端 ---------------------------------- */
// 客户端初始化
void start_client(const std::string& request_path) {
    int sock = 0;
    struct sockaddr_in serv_addr;
    char buffer[BUFFER_SIZE] = {0};

    // 创建 IPv4 + TCP 的socket
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        std::cout << "Socket creation error" << std::endl;
        return;
    }

    // 设置服务器的地址信息
    serv_addr.sin_family = AF_INET;           // 使用的协议
    serv_addr.sin_port = htons(PORT);  // 服务器端应用进程在服务器主机的端口号
    if (inet_pton(AF_INET, "172.17.0.1", &serv_addr.sin_addr) <= 0) {
        std::cout << "Invalid address / Address not supported" << std::endl;
        return;
    }

    // 通过本地 socket 向 设置好的服务器目标 发起连接
    if (connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
        std::cout << "Connection failed" << std::endl;
        return;
    }

    // 发送 HTTP 请求报文
    std::string request = "GET " + request_path + " HTTP/1.1\r\nHost: 127.0.0.1\r\nConnection: close\r\n\r\n";
    send(sock, request.c_str(), request.length(), 0);
    
    // 解析 HTTP 响应报文
    int valread = read(sock, buffer, BUFFER_SIZE);
    std::cout << "Server response:\n" << std::string(buffer, valread) << std::endl;
    
    close(sock);
}

int main(int argc, char* argv[]) {
    std::string path = "/text";
    start_client(path);

    return 0;
}
