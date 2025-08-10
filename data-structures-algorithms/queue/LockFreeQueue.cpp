#include <atomic>
#include <iostream>
#include <memory>

template<typename T>
class LockFreeQueue {
private:
    struct Node {
        std::shared_ptr<T> data;
        std::atomic<Node*> next;

        Node() : next(nullptr) {}
        Node(const T& value) : data(std::make_shared<T>(value)), next(nullptr) {}
    };

    std::atomic<Node*> head;
    std::atomic<Node*> tail;

public:
    LockFreeQueue() {
        Node* dummy = new Node();
        head.store(dummy);
        tail.store(dummy);
    }

    ~LockFreeQueue() {
        while (Node* old_head = head.load()) {
            head.store(old_head->next);
            delete old_head;
        }
    }

    void enqueue(const T& value) {
        Node* new_node = new Node(value);
        Node* old_tail;

        while (true) {
            old_tail = tail.load();
            Node* next = old_tail->next.load();
            if (old_tail == tail.load()) {
                if (next == nullptr) {
                    if (old_tail->next.compare_exchange_weak(next, new_node)) {
                        break;
                    }
                } else {
                    tail.compare_exchange_weak(old_tail, next);
                }
            }
        }
        tail.compare_exchange_weak(old_tail, new_node);
    }

    std::shared_ptr<T> dequeue() {
        while (true) {
            Node* old_head = head.load();
            Node* old_tail = tail.load();
            Node* next = old_head->next.load();

            if (old_head == head.load()) {
                if (old_head == old_tail) {
                    if (next == nullptr) {
                        return std::shared_ptr<T>(); // 队列为空，返回空指针
                    }
                    tail.compare_exchange_weak(old_tail, next);
                } else {
                    std::shared_ptr<T> res = next->data;
                    if (head.compare_exchange_weak(old_head, next)) {
                        delete old_head;
                        return res;
                    }
                }
            }
        }
    }
};
