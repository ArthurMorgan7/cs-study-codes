#include <atomic>
#include <iostream>

using namespace std;

template <typename T>
class SharedPtr{
private:
    atomic<int>* cnt;
    T* ptr;

public:
    // 1. 默认构造函数
    SharedPtr():cnt(nullptr),ptr(nullptr){ }
    
    // 2. 转换构造函数
    SharedPtr(T* p){
        this->ptr = p;
        this->cnt = new atomic<int>(1);
    }

    // 3. 拷贝构造函数
    SharedPtr(const SharedPtr& sp){
        this->cnt = sp.cnt;
        cnt->fetch_add(1);
        this->ptr = sp.ptr;
    }

    // 4. 移动构造函数
    SharedPtr(SharedPtr&& sp) noexcept{
        this->cnt = sp.cnt;
        this->ptr = sp.ptr;
        sp.cnt = nullptr;
        sp.ptr = nullptr;
    }

    // 析构函数
    ~SharedPtr(){
        if(cnt){
            cnt->fetch_sub(1);
            if(cnt->load() == 0){
                delete cnt;
                delete ptr;
            }
        }
        
    }

    SharedPtr& operator=(const SharedPtr& sp){
        this->cnt = sp.cnt;
        cnt->fetch_add(1);
        this->ptr = sp.ptr;
        return *this;
    }

    SharedPtr& operator=(SharedPtr&& sp){
        this->cnt = sp.cnt;
        this->ptr = sp.ptr;
        sp.cnt = nullptr;
        sp.ptr = nullptr;
        return *this;
    }

    T operator*(){
        return *(ptr);
    }

    // 返回内部的裸指针
    T* get(){
        return ptr;
    }

    // 返回引用计数
    int use_count(){
        return cnt ? cnt->load() : 0;
    }



};
