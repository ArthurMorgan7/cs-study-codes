#include <iostream>
#include <vector>

/*
 * 1. vector
 * 2. deque
 * 3. list
 */


template <typename T, typename Container = std::vector<T>>
class SimpleStack {
private:
    Container c;  // 底层容器

public:
    // 检查是否为空
    bool empty() const {
        return c.empty();
    }

    // 返回栈的大小
    size_t size() const {
        return c.size();
    }

    // 返回栈顶元素
    T& top() {
        return c.back();
    }

    const T& top() const {
        return c.back();
    }

    // 入栈
    void push(const T& value) {
        c.push_back(value);
    }

    // 出栈
    void pop() {
        c.pop_back();
    }
};


int main() {
    SimpleStack<int> stk;

    stk.push(10);
    stk.push(20);
    stk.push(30);

    std::cout << "Top: " << stk.top() << "\n";  // 输出 30

    stk.pop();
    std::cout << "After pop, Top: " << stk.top() << "\n";  // 输出 20

    std::cout << "Size: " << stk.size() << "\n";  // 输出 2

    while (!stk.empty()) {
        std::cout << "Pop: " << stk.top() << "\n";
        stk.pop();
    }

    return 0;
}
