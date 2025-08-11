#include <mutex>
#include <thread>
#include <chrono>

using namespace std;

mutex mtx_1;
mutex mtx_2;

void func1(){
    unique_lock<mutex> lock_1(mtx_1);
    this_thread::sleep_for(chrono::seconds(3));
    unique_lock<mutex> lock_2(mtx_2);

}

void func2(){
    unique_lock<mutex> lock_1(mtx_2);   // 打破循环等待条件：保持相同的加锁顺序能避免死锁
    this_thread::sleep_for(chrono::seconds(2));
    unique_lock<mutex> lock_2(mtx_1);

}


int main(){
    thread t1(func1);
    thread t2(func2);

    t1.join();
    t2.join();

    printf("\ndone\n");
}
