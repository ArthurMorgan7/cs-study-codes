#include <iostream>
#include <thread>
#include <semaphore>  // C++20 的信号量

std::binary_semaphore sem(0);  // 初始值为 0，表示线程会在信号量释放之前被阻塞

void thread_1() {
    std::cout << "Thread 1 is waiting for signal.\n";
    sem.acquire();  // 等待信号量
    std::cout << "Thread 1 received signal and continues execution.\n";
}

void thread_2() {
    std::cout << "Thread 2 is working and will release the semaphore.\n";
    std::this_thread::sleep_for(std::chrono::seconds(2));  // 模拟工作
    sem.release();  // 释放信号量
    std::cout << "Thread 2 released the semaphore.\n";
}

int main() {
    std::thread t1(thread_1);
    std::thread t2(thread_2);

    t1.join();
    t2.join();

    return 0;
}
