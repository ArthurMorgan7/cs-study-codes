#include <thread>
#include <mutex>
#include <condition_variable>
#include <iostream>
#include <chrono>

using namespace std;

mutex mtx;
condition_variable cv;

void wait(int thread_id){
    cout << "线程" << thread_id << "在等待" << endl;
    unique_lock<mutex> lock(mtx);
    cv.wait(lock);
    cout << "线程" << thread_id << "被唤醒"<< endl;
}

void hello(){
    this_thread::sleep_for(chrono::seconds(3));
    cv.notify_all();
}


int main(){
    thread t1(wait, 1);
    thread t2(wait, 2);
    thread t3(hello);

    t1.join();
    t2.join();
    t3.join();
}