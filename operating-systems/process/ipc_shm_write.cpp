// producer.cpp
#include <iostream>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <cstring>
#include <sys/stat.h>

#define SHM_NAME "/my_shm"  // 共享内存名称
#define SIZE 4096          // 共享内存大小

int main() {
    // 1. 创建共享内存对象
    int shm_fd = shm_open(SHM_NAME, O_CREAT | O_RDWR, 0666);
    if (shm_fd == -1) {
        std::cerr << "shm_open failed: " << strerror(errno) << std::endl;
        return 1;
    }

    // 2. 设置共享内存大小
    if (ftruncate(shm_fd, SIZE) == -1) {
        std::cerr << "ftruncate failed: " << strerror(errno) << std::endl;
        close(shm_fd);
        return 1;
    }

    // 3. 映射共享内存
    void* addr = mmap(NULL, SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
    if (addr == MAP_FAILED) {
        std::cerr << "mmap failed: " << strerror(errno) << std::endl;
        close(shm_fd);
        return 1;
    }

    // 4. 向共享内存写入数据
    const char* message = "Hello from parent process!";
    strncpy(reinterpret_cast<char*>(addr), message, SIZE);

    std::cout << "Message written to shared memory: " << message << std::endl;

    // 5. 关闭共享内存对象
    close(shm_fd);

    // 6. 等待子进程读取（可以使用 sleep 作为简单的示范，实际中可以用信号量等同步机制）
    std::cout << "Waiting for child process to read message..." << std::endl;
    sleep(5);

    // 7. 删除共享内存
    shm_unlink(SHM_NAME);

    return 0;
}
