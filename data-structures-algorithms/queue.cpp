#include <iostream>
#include <deque>  // 也可以改为 vector

template <typename T, typename Container = std::deque<T>>
class SimpleQueue {
private:
    Container c;

public:
    // 检查是否为空
    bool empty() const {
        return c.empty();
    }

    // 队列大小
    size_t size() const {
        return c.size();
    }

    // 返回队头元素
    T& front() {
        return c.front();
    }

    const T& front() const {
        return c.front();
    }

    // 返回队尾元素
    T& back() {
        return c.back();
    }

    const T& back() const {
        return c.back();
    }

    // 入队
    void push(const T& value) {
        c.push_back(value);
    }

    // 出队
    void pop() {
        c.pop_front();
    }
};


int main() {
    SimpleQueue<int> q;

    q.push(10);
    q.push(20);
    q.push(30);

    std::cout << "Front: " << q.front() << "\n";  // 10
    std::cout << "Back: " << q.back() << "\n";    // 30

    q.pop();
    std::cout << "After pop, Front: " << q.front() << "\n";  // 20

    std::cout << "Queue size: " << q.size() << "\n";  // 2

    while (!q.empty()) {
        std::cout << q.front() << " ";
        q.pop();
    }

    return 0;
}
