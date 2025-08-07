#include <iostream>
#include <thread>
#include <mutex>

std::mutex mtx;  // 创建互斥锁

void print_hello1(int thread_id) {
    mtx.lock();  // 加锁，确保只有一个线程能进入临界区
    std::cout << "Hello from thread " << thread_id << std::endl;
    mtx.unlock();  // 解锁
}

void print_hello2(int thread_id) {
    std::lock_guard<std::mutex> lock(mtx);  // 自动加锁
    std::cout << "Hello from thread " << thread_id << std::endl;
}

void print_hello3(int thread_id) {
    std::unique_lock<std::mutex> lock(mtx);  // 自动加锁
    std::cout << "Hello from thread " << thread_id << std::endl;
    // lock.unlock();  // 可以显式解锁
}

int main() {
    std::thread t1(print_hello3, 1);
    std::thread t2(print_hello3, 2);
    
    t1.join();  // 等待线程1执行完毕
    t2.join();  // 等待线程2执行完毕

    return 0;
}
