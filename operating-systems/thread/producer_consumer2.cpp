#include <iostream>
#include <queue>
#include <thread>
#include <condition_variable>
#include <mutex>

using namespace std;

queue<int> que;       // 共享队列
mutex mtx;            // 互斥锁
condition_variable cv; // 条件变量
bool done = false;    // 生产者是否完成生产

void producer() {
    this_thread::sleep_for(chrono::milliseconds(100)); // 等待，让消费者先阻塞
    for (int i = 0; i < 5; ++i) {
        {
            lock_guard<mutex> lock(mtx);
            que.push(i);
            cout << "Producer: Produced " << i << endl;
        }
        cv.notify_one();  // 只通知一个等待的消费者
        this_thread::sleep_for(chrono::milliseconds(100)); // 模拟生产间隔
    }

    // 生产者完成生产
    {
        lock_guard<mutex> lock(mtx);
        done = true;
    }
    cv.notify_all();  // 通知所有消费者退出
}

void consumer(int id) {
    while (true) {
        unique_lock<mutex> lock(mtx);

        /* ------------------------------------ - ----------------------------------- */
        // **使用 while 代替 if 避免虚假唤醒**
        while (que.empty()) {
            if (done) return;  // 生产者已完成且队列为空，退出
            cout << "Consumer " << id << " is waiting...\n";
            cv.wait(lock);  // 等待队列非空
        }

        // **使用 if，导致虚假唤醒**
        // if (que.empty()) {
        //     if (done) return;  // 生产者已完成且队列为空，退出
        //     cout << "Consumer " << id << " is waiting...\n";
        //     cv.wait(lock);  // 等待队列非空
        // }

        /* ------------------------------------ - ----------------------------------- */
        // 取出队列中的数据
        int item = que.front();
        que.pop();
        cout << "Consumer " << id << " consumed: " << item << endl;

        lock.unlock(); // 释放锁
        this_thread::sleep_for(chrono::milliseconds(50)); // 模拟处理时间
    }
}

int main() {
    thread t1(consumer, 1);
    thread t2(consumer, 2);
    thread t3(producer);

    t1.join();
    t2.join();
    t3.join();

    return 0;
}
