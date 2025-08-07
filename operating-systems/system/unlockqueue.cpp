#include <iostream>
#include <atomic>
#include <thread>
#include <vector>

template <typename T>
class LockFreeQueue {
private:
    struct Node {
        T data;
        std::atomic<Node*> next;

        Node(const T& value) : data(value), next(nullptr) {}
    };

    std::atomic<Node*> head; // 队列头
    std::atomic<Node*> tail; // 队列尾

public:
    LockFreeQueue() {
        Node* dummy = new Node(T()); // 哨兵节点
        head.store(dummy);
        tail.store(dummy);
    }

    ~LockFreeQueue() {
        while (Node* oldHead = head.load()) {
            head.store(oldHead->next);
            delete oldHead;
        }
    }

    // 入队操作
    void enqueue(const T& value) {
        Node* newNode = new Node(value);
        Node* oldTail = nullptr;
        Node* nullNode = nullptr;

        while (true) {
            oldTail = tail.load();
            Node* next = oldTail->next.load();

            // 检查 tail 是否仍然指向队尾
            if (oldTail == tail.load()) {
                if (next == nullptr) {
                    // 尝试将新节点插入队尾
                    if (oldTail->next.compare_exchange_weak(nullNode, newNode)) {
                        // 成功插入，更新 tail
                        tail.compare_exchange_weak(oldTail, newNode);
                        return;
                    }
                } else {
                    // 帮助其他线程更新 tail
                    tail.compare_exchange_weak(oldTail, next);
                }
            }
        }
    }

    // 出队操作
    bool dequeue(T& value) {
        Node* oldHead = nullptr;
        Node* oldTail = nullptr;
        Node* next = nullptr;

        while (true) {
            oldHead = head.load();
            oldTail = tail.load();
            next = oldHead->next.load();

            // 检查 head 是否仍然指向队头
            if (oldHead == head.load()) {
                if (oldHead == oldTail) {
                    // 队列为空
                    if (next == nullptr) {
                        return false;
                    }
                    // 帮助其他线程更新 tail
                    tail.compare_exchange_weak(oldTail, next);
                } else {
                    // 读取数据
                    value = next->data;

                    // 尝试更新 head
                    if (head.compare_exchange_weak(oldHead, next)) {
                        delete oldHead; // 释放旧头节点
                        return true;
                    }
                }
            }
        }
    }
};

// 测试代码
void producer(LockFreeQueue<int>& queue, int id, int numItems) {
    for (int i = 0; i < numItems; ++i) {
        queue.enqueue(i);
        std::cout << "Producer " << id << " enqueued: " << i << std::endl;
    }
}

void consumer(LockFreeQueue<int>& queue, int id, int numItems) {
    int value;
    for (int i = 0; i < numItems; ++i) {
        while (!queue.dequeue(value)) {
            // 忙等待
        }
        std::cout << "Consumer " << id << " dequeued: " << value << std::endl;
    }
}

int main() {
    LockFreeQueue<int> queue;

    const int numProducers = 1;
    const int numConsumers = 1;
    const int numItems = 10;

    std::vector<std::thread> producers;
    std::vector<std::thread> consumers;

    // 创建生产者线程
    for (int i = 0; i < numProducers; ++i) {
        producers.emplace_back(producer, std::ref(queue), i, numItems);
    }

    // 创建消费者线程
    for (int i = 0; i < numConsumers; ++i) {
        consumers.emplace_back(consumer, std::ref(queue), i, numItems);
    }

    // 等待生产者线程完成
    for (auto& t : producers) {
        t.join();
    }

    // 等待消费者线程完成
    for (auto& t : consumers) {
        t.join();
    }

    return 0;
}