#include <vector>
#include <iostream>
#include <functional>
#include <mutex>
#include <queue>
#include <utility>
#include <condition_variable>
#include <thread>

using namespace std;


class ThreadPool{
private:
    vector<thread> thread_pool;
    mutex mtx;
    queue<function<void()>> tasks;
    condition_variable cv;
    bool stop = false;
public:
    ThreadPool() = delete;
    ThreadPool(int nums){
        for(int i = 0; i < nums; i++){
            thread_pool.emplace_back(
                [this]{
                    while(true){
                        function<void()> task;
                        {
                            unique_lock<mutex> lock(mtx);
                            
                            cv.wait(lock, [this]{return stop || !tasks.empty();});
                            if(stop == true) return;
                            task = tasks.front();
                            tasks.pop();
                        }
                        task();
                    }
                    
                }
            );
        }
    }

    ~ThreadPool() {
        stop = true;
        cv.notify_all();
        for (auto& t : thread_pool) {
            t.join();
        }
    }

    template <typename F, typename Arg>
    void enqueue(F&& f, Arg&& arg){
        auto task = bind(forward<F>(f), forward<Arg>(arg));
        {
            unique_lock<mutex> lock(mtx);
            tasks.push(task);
        }
        cv.notify_all();
    }

};