#include <iostream>
#include <stdexcept>

template <typename T>
class RedBlackTreeSet {
private:
    enum Color { RED, BLACK };

    struct Node {
        T value;
        Node* parent;
        Node* left;
        Node* right;
        Color color;

        Node(T val) : value(val), parent(nullptr), left(nullptr), right(nullptr), color(RED) {}
    };

    Node* root;
    Node* TNULL;  // 代表叶子节点（黑色空节点）

    // 左旋转
    void leftRotate(Node* x) {
        Node* y = x->right;
        x->right = y->left;
        if (y->left != TNULL) {
            y->left->parent = x;
        }
        y->parent = x->parent;
        if (x->parent == nullptr) {
            root = y;
        } else if (x == x->parent->left) {
            x->parent->left = y;
        } else {
            x->parent->right = y;
        }
        y->left = x;
        x->parent = y;
    }

    // 右旋转
    void rightRotate(Node* x) {
        Node* y = x->left;
        x->left = y->right;
        if (y->right != TNULL) {
            y->right->parent = x;
        }
        y->parent = x->parent;
        if (x->parent == nullptr) {
            root = y;
        } else if (x == x->parent->right) {
            x->parent->right = y;
        } else {
            x->parent->left = y;
        }
        y->right = x;
        x->parent = y;
    }

    // 插入修复，确保红黑树的性质
    void fixInsert(Node* k) {
        Node* u;
        while (k->parent->color == RED) {
            if (k->parent == k->parent->parent->right) {
                u = k->parent->parent->left;
                if (u->color == RED) {
                    u->color = BLACK;
                    k->parent->color = BLACK;
                    k->parent->parent->color = RED;
                    k = k->parent->parent;
                } else {
                    if (k == k->parent->left) {
                        k = k->parent;
                        rightRotate(k);
                    }
                    k->parent->color = BLACK;
                    k->parent->parent->color = RED;
                    leftRotate(k->parent->parent);
                }
            } else {
                u = k->parent->parent->right;
                if (u->color == RED) {
                    u->color = BLACK;
                    k->parent->color = BLACK;
                    k->parent->parent->color = RED;
                    k = k->parent->parent;
                } else {
                    if (k == k->parent->right) {
                        k = k->parent;
                        leftRotate(k);
                    }
                    k->parent->color = BLACK;
                    k->parent->parent->color = RED;
                    rightRotate(k->parent->parent);
                }
            }
            if (k == root) {
                break;
            }
        }
        root->color = BLACK;
    }

    // 插入操作
    void insert(const T& key) {
        Node* node = new Node(key);
        Node* y = nullptr;
        Node* x = root;

        while (x != TNULL) {
            y = x;
            if (node->value < x->value) {
                x = x->left;
            } else if (node->value > x->value) {
                x = x->right;
            } else {
                return; // 如果元素已经存在，则不插入
            }
        }

        node->parent = y;
        if (y == nullptr) {
            root = node;
        } else if (node->value < y->value) {
            y->left = node;
        } else {
            y->right = node;
        }

        node->left = TNULL;
        node->right = TNULL;
        node->color = RED;

        fixInsert(node);
    }

    // 查找元素
    bool search(Node* node, const T& key) const {
        if (node == TNULL) {
            return false;
        }

        if (key == node->value) {
            return true;
        }

        if (key < node->value) {
            return search(node->left, key);
        } else {
            return search(node->right, key);
        }
    }

public:
    RedBlackTreeSet() {
        TNULL = new Node(T());
        TNULL->color = BLACK;
        root = TNULL;
    }

    ~RedBlackTreeSet() {
        // Destructor logic to free memory goes here (left as an exercise).
    }

    // 插入元素
    void insert(const T& key) {
        insert(key);
    }

    // 查找元素
    bool search(const T& key) const {
        return search(root, key);
    }

    // 打印树的中序遍历（按顺序打印元素）
    void inorderPrint(Node* node) const {
        if (node != TNULL) {
            inorderPrint(node->left);
            std::cout << node->value << " ";
            inorderPrint(node->right);
        }
    }

    // 打印整个树
    void print() const {
        inorderPrint(root);
        std::cout << std::endl;
    }
};


int main() {
    RedBlackTreeSet<int> set;

    // 插入元素
    set.insert(10);
    set.insert(20);
    set.insert(30);
    set.insert(15);
    set.insert(25);

    // 打印集合
    set.print();  // 应该按顺序打印：10 15 20 25 30

    // 查找元素
    if (set.search(15)) {
        std::cout << "15 is found in the set!" << std::endl;
    } else {
        std::cout << "15 is not found in the set!" << std::endl;
    }

    return 0;
}
