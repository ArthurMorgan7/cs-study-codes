#include <functional>
#include <vector>
#include <list>
#include <iostream>

using namespace std;

/* -------------------------------------------------------------------------- */
/*                             手写哈希表（开链法）                              */
/* -------------------------------------------------------------------------- */
// 内嵌类，仅自己可用


template<typename KeyType, typename ValueType>
class HashTable{

private:
    vector<list<pair<KeyType, ValueType>>> table;
    size_t size_;        // 节点数量
    size_t capacity_;    // 槽位数量
    size_t hashFunction(KeyType key){
        std::hash<KeyType> hash;
        return hash(key) % capacity_;
    }

public:
    HashTable(int cap = 10):capacity_(cap), size_(0){
        table.resize(capacity_);
    }

    // 插入
    void insert(pair<KeyType, ValueType> p){
        size_t idx = hashFunction(p.first);
        for(auto node : table[idx]){
            if(p.first == node.first){
                node.second = p.second;
                return;
            }
        }
        table[idx].push_back(p);
        ++size_;
    }

    // 查找
    bool find(KeyType key){
        size_t idx = hashFunction(key); // 得到哈希值
        for(auto node : table[idx]){    // 根据哈希值找到特定的一条链，然后根据 key 去寻找
            if(key == node.first){
                return true;
            }
        }
        return false;
    } 


    // 删除 
    bool erase(KeyType key){
        size_t idx = hashFunction(key);
        auto &ls = table[idx];
        for(auto it = ls.begin(); it != ls.end(); ++it){
            if((*it).first == key){
                ls.erase(it);
                --size_;
                return true;
            }
        }
        return false;
    }

    void print(){
        for(int i = 0; i < capacity_-1; i++){
            for(auto node : table[i]){
                cout << node.first << ":" << node.second << endl;
            }
        }
    }

    size_t getSize(){
        return size_;
    }
};


/* ---------------------------------- test ---------------------------------- */


int main() {
    HashTable<int, string> ht;

    // 插入元素
    ht.insert({1, "One"});
    ht.insert({2, "Two"});
    ht.insert({3, "Three"});
    ht.insert({10, "Ten"});
    ht.insert({20, "Twenty"});

    // 打印哈希表
    cout << "HashTable after insertions:" << endl;
    ht.print();

    // 查找元素
    string value;
    if (ht.find(2)) {
        cout << "Found key 2: " << value << endl;
    } else {
        cout << "Key 2 not found" << endl;
    }

    // 删除元素
    ht.erase(3);

    // 打印哈希表
    cout << "\nHashTable after removing key 3:" << endl;
    ht.print();

    // 打印哈希表的大小
    cout << "\nHashTable size: " << ht.getSize() << endl;

    return 0;
}