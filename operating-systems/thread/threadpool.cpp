#include <iostream>
#include <vector>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <functional>

// 线程池类
class ThreadPool {
public:
    ThreadPool(size_t numThreads) {
        start(numThreads);
    }

    ~ThreadPool() {
        stop();
    }

    // 提交任务到线程池
    template<typename F>
    void enqueue(F&& f) {
        {
            std::unique_lock<std::mutex> lock(mEventMutex);
            mTasks.emplace(std::forward<F>(f));
        }
        mEventVar.notify_one();
    }

private:
    std::vector<std::thread> mThreads;
    std::queue<std::function<void()>> mTasks;

    std::mutex mEventMutex;
    std::condition_variable mEventVar;
    bool mStopping = false;

    // 启动线程池
    void start(size_t numThreads) {
        for (size_t i = 0; i < numThreads; ++i) {
            mThreads.emplace_back([=] {
                while (true) {
                    std::function<void()> task;
                    {
                        std::unique_lock<std::mutex> lock(mEventMutex);
                        mEventVar.wait(lock, [=] { return mStopping || !mTasks.empty(); });
                        if (mStopping && mTasks.empty()) {
                            break;
                        }
                        task = std::move(mTasks.front());
                        mTasks.pop();
                    }
                    task();
                }
            });
        }
    }

    // 停止线程池
    void stop() noexcept {
        {
            std::unique_lock<std::mutex> lock(mEventMutex);
            mStopping = true;
        }
        mEventVar.notify_all();
        for (auto &thread : mThreads) {
            thread.join();
        }
    }
};

// 示例任务函数
void printNumber(int num) {
    std::cout << "Number: " << num << std::endl;
}


int main() {
    ThreadPool pool(4);  // 创建一个有4个线程的线程池

    // 提交多个任务到线程池
    for (int i = 0; i < 10; ++i) {
        pool.enqueue([i] {
            printNumber(i);
        });
    }

    // 由于线程池在析构时会等待所有任务完成，所以主线程可以在此处退出
    std::this_thread::sleep_for(std::chrono::seconds(1)); // 等待一段时间以确保所有任务执行完成

    return 0;
}
