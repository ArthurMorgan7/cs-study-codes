#include <iostream>
#include <vector>
#include <stdexcept>  // std::out_of_range

template <typename T>
class MaxHeap {
private:
    std::vector<T> heap;

    // 返回父节点的索引
    size_t parent(size_t index) const {
        return (index - 1) / 2;
    }

    // 返回左子节点的索引
    size_t leftChild(size_t index) const {
        return 2 * index + 1;
    }

    // 返回右子节点的索引
    size_t rightChild(size_t index) const {
        return 2 * index + 2;
    }

    // 维护堆的性质，向下调整
    void heapifyDown(size_t index) {
        size_t largest = index;
        size_t left = leftChild(index);
        size_t right = rightChild(index);

        if (left < heap.size() && heap[left] > heap[largest]) {
            largest = left;
        }

        if (right < heap.size() && heap[right] > heap[largest]) {
            largest = right;
        }

        if (largest != index) {
            std::swap(heap[index], heap[largest]);
            heapifyDown(largest);
        }
    }

    // 维护堆的性质，向上调整
    void heapifyUp(size_t index) {
        while (index > 0 && heap[parent(index)] < heap[index]) {
            std::swap(heap[index], heap[parent(index)]);
            index = parent(index);
        }
    }

public:
    // 构造函数
    MaxHeap() {}

    // 插入一个新元素到堆中
    void push(const T& value) {
        heap.push_back(value);
        heapifyUp(heap.size() - 1);  // 向上调整
    }

    // 删除堆顶元素
    void pop() {
        if (heap.empty()) {
            throw std::out_of_range("Heap is empty");
        }

        heap[0] = heap.back();  // 将堆底元素移动到堆顶
        heap.pop_back();
        heapifyDown(0);  // 向下调整
    }

    // 获取堆顶元素
    const T& top() const {
        if (heap.empty()) {
            throw std::out_of_range("Heap is empty");
        }
        return heap[0];
    }

    // 返回堆的大小
    size_t size() const {
        return heap.size();
    }

    // 判断堆是否为空
    bool empty() const {
        return heap.empty();
    }

    // 调试输出堆的内容
    void debug_print() const {
        for (const auto& val : heap) {
            std::cout << val << " ";
        }
        std::cout << "\n";
    }
};
int main() {
    MaxHeap<int> maxHeap;

    maxHeap.push(10);
    maxHeap.push(20);
    maxHeap.push(5);
    maxHeap.push(30);
    maxHeap.push(15);

    std::cout << "Heap after push operations: ";
    maxHeap.debug_print();  // 输出：30 20 5 10 15

    std::cout << "Top of heap: " << maxHeap.top() << "\n";  // 输出：30

    maxHeap.pop();
    std::cout << "Heap after pop operation: ";
    maxHeap.debug_print();  // 输出：20 15 5 10

    std::cout << "Top of heap after pop: " << maxHeap.top() << "\n";  // 输出：20

    return 0;
}
