// 使用 socket 实现进程间通信

#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h> // 添加此头文件
#include <unistd.h>
#include <cstring>
#include <iostream>
#include <thread>
#include <chrono>

using namespace std;


#define PORT 8080 // 服务器监听的端口号

int main() {

    // 1. 配置服务器信息
    struct sockaddr_in serv_addr;
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    // 将地址从主机字节序转换为网络字节序
    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
        perror("Invalid address/ Address not supported");
        return -1;
    }


    // 2. 创建 socket
    int sock = 0;
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Socket creation error");
        return -1;
    }


    // 3. 连接到服务器
    if (connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("Connection Failed");
        return -1;
    }

    // // 5. 读取服务器响应
    // char buffer[1024] = {0};
    // recv(sock, buffer, 1024, 0);
    // std::cout << "Received from server: " << buffer << std::endl;


    // 4. 发送消息到服务器
    // for(int i = 0; i < 1000000; i++){


        const char* message = "Hello from client";
        send(sock, message, strlen(message), 0);
        std::cout << "Message sent to server" << std::endl;



        // this_thread::sleep_for(chrono::seconds(3));
    // }
    

    // 6. 关闭 socket
    close(sock);


    return 0;
}
