#include <mutex>
#include <iostream>
#include <thread>
#include <chrono>

using namespace std;

std::mutex mtx_1;
std::mutex mtx_2;

// int i = 0;
void dead1(){
    mtx_1.lock();
    cout << "This is dead1" << endl;
    mtx_2.lock();
}

// int j = 0;
void dead2(){
    mtx_2.lock();
    cout << "This is dead2" << endl;
    mtx_1.lock();
}



int main(int argc, char** argv){
    std::thread t1(dead1);
    std::thread t2(dead2);

    t1.join();
    t2.join();

    return 0;
}
