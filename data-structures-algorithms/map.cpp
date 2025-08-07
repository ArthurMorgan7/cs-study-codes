#include <iostream>

enum Color { RED, BLACK };

template <typename K, typename V>
class RBMap {
private:
    struct Node {
        K key;
        V value;
        Color color;
        Node* left;
        Node* right;
        Node* parent;

        Node(K k, V v) : key(k), value(v), color(RED), left(nullptr), right(nullptr), parent(nullptr) {}
    };

    Node* root = nullptr;
    size_t _size = 0;

    // 左旋
    void rotateLeft(Node* x) {
        Node* y = x->right;
        x->right = y->left;
        if (y->left)
            y->left->parent = x;
        y->parent = x->parent;
        if (!x->parent)
            root = y;
        else if (x == x->parent->left)
            x->parent->left = y;
        else
            x->parent->right = y;
        y->left = x;
        x->parent = y;
    }

    // 右旋
    void rotateRight(Node* y) {
        Node* x = y->left;
        y->left = x->right;
        if (x->right)
            x->right->parent = y;
        x->parent = y->parent;
        if (!y->parent)
            root = x;
        else if (y == y->parent->left)
            y->parent->left = x;
        else
            y->parent->right = x;
        x->right = y;
        y->parent = x;
    }

    // 插入修复
    void fixInsert(Node* z) {
        while (z->parent && z->parent->color == RED) {
            Node* gp = z->parent->parent;
            if (z->parent == gp->left) {
                Node* y = gp->right;  // 叔叔
                if (y && y->color == RED) {
                    z->parent->color = BLACK;
                    y->color = BLACK;
                    gp->color = RED;
                    z = gp;
                } else {
                    if (z == z->parent->right) {
                        z = z->parent;
                        rotateLeft(z);
                    }
                    z->parent->color = BLACK;
                    gp->color = RED;
                    rotateRight(gp);
                }
            } else {
                Node* y = gp->left;
                if (y && y->color == RED) {
                    z->parent->color = BLACK;
                    y->color = BLACK;
                    gp->color = RED;
                    z = gp;
                } else {
                    if (z == z->parent->left) {
                        z = z->parent;
                        rotateRight(z);
                    }
                    z->parent->color = BLACK;
                    gp->color = RED;
                    rotateLeft(gp);
                }
            }
        }
        root->color = BLACK;
    }

    Node* insertNode(const K& key, const V& value) {
        Node* z = new Node(key, value);
        Node* y = nullptr;
        Node* x = root;

        while (x) {
            y = x;
            if (key < x->key)
                x = x->left;
            else if (key > x->key)
                x = x->right;
            else {
                x->value = value;  // key 已存在，更新 value
                delete z;
                return x;
            }
        }

        z->parent = y;
        if (!y)
            root = z;
        else if (key < y->key)
            y->left = z;
        else
            y->right = z;

        fixInsert(z);
        _size++;
        return z;
    }

    Node* findNode(const K& key) const {
        Node* curr = root;
        while (curr) {
            if (key == curr->key)
                return curr;
            else if (key < curr->key)
                curr = curr->left;
            else
                curr = curr->right;
        }
        return nullptr;
    }

    void deleteTree(Node* node) {
        if (!node) return;
        deleteTree(node->left);
        deleteTree(node->right);
        delete node;
    }

public:
    ~RBMap() {
        deleteTree(root);
    }

    void insert(const K& key, const V& value) {
        insertNode(key, value);
    }

    V& operator[](const K& key) {
        Node* node = findNode(key);
        if (!node)
            node = insertNode(key, V());
        return node->value;
    }

    bool contains(const K& key) const {
        return findNode(key) != nullptr;
    }

    V* find(const K& key) {
        Node* node = findNode(key);
        return node ? &node->value : nullptr;
    }

    size_t size() const {
        return _size;
    }

    bool empty() const {
        return _size == 0;
    }
};


int main() {
    RBMap<std::string, int> m;

    m.insert("apple", 5);
    m["banana"] = 10;
    m["cherry"] = 15;

    std::cout << "apple: " << m["apple"] << "\n";
    std::cout << "banana: " << m["banana"] << "\n";

    if (m.contains("cherry")) {
        std::cout << "cherry: " << *m.find("cherry") << "\n";
    }

    std::cout << "size: " << m.size() << "\n";
    return 0;
}
