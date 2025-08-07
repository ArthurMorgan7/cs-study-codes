#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>

std::mutex mtx;
std::condition_variable cv;
bool ready = false;

void print_hello(int thread_id) {
    std::cout << "Thread " << thread_id << " waiting for signal." << std::endl;
    std::unique_lock<std::mutex> lock(mtx);
    cv.wait(lock, [](){ return ready; });  // 等待条件变量通知
    std::cout << "Hello from thread " << thread_id << std::endl;
}

void set_ready() {
    std::this_thread::sleep_for(std::chrono::seconds(2));  // 模拟一些工作
    std::cout << "Main thread setting ready!" << std::endl;
    std::lock_guard<std::mutex> lock(mtx);
    ready = true;
    cv.notify_all();  // 通知所有等待的线程
}

int main() {
    std::thread t1(print_hello, 1);
    std::thread t2(print_hello, 2);

    std::thread t3(set_ready);  // 设置 ready 并通知

    t1.join();
    t2.join();
    t3.join();

    return 0;
}
