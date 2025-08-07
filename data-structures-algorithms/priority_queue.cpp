#include <iostream>
#include <vector>
#include <stdexcept>
#include <functional>

template <typename T, typename Container = std::vector<T>, typename Compare = std::less<T>>
class SimplePriorityQueue {
private:
    Container c;      // 底层容器
    Compare comp;     // 比较函数对象，默认是 less（最大堆）

    void heapify_up(int index) {
        while (index > 0) {
            int parent = (index - 1) / 2;
            if (comp(c[parent], c[index])) {  // 比较优先级
                std::swap(c[parent], c[index]);
                index = parent;
            } else {
                break;
            }
        }
    }

    void heapify_down(int index) {
        int size = c.size();
        while (true) {
            int left = 2 * index + 1;
            int right = 2 * index + 2;
            int largest = index;

            if (left < size && comp(c[largest], c[left])) {
                largest = left;
            }
            if (right < size && comp(c[largest], c[right])) {
                largest = right;
            }

            if (largest != index) {
                std::swap(c[index], c[largest]);
                index = largest;
            } else {
                break;
            }
        }
    }

public:
    SimplePriorityQueue() = default;

    void push(const T& value) {
        c.push_back(value);
        heapify_up(c.size() - 1);
    }

    void pop() {
        if (empty()) throw std::out_of_range("PriorityQueue is empty");
        std::swap(c[0], c.back());
        c.pop_back();
        heapify_down(0);
    }

    const T& top() const {
        if (empty()) throw std::out_of_range("PriorityQueue is empty");
        return c[0];
    }

    bool empty() const {
        return c.empty();
    }

    size_t size() const {
        return c.size();
    }
};


int main() {
    SimplePriorityQueue<int> pq;

    pq.push(5);
    pq.push(10);
    pq.push(3);
    pq.push(8);

    while (!pq.empty()) {
        std::cout << pq.top() << " ";  // 应该输出：10 8 5 3
        pq.pop();
    }

    return 0;
}
