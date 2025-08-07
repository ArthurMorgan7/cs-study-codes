#include <iostream>
#include <unordered_map>
#include <list>
#include <algorithm> // for std::max
#include <cstring>   // for strcmp
#include <cstdio>    // for scanf/printf

using namespace std;

// 哈希表的主要作用
// 1. 判断插入的键是否已经在链表中；
// 2. 方便快速访问键所在的位置；


// 接口：get()，put(), printCache()

// 实现方式：
// 1. 双向链表 + 哈希表
// 2. 动态数组 + 哈希表



// 定义 LRUCache 类
class LRUCache {
private:
    int capacity;
    list<pair<int, int>> cache; // 存储键值对的双向链表
    unordered_map<int, list<pair<int, int>>::iterator> map; // 哈希表，键对应链表的迭代器

public:
    LRUCache(int cap) : capacity(cap) {}

    int get(int key) {
        if (map.find(key) == map.end()) {
            cout << "[GET] Key " << key << " not found.\n";
            return -1; // 如果 key 不存在，返回 -1
        }
        // 移动访问的键到链表头部
        auto it = map[key];
        cache.splice(cache.begin(), cache, it);
        cout << "[GET] Key " << key << " found, value = " << it->second << ".\n";
        return it->second;
    }

    void put(int key, int value) {
        if (map.find(key) != map.end()) {
            // 如果键已经存在，更新值并移动到链表头部
            auto it = map[key];
            cache.splice(cache.begin(), cache, it);
            it->second = value;
            cout << "[PUT] Key " << key << " updated with value " << value << ".\n";
        } else {
            // 如果键不存在，检查容量
            if (cache.size() >= static_cast<size_t>(capacity)) {
                // 移除链表尾部的最久未使用元素
                auto toRemove = cache.back();
                map.erase(toRemove.first);
                cache.pop_back();
                cout << "[PUT] Cache full, evicting key " << toRemove.first << ".\n";
            }
            // 插入新元素到链表头部
            cache.emplace_front(key, value);
            map[key] = cache.begin();
            cout << "[PUT] Key " << key << " inserted with value " << value << ".\n";
        }
        printCache();
    }

    void printCache() {
        cout << "[CACHE STATE] ";
        for (const auto& pair : cache) {
            cout << "(" << pair.first << ", " << pair.second << ") ";
        }
        cout << "\n";
    }
};

int main() {
    int capacity;
    printf("Enter the capacity of LRU Cache: ");
    scanf("%d", &capacity);

    // 创建 LRUCache 对象
    LRUCache cache(max(0, capacity)); // 确保容量非负

    // 处理输入操作
    char operation[4];
    int key, value;
    printf("Enter operations (e.g., put 1 10, get 1):\n");
    while (scanf("%s%d", operation, &key) != EOF) {
        // 完全相同
        if (strcmp(operation, "get") == 0) {    
            printf("[RESULT] Value for key %d: %d\n", key, cache.get(key)); // get
        } 
        else if (strcmp(operation, "put") == 0) {
            scanf("%d", &value);    
            cache.put(key, value);  // put
        } 
        else {
            printf("Invalid operation: %s\n", operation);
        }
    }

    return 0;
}
