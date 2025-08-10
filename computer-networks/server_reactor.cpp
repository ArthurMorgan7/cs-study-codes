#include <arpa/inet.h>
#include <errno.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include <atomic>
#include <condition_variable>
#include <functional>
#include <iostream>
#include <memory>
#include <mutex>
#include <queue>
#include <string>
#include <thread>
#include <vector>

// 一个现代 C++ 风格的 epoll Reactor 示例
// 特性：
// - main thread: accept + epoll_wait
// - client sockets: non-blocking, EPOLLET | EPOLLONESHOT
// - 使用线程池处理 I/O 事件（读/写），处理完后重新 arm
// - 简单的 echo 服务作为示例
// 注意：示例为学习/演示用途，生产环境需增强错误处理、限流、超时、负载控制等。

constexpr int DEFAULT_PORT = 9090;
constexpr int MAX_EVENTS = 1024;
constexpr int BACKLOG = 128;
constexpr size_t BUFFER_SIZE = 4 * 1024;
constexpr int DEFAULT_THREAD_POOL = 4;

static std::atomic<bool> g_running{true};

inline int setNonBlocking(int fd) {
    int flags = fcntl(fd, F_GETFL, 0);
    if (flags == -1) return -1;
    return fcntl(fd, F_SETFL, flags | O_NONBLOCK);
}

class ThreadPool {
public:
    ThreadPool(size_t n = DEFAULT_THREAD_POOL) { start(n); }
    ~ThreadPool() { stop(); }

    void enqueue(std::function<void()> job) {
        {
            std::unique_lock<std::mutex> lock(mutex_);
            jobs_.push(std::move(job));
        }
        cond_.notify_one();
    }

    void start(size_t n) {
        for (size_t i = 0; i < n; ++i) {
            threads_.emplace_back([this]() {
                while (g_running || !jobs_.empty()) {
                    std::function<void()> job;
                    {
                        std::unique_lock<std::mutex> lock(mutex_);
                        cond_.wait(lock, [this]() { return !g_running || !jobs_.empty(); });
                        if (!jobs_.empty()) {
                            job = std::move(jobs_.front());
                            jobs_.pop();
                        } else {
                            continue;
                        }
                    }
                    try {
                        job();
                    } catch (const std::exception& e) {
                        std::cerr << "ThreadPool job exception: " << e.what() << std::endl;
                    } catch (...) {
                        std::cerr << "ThreadPool job unknown exception" << std::endl;
                    }
                }
            });
        }
    }

    void stop() {
        g_running = false;
        cond_.notify_all();
        for (auto& t : threads_) if (t.joinable()) t.join();
        threads_.clear();
    }

private:
    std::vector<std::thread> threads_;
    std::queue<std::function<void()>> jobs_;
    std::mutex mutex_;
    std::condition_variable cond_;
};

class EpollReactor {
public:
    EpollReactor(int port, int threads = DEFAULT_THREAD_POOL)
        : port_(port), epoll_fd_(-1), listen_fd_(-1), pool_(threads) {}

    ~EpollReactor() { stop(); }

    bool start() {
        // create listen socket
        listen_fd_ = ::socket(AF_INET, SOCK_STREAM, 0);
        if (listen_fd_ == -1) {
            perror("socket");
            return false;
        }

        int on = 1;
        setsockopt(listen_fd_, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));

        sockaddr_in addr{};
        addr.sin_family = AF_INET;
        addr.sin_addr.s_addr = INADDR_ANY;
        addr.sin_port = htons(port_);

        if (bind(listen_fd_, (sockaddr*)&addr, sizeof(addr)) == -1) {
            perror("bind");
            ::close(listen_fd_);
            return false;
        }

        if (listen(listen_fd_, BACKLOG) == -1) {
            perror("listen");
            ::close(listen_fd_);
            return false;
        }

        if (setNonBlocking(listen_fd_) == -1) {
            perror("setNonBlocking listen");
            ::close(listen_fd_);
            return false;
        }

        epoll_fd_ = epoll_create1(0);
        if (epoll_fd_ == -1) {
            perror("epoll_create1");
            ::close(listen_fd_);
            return false;
        }

        epoll_event ev{};
        ev.events = EPOLLIN; // listen socket use level-triggered accept
        ev.data.fd = listen_fd_;
        if (epoll_ctl(epoll_fd_, EPOLL_CTL_ADD, listen_fd_, &ev) == -1) {
            perror("epoll_ctl add listen");
            ::close(listen_fd_);
            ::close(epoll_fd_);
            return false;
        }

        worker_thread_ = std::thread([this]() { this->loop(); });
        std::cout << "EpollReactor listening on port " << port_ << "\n";
        return true;
    }

    void stop() {
        if (epoll_fd_ != -1) {
            ::close(epoll_fd_);
            epoll_fd_ = -1;
        }
        if (listen_fd_ != -1) {
            ::close(listen_fd_);
            listen_fd_ = -1;
        }
        g_running = false;
        if (worker_thread_.joinable()) worker_thread_.join();
        pool_.stop();
    }

private:
    void loop() {
        std::vector<epoll_event> events(MAX_EVENTS);
        while (g_running) {
            int n = epoll_wait(epoll_fd_, events.data(), (int)events.size(), 1000);
            if (n < 0) {
                if (errno == EINTR) continue;
                perror("epoll_wait");
                break;
            }

            for (int i = 0; i < n; ++i) {
                int fd = events[i].data.fd;
                uint32_t evts = events[i].events;
                if (fd == listen_fd_) {
                    // accept loop
                    while (true) {
                        sockaddr_in cli{};
                        socklen_t cli_len = sizeof(cli);
                        int conn = accept(listen_fd_, (sockaddr*)&cli, &cli_len);
                        if (conn == -1) {
                            if (errno == EAGAIN || errno == EWOULDBLOCK) break;
                            if (errno == EINTR) continue;
                            perror("accept");
                            break;
                        }
                        if (setNonBlocking(conn) == -1) {
                            perror("setNonBlocking conn");
                            ::close(conn);
                            continue;
                        }

                        epoll_event cev{};
                        cev.events = EPOLLIN | EPOLLET | EPOLLONESHOT;
                        cev.data.fd = conn;
                        if (epoll_ctl(epoll_fd_, EPOLL_CTL_ADD, conn, &cev) == -1) {
                            perror("epoll_ctl add conn");
                            ::close(conn);
                            continue;
                        }

                        char buf[64];
                        inet_ntop(AF_INET, &cli.sin_addr, buf, sizeof(buf));
                        std::cout << "accepted " << buf << ":" << ntohs(cli.sin_port) << " fd=" << conn << "\n";
                    }
                } else {
                    // dispatch to thread pool
                    // capture fd and evts by value
                    pool_.enqueue([this, fd, evts]() { this->handleEvent(fd, evts); });
                }
            }
        }
    }

    void handleEvent(int fd, uint32_t events) {
        // Handle errors / hangups quickly
        if (events & (EPOLLHUP | EPOLLERR)) {
            closeConnection(fd);
            return;
        }

        // Read available data until EAGAIN
        if (events & EPOLLIN) {
            std::string outbuf;
            outbuf.reserve(BUFFER_SIZE);
            char buffer[BUFFER_SIZE];
            while (true) {
                ssize_t n = ::read(fd, buffer, sizeof(buffer));
                if (n > 0) {
                    // echo: append to outbuf
                    outbuf.append(buffer, buffer + n);
                    continue; // keep reading
                } else if (n == 0) {
                    // peer closed
                    closeConnection(fd);
                    return;
                } else {
                    if (errno == EAGAIN || errno == EWOULDBLOCK) {
                        break; // read finished
                    }
                    if (errno == EINTR) continue;
                    // other error
                    closeConnection(fd);
                    return;
                }
            }

            // write back (simple echo). Try to write whole buffer now.
            size_t written = 0;
            while (written < outbuf.size()) {
                ssize_t w = ::write(fd, outbuf.data() + written, outbuf.size() - written);
                if (w > 0) {
                    written += (size_t)w;
                    continue;
                } else {
                    if (errno == EAGAIN || errno == EWOULDBLOCK) {
                        // kernel buffer full. For simplicity, we stop and rearm EPOLLIN only.
                        break;
                    }
                    if (errno == EINTR) continue;
                    closeConnection(fd);
                    return;
                }
            }
        }

        // Re-arm the fd for the next one-shot event
        epoll_event ev{};
        ev.events = EPOLLIN | EPOLLET | EPOLLONESHOT;
        ev.data.fd = fd;
        if (epoll_ctl(epoll_fd_, EPOLL_CTL_MOD, fd, &ev) == -1) {
            // if mod fails, close fd
            // mod may fail if fd is already closed on the other side
            perror("epoll_ctl mod");
            closeConnection(fd);
        }
    }

    void closeConnection(int fd) {
        // remove from epoll and close
        epoll_ctl(epoll_fd_, EPOLL_CTL_DEL, fd, nullptr);
        ::close(fd);
        // note: in production you'd also remove any per-connection state
    }

private:
    int port_;
    int epoll_fd_;
    int listen_fd_;
    ThreadPool pool_;
    std::thread worker_thread_;
};

int main(int argc, char* argv[]) {
    int port = DEFAULT_PORT;
    if (argc >= 2) port = std::stoi(argv[1]);

    signal(SIGINT, [](int) { g_running = false; });
    signal(SIGTERM, [](int) { g_running = false; });

    EpollReactor server(port, /*threads=*/std::thread::hardware_concurrency() ? std::thread::hardware_concurrency() : DEFAULT_THREAD_POOL);
    if (!server.start()) {
        std::cerr << "Failed to start server" << std::endl;
        return 1;
    }

    // main thread waits until running set false by signal
    while (g_running) {
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
    }

    std::cout << "shutting down..." << std::endl;
    server.stop();
    return 0;
}
