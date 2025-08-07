#include <iostream>
#include <string>
#include <sstream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <memory>   // 不建议用智能指针管理单例对象，因为它并不需要担心内存泄漏，就一份，记得释放就行
using namespace std;



/* ----------------------------------- 懒汉式 ---------------------------------- */
mutex mtx;
class SingletonL{
private:    // 设置为私有，禁止在栈上创建，禁止外部构造单例对象
    // 在类的定义内部定义，默认为内联的
    SingletonL(){
        cout << "懒汉式构造" << endl;
    }
    ~SingletonL(){
        cout << "懒汉式析构" << endl;   
    }
    static SingletonL *instance; // 为了避免在程序一开始就创建单例对象，必须定义为指针
public:
    // 只留下统一的接口，接口也是静态的，因为没法创建单例对象。
    static SingletonL* getInstance(){   
        // 静态成员函数内部只能访问静态变量
        // mtx.lock(); // 在这里加锁的话粒度太大，影响性能
        if(instance == nullptr){
            lock_guard<mutex> lock(mtx);  // 多个线程如果全阻塞在这，也会导致创建多个单例对象，所以必须加一次判断
            if(instance == nullptr){
                instance = new SingletonL();  // 记得释放内存
            }
            
        }
        return instance;
    }

    // 功能函数得定义成公共的，不然不能从外部访问
    void print(){
        cout << "正在调用懒汉式的功能函数" << endl;
    }

    void destory(){
        delete instance;
        instance = nullptr;  // 避免悬空指针
    }

    SingletonL(SingletonL&) = delete;
    SingletonL& operator=(SingletonL&) = delete; // 一定返回的是引用，不然返回值是个右值，不能链式的调用拷贝赋值运算符。
};

SingletonL* SingletonL::instance = nullptr;


/* ----------------------------------- 饿汉式 ---------------------------------- */

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
    /* ---------------------------------- 测试懒汉式 --------------------------------- */
    // 测试一：能否在外部构造
    // SingletonL s1;  // 不能。因为析构私有，不允许创建局部变量，只能在堆上创建
    // SingletonL* s2 = new SingletonL(); // 不能。因为构造函数是私有的，不能在外部调用

    // 测试二：验证是否为同一个实例
    // SingletonL* s1 = SingletonL::getInstance();  // 第一次获取实例
    // SingletonL* s2 = SingletonL::getInstance();  // 第二次获取实例
    // cout << (s1 == s2) << endl;  // 输出 1，说明是同一个实例

    // 测试三：多线程
    // thread t1(SingletonL::getInstance);  // 如果getInstance内部构造不加锁，就会创建多个单例对象
    // thread t2(SingletonL::getInstance);
    // thread t3(SingletonL::getInstance);
    // t1.join();
    // t2.join();
    // t3.join();

    // SingletonL* s9 = SingletonL::getInstance();
    // s9->destory();


    /* ---------------------------------- 测试饿汉式 --------------------------------- */
    // SingletonE* s0 = SingletonE::getInstance();
    // s0 -> print();
    // 测试一：能否在外部构造
    // SingletonE s1;  // 不能。因为析构私有，不允许创建局部变量，只能在堆上创建
    // SingletonE* s2 = new SingletonE(); // 不能。因为构造函数是私有的，不能在外部调用

    return 0;
}
