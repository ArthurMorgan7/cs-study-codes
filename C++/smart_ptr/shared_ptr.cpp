#include <atomic>
#include <iostream>

template <typename T>
class Shared_ptr {
private:
    std::atomic<int>* cnt; // 引用计数
    T* ptr; // 原始指针

public:
    // 默认构造函数
    Shared_ptr() : cnt(nullptr), ptr(nullptr) {}

    // 类型转换构造函数
    explicit Shared_ptr(T* p) : ptr(p) {
        cnt = new std::atomic<int>(1); // 初始化引用计数为1
    }

    // 拷贝构造函数
    Shared_ptr(const Shared_ptr<T>& sp) : ptr(sp.ptr), cnt(sp.cnt) {
        if (cnt) {
            (*cnt)++; // 增加引用计数
        }
    }

    // 拷贝赋值运算符
    Shared_ptr& operator=(const Shared_ptr<T>& sp) {
        if (this != &sp) {
            // 先释放当前对象
            release();

            ptr = sp.ptr;
            cnt = sp.cnt;
            if (cnt) {
                (*cnt)++; // 增加引用计数
            }
        }
        return *this;
    }

    // 移动构造函数
    Shared_ptr(Shared_ptr<T>&& sp) noexcept : ptr(sp.ptr), cnt(sp.cnt) {
        sp.ptr = nullptr;
        sp.cnt = nullptr;
    }

    // 移动赋值运算符
    Shared_ptr& operator=(Shared_ptr<T>&& sp) noexcept {
        if (this != &sp) {
            // 释放当前对象
            release();

            ptr = sp.ptr;
            cnt = sp.cnt;
            sp.ptr = nullptr;
            sp.cnt = nullptr;
        }
        return *this;
    }

    // 析构函数
    ~Shared_ptr() {
        release();
    }

    // 解引用运算符
    T& operator*() const {
        return *ptr;
    }

    // 成员访问运算符
    T* operator->() const {
        return ptr;
    }

    // 获取引用计数
    int use_count() const {
        return *cnt;
    }

    // 获取裸指针
    T* get() const {
        return ptr;
    }

private:
    // 释放资源
    void release() {
        if (cnt && --(*cnt) == 0) {
            delete ptr;
            delete cnt;
        }
    }
};


int main() {
    // 测试代码
    Shared_ptr<int> sp1 = make_shared<int>(10);
    Shared_ptr<int> sp2 = sp1;

    std::cout << "sp1 use_count: " << sp1.use_count() << std::endl;
    std::cout << "sp2 use_count: " << sp2.use_count() << std::endl;

    Shared_ptr<int> sp3 = std::move(sp2);

    // std::cout << "sp2 use_count (after move): " << sp2.use_count() << std::endl;
    std::cout << "sp3 use_count: " << sp3.use_count() << std::endl;

    return 0;
}
