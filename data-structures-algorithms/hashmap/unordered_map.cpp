#include <iostream>
#include <vector>
#include <list>
#include <functional>  // std::hash

template <typename K, typename V>
class UnorderedMap {
private:
    static const size_t INITIAL_BUCKET_COUNT = 8;
    static constexpr float LOAD_FACTOR_THRESHOLD = 0.75f;

    struct Node {
        K key;
        V value;
        Node(const K& k, const V& v) : key(k), value(v) {}
    };

    std::vector<std::list<Node>> buckets;
    size_t _size;

    size_t getBucketIndex(const K& key) const {
        return std::hash<K>{}(key) % buckets.size();
    }

    void rehash() {
        size_t newBucketCount = buckets.size() * 2;
        std::vector<std::list<Node>> newBuckets(newBucketCount);

        for (const auto& bucket : buckets) {
            for (const auto& node : bucket) {
                size_t newIndex = std::hash<K>{}(node.key) % newBucketCount;
                newBuckets[newIndex].push_back(node);
            }
        }

        buckets = std::move(newBuckets);
    }

public:
    UnorderedMap() : buckets(INITIAL_BUCKET_COUNT), _size(0) {}

    bool insert(const K& key, const V& value) {
        if (find(key) != nullptr) return false;  // key 已存在

        if ((float)(_size + 1) / buckets.size() > LOAD_FACTOR_THRESHOLD) {
            rehash();
        }

        size_t index = getBucketIndex(key);
        buckets[index].push_back(Node(key, value));
        ++_size;
        return true;
    }

    V& operator[](const K& key) {
        Node* node = find(key);
        if (!node) {
            insert(key, V());  // 如果不存在，插入一个默认值
            node = find(key);
        }
        return node->value;
    }

    Node* find(const K& key) {
        size_t index = getBucketIndex(key);
        for (auto& node : buckets[index]) {
            if (node.key == key) return &node;
        }
        return nullptr;
    }

    bool erase(const K& key) {
        size_t index = getBucketIndex(key);
        auto& bucket = buckets[index];
        for (auto it = bucket.begin(); it != bucket.end(); ++it) {
            if (it->key == key) {
                bucket.erase(it);
                --_size;
                return true;
            }
        }
        return false;
    }

    size_t size() const {
        return _size;
    }

    bool empty() const {
        return _size == 0;
    }

    void debug_print() const {
        for (size_t i = 0; i < buckets.size(); ++i) {
            std::cout << "Bucket " << i << ": ";
            for (const auto& node : buckets[i]) {
                std::cout << "[" << node.key << ": " << node.value << "] ";
            }
            std::cout << "\n";
        }
    }
};


int main() {
    UnorderedMap<std::string, int> map;

    // 插入键值对
    map.insert("apple", 10);
    map.insert("banana", 20);
    map.insert("cherry", 30);

    // 使用 operator[] 访问并插入默认值
    std::cout << "apple: " << map["apple"] << "\n";  // 10
    map["orange"] = 40;
    std::cout << "orange: " << map["orange"] << "\n"; // 40

    // 查找键
    auto* found = map.find("banana");
    if (found) {
        std::cout << "Found banana: " << found->value << "\n";  // 20
    }

    // 删除键
    map.erase("apple");

    std::cout << "Contains apple? " << (map.find("apple") ? "Yes" : "No") << "\n";  // No

    std::cout << "Size: " << map.size() << "\n";

    map.debug_print();

    return 0;
}
