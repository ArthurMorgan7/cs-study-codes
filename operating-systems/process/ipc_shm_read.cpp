// consumer.cpp
#include <iostream>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <cstring>
#include <sys/stat.h>

#define SHM_NAME "/my_shm"  // 共享内存名称
#define SIZE 4096          // 共享内存大小

int main() {
    // 1. 打开共享内存对象
    int shm_fd = shm_open(SHM_NAME, O_RDONLY, 0666);
    if (shm_fd == -1) {
        std::cerr << "shm_open failed: " << strerror(errno) << std::endl;
        return 1;
    }

    // 2. 映射共享内存
    void* addr = mmap(NULL, SIZE, PROT_READ, MAP_SHARED, shm_fd, 0);
    if (addr == MAP_FAILED) {
        std::cerr << "mmap failed: " << strerror(errno) << std::endl;
        close(shm_fd);
        return 1;
    }

    // 3. 读取共享内存中的数据
    std::cout << "Message from shared memory: " << reinterpret_cast<char*>(addr) << std::endl;

    // 4. 关闭共享内存对象
    close(shm_fd);

    return 0;
}
