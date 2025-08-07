// 只靠一把互斥锁实现线程同步
// 问题：需要一直判断是否有数据

#include <mutex>
#include <iostream>
#include <chrono>
#include <thread>
#include <queue>

using namespace std;

mutex mtx;

// 生产者消费者

class Task{
public:
    void dotask(){
        cout << "正在执行任务" << endl;
        this_thread::sleep_for(chrono::seconds(3));
    }
};


void producer(int id, queue<Task*> *ptr){
    for(int i = 0; i < 10; i++){
        this_thread::sleep_for(chrono::seconds(1));
        mtx.lock();
        cout << "线程" << id << "正在生成任务" << i << endl;
        ptr->push(new Task());
        mtx.unlock();
    }
}


void consumer(int id, queue<Task*> *ptr){
    while(true){
        mtx.lock();
        if(ptr->size() != 0){
            // mtx.lock();
            Task *task = ptr->front();
            ptr->pop();
            cout << "线程" << id;
            task->dotask();
            delete task;
            // mtx.unlock();
        }
        // else cout << "空" << endl;
        mtx.unlock();
    }

}

    
int main(){
    queue<Task*> que; // 用一个队列来存放任务
    thread t1(producer, 1, &que);
    thread t2(consumer, 1, &que);
    t1.join();
    t2.join();
    exit(0);
}


