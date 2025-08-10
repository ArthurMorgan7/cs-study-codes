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
/* ----------------------------------- 服务器 ---------------------------------- */

// 处理请求
void handle_client(int client_socket) {
    char buffer[BUFFER_SIZE] = {0};
    read(client_socket, buffer, BUFFER_SIZE);
    
    std::string request(buffer);
    std::cout << "Received request:\n" << request << std::endl;
    
    std::string response;
    if (request.find("GET /text") != std::string::npos) {
        response = "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\n\r\nHello, this is a text response!";
    } else if (request.find("GET /audio") != std::string::npos) {
        std::ifstream file("audio.mp3", std::ios::binary);
        if (file) {
            // 将音频文件存储在字符串中（原始二进制数据）
            std::ostringstream oss;
            oss << file.rdbuf();
            std::string body = oss.str();
            // 在响应体中进行传输
            response = "HTTP/1.1 200 OK\r\nContent-Type: audio/mpeg\r\nContent-Length: " + std::to_string(body.size()) + "\r\n\r\n" + body;
        } else {
            response = "HTTP/1.1 404 Not Found\r\n\r\n";
        }
    } else {
        response = "HTTP/1.1 404 Not Found\r\n\r\n";
    }
    
    send(client_socket, response.c_str(), response.size(), 0);
    close(client_socket);
}

// 服务器初始化
void start_server() {
    int server_fd, client_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == 0) {
        perror("Socket failed");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;   // 当前不知道目标地址，可以是任何
    address.sin_port = htons(PORT);         // 绑定到 服务进程 的特定端口

    if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0) {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }
    
    // 阻塞
    if (listen(server_fd, 3) < 0) {
        perror("Listen failed");
        exit(EXIT_FAILURE);
    }
    
    std::cout << "Server listening on port " << PORT << "..." << std::endl;
    
    while (true) {
        client_socket = accept(server_fd, (struct sockaddr*)&address, (socklen_t*)&addrlen);
        if (client_socket < 0) {
            perror("Accept failed");
            continue;
        }
        handle_client(client_socket);
    }
    
    close(server_fd);
}

int main(int argc, char* argv[]) {

    start_server();
    
    return 0;
}
