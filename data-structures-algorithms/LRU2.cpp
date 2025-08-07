#include <iostream>
#include <list>
#include <unordered_map>
#include <utility>
#include <sstream>
#include <string>

using namespace std;

class LRUCache{
private:
    int capacity;
    list<pair<int, int>> ls;
    unordered_map<int, list<pair<int, int>>::iterator> mp;
public:
    LRUCache(int cap):capacity(cap){}

    void get(int key){
        auto it = mp.find(key);
        if(it == mp.end()){
            cout << "The key is unfinded!" << endl;
            return;
        }
        auto need_move = mp[key];
        ls.splice(ls.begin(), ls, need_move);
        mp[key] = ls.begin();
        cout << need_move->second << endl;
    }

    void push(int key, int value){
        // 首先看是否已存在，如果存在，就不放入，把它移动到链表头部
        auto it = mp.find(key);
        if(it != mp.end()){
            auto need_move = mp[key];
            ls.splice(ls.begin(), ls, need_move);
        }
        // 如果不存在，则直接放到链表的首部
        else{
            // 还得看容量是否已经满了
            if(ls.size() == capacity){
                
                mp.erase(ls.back().first);
                ls.pop_back();
            }
            ls.push_front({key,value});
            mp[key] = ls.begin();
        }
    }

    // 主要是为了查看Cache的内容
    void printCache(){
        int cnt = 0;
        for(auto &p : ls){
            cout << cnt++ << ": " << p.first << " " << p.second << endl;
        }
    }
};


int main(){
    LRUCache lru(5);
    // lru.push(1, 11);
    // lru.push(2, 22);
    // lru.push(3, 33);
    // lru.push(4, 44);
    // lru.printCache();
    // lru.get(1);
    string input;
    while(getline(cin, input)){
        stringstream ss(input);
        string control;
        int key, value;
        ss >> control >> key >> value;
        if(control == "push"){
            lru.push(key, value);
        }
        else if(control == "get"){
            lru.get(key);
        }
        else if(control == "print"){
            lru.printCache();
        }

    }
    return 0;
}
