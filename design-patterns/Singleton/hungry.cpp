#include <iostream>
#include <string>
#include <sstream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <memory>   // 不建议用智能指针管理单例对象，因为它并不需要担心内存泄漏，就一份，记得释放就行
using namespace std;


class SingletonE{
private:
    SingletonE(){
        cout << "饿汉式构造" << endl;
    }
    ~SingletonE(){
        cout << "饿汉式析构" << endl;
    }
    static SingletonE instance; // 由于饿汉式是在一开始就创建，所以可以声明为对象本身，不必要声明为指针

public:
    static SingletonE* getInstance(){
        return &instance;
    }
    void print(){
        cout << "正在调用饿汉式的功能函数" << endl;
    }
    SingletonE(SingletonE&) = delete;
    SingletonE& operator=(SingletonE&) = delete;
};

SingletonE SingletonE::instance;


int main(){

    SingletonE* s0 = SingletonE::getInstance();
    s0 -> print();
    // 测试一：能否在外部构造
    SingletonE s1;  // 不能。因为析构私有，不允许创建局部变量，只能在堆上创建
    SingletonE* s2 = new SingletonE(); // 不能。因为构造函数是私有的，不能在外部调用

    return 0;
}
