#include <iostream>
#include <string>
#include <thread>
#include <cstring>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdexcept>

class TcpClient {
public:
    TcpClient(const std::string& host, int port) : serverHost(host), serverPort(port) {
        sockfd = socket(AF_INET, SOCK_STREAM, 0);
        if (sockfd < 0) throw std::runtime_error("socket creation failed");

        sockaddr_in serv_addr{};
        serv_addr.sin_family = AF_INET;
        serv_addr.sin_port = htons(serverPort);

        if (inet_pton(AF_INET, serverHost.c_str(), &serv_addr.sin_addr) <= 0) {
            throw std::runtime_error("invalid address");
        }

        if (connect(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
            throw std::runtime_error("connection failed");
        }
        std::cout << "Connected to " << serverHost << ":" << serverPort << "\n";
    }

    ~TcpClient() {
        if (sockfd >= 0) close(sockfd);
    }

    void sendMessage(const std::string& msg) {
        ssize_t n = send(sockfd, msg.data(), msg.size(), 0);
        if (n < 0) throw std::runtime_error("send failed");
    }

    std::string receiveMessage() {
        char buffer[4096];
        ssize_t n = recv(sockfd, buffer, sizeof(buffer) - 1, 0);
        if (n < 0) throw std::runtime_error("recv failed");
        if (n == 0) {
            std::cout << "Server closed connection\n";
            return {};
        }
        buffer[n] = '\0';
        return std::string(buffer);
    }

private:
    int sockfd;
    std::string serverHost;
    int serverPort;
};

int main() {
    try {
        TcpClient client("127.0.0.1", 9090);
        std::string line;
        while (std::getline(std::cin, line)) {
            client.sendMessage(line);
            std::string reply = client.receiveMessage();
            if (reply.empty()) break;
            std::cout << "Server replied: " << reply << "\n";
        }
    } catch (const std::exception& ex) {
        std::cerr << "Error: " << ex.what() << "\n";
        return 1;
    }
    return 0;
}
