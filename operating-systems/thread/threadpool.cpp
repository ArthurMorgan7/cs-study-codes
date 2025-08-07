#include <queue>
#include <vector>
#include <iostream>
#include <thread>
#include <functional>
#include <mutex>
#include <condition_variable>
#include <utility>

using namespace std;


class ThreadPool{
private:
    vector<thread> vec;
    mutex mtx;
    queue<function<void()>> que;
    const int SIZE;
    condition_variable cv;
    bool stop = false;
public:
    ThreadPool(int num):SIZE(10){
        for(int i = 0; i < num; i++){
            vec.emplace_back([this]{
                while(true){
                    function<void()> task;
                    {
                        unique_lock<mutex> lock(mtx);
                        cv.wait(lock, [this]{return !que.empty() || stop;});
                        if(stop && que.empty()) break;
                        task = move(que.front());   // 注意！move后原对象还在，需要pop()
                        que.pop();
                    }
                    task(); // 执行函数，在函数执行的时候，可以释放锁了，所以只把获取任务的过程锁住
                }
            });
        }
    }

    template <typename F, typename Arg>
    void enqueue(F&& f, Arg&& arg){
        {
            unique_lock<mutex> lock(mtx);
            function<void()> task = bind(forward<F>(f), forward<Arg>(arg));
            que.push(task);
        }
        cv.notify_one();
    }

    ~ThreadPool(){
        {
            unique_lock<mutex> lock(mtx);
            stop = true;
        }
        
        cv.notify_all();                                                
        for(auto& t : vec){
            if(t.joinable()){
                t.join();
            }
        }
    }

};

void p(int num){
    cout << num << endl;
}

int main(){
    ThreadPool thread_pool(4);
    for(int i = 0; i < 100 ; i++){
        thread_pool.enqueue(p, i);
    }

    return 0;
}
































// #include <vector>   // 向量
// #include <queue>    // 队列
// #include <memory>   // 内存
// #include <thread>   // 线程库
// #include <mutex>    // 互斥量
// #include <condition_variable>   // 条件变量
// #include <functional>
// #include <atomic>   // 原子类型
// #include <iostream> // 输入输出
// #include <unistd.h>

// /* 手撕线程池：
//  *  1. 创建线程池，并初始化
//  *  2. 向任务列表增加任务，并唤醒线程去处理
//  *  3. 销毁线程池资源
//  */

// class ThreadPool
// {
// public:
//     ThreadPool(size_t);
//     ~ThreadPool();

//     template <class F, class... Args>
//     void enqueue(F &&f, Args &&...args);
    
// private:
//     std::vector<std::thread> workers;   // 线程池
//     std::queue<std::function<void()>> tasks;    // 任务队列
//     std::mutex queue_mutex;
//     std::condition_variable condition;  // 
//     std::atomic<bool> stop;     // bool类型的原子变量 stop
// };


// // 线程池实现
// inline ThreadPool::ThreadPool(size_t threads) 
//     : stop(false)
// {
//     for (size_t i = 0; i < threads; ++i)
//         workers.emplace_back(   // thread对象的移动构造函数
//             [this]
//             {
//                 // 无限循环，从任务队列中获取任务
//                 for (;;)
//                 {
//                     std::function<void()> task;

//                     {
//                         // 需要对任务队列操作，先获取锁
//                         std::unique_lock<std::mutex> lock(this->queue_mutex);

//                         // 等待条件成立，等待期间释放互斥锁，允许其他线程访问任务队列
//                         // [ThreadPool停止] 或 [tasks不为空] --> 唤醒线程，重新获取锁
//                         this->condition.wait(lock,
//                                              [this]{ return this->stop || !this->tasks.empty(); });
                        
//                         // [ThreadPool停止] 且 [tasks为空] --> 工作线程返回
//                         // 保证即使线程池停止，也要将任务队列消费完
//                         if (this->stop && this->tasks.empty())
//                             return;
                        
//                         // 将任务队列头部的任务取出执行
//                         task = std::move(this->tasks.front());
//                         this->tasks.pop();
//                     }

//                     task();
//                 }
//             });
// }

// // 向线程池增加任务
// template <class F, class... Args>
// void ThreadPool::enqueue(F &&f, Args &&...args) // 生产者线程
// {
//     // 将可调用对象f与参数args绑定为一个函数对象task
//     auto task = std::bind(std::forward<F>(f), std::forward<Args>(args)...);

//     {
//         // 获取互斥锁以确保对任务队列的安全访问
//         std::unique_lock<std::mutex> lock(queue_mutex);

//         if (stop){
//             throw std::runtime_error("enqueue on stopped ThreadPool");
//         }
            
//         // 将任务添加到任务队列中
//         tasks.emplace([task](){ (task)(); });
//     }// 用局部变量消失来解锁

//     // 通知一个线程有新任务可用
//     condition.notify_one(); // 多线程抢占
// }

// // the destructor joins all threads
// inline ThreadPool::~ThreadPool()
// {
//     // 设置stop标志为true，以通知所有线程停止工作
//     stop = true;
//     // 唤醒所有线程
//     condition.notify_all();
//     // 等待所有线程结束，并回收资源
//     for (std::thread &worker : workers)
//         worker.join();
// }

// void taskFunction(int id)
// {
//     sleep(id);
//     std::cout << "Task " << id << " is being processed by thread " << std::this_thread::get_id() << std::endl;
// }

// int main()
// {
//     // 创建一个具有4个线程的线程池
//     ThreadPool pool(4);

//     // 向线程池中添加10个任务
//     for (int i = 0; i < 10; ++i)
//     {
//         pool.enqueue(taskFunction, i);
//     }

//     return 0;
// }
