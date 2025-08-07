#include <iostream>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define BUFFER_SIZE 5  // 缓冲区大小

// 缓冲区和信号量
int buffer[BUFFER_SIZE];
int in = 0;  // 生产者写入的位置
int out = 0; // 消费者读取的位置

sem_t empty;  // 用于同步空缓冲区
sem_t full;   // 用于同步满缓冲区
pthread_mutex_t mutex; // 用于保护共享资源的互斥锁

// 生产者线程函数
void* producer(void* arg) {
    for (int i = 0; i < 10; i++) {
        // 等待空缓冲区
        sem_wait(&empty);

        // 获取互斥锁，进入临界区
        pthread_mutex_lock(&mutex);

        // 生产数据
        buffer[in] = i;
        std::cout << "Produced: " << i << std::endl;
        in = (in + 1) % BUFFER_SIZE;

        // 释放互斥锁
        pthread_mutex_unlock(&mutex);

        // 增加满缓冲区的信号量
        sem_post(&full);

        // 模拟生产时间
        sleep(1);
    }
    return NULL;
}

// 消费者线程函数
void* consumer(void* arg) {
    for (int i = 0; i < 10; i++) {
        // 等待满缓冲区
        sem_wait(&full);

        // 获取互斥锁，进入临界区
        pthread_mutex_lock(&mutex);

        // 消费数据
        int data = buffer[out];
        std::cout << "Consumed: " << data << std::endl;
        out = (out + 1) % BUFFER_SIZE;

        // 释放互斥锁
        pthread_mutex_unlock(&mutex);

        // 增加空缓冲区的信号量
        sem_post(&empty);

        // 模拟消费时间
        sleep(2);
    }
    return NULL;
}

int main() {
    // 初始化信号量
    sem_init(&empty, 0, BUFFER_SIZE);  // 初始为空缓冲区的信号量为缓冲区大小
    sem_init(&full, 0, 0);              // 初始为0，表示没有满的缓冲区
    pthread_mutex_init(&mutex, NULL);   // 初始化互斥锁

    // 创建生产者和消费者线程
    pthread_t prod_thread, cons_thread;
    pthread_create(&prod_thread, NULL, producer, NULL);
    pthread_create(&cons_thread, NULL, consumer, NULL);

    // 等待线程结束
    pthread_join(prod_thread, NULL);
    pthread_join(cons_thread, NULL);

    // 销毁信号量和互斥锁
    sem_destroy(&empty);
    sem_destroy(&full);
    pthread_mutex_destroy(&mutex);

    return 0;
}
