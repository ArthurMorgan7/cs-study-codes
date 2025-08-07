#include <iostream>
#include <semaphore.h>
#include <fcntl.h>
#include <unistd.h>
#include <cstring>
#include <cerrno>

#define SEM_EMPTY "/sem_empty"   // 空槽信号量
#define SEM_FULL "/sem_full"     // 填充槽信号量
#define SEM_MUTEX "/sem_mutex"   // 互斥信号量
#define BUFFER_SIZE 10           // 缓冲区大小

int main() {
    // 1. 打开信号量
    sem_t *sem_empty = sem_open(SEM_EMPTY, O_CREAT, 0666, BUFFER_SIZE);
    sem_t *sem_full = sem_open(SEM_FULL, O_CREAT, 0666, 0);
    sem_t *sem_mutex = sem_open(SEM_MUTEX, O_CREAT, 0666, 1);

    if (sem_empty == SEM_FAILED || sem_full == SEM_FAILED || sem_mutex == SEM_FAILED) {
        std::cerr << "sem_open failed: " << strerror(errno) << std::endl;
        return 1;
    }

    // 生产者生产数据
    for (int i = 0; i < 20; ++i) {
        // 生产前等待空槽
        sem_wait(sem_empty);

        // 获取互斥锁
        sem_wait(sem_mutex);

        // 生产数据（例如，数据是简单的整数）
        std::cout << "Producer: Producing item " << i << std::endl;

        // 放入数据后释放互斥锁
        sem_post(sem_mutex);

        // 生产后增加填充槽
        sem_post(sem_full);

        sleep(1);  // 模拟生产过程的时间
    }

    // 2. 关闭信号量
    sem_close(sem_empty);
    sem_close(sem_full);
    sem_close(sem_mutex);

    // 3. 删除信号量
    sem_unlink(SEM_EMPTY);
    sem_unlink(SEM_FULL);
    sem_unlink(SEM_MUTEX);

    return 0;
}
