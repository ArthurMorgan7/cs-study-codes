#include <iostream>
#include <thread>
#include <shared_mutex>
#include <mutex>

std::shared_mutex rw_mutex;
int shared_data = 0;

void read_data(int thread_id) {
    std::shared_lock<std::shared_mutex> lock(rw_mutex);  // 读锁
    std::cout << "Thread " << thread_id << " reads shared data: " << shared_data << std::endl;
}

void write_data(int thread_id, int value) {
    std::unique_lock<std::shared_mutex> lock(rw_mutex);  // 写锁
    shared_data = value;
    std::cout << "Thread " << thread_id << " writes shared data: " << shared_data << std::endl;
}

int main() {
    std::thread t1(read_data, 1);
    std::thread t2(read_data, 2);
    std::thread t3(write_data, 3, 100);

    t1.join();
    t2.join();
    t3.join();

    return 0;
}
