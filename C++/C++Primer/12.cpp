#include <memory>
#include <vector>
#include <string>
#include <iostream>

using namespace std;

 /* -------------------------------------------------------------------------- */
 /*                               12.1 动态内存与智能指针                               */
 /* -------------------------------------------------------------------------- */

// 一个管理 string 的类
class StrBlob{
public:
    typedef vector<string>::size_type size_type;

    StrBlob():data(make_shared<vector<string>>()){ }

    StrBlob(initializer_list<string> il):data(make_shared<vector<string>>(il)){ }

    string& front(){    // 注意返回的是引用，方便外部修改
        check(0, "front on empty StrBlob");
        return data->front();
    }

    string& back(){

        return data->back();
    }

    void push_back(const string &t){
        data->push_back(t);
    }

    void pop_back(){

    }

    size_type size() const{
        return data->size();
    }

    bool empty() const{
        return data->empty();
    }

private:
    shared_ptr<vector<string>> data;    // 保存string的数组
    void check(size_type i, const string &msg) const{
        if(i >= data->size()){
            throw out_of_range(msg);
        }
    }
};


void homework_12_1(){
    StrBlob b1;
    {
        StrBlob b2 = {"a", "an", "the"};
        
        cout << b1.size();
        b1 = b2;
        cout << b1.size();
        
        b2.push_back("about");
        cout << b2.size() << endl;
    }
}
/* ---------------------------------- ---- ---------------------------------- */

bool homework_12_8(){
    int *p = new int;
    return p;   // 虽然指针可以隐式转为bool，但会内存泄漏
}

void homework_12_9(){
    int *q = new int(42), *r = new int(100);
    r = q;  // 100占用内存会泄露，无人管理
    auto q2 = make_shared<int>(42);
    auto r2 = make_shared<int>(100);
    r2 = q2;    // 在拷贝赋值时，引用计数减一，100 占用的内存会自动释放
}


/* ---------------------------------- ---- ---------------------------------- */
void process(shared_ptr<int> ptr){

}

void homework_12_10(){
    shared_ptr<int> p(new int(42));
    process(shared_ptr<int>(p));
}

void homework_12_11(){
    shared_ptr<int> p(new int(42));
    process(shared_ptr<int>(p.get()));
}

void homework_12_12(){
    auto p = new int(); // 值初始化：0
    auto sp = make_shared<int>(); // 模板函数
    process(sp);
    // process(new int());     // 参数不匹配
    // process(p);             // 参数不匹配
    process(shared_ptr<int>(p));
}

void homework_12_13(){
    auto sp = make_shared<int>();
    auto p = sp.get();
    delete p;
}




int main(){
    // homework_12_1();
    homework_12_8();
    homework_12_9();
    // homework_12_10();
    // homework_12_11();
    // homework_12_12();
    // homework_12_13();
}