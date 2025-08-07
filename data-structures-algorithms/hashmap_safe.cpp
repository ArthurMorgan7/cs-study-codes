#include <iostream>
#include <vector>
#include <list>
#include <mutex>
#include <shared_mutex>
#include <stdexcept>

template <typename Key, typename Value>
class ThreadSafeHashMap {
private:
    static const size_t NUM_BUCKETS = 16;  // 哈希表的桶数
    std::vector<std::list<std::pair<Key, Value>>> table;  // 存储哈希表的桶
    std::vector<std::mutex> bucket_locks;  // 每个桶的锁

    size_t hash(const Key& key) const {
        return std::hash<Key>{}(key) % NUM_BUCKETS;  // 哈希函数
    }

public:
    ThreadSafeHashMap() : table(NUM_BUCKETS), bucket_locks(NUM_BUCKETS) {}

    // 插入元素
    void insert(const Key& key, const Value& value) {
        size_t bucket_index = hash(key);
        std::lock_guard<std::mutex> lock(bucket_locks[bucket_index]);

        auto& bucket = table[bucket_index];
        for (auto& pair : bucket) {
            if (pair.first == key) {
                pair.second = value;  // 更新值
                return;
            }
        }

        // 如果没有找到相同的key，插入新的元素
        bucket.push_back(std::make_pair(key, value));
    }

    // 查找元素
    bool find(const Key& key, Value& result) const {
        size_t bucket_index = hash(key);
        std::lock_guard<std::mutex> lock(bucket_locks[bucket_index]);

        const auto& bucket = table[bucket_index];
        for (const auto& pair : bucket) {
            if (pair.first == key) {
                result = pair.second;
                return true;
            }
        }

        return false;  // 没有找到该key
    }

    // 删除元素
    void erase(const Key& key) {
        size_t bucket_index = hash(key);
        std::lock_guard<std::mutex> lock(bucket_locks[bucket_index]);

        auto& bucket = table[bucket_index];
        for (auto it = bucket.begin(); it != bucket.end(); ++it) {
            if (it->first == key) {
                bucket.erase(it);  // 删除该元素
                return;
            }
        }
    }

    // 打印哈希表内容（用于调试）
    void print() const {
        for (size_t i = 0; i < NUM_BUCKETS; ++i) {
            std::cout << "Bucket " << i << ": ";
            for (const auto& pair : table[i]) {
                std::cout << "(" << pair.first << ", " << pair.second << ") ";
            }
            std::cout << "\n";
        }
    }
};

int main() {
    ThreadSafeHashMap<int, std::string> map;

    // 创建多个线程进行并发插入、查找和删除操作
    std::vector<std::thread> threads;

    // 插入线程
    threads.push_back(std::thread([&]() {
        map.insert(1, "One");
        map.insert(2, "Two");
        map.insert(3, "Three");
    }));

    threads.push_back(std::thread([&]() {
        map.insert(4, "Four");
        map.insert(5, "Five");
    }));

    // 查找线程
    threads.push_back(std::thread([&]() {
        std::string result;
        if (map.find(2, result)) {
            std::cout << "Found key 2: " << result << "\n";
        }
        if (map.find(5, result)) {
            std::cout << "Found key 5: " << result << "\n";
        }
    }));

    // 删除线程
    threads.push_back(std::thread([&]() {
        map.erase(3);
        map.erase(4);
    }));

    // 等待所有线程完成
    for (auto& t : threads) {
        t.join();
    }

    // 打印哈希表的内容
    map.print();  // 输出哈希表的最终状态

    return 0;
}
