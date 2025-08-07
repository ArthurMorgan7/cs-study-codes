#include <sys/socket.h>
#include <iostream>

using namespace std;

int main(){
    int server_fd;
    server_fd = socket(AF_INET6, SOCK_DGRAM, 0);
    if(server_fd == -1){
        cout << "Build socket failed!" << endl;
        exit(1);
    }
    
    struct sockaddr_in addr; // 在C中，使用结构体必须加struct
    int addr_len = sizeof(addr);
    bind(server_fd, addr, addr_len);

    cout << server_fd << endl;

    return 0;
}