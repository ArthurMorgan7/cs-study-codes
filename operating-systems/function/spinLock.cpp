#include <atomic>
#include <thread>
#include <iostream>
#include <chrono>

using namespace std;



class SpinLock{
private:
    atomic<bool> flag;
    bool expect;
public:
    SpinLock():flag(true), expect(true){}

    void lock(){
        while(!flag.compare_exchange_weak(expect, false)){
            expect = true;
        }
    }

    void unlock(){
        flag.store(true);
    }
};

SpinLock spinLock;

void func(int i){
    spinLock.lock();
    cout << "线程" << i << "获得了资源！" << endl;
    this_thread::sleep_for(chrono::seconds(30));
    spinLock.unlock();
}


int main(){
    thread t1(func, 1);
    thread t2(func, 2);


    t1.join();
    t2.join();
}