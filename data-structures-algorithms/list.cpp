#include <iostream>

template <typename T>
class List {
private:
    // 节点结构
    struct Node {
        T value;
        Node* prev;
        Node* next;
        Node(const T& val) : value(val), prev(nullptr), next(nullptr) {}
    };

    Node* head;  // 指向链表的头部
    Node* tail;  // 指向链表的尾部
    size_t _size;  // 链表中的元素个数

public:
    // 构造函数
    List() : head(nullptr), tail(nullptr), _size(0) {}

    // 析构函数，释放所有节点
    ~List() {
        clear();
    }

    // 清空链表
    void clear() {
        Node* current = head;
        while (current) {
            Node* temp = current;
            current = current->next;
            delete temp;
        }
        head = tail = nullptr;
        _size = 0;
    }

    // 返回链表的大小
    size_t size() const {
        return _size;
    }

    // 是否为空
    bool empty() const {
        return _size == 0;
    }

    // 在链表末尾插入元素
    void push_back(const T& value) {
        Node* newNode = new Node(value);
        if (empty()) {
            head = tail = newNode;
        } else {
            tail->next = newNode;
            newNode->prev = tail;
            tail = newNode;
        }
        ++_size;
    }

    // 在链表头部插入元素
    void push_front(const T& value) {
        Node* newNode = new Node(value);
        if (empty()) {
            head = tail = newNode;
        } else {
            newNode->next = head;
            head->prev = newNode;
            head = newNode;
        }
        ++_size;
    }

    // 移除链表末尾的元素
    void pop_back() {
        if (empty()) return;
        Node* temp = tail;
        if (head == tail) {
            head = tail = nullptr;
        } else {
            tail = tail->prev;
            tail->next = nullptr;
        }
        delete temp;
        --_size;
    }

    // 移除链表头部的元素
    void pop_front() {
        if (empty()) return;
        Node* temp = head;
        if (head == tail) {
            head = tail = nullptr;
        } else {
            head = head->next;
            head->prev = nullptr;
        }
        delete temp;
        --_size;
    }

    // 在指定位置后插入元素
    void insert_after(Node* node, const T& value) {
        if (!node) return;

        Node* newNode = new Node(value);
        newNode->next = node->next;
        if (node->next) {
            node->next->prev = newNode;
        }
        node->next = newNode;
        newNode->prev = node;

        if (node == tail) {
            tail = newNode;
        }
        ++_size;
    }

    // 删除指定位置的元素
    void erase(Node* node) {
        if (!node) return;

        if (node == head) {
            pop_front();
        } else if (node == tail) {
            pop_back();
        } else {
            node->prev->next = node->next;
            node->next->prev = node->prev;
            delete node;
            --_size;
        }
    }

    // 返回指向链表头部的迭代器
    Node* begin() {
        return head;
    }

    // 返回指向链表尾部后一个位置的迭代器
    Node* end() {
        return nullptr;
    }

    // 调试打印
    void debug_print() const {
        Node* current = head;
        while (current) {
            std::cout << current->value << " ";
            current = current->next;
        }
        std::cout << std::endl;
    }
};
int main() {
    List<int> list;

    list.push_back(10);
    list.push_back(20);
    list.push_back(30);
    list.push_front(5);
    list.push_front(1);

    std::cout << "List after push operations: ";
    list.debug_print();  // 输出：1 5 10 20 30

    list.pop_back();
    list.pop_front();
    std::cout << "List after pop operations: ";
    list.debug_print();  // 输出：5 10 20

    // 在指定节点后插入
    auto node = list.begin();
    list.insert_after(node, 7);  // 插入 7 到头部后
    std::cout << "List after insert_after: ";
    list.debug_print();  // 输出：5 7 10 20

    // 删除一个节点
    list.erase(node->next);  // 删除值为 7 的节点
    std::cout << "List after erase: ";
    list.debug_print();  // 输出：5 10 20

    return 0;
}
