#include <sys/socket.h>
#include <netinet/in.h>
#include <iostream>
#include <cstring>  // 添加此头文件用于strlen
#include <unistd.h> // 添加此头文件用于close

using namespace std;

int main(){

    // 创建 socket
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);

    // 绑定端口号
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(8080);
    bind(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr));

    // 开始监听端口
    listen(sockfd, 5);  // 最多支持5个缓存

    // 阻塞在accept
    struct sockaddr_in client_addr;
    socklen_t client_len = sizeof(client_addr);
    int new_sockfd = accept(sockfd, (struct sockaddr*)&client_addr, &client_len);

    char* mes = "Hello1";
    send(new_sockfd, mes, strlen(mes), 0);

    // while(true){
        char buffer[1024];
        if(recv(new_sockfd, buffer, sizeof(buffer), 0) > 0){
            cout  << "接受成功" << endl;
        }
    // }

    

    close(new_sockfd);
    close(sockfd);

    return 0;


}






















// #include <iostream>
// #include <sys/types.h>
// #include <sys/socket.h>
// #include <netinet/in.h>
// #include <unistd.h>
// #include <cstring>

// #define PORT 8080 // 服务器监听的端口号

// int main() {
//     int server_fd, new_socket;
//     struct sockaddr_in address;
//     int addrlen = sizeof(address);
//     char buffer[1024] = {0};
//     const char* response = "Hello from server";

//     // 创建 socket
//     if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
//         perror("socket failed");
//         exit(EXIT_FAILURE);
//     }

//     // 配置 socket 地址
//     address.sin_family = AF_INET;
//     address.sin_addr.s_addr = INADDR_ANY;   // 绑定到本机的所有可用网络接口（即监听所有 IP）
//     address.sin_port = htons(PORT);


//     // 绑定 socket 到地址
//     if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0) {
//         perror("bind failed");
//         close(server_fd);
//         exit(EXIT_FAILURE);
//     }


//     // 监听连接
//     if (listen(server_fd, 3) < 0) {
//         perror("listen");
//         close(server_fd);
//         exit(EXIT_FAILURE);
//     }

//     std::cout << "Server listening on port " << PORT << std::endl;



//     // 接受连接
//     if ((new_socket = accept(server_fd, (struct sockaddr*)&address, (socklen_t*)&addrlen)) < 0) {
//         perror("accept");
//         close(server_fd);
//         exit(EXIT_FAILURE);
//     }

//     // 读取客户端发送的数据
//     read(new_socket, buffer, 1024);
//     std::cout << "Received from client: " << buffer << std::endl;


//     // 发送响应到客户端
//     send(new_socket, response, strlen(response), 0);
//     std::cout << "Response sent to client" << std::endl;


//     // 关闭 socket
//     close(new_socket);
//     close(server_fd);

//     return 0;
// }
