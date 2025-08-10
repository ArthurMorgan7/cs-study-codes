#include <iostream>
#include <stdexcept>  // 用于异常处理

template <typename T>
class ForwardList {
private:
    // 定义节点结构
    struct Node {
        T value;
        Node* next;
        Node(const T& val) : value(val), next(nullptr) {}
    };

    Node* head;  // 指向链表头部
    size_t _size; // 链表的大小

public:
    // 构造函数
    ForwardList() : head(nullptr), _size(0) {}

    // 析构函数
    ~ForwardList() {
        clear();
    }

    // 清空链表
    void clear() {
        while (head) {
            Node* temp = head;
            head = head->next;
            delete temp;
        }
        _size = 0;
    }

    // 返回链表的大小
    size_t size() const {
        return _size;
    }

    // 判断链表是否为空
    bool empty() const {
        return _size == 0;
    }

    // 在链表头部插入元素
    void push_front(const T& value) {
        Node* newNode = new Node(value);
        newNode->next = head;
        head = newNode;
        ++_size;
    }

    // 删除链表头部元素
    void pop_front() {
        if (empty()) {
            throw std::out_of_range("List is empty");
        }
        Node* temp = head;
        head = head->next;
        delete temp;
        --_size;
    }

    // 在指定位置后插入元素
    void insert_after(Node* node, const T& value) {
        if (!node) return;

        Node* newNode = new Node(value);
        newNode->next = node->next;
        node->next = newNode;
        ++_size;
    }

    // 删除指定位置后的元素
    void erase_after(Node* node) {
        if (!node || !node->next) return;

        Node* temp = node->next;
        node->next = node->next->next;
        delete temp;
        --_size;
    }

    // 返回指向链表头部的迭代器
    Node* begin() {
        return head;
    }

    // 返回指向链表尾部后的迭代器
    Node* end() {
        return nullptr;
    }

    // 打印链表的元素
    void debug_print() const {
        Node* current = head;
        while (current) {
            std::cout << current->value << " ";
            current = current->next;
        }
        std::cout << "\n";
    }
};

int main() {
    ForwardList<int> flist;

    flist.push_front(10);
    flist.push_front(20);
    flist.push_front(30);
    flist.push_front(40);

    std::cout << "List after push_front operations: ";
    flist.debug_print();  // 输出：40 30 20 10

    flist.pop_front();
    std::cout << "List after pop_front operation: ";
    flist.debug_print();  // 输出：30 20 10

    // 插入元素在指定节点后
    auto node = flist.begin();
    flist.insert_after(node, 25);
    std::cout << "List after insert_after: ";
    flist.debug_print();  // 输出：30 25 20 10

    // 删除指定节点后面的元素
    node = node->next;  // 当前指向 30 后面的节点
    flist.erase_after(node);
    std::cout << "List after erase_after: ";
    flist.debug_print();  // 输出：30 25 10

    return 0;
}

