#include <mutex>
#include <thread>
#include <chrono>
#include <condition_variable>

using namespace std;

mutex mtx;
condition_variable cv;
int count = 0;

void func(int idx, char c){
    while(1){
        unique_lock<mutex> lock(mtx);
        cv.wait(lock, [idx](){return idx == count;});
        printf("%c", c);
        count = (++count)%3;
        cv.notify_all();
    }
}


int main(){
    thread t1(func, 0, 'a');
    thread t2(func, 1, 'b');
    thread t3(func, 2, 'c');

    t1.join();
    t2.join();
    t3.join();

    printf("\ndone\n");
}
